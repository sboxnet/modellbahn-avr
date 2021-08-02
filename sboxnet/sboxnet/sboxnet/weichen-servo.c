/***************************************************************************
 *   Copyright (C) 2012-2014
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
 * Die Servos werden per Multiplex angesteuert, ein Servo hat eine F=50Hz, 8x50 = 400 = 1250 Timer Ticks
 */

#include "common.h"

#include <avr/eeprom.h>

#define ws_PRODUCT_ID   0x0001
#define ws_VENDOR_ID    0x0001
#define ws_FIRMWARE_VERSION 0x0102
#define ws_DEVICE_DESC  "WeichenServo:1"
/*
APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)
*/

// Anzahl der Servos
#define ws_CHANNEL_COUNT 8
// für g_curchannel
#define ws_CHANNEL_MASK  0x07

// Servo Timer TCC0

// min Pulsweite in Microsekunden: 1ms
#define ws_SERVO_PULS_MIN_USEC		1000u
// max Pulsweite in Mikrosekunden: 2 ms
#define ws_SERVO_PULS_MAX_USEC		2000u
// TimerPrescaler: 
#define ws_SERVO_TIMER_PRESCALER	64
// ServoTimer HZ:
#define ws_SERVO_TIMER_FREQ_HZ		400
// 400 / 8 = 50 Hz
#define ws_SERVO_TIMER_CHANNEL_FREQ_HZ (ws_SERVO_TIMER_FREQ_HZ / ws_CHANNEL_COUNT) // 50
// Servo Timer TOP: Periode für TCC0
#define ws_SERVO_TIMER_TOP         (F_CPU / (ws_SERVO_TIMER_PRESCALER * ws_SERVO_TIMER_FREQ_HZ))  // 1250
// Mikrosekunden -> Puls wert
#define ws_SERVO_TIMER_PULS(_us)	((_us) * F_CPU_MHZ / (ws_SERVO_TIMER_PRESCALER))
// Puls Wert für Min Puls
#define ws_SERVO_TIMER_PULS_MIN	ws_SERVO_TIMER_PULS(ws_SERVO_PULS_MIN_USEC)				// 500 Timerticks
// Puls Wert für Max Puls
#define ws_SERVO_TIMER_PULS_MAX	ws_SERVO_TIMER_PULS(ws_SERVO_PULS_MAX_USEC)				// 1000 Timerticks
// Schritte für Timer Puls
#define ws_SERVO_TIMER_PULS_STEP	((ws_SERVO_TIMER_PULS_MAX - ws_SERVO_TIMER_PULS_MIN)/100)	// 5


// Timer hat eine auflösung von 16 ms, 16*7=112ms
#define ws_WAIT_TIMER  7 // 112ms
// Timer hat eine auflösung von 16 ms, 16*31=496ms
#define ws_LED_TIMER 31 // ~500 ms
// Timer hat eine auflösung von 16 ms, 16*5=80ms
#define ws_SWITCH_TIMER  5 // 5*16ms = 80 ms

// Struct für einen Servo
struct ws_Servo {
	// aktueller Zustand
	struct State {
		unsigned domove:1; // 1bit: in bewegung
		unsigned dstpos:1; // 1bit: ist Zielposition (0: am Anfang, 1: am Ende)
		unsigned curpos:1; // 1bit: aktuelle Position (0: am Anfang, 1: am Ende)
		unsigned minchanged:1; // soll die Min Position in das EePROM geschrieben werden?
		unsigned maxchanged:1; // soll die Max Position in das EEPROM geschrieben werden?
        unsigned movtchanged:1; // soll die Bewegungzeit (servo->movetime) in das EEPROM geschrieben werden?
    } state;
	uint16_t mintime; // Zeit für Min Puls in TCC0 Schritten
	uint16_t maxtime; // Zeit für Max Puls in TCC0 Schritten
	uint16_t curtime; // aktuell berechnete Position
	uint16_t deltatime; // um soviel soll die Position pro Schritt geändert werden
    uint16_t perc_minv; // Prozent für Min Wert
    uint16_t perc_maxv; // Prozent für Max Wert
    uint16_t movetime;  // Bewegungszeit in us
    uint8_t  moving;    // bewegt sich der Servo gerade? (ist ein Semaphore 0=bewegt sich nicht)
    uint8_t  retry_timer; // Timer in welcher Zeit die Rückmeldung wieder verschickt wird solange nicht bestätigt, wird in main heruntergezählt
    uint8_t  notack; // war die SBOXNET_CMD_FB_CHANGED schon bestätigt?
    uint8_t  last_seq; // letzte Sequencenr der SBOXNET_CMD_FB_CHANGED Nachricht
};

