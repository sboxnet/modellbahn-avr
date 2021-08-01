/***************************************************************************
 *   Copyright (C) 2014-2015
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

#define USE_BOOSTER 1

#include "common.h"

/*
 * - DCC Cutout is generated only at the end of the packet
 * - At the end of a cutout, shortcut detection may be triggered once or two times.
 * - Current/shortcut detection: MAX_SHORTCUT_CNT shortcuts must have occured in TIMER_SHORT_CUT time
 * - Shortcut detection is disabled in the TIMER_STARTUP time after DCC startup
 * - Shortcut detection is disabled in the first half of the first DCC bit after a cutout
 * PC0 IN1
 * PC1 IN2
 * PC2 EN
 * PC3 RC (OVERFLOW) für Kurzschlusserkennung und zu hoher Strom,
 *   ein Kurzschluss wird erkannt wenn in einen bestimmten Zeitintervall shortcut_interval (default: BOOSTER_DEFAULT_SHORTCUT_INTERVAL, 70ms)
 *   die Anzahl der Kürzschlüsse >= shortcut_limit (default: BOOSTER_DEFAULT_SHORTCUT_LIMIT, 4300) ist.
 * PC4 DCC IN
 * PC5 Notaus
 */


/*
 * TCC0
 * - timer not running
 * - CCA: AWEXC for booster IN1/IN2. PC0, PC1.
 * - CCB: AWEXC for booster EN. PC2 and PC3 wired or.
 * 
 * TCD0
 * - general timer 100 Hz, normal mode, 16bit
 * - CCA: timer
 * - CCB: booster shortcut detector
 * - CCC: DCC decoder, cutout timer
 * 
 * TCD1 aus common.c
 * - Timer 1khz 
 */

#define VENDOR_ID    0x9999
#define FIRMWARE_VERSION 0x0200
#define PRODUCT_ID   0x0003
#define DEVICE_DESC  "Booster:2"

APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)

#define TIMER_PRESCALER   64
#define TIMER_FREQ_HZ     100
#define TIMER_PERIOD      (F_CPU / (TIMER_PRESCALER * TIMER_FREQ_HZ))  // 5000

#define LED_PORT  PORTD

//#define NOTAUS_PORT PORTC
//#define NOTAUS_b    5

static struct timer g_timer_10ms;

struct booster_eeprom {
    uint16_t   shortcut_limit; // offset 0
    uint16_t   shortcut_interval; // offset 2
};

struct Eeprom {
    struct booster_eeprom booster;
    uint8_t reserved[128-sizeof(struct booster_eeprom)];
};
struct Eeprom g_eeprom EEMEM;
// ab 0x1000

// LED Port
#define LED_PORT  PORTD
// Bits für LEDs
#define LED_CUR_OV_b      0 // Strom zu hoch
#define LED_CUR_SHORT_b   1 // Kurzschluss
#define LED_NOTAUS_b      2 // Notaus
#define LED_BO_ON_b       3 // Booster EIN

// Port für Strombrücke
#define DCCM_PORT  PORTC
// Bits für Strombrücke
#define DCCM_IN1_b  0 // IN1
#define DCCM_IN2_b  1 // IN2
#define DCCM_EN_b   2 // Enable
#define DCCM_OV_b   3 // OV
#define DCCM_DCCIN_b  4 // DCC IN
#define DCCM_NOTAUS_b 5 // NOTAUS

#define CUR_OV_PORT  DCCM_PORT
#define CUR_OV_b     DCCM_OV_b

//#define DCC_CUTOUT_TEST_PORT  PORTC
//#define DCC_CUTOUT_TEST_b   6

#define TIMER_STARTUP  62 // ~1 s

#define BOOSTER_FLAG_ON_b      0
#define BOOSTER_FLAG_NOTAUS_b  1
#define BOOSTER_FLAG_CUR_OV_b  2

//#define DCC_WATCHDOG_VAL 4 // 4*16ms = 64ms

#define BOOSTER_DEFAULT_SHORTCUT_INTERVAL  35000 // 70ms
#define BOOSTER_DEFAULT_SHORTCUT_LIMIT     4300

