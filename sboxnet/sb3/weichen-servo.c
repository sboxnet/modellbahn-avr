/***************************************************************************
 *   Copyright (C) 2012-2020
 *   by Thomas Maier <balagi@justmail.de>
 *
 *   Copyright: See COPYING file that comes with this distribution         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

/*
 * Ansteuerung von 8 Servos zum Antrieb von Weichen.
 * Mit Rückmeldung der Servostellung über Taster und Programmierinterface
 * zur Einstellung des Servoswegs.
 */

#include "common.h"

#include <avr/eeprom.h>

#define PRODUCT_ID   0x0001
#define VENDOR_ID    0x9999
#define FIRMWARE_VERSION 0x0200
#define DEVICE_DESC  "WeichenServo:1"

APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)


#define CHANNEL_COUNT 8
#define CHANNEL_MASK  0x07

#define SERVO_PULS_MIN_USEC		1000u
#define SERVO_PULS_MAX_USEC		2000u
#define SERVO_TIMER_PRESCALER	64
#define SERVO_TIMER_FREQ_HZ		400
#define SERVO_TIMER_CHANNEL_FREQ_HZ (SERVO_TIMER_FREQ_HZ / CHANNEL_COUNT) // 50
#define SERVO_TIMER_TOP         (F_CPU / (SERVO_TIMER_PRESCALER * SERVO_TIMER_FREQ_HZ))  // 1250
#define SERVO_TIMER_PULS(_us)	((_us) * F_CPU_MHZ / (SERVO_TIMER_PRESCALER))
#define SERVO_TIMER_PULS_MIN	SERVO_TIMER_PULS(SERVO_PULS_MIN_USEC)				// 500
#define SERVO_TIMER_PULS_MAX	SERVO_TIMER_PULS(SERVO_PULS_MAX_USEC)				// 1000
#define SERVO_TIMER_PULS_STEP	((SERVO_TIMER_PULS_MAX - SERVO_TIMER_PULS_MIN)/100)	// 5


#define LED_TIMER 31 // ~500 ms

struct Servo {
	struct State {
		unsigned domove:1;
		unsigned dstpos:1;
		unsigned curpos:1;
		unsigned minchanged:1;
		unsigned maxchanged:1;
        unsigned movtchanged:1;
    } state;
	uint16_t mintime;
	uint16_t maxtime;
	uint16_t curtime;
	uint16_t deltatime;
    uint16_t perc_minv;
    uint16_t perc_maxv;
    uint16_t movetime;
    uint8_t  moving;
    uint8_t  retry_timer;
    uint8_t  notack;
    uint8_t  last_seq;
};

struct Servo g_servos[CHANNEL_COUNT];
uint8_t g_curchannel = 0;
uint8_t g_selservoid = 0;
struct Servo* pselservo = 0;
uint8_t g_servo_curpos = 0;
uint8_t g_servo_set = 0;


struct ServoEeprom {
    uint16_t minv;
    uint16_t maxv;
    uint16_t movetime;
    uint8_t  reserved[10];
};

struct Eeprom {
	struct ServoEeprom servo[CHANNEL_COUNT];
};
struct Eeprom eeprom EEMEM;

uint8_t g_transmit_seq = 0;

uint8_t g_switches = 0;
uint8_t g_switches_old = 0;

#define KEY_SEL		0x01
#define KEY_ACTION	0x02
#define KEY_UP		0x04
#define KEY_DOWN	0x08
uint8_t g_keys = 0;

uint8_t g_keystate = 0;
struct timer g_wait_timer;
struct timer g_switch_timer;
uint8_t g_servos_enabled = 0;

struct timer g_led_timer;
uint8_t g_led_toggle = 0;
uint8_t g_keys_t = 0;
uint8_t g_switches_t = 0;

uint8_t g_move_sema = 0;

uint8_t g_manual_mode = 0;

static inline uint16_t check_servo_minmax(uint16_t v, uint16_t vmin, uint16_t vmax) {
    if (v > vmax)
        return vmax;
    if (v < vmin)
        return vmin;
    return v;
}