// Struktur die die Variablen für die Servos hat
struct ws_v_t {
	// pro Server eine struct ws_Servo
	struct ws_Servo g_servos[ws_CHANNEL_COUNT];
	uint8_t g_curchannel;		// aktueller Kanal
	uint8_t g_selservoid;		// aktuell gewählter Servo für manuellen Modus
	struct ws_Servo* pselservo; // Zeiger auf aktuellen Server
	uint8_t g_servo_curpos;		// aktuelle Position 0: Anfang=Min, 1: Ende=Max
	uint8_t g_servo_set;		// Wert der Rückmeldung, Bitmaske
	uint8_t g_transmit_seq;		// letzte Sequenz für die SBOXNET_CMD_FB_CHANGED Meldung
	uint8_t g_switches;			// Rückmeldungen
	uint8_t g_switches_old;		// vorige Rückmeldungen
	uint8_t g_keys;				// Taster
	uint8_t g_keystate;			// Taster Zustand: betätigt?
	struct timer g_wait_timer;	// Wait Timer 16 ms Auflösung: Zeit nach Start nachdem Servos eingeschaltert werden
	struct timer g_switch_timer;// Schalter Abfragetimer 16 ms Auflösung: In welchem Interval werden die Rückmelder und Taster abgefragt, ~80ms
	uint8_t g_servos_enabled;	// sind die Servos eingeschaltet?
	struct timer g_led_timer;	// LED Timer 16 ms Auflösung: Blinkrate für die Anzeige ws_LED_TIMER == ~500 ms
	uint8_t g_led_toggle;		// wenn im manual mode, wird für das Anzeigen des selektierten Servos verwendet
	uint8_t g_keys_t;			// zeitversetzte alte Wert für g_keys, wird in debounce_keys() verwendet
	uint8_t g_switches_t;		// zeitversetzte alte Wert für g_switches, wird in debounce_keys() verwendet
	uint8_t g_move_sema;		// Bewegungssemaphore, wird hochgezählt bis 2
	uint8_t g_manual_mode;		// Manueller Modus aktiv
};

struct ws_v_t ws_v = { 0 };  // Speicher für g_ Variablen

// TODO: pro Modul ein eigener Bereich
// Struktur im EEPROM
struct wsServoEeprom_t {
	// 16 bytes
    uint16_t minv; // Wert für Min Ausschlag in Prozent * 10
    uint16_t maxv; // Wert für Max Ausschlag in Prozent * 10
    uint16_t movetime; // Bewegungszeit in us
    uint8_t  reserved[10];  // Reserve
};

struct wsEeprom_t {
	// pro Kanal eine struct wsServoEeprom_t
	struct wsServoEeprom_t servo[ws_CHANNEL_COUNT];
};
struct wsEeprom_t ws_eeprom EEMEM;

// Taster
#define ws_KEY_SEL		0x01	// SEL
#define ws_KEY_ACTION	0x02	// AKTION
#define ws_KEY_UP		0x04	// UP
#define ws_KEY_DOWN		0x08	// DOWN	

/* int16_t ws_check_servo_minmax(uint16_t v, uint16_t vmin, uint16_t vmax)
Überprüfung der Werte für minimaler und maximaler Serveausschlag.
	v		zu überprüfender Wert
	vmin	Minimalwert
	vmax	Maximalwert
*/
static inline uint16_t ws_check_servo_minmax(uint16_t v, uint16_t vmin, uint16_t vmax) {
    if (v > vmax)
        return vmax;
    if (v < vmin)
        return vmin;
    return v;
}