struct booster {
    uint8_t        flags;
    struct timer   timer_startup;
    //struct timer   timer_dcc_watchdog;
    uint16_t       advals[2];
    uint16_t       shortcut_cnt;
    uint16_t       shortcut_limit;
    uint16_t       shortcut_interval;
    uint16_t       shortcut_nummax;
    struct {
        unsigned write_shortcut_limit:1;
        unsigned write_shortcut_interval:1;
    }              eeprom_flags;
};
struct booster g_booster = { 0, };
// DCC IN Port
#define DCC_IN_PORT PORTC
// DCC IN Bit im DCC_IN_PORT
#define DCC_IN_b   4


#define LED_DCC_ON_b      6
#define LED_DCC_NOTAUS_b  7

#define DCCGEN_FLAG_ON_b      0
#define DCCGEN_FLAG_NOTAUS_b  1

#define SWITCH_NOTAUS_b  0

#define DCC_PORT        DCCM_PORT
#define DCC_PORT_BIT    DCCM_DCCIN_b


//#define DCC_1  ((58/2)-1)
//#define DCC_0  ((116/2)-1)

#include "dccdec.c"

/* forward declaration
 */
// DCC Decoder
static void do_dec_parse_packet(void);

// LED Utility
static void led_set(uint8_t bit, uint8_t on);
// EEPROM Utility
static uint8_t read_production_signature_row(uint8_t offset);

// Booster Sensoren
static void booster_sensors_off(void);
static void booster_sensors_shortcut_off(void);

static void booster_init(void);
void booster_sensors_init(void);
void booster_power_off_all(void);
void booster_power_on_track(void);

// Geräte Callbacks
void do_init_system(void);
uint8_t do_msg(struct sboxnet_msg_header *pmsg);

/* static void do_dec_parse_packet(void)
 *
 * Aktion die bei DCC Paketende ausgeführt werden soll.
 */
static void do_dec_parse_packet(void) {
    //timer_set(&g_booster.timer_dcc_watchdog, DCC_WATCHDOG_VAL);
    
    // ist es ein Broadcast/Reset oder Idle Paket 
    if (g_dccdec.buf[0] != 0 // kein broadcast or reset
            && g_dccdec.buf[0] != 0xff) { // kein idle
        // produce cutout with TCD0.CCC
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            TCD0.CCC = TCD0.CNT + (28/2); // + 14
        }
        TCD0.INTFLAGS = Bit(TC0_CCCIF_bp);
        TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_LO_gc;
        g_dccdec.cutout = 1;
        //port_setbit(DCC_CUTOUT_TEST_PORT, DCC_CUTOUT_TEST_b); // cutout test point
    }
}

/* static void led_set(uint8_t bit, uint8_t on)
 *  bit - LED Bit on LED_PORT
 *  on  - LED Ein !=0 oder Aus ==0
 *
 * LED ein oder ausschalten
 */
static void led_set(uint8_t bit, uint8_t on) {
    // soll LED eingeschaltet werden?
    if (on) {
        // ja
        port_clrbit(LED_PORT, bit);
    } else {
        // nein
        port_setbit(LED_PORT, bit);
    }
}

/* static uint8_t read_production_signature_row(uint8_t offset)
 *  offset - der Offset
 *
 * Von der Production Signature Row lesen aus dem NVM, offset ist der Offset innerhalb von struct NVM_PROD_SIGNATURES_struct.
 */
static uint8_t read_production_signature_row(uint8_t offset) {
    uint8_t value;
    // NVM Kommando: von Calibration Zeile lesen 
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    // Wert holen
    value = pgm_read_byte(offset);
    // Kommando auf NOP
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    return value;
}

/* void booster_sensors_init(void)
 *
 * Booster Sensoren init.
 */