uint16_t servo_time_to_percent(uint16_t t) {
    if (t < SERVO_TIMER_PULS_MIN)
        t = SERVO_TIMER_PULS_MIN;
    else if (t > SERVO_TIMER_PULS_MAX)
        t = SERVO_TIMER_PULS_MAX;
    return (t - SERVO_TIMER_PULS_MIN) * (1000 / (SERVO_TIMER_PULS_MAX - SERVO_TIMER_PULS_MIN));
}

uint16_t percent_to_servo_time(uint16_t t) {
    if (t > 1000)
        t = 1000;
    return t / (1000/(SERVO_TIMER_PULS_MAX - SERVO_TIMER_PULS_MIN)) + SERVO_TIMER_PULS_MIN;
}

// read_switches(): suppresses key bounce. changed key is recognized
// if its state is unchanged during the last 2 calls.
static void read_switches(void) {
    uint8_t row_k, row_s;
    
    row_k = ~(port_in(PORTA)) & 0x0f;

    row_s = 0;
    for (int8_t i = 7; i >= 0; i--) {
        port_out(PORTB) = (port_out(PORTB) & 0xf8)|i;
        row_s <<= 1;
        _NOP();
        _NOP();
        _NOP();
        if (port_in(PORTB) & 0x08) {
            row_s |= 1;
        }
    }
    row_s = ~row_s;
    
    debounce_keys(&g_keys, &g_keys_t, row_k);
    debounce_keys(&g_switches, &g_switches_t, row_s);
}

static inline uint16_t get_eeprom_word(uint16_t* p, uint16_t dflt) {
    uint16_t v = e2prom_get_word(p);
    if (v == 0xffff) // not programmed
        v = dflt;
    return v;
}

static void set_servo_deltatime(struct Servo* s) {
    s->deltatime = (s->maxtime - s->mintime) * (100 / SERVO_TIMER_CHANNEL_FREQ_HZ) / s->movetime;
    if (s->deltatime == 0) {
        s->deltatime = 1;
    }
}

static NOINLINE void init_servos(void) {
    uint8_t i, switchmask;
    struct Servo* s;
    
    // read current switches. 2 calls needed because of key debounce.
    read_switches();
    read_switches();
    // now we have the current switches state in g_switches
    
    g_servo_curpos = 0;
    g_servo_set = g_switches;
    g_switches_old = g_switches;
    
    for (i = 0, switchmask = 0x01, s = g_servos; s != (g_servos+CHANNEL_COUNT); i++, switchmask <<= 1, s++) {
        uint16_t vmin, vmax, mtime;
        vmin = get_eeprom_word(&eeprom.servo[i].minv, 400);
        vmax = get_eeprom_word(&eeprom.servo[i].maxv, 600);
        mtime = get_eeprom_word(&eeprom.servo[i].movetime, 100);
        
        mtime = check_servo_minmax(mtime, 20, 1000);
        
        vmin = percent_to_servo_time(vmin);
        vmax = percent_to_servo_time(vmax);
        
        vmin = check_servo_minmax(vmin, SERVO_TIMER_PULS_MIN, SERVO_TIMER_PULS_MAX);
        vmax = check_servo_minmax(vmax, vmin, SERVO_TIMER_PULS_MAX);
        s->mintime = vmin;
        s->maxtime = vmax;
        s->movetime = mtime;
        s->perc_minv = servo_time_to_percent(vmin);
        s->perc_maxv = servo_time_to_percent(vmax);
        
        s->state.curpos = 0;
        s->moving = 0;
        s->state.minchanged = 0;
        s->state.maxchanged = 0;
        s->state.movtchanged = 0;
        // abhaengig von Servoposition-Rueckmeldung Startposition setzen und anfahren
        s->state.dstpos = 0;
        s->state.domove = 1;
        if (g_switches & switchmask) {
            s->state.dstpos = 1;
            s->curtime = s->maxtime;
        } else {
            s->curtime = s->mintime;
        }
        
        s->notack = 0;
        s->retry_timer = 0;
        s->last_seq = 0;
        
        set_servo_deltatime(s);
    }
}