/* uint16_t ws_servo_time_to_percent(uint16_t t)
	t	Zeitwert in Timerticks -> Prozent 2 ~0% .. 1000 ~100%
*/
uint16_t ws_servo_time_to_percent(uint16_t t) {
    if (t < ws_SERVO_TIMER_PULS_MIN)
        t = ws_SERVO_TIMER_PULS_MIN; // 500 Timertick
    else if (t > ws_SERVO_TIMER_PULS_MAX)
        t = ws_SERVO_TIMER_PULS_MAX; // 1000 Timerticks
    return (t - ws_SERVO_TIMER_PULS_MIN) * (1000 / (ws_SERVO_TIMER_PULS_MAX - ws_SERVO_TIMER_PULS_MIN));
	//    bei t=500: t-500=0*(1000/(1000-500))=2
	//    bei t=1000: t-500=500*(1000/(1000-500))=1000
}
/* uint16_t ws_percent_to_servo_time(uint16_t t)
	 t	Prozent 0..1000 -> Timerticks
*/
uint16_t ws_percent_to_servo_time(uint16_t t) {
    if (t > 1000)
        t = 1000;
    return t / (1000/(ws_SERVO_TIMER_PULS_MAX - ws_SERVO_TIMER_PULS_MIN)) + ws_SERVO_TIMER_PULS_MIN;
}

/* void ws_read_switches(void)
Tastenprellen unterdrücken, wenn sich der zustand während er letzten 2 Aufrufe nicht geändert hat
*/
static void ws_read_switches(void) {
    uint8_t row_k, row_s;
    
	// PORTA Taster
    row_k = ~(port_in(PORTA)) & 0x0f; // gedrückt ist es ein Low Wert!

	// PORTA Rückmelde Multiplexer PA4 (lowest Bit), PA5, PA6 
    row_s = 0;
    for (int8_t i = 7; i >= 0; i--) {
		// Kanal i
		// i um 4 Bits nach links shiften->Selektieren von Kanal
        port_out(PORTA) = (port_out(PORTA) & 0x70)|(i<<4);
		// bisheriges Ergebnis um eins nach links
        row_s <<= 1;
		// bischen warten
        _NOP();
        _NOP();
        _NOP();
		// Bit PA7 ist der Rückmeldewert für den Kanal
        if (port_in(PORTA) & 0x80) {
            row_s |= 1;
        }
    }
    row_s = ~row_s;
    
    debounce_keys(&ws_v.g_keys, &ws_v.g_keys_t, row_k);  // entprellen Tastendrücke
    debounce_keys(&ws_v.g_switches, &ws_v.g_switches_t, row_s); // entprellen Rückmelder
}

/* uint16_t ws_get_eeprom_word(uint16_t* p, uint16_t dflt)
	p		Zeiger auf EEPROM Bereich der zu lesen ist
	dflt	Deaultwert wenn Bereich nicht beschrieben ist
*/
static inline uint16_t ws_get_eeprom_word(uint16_t* p, uint16_t dflt) {
    uint16_t v = e2prom_get_word(p);
    if (v == 0xffff) // not programmed
        v = dflt;
    return v;
}

/* void ws_set_servo_deltatime(struct ws_Servo* s)
Servo deltatime, die Zeit für einen Servoschritt abhängig von ws_Servo.movetime, immer >= 1!
	s	Zeiger auf den Servo, Struct ws_Servo
*/
static void ws_set_servo_deltatime(struct ws_Servo* s) {
    s->deltatime = (s->maxtime - s->mintime) * (100 / ws_SERVO_TIMER_CHANNEL_FREQ_HZ) / s->movetime;
    if (s->deltatime == 0) {
        s->deltatime = 1;
    }
}