void booster_sensors_init(void) {
    // LEDs aus: NOTAUS, Kurzschluss
    port_set(LED_PORT, Bit(LED_NOTAUS_b)|Bit(LED_CUR_SHORT_b));
    // Flags für Strom zu hoch zurücksetzen
    clrbit(g_booster.flags, BOOSTER_FLAG_CUR_OV_b);

    // L6206 Stromüberwachung: verwende PortC Interrupt, Level 0 Int
    PORTC.INT0MASK = Bit(CUR_OV_b);
    // ggf Interrupt Flags zurücksetzen
    PORTC.INTFLAGS = Bit(PORT_INT0IF_bp); // clear interrupt flags;
    // PC Level 0 Interrupt aus
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    // Interrupt on Falling Edge, PullUp ein
    PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_FALLING_gc;
    
    // Kurzschluss Zähler auf 0
    g_booster.shortcut_cnt = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Zeitpunkt für  nächste Kurzschlussprüfung, default ist 
        // BOOSTER_DEFAULT_SHORTCUT_INTERVAL (~70ms) wird im EEPROM abgelegt.
        // über Register R_BOOSTER_SHORTCUT_LIMIT (201) gelesen/geschrieben werden.
        TCD0.CCB = TCD0.CNT + g_booster.shortcut_interval;
/*R_BOOSTER_SHORTCUT_LIMIT = 201,
 *R_BOOSTER_SHORTCUT_INTERVAL = 202,
 *R_BOOSTER_SHORTCUT_CNT = 203
 */
        // Intflags ggf löschen
        TCD0.INTFLAGS = Bit(TC0_CCBIF_bp);
        // TCD0 CCB auf LO Level Interrupt.
        TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCBINTLVL_gm) | TC_CCBINTLVL_LO_gc;
    }
}

/* static void booster_sensors_off(void)
 *
 * Booster Sensoren aus.
 */
static void booster_sensors_off(void) {
    // Port Sensor aus: PC3 
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    PORTC.INT0MASK = 0; // L6206 Stromüberwachung
    // lösche TCD0.CCB Interrupt.
    TCD0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
    
    // Kurzschlusszähler auf 0.
    g_booster.shortcut_cnt = 0;
}

/* static void booster_sensors_shortcut_off(void)
 *
 * Kurzschlussüberwachung aus.
 */
static void booster_sensors_shortcut_off(void) {
    // PortC Level 0 Interrupt aus.
    PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
}

/* void booster_power_off_all(void)
 *
 * Booster AUS.
 */
void booster_power_off_all(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Sensoren AUS
        booster_sensors_off();

        // Strombrücke AUS
        //port_clr(DCCM_PORT, Bit(DCCM_EN1_b)|Bit(DCCM_EN2_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
        port_clr(DCCM_PORT, Bit(DCCM_EN_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
        //AWEXC.OUTOVEN = 0;

        // Kurzschluss Sensor AUS
        booster_sensors_shortcut_off();
        
        // PC LEVEL 1 Interrupt AUS == DCC_IN
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_OFF_gc;
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp)|Bit(PORT_INT0IF_bp);

        // TCD0 CCB Interrupt level AUS
        TCD0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
        // Startup Timer Init
        timer_set(&g_booster.timer_startup, 0xff);
        
        // Decoder Stop
        dec_stop();
        
        // Ein LED aus.
        led_set(LED_BO_ON_b, 0);
    }
}

void booster_power_on_track(void) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        PORTC.INT1MASK = Bit(DCC_IN_b);
        PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_HI_gc;
        
        booster_sensors_init();
        
        timer_set(&g_booster.timer_startup, TIMER_STARTUP);
        //timer_set(&g_booster.timer_dcc_watchdog, DCC_WATCHDOG_VAL);

        dec_start();
        
        uint8_t v = 0;
        if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
            v = Bit(TC0_CMPA_bp)|Bit(TC0_CMPB_bp);
        }
        TCC0.CTRLC = v;
        AWEXC.OUTOVEN = Bit(3)|Bit(2)|Bit(1)|Bit(0);
    }
}

static void booster_init(void) {
    // Startup Timer mit 16ms Auslösung
    timer_register(&g_booster.timer_startup, TIMER_RESOLUTION_16MS);
    
    //timer_register(&g_booster.timer_dcc_watchdog, TIMER_RESOLUTION_16MS);
    
    // DCC Decoder Init: portc pin 4 = DCC_IN
    dec_init(EVSYS_CHMUX_PORTC_PIN4_gc);
    
    // Booster Flags löschen
    g_booster.flags = 0;
    // Init Kurzschluss Grenze aus dem EEPROM
    g_booster.shortcut_limit = e2prom_get_word(&g_eeprom.booster.shortcut_limit);
    // Falls noch nicht gesetzt nehme BOOSTER_DEFAULT_SHORTCUT_LIMIT=4300
    if (g_booster.shortcut_limit == 0xffff) {
        g_booster.shortcut_limit = BOOSTER_DEFAULT_SHORTCUT_LIMIT;
    }
    // Init Kurzschluss Intervall aus dem EEPROM
    g_booster.shortcut_interval = e2prom_get_word(&g_eeprom.booster.shortcut_interval);
    // falls noch nicht gesetzt nehme BOOSTER_DEFAULT_SHORTCUT_INTERVAL=35000 ~70ms
    if (g_booster.shortcut_interval == 0xffff) {
        g_booster.shortcut_interval = BOOSTER_DEFAULT_SHORTCUT_INTERVAL;
    }
    // Maximale bisherige Anzahl der Kurzschlüsse
    g_booster.shortcut_nummax = 0;
    // soll Kurzschluss Grenze in das EEPROM geschrieben werden in do_main()
    g_booster.eeprom_flags.write_shortcut_limit = 0;
    // soll Kurzschluss Intervall in das EEPROM geschrieben werden in do_main()
    g_booster.eeprom_flags.write_shortcut_interval = 0;
    // Booster AUS
    booster_power_off_all();
}