void do_init_system(void) {
    
    port_out(PORTD) = 0;
    port_dirout(PORTD, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    port_out(PORTC) = 0;
    port_dirout(PORTC, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    port_dirin(PORTA, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;
    
    port_clr(PORTB, 0x07);
    port_dirout(PORTB, 0x07);
    port_dirin(PORTB, 0x08);
    PORTCFG_MPCMASK = 0x07;
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // power reduction
    PR.PRPA = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPB = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp);
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;

    // timer for servos
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC0.CTRLD = 0;
    TCC0.CTRLE = 0;
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc;
    TCC0.INTFLAGS = 0xff;
    TCC0.PER = SERVO_TIMER_TOP;
    TCC0.CCA = SERVO_TIMER_PULS_MIN;
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;

    timer_register(&g_wait_timer, TIMER_RESOLUTION_16MS);
    timer_register(&g_led_timer,  TIMER_RESOLUTION_16MS);
    timer_register(&g_switch_timer, TIMER_RESOLUTION_16MS);
    
    pselservo = &g_servos[g_selservoid];

    init_servos();

    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    g_com.capabilities = CAP_CNTRL_TURNOUT;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC);
}

static NOINLINE struct Servo* get_channel(uint8_t ch) {
	return &g_servos[ch];
}

static inline void set_channels_off(void) {
    port_out(PORTC) = 0xff;
}

// every 2.5ms
ISR(TCC0_OVF_vect) {
	if (!g_servos_enabled) {
        return;
    }
    
    register struct Servo* s = get_channel(g_curchannel);
    
    if (s->moving == 0 && s->state.domove && g_move_sema < 2) {
        g_move_sema++;
        s->moving = 15;
        s->state.curpos = s->state.dstpos;
        s->state.domove = 0;
    }

    uint8_t mask = bitmask(g_curchannel);
    
    set_channels_off();
    
    if (s->moving) {
        port_clr(PORTC, mask);

        uint16_t a,b;
        b = s->state.curpos ? s->maxtime : s->mintime;
        if (s->state.curpos) {
            a = s->curtime + s->deltatime;
            if (a >= b) {
                g_servo_curpos |= mask;
                goto pos_reached;
            }
        } else {
            a = s->curtime - s->deltatime;
            if ((int16_t)a <= (int16_t)b) {
                g_servo_curpos &= ~mask;
pos_reached:
                a = b;
                s->moving--;
            }
        }
        s->curtime = a;
        if (s->moving == 0) {
            g_move_sema--;
        }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            TCC0.CCA = a;
        }
    }
    g_curchannel = (g_curchannel + 1) & CHANNEL_MASK;
}

ISR(TCC0_CCA_vect) {
    if (g_servos_enabled) {
        set_channels_off();
    }
}

static void init_sel_leds(void) {
    port_out(PORTD) = bitmask(g_selservoid);
    timer_set(&g_led_timer, LED_TIMER);
}

static NOINLINE uint8_t check_key_sel(uint8_t keys) {
	if (keys & KEY_SEL) {
		if (!(g_keystate & KEY_SEL)) {
		
            if (!g_manual_mode) {
                g_manual_mode = 1;
                init_sel_leds();
                g_keystate |= KEY_SEL;
                
            } else {
                g_keystate |= KEY_SEL;
                
                if (keys & KEY_DOWN) {
                    g_manual_mode = 0;
                } else {
                    g_selservoid = (g_selservoid + 1) & CHANNEL_MASK;
                    pselservo = &g_servos[g_selservoid];
                    
                    init_sel_leds();
                }
            }
		}
		return 1;
	} else {
		g_keystate &= ~KEY_SEL;
	}
	return 0;
}

/*
 * KEY_SEL          select servo
 * KEY_ACT          action
 * KEY_UP+KEY_ACT   servo up one step
 * KEY_DOWN+KEY_ACT servo down one step
 */