/* static NOINLINE void ws_init_servos(void)
Servo's Init.
*/
static NOINLINE void ws_init_servos(void) {
    uint8_t i, switchmask;
    struct ws_Servo* s;
    
    // read current switches. 2 calls needed because of key debounce.
	// Lese die aktuellen Rückmelder, 2mal wegen Entprellung.
    ws_read_switches();
    ws_read_switches();
    // now we have the current switches state in ws_v.g_switches
    
    ws_v.g_servo_curpos = 0;
    ws_v.g_servo_set = ws_v.g_switches;
    ws_v.g_switches_old = ws_v.g_switches;
	
	// für jeden der 8 Servos
    for (i = 0, switchmask = 0x01, s = ws_v.g_servos; s != (ws_v.g_servos+ws_CHANNEL_COUNT); i++, switchmask <<= 1, s++) {
        uint16_t vmin, vmax, mtime;
		// lese aus EEPROM: Minimalwert
        vmin = ws_get_eeprom_word(&ws_eeprom.servo[i].minv, 400);
		// lese aus EEPROM: Maximalwert
        vmax = ws_get_eeprom_word(&ws_eeprom.servo[i].maxv, 600);
		// lese aus EEPROM: Bewegungszeit in us
        mtime = ws_get_eeprom_word(&ws_eeprom.servo[i].movetime, 100);
        
		// prüfe Bewegungszeit x: 20 < x < 1000 
        mtime = ws_check_servo_minmax(mtime, 20, 1000);
        
		// Prozentwerte in Timerticks umrechnen
        vmin = ws_percent_to_servo_time(vmin);
        vmax = ws_percent_to_servo_time(vmax);
        
		// prüfe min und max werte
        vmin = ws_check_servo_minmax(vmin, ws_SERVO_TIMER_PULS_MIN, ws_SERVO_TIMER_PULS_MAX);
        vmax = ws_check_servo_minmax(vmax, vmin, ws_SERVO_TIMER_PULS_MAX);
		
		// Minimale Zeit für den Servopuls in Timerticks
        s->mintime = vmin;
		// Maximale Zeit für den Servopuls in Timerticks
        s->maxtime = vmax;
		// Bewegungszeit für diesen Servo in us
        s->movetime = mtime;
		// und wieder in Prozent umrechnen
        s->perc_minv = ws_servo_time_to_percent(vmin);
        s->perc_maxv = ws_servo_time_to_percent(vmax);
        
		// Servostatus: 0:am Anfang
        s->state.curpos = 0;
		// bewegt sich der servo gerade?
        s->moving = 0;
		// ist Minimal Wert geändert worden? wenn ja, in das EEPROM schreiben
        s->state.minchanged = 0;
		// ist Maximal Wert geändert worden? wenn ja, in das EEPROM schreiben
        s->state.maxchanged = 0;
		// ist bewegungszeit geändert worden? wenn ja, in das EEPROM schreiben
        s->state.movtchanged = 0;
        // abhaengig von Servoposition-Rueckmeldung Startposition setzen und anfahren
        s->state.dstpos = 0;
        s->state.domove = 1;
		// abhängig von der Rückmeldung Position setzen
        if (ws_v.g_switches & switchmask) {
            s->state.dstpos = 1;
            s->curtime = s->maxtime;
        } else {
            s->curtime = s->mintime;
        }
        // war die SBOXNET_CMD_FB_CHANGED schon bestätigt?
        s->notack = 0;
		// Timer in welcher Zeit die Rückmeldung wieder verschickt wird solange nicht bestätigt
        s->retry_timer = 0;
		// letzte Sequencenr der SBOXNET_CMD_FB_CHANGED Nachricht
        s->last_seq = 0;
        
		// Servo deltatime, die Zeit für einen Servoschritt abhängig von ws_Servo.movetime, immer >= 1!
        ws_set_servo_deltatime(s);
    }
}