/* void init_leds(void)
 *
 * LED Init. Ports auf Ausgabe und Totempole.
 */
void init_leds_ports(void) {
    // LED_PORT auf LEDs off
    port_out(LED_PORT) = Bit(LED_CUR_OV_b)|Bit(LED_CUR_SHORT_b)|Bit(LED_NOTAUS_b)|Bit(LED_BO_ON_b);
    // LED_PORT auf Ausgabe
    port_dirout(LED_PORT, Bit(7)|Bit(LED_CUR_OV_b)|Bit(LED_CUR_SHORT_b)|Bit(LED_NOTAUS_b)|Bit(LED_BO_ON_b));
    // LED_PORTs auf totem pole
    PORTCFG_MPCMASK = Bit(7)|Bit(LED_CUR_OV_b)|Bit(LED_CUR_SHORT_b)|Bit(LED_NOTAUS_b)|Bit(LED_BO_ON_b); // PD2..0
    LED_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc;
}

/* void init_dccm(void)
 *
 * Ports für Strombrücke.
 */
void init_dccm_ports(void) {
    // DCCM_PORT
    // alles OFF vorbereiten
    port_out(DCCM_PORT) = 0;
    // PC0 (IN1), PC1(IN2), PC2 (EN) als Ausgabe
    port_dirout(DCCM_PORT, /*Bit(DCCM_DCCOUT_b)|*/Bit(DCCM_EN_b)|Bit(DCCM_IN2_b)|Bit(DCCM_IN1_b));
    // PC3(OV), PC4 (DCCIN), PC5(NOTAUS) als Eingabe
    port_dirin(DCCM_PORT, Bit(DCCM_OV_b)|Bit(DCCM_DCCIN_b)|Bit(DCCM_NOTAUS_b));
    // Ausgabe ports als totem pole
    PORTCFG_MPCMASK = /*Bit(DCCM_DCCOUT_b)|*/Bit(DCCM_EN_b)|Bit(DCCM_IN2_b)|Bit(DCCM_IN1_b);
    DCCM_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc;
    // Eingabeports mit PullUps;
    PORTCFG_MPCMASK = Bit(DCCM_OV_b)|Bit(DCCM_DCCIN_b)|Bit(DCCM_NOTAUS_b);
    DCCM_PORT.PIN3CTRL = PORT_OPC_PULLUP_gc;
}