static NOINLINE void check_key_action(uint8_t keys) {
    if (!g_manual_mode)
        return;
    
	if (keys & KEY_ACTION) {
		if (!(g_keystate & KEY_ACTION)) {
			g_keystate |= KEY_ACTION;
			
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                struct Servo* servo = pselservo;
                uint8_t updown = (keys & (KEY_UP|KEY_DOWN));
                if (updown) {
                    if (servo->moving == 0) {
                        // set min/max
                        uint8_t ismax = (servo->state.curpos ? 1:0);
                        uint16_t vmin, vmax, v;
                        if (ismax) {
                            vmin = servo->mintime;
                            vmax = SERVO_TIMER_PULS_MAX;
                            servo->state.maxchanged = 1;
                            v = servo->maxtime;
                        } else {
                            vmin = SERVO_TIMER_PULS_MIN;
                            vmax = servo->maxtime;
                            servo->state.minchanged = 1;
                            v = servo->mintime;
                        }
                        v = check_servo_minmax(v + ((keys & KEY_UP) ? SERVO_TIMER_PULS_STEP : -SERVO_TIMER_PULS_STEP),
                                                vmin, vmax);
                        if (ismax) {
                            servo->maxtime = v;
                        } else {
                            servo->mintime = v;
                        }
                        servo->perc_minv = servo_time_to_percent(servo->mintime);
                        servo->perc_maxv = servo_time_to_percent(servo->maxtime);                    
                        servo->curtime = v;
                        servo->state.domove = 1;
                        servo->state.dstpos = servo->state.curpos;
                        set_servo_deltatime(servo);
                    }
                } else {
                    servo->state.domove = 1;
                    servo->state.dstpos = !servo->state.dstpos;
                }
            }
		}
	} else {
		g_keystate &= ~KEY_ACTION;
	}
}

static NOINLINE void check_input(void) {
	uint8_t keys = g_keys;
	if (!check_key_sel(keys)) {
		check_key_action(keys);
	}
}


static NOINLINE void show_status(void) {
    if (g_manual_mode) {
        if (timer_timedout(&g_led_timer)) {
            port_out(PORTD) ^= bitmask(g_selservoid);
            timer_set(&g_led_timer, LED_TIMER);
        }
    } else {
        uint8_t d = g_switches;
        uint8_t mask = g_switches ^ g_servo_set;
        if (mask) {
            d = (g_led_toggle ? d | mask : d & ~mask);
            if (timer_timedout(&g_led_timer)) {
                g_led_toggle = !g_led_toggle;
                timer_set(&g_led_timer, LED_TIMER);
            }
        } else {
            g_led_toggle = 0;
        }
        port_out(PORTD) = d;
    }
}

uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_FB_NUM:      *pdata = 8; return 0;
        case R_FB_VALUE0:   *pdata = g_switches; return 0;
        
        case R_CNTRL_NUM:      *pdata = 8; return 0;
        case R_CNTRL_VALUE0:   *pdata = g_servo_set; return 0;
        case R_CNTRL_PENDING0: *pdata = g_servo_set ^ g_servo_curpos; return 0;
    }
    
    if (reg >= R_TURNOUT_PAR_0 && reg < R_TURNOUT_PAR_8) {
        uint8_t roffs = reg & NUM_TURNOUT_PARS_MASK;
        uint8_t servonum = (reg-R_TURNOUT_PAR_0) / NUM_TURNOUT_PARS;
        switch (roffs) {
            case ROFFS_TURNOUT_PAR_MINV: {
                *pdata = g_servos[servonum].perc_minv;
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MAXV: {
                *pdata = g_servos[servonum].perc_maxv;
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MOVETIME: {
                *pdata = g_servos[servonum].movetime;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_CNTRL_VALUE0: {
            g_servo_set = (g_servo_set & ~mask) | (data & mask);
            for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
                if (mask & 0x01) {
                    struct Servo* s = get_channel(i);
                    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                        s->state.dstpos = (data & 0x01);
                        s->state.domove = 1;
                    }
                }
                mask >>= 1;
                data >>= 1;
            }
            return 0;
        }
    }
    if (reg >= R_TURNOUT_PAR_0 && reg < R_TURNOUT_PAR_8) {
        uint8_t roffs = reg & NUM_TURNOUT_PARS_MASK;
        uint8_t servonum = (reg-R_TURNOUT_PAR_0) / NUM_TURNOUT_PARS;
        struct Servo *servo = g_servos + servonum;
        switch (roffs) {
            case ROFFS_TURNOUT_PAR_MINV: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->perc_minv = check_servo_minmax(data, 0, servo->perc_maxv);
                    servo->mintime = percent_to_servo_time(servo->perc_minv);
                    set_servo_deltatime(servo);
                    servo->state.minchanged = 1;
                }
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MAXV: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->perc_maxv = check_servo_minmax(data, servo->perc_minv, 1000);
                    servo->maxtime = percent_to_servo_time(servo->perc_maxv);
                    set_servo_deltatime(servo);
                    servo->state.maxchanged = 1;
                }
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MOVETIME: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->movetime = check_servo_minmax(data, 20, 1000);
                    set_servo_deltatime(servo);
                    servo->state.movtchanged = 1;
                }
                return 0;
            }
            default: {
                break;
            }
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