void ws_do_init_system(void) {
    // PD als LED Anzeige
    port_out(PORTD) = 0;				// erstmal aus
    port_dirout(PORTD, 0xff);			// alles OUTPUT
    PORTCFG_MPCMASK = 0xff;				// alle PD Pins
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc; // Output als Totem Pole
    
	// PC als Servo Ausgänge
    port_out(PORTC) = 0;				// erstmal aus
    port_dirout(PORTC, 0xff);			// PC0..7 Output
    PORTCFG_MPCMASK = 0xff;				// alle PC Pins
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc; // Output als Totem Pole
    
	// PA0  ws_KEY_SEL
	// PA1  ws_KEY_ACTION
	// PA2  ws_KEY_UP
	// PA3  ws_KEY_DOWN
	// PA4..PA6 Rückmeldung Multiplexer
	// PA7  ausgewählte Rückmeldung 
    port_dirin(PORTA, 0x8f);			// PA0..PA3,PA7 Eingang
	port_dirout(PORTA, 0x70);			// PA4..PA6 Ausgang
    PORTCFG_MPCMASK = 0x8f;				// PA0..PA3,PA7 Pull Up
    PORTA.PIN0CTRL = PORT_OPC_PULLUP_gc;
	PORTCFG_MPCMASK = 0x70;				// PA4..PA6 Totem Pole
	PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    // Konfigure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // Power Reduction
    PR.PRPA = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp); // PORTA DAC, ADC,  Analog Comparator
    PR.PRPB = Bit(PR_DAC_bp)|Bit(PR_ADC_bp)|Bit(PR_AC_bp); // PORTB DAC, ADC,  Analog Comparator
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PORTC TWI, USART1, USART0, SPI, HIRES
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PORTD TWI, USART1, USART0, SPI, HIRES

    // TCC0 Timer für Servos
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc; // Normal Mode
    TCC0.CTRLD = 0;
    TCC0.CTRLE = 0;
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc; // OVF Int Low Level
    TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc; // CCA Int Low Level
    TCC0.INTFLAGS = 0xff;               // Interrupt Flags löschen
    TCC0.PER = ws_SERVO_TIMER_TOP;		// Periode für 8 Servos, pro Kanal 50 Hz == 1250
    TCC0.CCA = ws_SERVO_TIMER_PULS_MIN; // Pulsweite für minimal Puls
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;	// Timer aktivieren

	// Wait Timer 16 ms Auflösung: Zeit nach Start nachdem Servos eingeschaltet werden
    timer_register(&ws_v.g_wait_timer, TIMER_RESOLUTION_16MS);
	// LED Timer 16 ms Auflösung: Blinkrate für die Anzeige ws_LED_TIMER == ~500 ms
    timer_register(&ws_v.g_led_timer,  TIMER_RESOLUTION_16MS);
	// Schalter Abfragetimer 16 ms Auflösung: In welchem Interval werden die Rückmelder und Taster abgefragt, ~80ms
    timer_register(&ws_v.g_switch_timer, TIMER_RESOLUTION_16MS);
    
    ws_v.pselservo = &ws_v.g_servos[ws_v.g_selservoid];

    ws_init_servos();

	g_com.productid = ws_PRODUCT_ID;
    g_com.vendorid = ws_VENDOR_ID;
    g_com.firmware_version = ws_FIRMWARE_VERSION;
    g_com.capabilities = CAP_CNTRL_TURNOUT;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(ws_DEVICE_DESC);
}

static NOINLINE struct ws_Servo* ws_get_channel(uint8_t ch) {
	return &ws_v.g_servos[ch];
}

static inline void ws_set_channels_off(void) {
    port_out(PORTC) = 0xff;
}