void init_adc_ports(void)  {
    // ADCA Kanäle als Eingabe
    port_dirin(PORTA, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    // mit Pulldown
    PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
}

void init_timers(void) {
    // timer D0 als Normal Timer für 100Hz, und Kurzschluss Timer + Shortcut Timer, und Cutout Timer
    //  CCA für 100Hz
    //  CCB Kurzschlusstimer
    //  CCC DCC Cutout Timer
    TCD0.CTRLB = TC_WGMODE_NORMAL_gc; // Normal Modus
    TCD0.CTRLC = 0; // kein CMPx
    TCD0.CTRLD = 0; // keine Events
    TCD0.CTRLE = 0; // kein Bytemode
    TCD0.INTCTRLA = 0; // kein ERR oder OVF Interrupt
    TCD0.INTCTRLB = TC_CCAINTLVL_LO_gc/*|TC_CCCINTLVL_LO_gc*/; // aber CCA Low Interrupt Level für 100Hz /*CCC als Cutout Timer*/
    TCD0.INTFLAGS = 0xff; // Lösche Interrupt Flags
    TCD0.PER = 0xffff; // Timer auf endlos
    TCD0.CCA = TCD0.CNT + TIMER_PERIOD; // CCA Periode für 100Hz einstellen
    //TCD0.CCC = TCD0.CNT;
    TCD0.CTRLA = TC_CLKSEL_DIV64_gc; // Timer starten mit 1:64
}

void init_adc(void) {
    // ADC: benutze mit Vorzeichen (unsigned mode may be broken) and 1V Interne Referenz
    // - ADC_CONMODE_bp mit Vorzeichen
    // - ADC_FREERUN_bp nein muss angestossen werden
    // - ADC_RESOLUTION_12BIT_gc 12-bit result, right adjusted
    // ADC: use signed mode (unsigned mode may be broken) and Vcc/1.6 reference (1V internal reference may be broken)
    //      see Atmel Xmega Errata
    ADCA.CTRLB = Bsv(ADC_CONMODE_bp,1)|Bsv(ADC_FREERUN_bp,0)|ADC_RESOLUTION_12BIT_gc;
    // Referenz Interne 1V
    ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
    // Keine Events
    ADCA.EVCTRL = 0;
    // ADC Prescaler auf 1:128
    ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc;
    // Lösche ggf. Int Flags
    ADCA.INTFLAGS = 0x0f;
    // ADCA Kalibrieren
    ADCA.CALL = read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    ADCA.CALH = read_production_signature_row(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    // ADCA einschschalten
    ADCA.CTRLA = Bit(ADC_ENABLE_bp);
    // ADC Channel 0: current
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN0_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 1: voltage
    ADCA.CH1.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN1_gc;
    ADCA.CH1.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // ADC Channel 2: current
    ADCA.CH0.CTRL = ADC_CH_GAIN_1X_gc|ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH0.INTCTRL = ADC_CH_INTMODE_COMPLETE_gc|ADC_CH_INTLVL_OFF_gc;
    // start ADCA conversions
    ADCA.CTRLA |= Bit(4)|Bit(3)|Bit(2);
}

void do_init_system(void) {
    init_leds_ports();
    init_dccm_ports();
    init_adc_ports();


    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    init_timers();
    init_adc();
   
    //AWEXC.CTRL = Bit(AWEX_DTICCBEN_bp)|Bit(AWEX_DTICCAEN_bp);
    //AWEXC.DTBOTH = F_CPU_MHZ * 3; // 3us
    //AWEXC.OUTOVEN = 0; // Bit(3)|Bit(2)|Bit(1)|Bit(0);
    
    // Setze ProductID, VendorID, ... im Common Bereich
    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    g_com.capabilities = CAP_DCC_BOOSTER;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC);

    timer_register(&g_timer_10ms, TIMER_RESOLUTION_1MS);
    
    booster_init();
}

/* uint8_t do_msg(struct sboxnet_msg_header *pmsg)
 *  process a message pmsg: pointer to a struct sboxnet_msg_header
 * return a SBOXNET_ACKRC_* retcode
 */
uint8_t do_msg(struct sboxnet_msg_header *pmsg) {

    if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
        return SBOXNET_ACKRC_CMD_UNKNOWN;
    }

    switch (pmsg->cmd) {
        case SBOXNET_CMD_LOCO_POWER: {
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
                if (bit_is_set(g_booster.flags, BOOSTER_FLAG_NOTAUS_b)) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                }

                if (bit_is_clear(g_booster.flags, BOOSTER_FLAG_ON_b)) {
                    booster_power_on_track();
                }
                setbit(g_booster.flags, BOOSTER_FLAG_ON_b);
                led_set(LED_BO_ON_b, 1);

            } else { // off

                booster_power_off_all();
                clrbit(g_booster.flags, BOOSTER_FLAG_ON_b);
                clrbit(g_booster.flags, BOOSTER_FLAG_NOTAUS_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }
    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}
static void booster_sensors_shortcut_on(void) {
    if ((PORTC.INTCTRL & PORT_INT0LVL_gm) == PORT_INT0LVL_OFF_gc) {
        PORTC.INTFLAGS = Bit(PORT_INT0IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_LO_gc;
    }
}


// do_main: Hauptroutine
// wird periodisch aufgerufen
// return none
void do_main(void) {
/*    if (timer_timedout(&g_timer_10ms)) {
        timer_set(&g_timer_10ms, 10);
    }
    if (timer_timedout(&g_booster.timer_startup)) {
        timer_set(&g_booster.timer_startup, 0xff);
        if (PORTC.INTCTRL & PORT_INT1LVL_gm) {
            booster_sensors_shortcut_on();
        }
    }
*/
/*    if (bit_is_set(g_dev_state, DEV_STATE_FLG_WATCHDOG_b) || (bit_is_set(g_booster.flags, BOOSTER_FLAG_ON_b) && timer_timedout(&g_booster.timer_dcc_watchdog))) {
        booster_notaus();
        clrbit(g_booster.flags, BOOSTER_FLAG_ON_b);
    }
*/   
//    led_set(LED_BO_NOTAUS_b, bit_is_set(g_booster.flags, BOOSTER_FLAG_NOTAUS_b));

/*    measure_task();
    if (g_booster.eeprom_flags.write_shortcut_limit && eeprom_is_ready()) {
        eeprom_update_word(&g_eeprom.booster.shortcut_limit, g_booster.shortcut_limit);
        g_booster.eeprom_flags.write_shortcut_limit = 0;
    }
    if (g_booster.eeprom_flags.write_shortcut_interval && eeprom_is_ready()) {
        eeprom_update_word(&g_eeprom.booster.shortcut_interval, g_booster.shortcut_interval);
        g_booster.eeprom_flags.write_shortcut_interval = 0;
    }
*/ 
static uint8_t first = 0;
 if (first == 0) {
    first = 1;
    port_setbit(LED_PORT,7);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        PORTC.INT1MASK = Bit(DCC_IN_b);
        PORTC.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
        PORTC.INTFLAGS = Bit(PORT_INT1IF_bp);
        PORTC.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_HI_gc;
        
        //booster_sensors_init();
        
        //timer_set(&g_booster.timer_startup, TIMER_STARTUP);
        //timer_set(&g_booster.timer_dcc_watchdog, DCC_WATCHDOG_VAL);

        //dec_start();
        
        /*
        uint8_t v = 0;
        if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
            v = Bit(TC0_CMPA_bp)|Bit(TC0_CMPB_bp);
        }
        TCC0.CTRLC = v;
        AWEXC.OUTOVEN = Bit(3)|Bit(2)|Bit(1)|Bit(0);
        */
    }
}
    //sleep_cpu();
}


// do_reg_read: read a register (reg: 16bit register, pdata: pointer to data)
// return a SBOXNET_ACKRC_* retcode
uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        case R_BOOSTER_FLAGS: *pdata = g_booster.flags; return 0;
        case R_ADCVAL_NUM: *pdata = 2; return 0;
        case R_ADCVAL_0: *pdata = g_booster.advals[0]; return 0;
        case R_ADCVAL_1: *pdata = g_booster.advals[1]; return 0;
            
        case R_BOOSTER_SHORTCUT_LIMIT:  *pdata = g_booster.shortcut_limit; return 0;
        case R_BOOSTER_SHORTCUT_CNT:    *pdata = g_booster.shortcut_nummax; g_booster.shortcut_nummax = 0; return 0;
        case R_BOOSTER_SHORTCUT_INTERVAL: *pdata = g_booster.shortcut_interval; return 0;
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_reg_write: write a register (reg: 16 bit register, data: 16 bit data, mask: 16 bit maske)
// return a SBOXNET_ACKRC_* retcode
uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    switch (reg) {
        case R_BOOSTER_SHORTCUT_LIMIT: {
            g_booster.shortcut_limit = data;
            g_booster.eeprom_flags.write_shortcut_limit = 1;
            return 0;
        }
        case R_BOOSTER_SHORTCUT_INTERVAL: {
            g_booster.shortcut_interval = data;
            g_booster.eeprom_flags.write_shortcut_interval = 1;
            return 0;
        }
    }
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_setup: init the device user defined
// return none
void do_setup(void) {
    return;
}

// do_before_bldr_activate: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
// return none
void do_before_bldr_activate(void) {
     booster_power_off_all();
        
     PORTC.INT0MASK = 0;
     PORTC.INT1MASK = 0;
     PORTC.INTFLAGS = 0xff; // clear interrupt flags;
     PORTC.INTCTRL = 0;

    return;
}

/*********** Interrupts ************************/
ISR(PORTC_INT1_vect) { // DCC Input Signal
    volatile uint8_t v = 0;
    if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
        v = Bit(TC0_CMPA_bp)|Bit(TC0_CMPB_bp);
    }
    TCC0.CTRLC = v;
    port_tglbit(PORTD, 7);
}