uint8_t get_next_transmit_seq(void) {
    uint8_t seq = g_transmit_seq + 1;
    if (seq < 10) {
        seq = 10;
    }
    
    for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
        if (g_servos[i].notack && seq == g_servos[i].last_seq) {
            seq++;
            i = 0; // retry
        }   
    }

    g_transmit_seq = seq;
    return seq;
}

uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }
    switch (pmsg->cmd) {
        case SBOXNET_CMD_FB_CHANGED|0x80: {
            if (pmsg->opt.len != 0) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
                if (g_servos[i].notack && pmsg->seq == g_servos[i].last_seq) {
                    g_servos[i].notack = 0;
                    g_servos[i].last_seq = 0;
                }
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

void do_setup(void) {
    timer_set(&g_wait_timer, 7); // ~100 ms
    timer_set(&g_switch_timer, 5); // 80ms
}

void do_main(void) {
    if (timer_timedout(&g_switch_timer)) {
        timer_set(&g_switch_timer, 5);
        read_switches();
        
        if (g_servos_enabled) {
            uint8_t diff = g_switches ^ g_switches_old;
            g_switches_old = g_switches;
            uint8_t mask = 1;
            for (uint8_t i = 0; i < CHANNEL_COUNT; i++, mask <<= 1) {
                struct Servo* pservo = &g_servos[i];
                if (diff & mask) {
                    pservo->notack = 1;
                    pservo->retry_timer = 0;
                }
                if (pservo->retry_timer) {
                    pservo->retry_timer--;
                }
            }
        }
    }
    
    check_input();
    show_status();
    
    if (g_servos_enabled == 0 && timer_timedout(&g_wait_timer)) {
        g_servos_enabled = 1;
    }
    
    if (!g_manual_mode) {
        for (uint8_t i = 0; i < CHANNEL_COUNT; i++) {
            struct Servo *servo = g_servos + i;
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (servo->state.maxchanged) {
                    if (eeprom_is_ready()) {    // note: eeprom erase+write: ca. 4ms !
                        servo->state.maxchanged = 0;
                        eeprom_update_word(&eeprom.servo[i].maxv, servo->perc_maxv);// burns only if new value is different from old one.
                    }
                } else if (servo->state.minchanged) {
                    if (eeprom_is_ready()) {
                        servo->state.minchanged = 0;
                        eeprom_update_word(&eeprom.servo[i].minv, servo->perc_minv);
                    }
                } else if (servo->state.movtchanged) {
                    if (eeprom_is_ready()) {
                        servo->state.movtchanged = 0;
                        eeprom_update_word(&eeprom.servo[i].movetime, servo->movetime);
                    }
                }
            }
        }
    }
    
    if (bit_is_clear(g_dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
        uint8_t switches = g_switches;
        for (uint8_t i = 0, mask = 1; i < CHANNEL_COUNT; i++, mask <<= 1) {
            if (g_servos[i].retry_timer == 0 && g_servos[i].notack) {
                g_servos[i].retry_timer = 50;
                
                struct sboxnet_msg_max msg;
                msg.msgh.dstaddr = 0;
                msg.msgh.seq = g_servos[i].last_seq = get_next_transmit_seq();
                msg.msgh.opt.len = 1;
                msg.msgh.cmd = SBOXNET_CMD_FB_CHANGED;
                if (switches & mask) {
                    msg.data[0] = i|0x80;
                } else {
                    msg.data[0] = i;
                }            
                sboxnet_send_msg(&msg.msgh);
                break; // only send one per do_main() loop
            }
        }
    }
    
    sleep_cpu();
}

void do_before_bldr_activate(void) {    
    port_out(PORTC) = 0xff;
    port_out(PORTD) = 0;
}