// every 2.5ms
ISR(TCC0_OVF_vect) {
	if (!ws_v.g_servos_enabled) {
        return;
    }
    
    register struct ws_Servo* s = ws_get_channel(ws_v.g_curchannel);
    
    if (s->moving == 0 && s->state.domove && ws_v.g_move_sema < 2) {
        ws_v.g_move_sema++;
        s->moving = 15;
        s->state.curpos = s->state.dstpos;
        s->state.domove = 0;
    }

    uint8_t mask = bitmask(ws_v.g_curchannel);
    
    ws_set_channels_off();
    
    if (s->moving) {
        port_clr(PORTC, mask);

        uint16_t a,b;
        b = s->state.curpos ? s->maxtime : s->mintime;
        if (s->state.curpos) {
            a = s->curtime + s->deltatime;
            if (a >= b) {
                ws_v.g_servo_curpos |= mask;
                goto pos_reached;
            }
        } else {
            a = s->curtime - s->deltatime;
            if ((int16_t)a <= (int16_t)b) {
                ws_v.g_servo_curpos &= ~mask;
pos_reached:
                a = b;
                s->moving--;
            }
        }
        s->curtime = a;
        if (s->moving == 0) {
            ws_v.g_move_sema--;
        }
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            TCC0.CCA = a;
        }
    }
    ws_v.g_curchannel = (ws_v.g_curchannel + 1) & ws_CHANNEL_MASK;
}

ISR(TCC0_CCA_vect) {
    if (ws_v.g_servos_enabled) {
        ws_set_channels_off();
    }
}

static void ws_init_sel_leds(void) {
    port_out(PORTD) = bitmask(ws_v.g_selservoid);
    timer_set(&ws_v.g_led_timer, ws_LED_TIMER);
}

static NOINLINE uint8_t ws_check_key_sel(uint8_t keys) {
	if (keys & ws_KEY_SEL) {
		if (!(ws_v.g_keystate & ws_KEY_SEL)) {
		
            if (!ws_v.g_manual_mode) {
                ws_v.g_manual_mode = 1;
                ws_init_sel_leds();
                ws_v.g_keystate |= ws_KEY_SEL;
                
            } else {
                ws_v.g_keystate |= ws_KEY_SEL;
                
                if (keys & ws_KEY_DOWN) {
                    ws_v.g_manual_mode = 0;
                } else {
                    ws_v.g_selservoid = (ws_v.g_selservoid + 1) & ws_CHANNEL_MASK;
                    ws_v.pselservo = &ws_v.g_servos[ws_v.g_selservoid];
                    
                    ws_init_sel_leds();
                }
            }
		}
		return 1;
	} else {
		ws_v.g_keystate &= ~ws_KEY_SEL;
	}
	return 0;
}

static NOINLINE void ws_check_key_action(uint8_t keys) {
    if (!ws_v.g_manual_mode)
        return;
    
	if (keys & ws_KEY_ACTION) {
		if (!(ws_v.g_keystate & ws_KEY_ACTION)) {
			ws_v.g_keystate |= ws_KEY_ACTION;
			
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                struct ws_Servo* servo = ws_v.pselservo;
                uint8_t updown = (keys & (ws_KEY_UP|ws_KEY_DOWN));
                if (updown) {
                    if (servo->moving == 0) {
                        // set min/max
                        uint8_t ismax = (servo->state.curpos ? 1:0);
                        uint16_t vmin, vmax, v;
                        if (ismax) {
                            vmin = servo->mintime;
                            vmax = ws_SERVO_TIMER_PULS_MAX;
                            servo->state.maxchanged = 1;
                            v = servo->maxtime;
                        } else {
                            vmin = ws_SERVO_TIMER_PULS_MIN;
                            vmax = servo->maxtime;
                            servo->state.minchanged = 1;
                            v = servo->mintime;
                        }
                        v = ws_check_servo_minmax(v + ((keys & ws_KEY_UP) ? ws_SERVO_TIMER_PULS_STEP : -ws_SERVO_TIMER_PULS_STEP),
                                                vmin, vmax);
                        if (ismax) {
                            servo->maxtime = v;
                        } else {
                            servo->mintime = v;
                        }
                        servo->perc_minv = ws_servo_time_to_percent(servo->mintime);
                        servo->perc_maxv = ws_servo_time_to_percent(servo->maxtime);                    
                        servo->curtime = v;
                        servo->state.domove = 1;
                        servo->state.dstpos = servo->state.curpos;
                        ws_set_servo_deltatime(servo);
                    }
                } else {
                    servo->state.domove = 1;
                    servo->state.dstpos = !servo->state.dstpos;
                }
            }
		}
	} else {
		ws_v.g_keystate &= ~ws_KEY_ACTION;
	}
}