// dcc detector cutout generator
ISR(TCD0_CCC_vect) {
    switch (g_dccdec.cutout) {
        case 1: { // cutout enable
            //port_set(DCCM_PORT, Bit(DCCM_EN1_b)|Bit(DCCM_EN2_b)); // EN* on
            port_clr(DCCM_PORT, Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b)); // 2 lower MOSFETs on, connect to 0V
            AWEXC.OUTOVEN = 0;                                    // output override enable off
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                TCD0.CCC = TCD0.CNT + (432/2);
            }
            g_dccdec.cutout = 2;
            booster_sensors_shortcut_off(); // disable shortcut sensors
            break;
        }
        case 2: { //cutout disable
            AWEXC.OUTOVEN = Bit(3)|Bit(2)|Bit(1)|Bit(0);
            //port_clr(DCCM_PORT, Bit(DCCM_EN1_b)|Bit(DCCM_EN2_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
            //port_clrbit(DCC_CUTOUT_TEST_PORT, DCC_CUTOUT_TEST_b); // cutout test point
            if (g_dccdec.state != DEC_STATE_OFF) {
                g_dccdec.state = DEC_STATE_FIRST;
            }
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                TCD0.CCC = TCD0.CNT + (2*58/2);
            }
            g_dccdec.cutout = 3;
            break;
        };
        case 3:
        default: {
            TCD0.INTCTRLB = (TCD0.INTCTRLB & ~TC0_CCCINTLVL_gm)|TC_CCCINTLVL_OFF_gc;
            g_dccdec.cutout = 0;
            if (g_booster.timer_startup.value <= 0) {
                booster_sensors_shortcut_on(); // enable shortcut sensors after first preamble bit to avoid
                // shortcut detections during powerup after cutout
            }
        }
    }
}


#if 0
///////////////////////////////////////////////////////////////////////////////



static void booster_notaus(void) {
    booster_power_off_all();
    setbit(g_booster.flags, BOOSTER_FLAG_NOTAUS_b);
}

/* PORTC_INT0_vect
 *
 * Kurzschlusszähler.
 */
ISR(PORTC_INT0_vect) { // L6206 current
    // Kurzschluss LED ein
    //port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    led_set(LED_CUR_SHORT_b, 1)
    
    // Wenn die Anzahl der Kurzschlüsse kleiner als die Kurzschlussanzahlgrenze ist
    if (g_booster.shortcut_cnt <= g_booster.shortcut_limit) {
        // Dann Kurzschlussanzahl erhöhen
        g_booster.shortcut_cnt++;
    }
    // Maximale Anzahl der bisherigen Kurzschlüsse
    if (g_booster.shortcut_cnt > g_booster.shortcut_nummax ) {
        g_booster.shortcut_nummax = g_booster.shortcut_cnt;
    }
}

/* Kurzschlussdetektor.
 * Hier wird geprüft ob die Anzahl der Kurzschlüsse in einem Intervall die Kurzschlussanzahlgrenze übersteigt.
 * Timer TCD0 CCB
 */
ISR(TCD0_CCB_vect) { // shortcut detector
    // um Kurzschluss Intervall erhöhen.
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCD0.CCB = TCD0.CNT + g_booster.shortcut_interval;
    }
    if (g_booster.shortcut_cnt >= g_booster.shortcut_limit) {
        booster_power_off_all();
        setbit(g_booster.flags, BOOSTER_FLAG_CUR_OV_b);
        port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    } else {
        port_setbit(LED_PORT, LED_CUR_SHORT_b);
    }
    g_booster.shortcut_cnt = 0;
}



static void measure_task(void) {
    if (ADCA.INTFLAGS == 0x03) {
        int16_t ch0;
        int16_t ch1;
        // channel 0 and 1 conversion finished
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ch0 = (int16_t)ADCA.CH0.RES;
            ch1 = (int16_t)ADCA.CH1.RES;
        }
        if (ch0 < 0)
            ch0 = 0;
        if (ch1 < 0)
            ch1 = 0;
        g_booster.advals[0] = ch0;
        g_booster.advals[1] = ch1;
        ADCA.INTFLAGS = 0x03; // clear intflags
        ADCA.CTRLA |= Bit(3)|Bit(2); // start conversions
    }
}

#endif