static NOINLINE void ws_check_input(void) {
	uint8_t keys = ws_v.g_keys;
	if (!ws_check_key_sel(keys)) {
		ws_check_key_action(keys);
	}
}


static NOINLINE void ws_show_status(void) {
    if (ws_v.g_manual_mode) {
        if (timer_timedout(&ws_v.g_led_timer)) {
            port_out(PORTD) ^= bitmask(ws_v.g_selservoid);
            timer_set(&ws_v.g_led_timer, ws_LED_TIMER);
        }
    } else {
        uint8_t d = ws_v.g_switches;
        uint8_t mask = ws_v.g_switches ^ ws_v.g_servo_set;
        if (mask) {
            d = (ws_v.g_led_toggle ? d | mask : d & ~mask);
            if (timer_timedout(&ws_v.g_led_timer)) {
                ws_v.g_led_toggle = !ws_v.g_led_toggle;
                timer_set(&ws_v.g_led_timer, ws_LED_TIMER);
            }
        } else {
            ws_v.g_led_toggle = 0;
        }
        port_out(PORTD) = d;
    }
}

uint8_t ws_do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_FB_NUM:      *pdata = 8; return 0;
        case R_FB_VALUE0:   *pdata = ws_v.g_switches; return 0;
        
        case R_CNTRL_NUM:      *pdata = 8; return 0;
        case R_CNTRL_VALUE0:   *pdata = ws_v.g_servo_set; return 0;
        case R_CNTRL_PENDING0: *pdata = ws_v.g_servo_set ^ ws_v.g_servo_curpos; return 0;
    }
    
    if (reg >= R_TURNOUT_PAR_0 && reg < R_TURNOUT_PAR_8) {
        uint8_t roffs = reg & NUM_TURNOUT_PARS_MASK;
        uint8_t servonum = (reg-R_TURNOUT_PAR_0) / NUM_TURNOUT_PARS;
        switch (roffs) {
            case ROFFS_TURNOUT_PAR_MINV: {
                *pdata = ws_v.g_servos[servonum].perc_minv;
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MAXV: {
                *pdata = ws_v.g_servos[servonum].perc_maxv;
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MOVETIME: {
                *pdata = ws_v.g_servos[servonum].movetime;
                return 0;
            }
            default: {
                break;
            }
        }
    }
    
    return SBOXNET_ACKRC_REG_INVALID;
};

uint8_t ws_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_CNTRL_VALUE0: {
            ws_v.g_servo_set = (ws_v.g_servo_set & ~mask) | (data & mask);
            for (uint8_t i = 0; i < ws_CHANNEL_COUNT; i++) {
                if (mask & 0x01) {
                    struct ws_Servo* s = ws_get_channel(i);
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
        struct ws_Servo *servo = ws_v.g_servos + servonum;
        switch (roffs) {
            case ROFFS_TURNOUT_PAR_MINV: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->perc_minv = ws_check_servo_minmax(data, 0, servo->perc_maxv);
                    servo->mintime = ws_percent_to_servo_time(servo->perc_minv);
                    ws_set_servo_deltatime(servo);
                    servo->state.minchanged = 1;
                }
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MAXV: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->perc_maxv = ws_check_servo_minmax(data, servo->perc_minv, 1000);
                    servo->maxtime = ws_percent_to_servo_time(servo->perc_maxv);
                    ws_set_servo_deltatime(servo);
                    servo->state.maxchanged = 1;
                }
                return 0;
            }
            case ROFFS_TURNOUT_PAR_MOVETIME: {
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                    servo->movetime = ws_check_servo_minmax(data, 20, 1000);
                    ws_set_servo_deltatime(servo);
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

uint8_t ws_get_next_transmit_seq(void) {
    uint8_t seq = ws_v.g_transmit_seq + 1;
    if (seq < 10) {
        seq = 10;
    }
    
    for (uint8_t i = 0; i < ws_CHANNEL_COUNT; i++) {
        if (ws_v.g_servos[i].notack && seq == ws_v.g_servos[i].last_seq) {
            seq++;
            i = 0; // retry
        }   
    }

    ws_v.g_transmit_seq = seq;
    return seq;
}

uint8_t ws_do_msg(struct sboxnet_msg_header *pmsg) {
    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }
    switch (pmsg->cmd) {
        case SBOXNET_CMD_FB_CHANGED|0x80: {
            if (pmsg->opt.len != 0) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            for (uint8_t i = 0; i < ws_CHANNEL_COUNT; i++) {
                if (ws_v.g_servos[i].notack && pmsg->seq == ws_v.g_servos[i].last_seq) {
                    ws_v.g_servos[i].notack = 0;
                    ws_v.g_servos[i].last_seq = 0;
                }
            }
            return SBOXNET_ACKRC_SEND_NO_ANSWER;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

void ws_do_setup(void) {
    timer_set(&ws_v.g_wait_timer, 7); // ~100 ms
    timer_set(&ws_v.g_switch_timer, ws_SWITCH_TIMER); // 80ms
}

void ws_do_main(void) {
    if (timer_timedout(&ws_v.g_switch_timer)) {
        timer_set(&ws_v.g_switch_timer, ws_SWITCH_TIMER);
        ws_read_switches();
        
        if (ws_v.g_servos_enabled) {
            uint8_t diff = ws_v.g_switches ^ ws_v.g_switches_old;
            ws_v.g_switches_old = ws_v.g_switches;
            uint8_t mask = 1;
            for (uint8_t i = 0; i < ws_CHANNEL_COUNT; i++, mask <<= 1) {
                struct ws_Servo* pservo = &ws_v.g_servos[i];
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
    
    ws_check_input();
    ws_show_status();
    
    if (ws_v.g_servos_enabled == 0 && timer_timedout(&ws_v.g_wait_timer)) {
        ws_v.g_servos_enabled = 1;
    }
    
    if (!ws_v.g_manual_mode) {
        for (uint8_t i = 0; i < ws_CHANNEL_COUNT; i++) {
            struct ws_Servo *servo = ws_v.g_servos + i;
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                if (servo->state.maxchanged) {
                    if (eeprom_is_ready()) {    // note: eeprom erase+write: ca. 4ms !
                        servo->state.maxchanged = 0;
                        eeprom_update_word(&ws_eeprom.servo[i].maxv, servo->perc_maxv);// burns only if new value is different from old one.
                    }
                } else if (servo->state.minchanged) {
                    if (eeprom_is_ready()) {
                        servo->state.minchanged = 0;
                        eeprom_update_word(&ws_eeprom.servo[i].minv, servo->perc_minv);
                    }
                } else if (servo->state.movtchanged) {
                    if (eeprom_is_ready()) {
                        servo->state.movtchanged = 0;
                        eeprom_update_word(&ws_eeprom.servo[i].movetime, servo->movetime);
                    }
                }
            }
        }
    }
    
    if (bit_is_clear(g_dev_state, DEV_STATE_FLG_WATCHDOG_b)) {
        uint8_t switches = ws_v.g_switches;
        for (uint8_t i = 0, mask = 1; i < ws_CHANNEL_COUNT; i++, mask <<= 1) {
            if (ws_v.g_servos[i].retry_timer == 0 && ws_v.g_servos[i].notack) {
                ws_v.g_servos[i].retry_timer = 50;
                
                struct sboxnet_msg_max msg;
                msg.msgh.dstaddr = 0;
                msg.msgh.seq = ws_v.g_servos[i].last_seq = ws_get_next_transmit_seq();
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

void ws_do_before_bldr_activate(void) {    
    port_out(PORTC) = 0xff;
    port_out(PORTD) = 0;
}

