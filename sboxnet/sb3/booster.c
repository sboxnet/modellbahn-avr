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

/*
 * TCC0.CCA 100Hz Timer
 * TCC0.CCB Kurzschluss Timer und Shortcut Timer
 * PORTC.INT0 Stromabschaltung
 * PORTC.INT1 DCC Input Signal
 */
 
#define USE_BOOSTER
//#define USE_DCCGEN

#include "common.h"

/*
 * - At the end of a cutout, shortcut detection may be triggered once or two times.
 * - Current/shortcut detection: MAX_SHORTCUT_CNT shortcuts must have occured in TIMER_SHORT_CUT time
 * - Shortcut detection is disabled in the TIMER_STARTUP time after DCC startup
 * - Shortcut detection is disabled in the first half of the first DCC bit after a cutout
 */

 
#define PRODUCT_ID   0x0003
#define VENDOR_ID    0x0001
#define FIRMWARE_VERSION 0x0100
#define DEVICE_DESC  "Booster:2"

// Header für Firmware Infos
APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)

// Timer Einstellungen für 100 Hz für 32MHz Takt
#define TIMER_PRESCALER   64
#define TIMER_FREQ_HZ     100
#define TIMER_PERIOD      (F_CPU / (TIMER_PRESCALER * TIMER_FREQ_HZ))  // 5000

// 100 Hz Timer, wird incrementiert
//static uint8_t g_timer = 0;

// LED Port
#define LED_PORT  PORTD
// Bits für LEDs
#define LED_NOTAUS_b      2 // Notaus
#define LED_CUR_OV_b      0 // Strom zu hoch
#define LED_CUR_SHORT_b   1 // Kurzschluss

#define LED_BO_ON_b       3 // Booster EIN

// Port für Strombrücke
#define DCCM_PORT  PORTC
// Bits für Strombrücke
#define DCCM_EN_b   2 // Enable
#define DCCM_IN1_b  0 // IN1
#define DCCM_IN2_b  1 // IN2
#define DCCM_OV_b   3 // OV
#define DCCM_DCCIN_b  4
#define DCCM_NOTAUS_b 5 // NOTAUS
#define DCCM_DCCOUT_b 7
//#define DCCM_CUTOUT_b 7

// Strom zu hoch detektor: PORT
//#define CUR_OV_PORT  PORTC
// und Bit: ist verbundnen mit RC(OCDa, OCDb: L6206 hat wegen einem zu hohen Strom abgeschaltet)
//#define CUR_OV_b   3

// Booster NOTAUS betätigt: PORT
//#define NOTAUS_PORT  PORTC
// und Bit:
//#define NOTAUS_b   5

// nach dem starten 1s warten bis Kurzschluss Überwachung aktiviert wird nachdem das Gleissignal aktiviert wird
#define TIMER_STARTUP  100 // 1s
// Timer für Startup 
static uint8_t g_timer_startup = 0;

// Werte für Strom und Spannungsmessung
// g_advals[0] Booster Strom 1
// g_advals[1] Booster Spannung
// g_advals[2] Booster Strom 2
static uint16_t g_advals[3] = { 0, 0, 0 };

// Notaus PC5
#define SWITCH_NOTAUS_b  0
// Button zwischenspeichern für Entprellung
static uint8_t g_switches = 0;
static uint8_t g_switches_t = 0;

// Booster Flags
#define BOOSTER_FLG_ON_b      0 // Booster ist EIN
#define BOOSTER_FLG_NOTAUS_b  1 // Booster NOTAUS
#define BOOSTER_FLG_CUR_OV_b  2 // Booster Strom zu hoch
static uint8_t g_booster_flags = 0; // Booster Flags


// DCC IN Pin
#define DCC_IN_PORT DCCM_PORT
#define DCC_IN_b   DCCM_DCCIN_b


#define TIMER_SHORT_CUT 35000 // 70ms
// wieviele Kurzschlüsse dürfen in TIMER_SHORT_CUT auftreten? 
#define MAX_SHORTCUT_CNT 5000 // * ca.13us * 5100 = 70ms
// anzahl der Kurzschlüsse
uint16_t g_dcc_shortcut_cnt = 0;
// Maximale Anzahl der Kurzschlüsse
uint16_t g_shortcutnummax = 0;

/* void dcc_sensors_off(void)
 *
 * Kurzschluss Sensor aus
 */
void dcc_sensors_off(void) {
    // PC3 Sensor Interrupt aus: PORTC INT0
    DCCM_PORT.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    // Kein PORTC INT0 mehr
    DCCM_PORT.INT0MASK = 0; // L6206 Stromüberwachung
    // Kurzschlusstimer INT CCB aus
    TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
    // Anzahl der Kurzsclüsse zurücksetzen
    g_dcc_shortcut_cnt = 0;
}

/* void dcc_sensors_init(void)
 *
 * Sendsoren und Kurzschlusstimer Init
 */
void dcc_sensors_init(void) {
    // zuerst LEDs zurücksetzen
    port_set(LED_PORT, Bit(LED_NOTAUS_b)|Bit(LED_CUR_SHORT_b));

    // Flags zurücksetzen
    clrbit(g_booster_flags, BOOSTER_FLG_CUR_OV_b); // Strom zu hoch

    // L6206 Stromüberwachung
    DCCM_PORT.INT0MASK = Bit(DCCM_OV_b);
    DCCM_PORT.INTFLAGS = Bit(PORT_INT0IF_bp); // clear interrupt flags;
    
    DCCM_PORT.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
    // DCCM_OV_b (PC3) auf Falling Edge
    DCCM_PORT.PIN3CTRL = PORT_ISC_FALLING_gc;
    
    // Anzahl Kurzschlüsse zurücksetzen
    g_dcc_shortcut_cnt = 0;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Kurzschlusse Timer TCC0 CCB:
        // CC timeout in TIMER_SHORT_CUT ~70 ms
        TCC0.CCB = TCC0.CNT + TIMER_SHORT_CUT;
        // Interrupt CCB Flags löschen
        TCC0.INTFLAGS = Bit(TC0_CCBIF_bp);
        // LO Level CCB Interrupt 
        TCC0.INTCTRLB = (TCC0.INTCTRLB & ~TC0_CCBINTLVL_gm) | TC_CCBINTLVL_LO_gc;
    }
}

/* void dcc_sensors_shortcut_on(void)
 *
 * Kurzschluss Überwachung einschalten
 */
void dcc_sensors_shortcut_on(void) {
    // ist der PORTC INT0 Interrupt Off ?
    if ((DCCM_PORT.INTCTRL & PORT_INT0LVL_gm) == PORT_INT0LVL_OFF_gc) {
        // ja , dann einschalten
        
        // Interrupt Flag löschen
        DCCM_PORT.INTFLAGS = Bit(PORT_INT0IF_bp);
        // PORTC INT0 LO Level Interrupt
        DCCM_PORT.INTCTRL = (PORTC.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_LO_gc;
    }
}

/* void dcc_sensors_shortcut_off(void)
 *
 * Kurzschluss Sensor ausschalten.
 */
void dcc_sensors_shortcut_off(void) {
    // Interrupt PORTC INT0 Aus
    DCCM_PORT.INTCTRL = (DCCM_PORT.INTCTRL & ~PORT_INT0LVL_gm) | PORT_INT0LVL_OFF_gc;
}

/* void dcc_power_off_all(void)
 *
 * DCC Signal vom Gleis nehmen.
 */
void dcc_power_off_all(void) {
    // atomar
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // Strom Überwachung aus
        dcc_sensors_off();

        // Strombrücke aus
        port_clr(DCCM_PORT, Bit(DCCM_EN_b)|Bit(DCCM_IN1_b)|Bit(DCCM_IN2_b));
        
        // DCC Input aus
        DCCM_PORT.INTFLAGS = Bit(PORT_INT1IF_bp);
        DCCM_PORT.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_OFF_gc;
        // TCC0 CCB Off Kurzschluss Timer aus
        TCC0.INTCTRLB &= ~TC0_CCBINTLVL_gm;
        // Startup Timer zurücksetzen (Zeit nach Startup nachdem die Kurzschluss überwachung aktiviert wird)
        g_timer_startup = 0;
    }
    port_setbit(LED_PORT, LED_BO_ON_b);
}

/* dcc_power_on_track(void)
 *
 * DCC auf Gleis einschalten.
 */
void dcc_power_on_track(void) {
    // atomar
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // DCC IN Interrupt
        // DCCM_PORT.DCCM_DCCIN_b als PORTC Int1
        DCCM_PORT.INT1MASK = Bit(DCC_IN_b);
        // PC4 beidseitige Flanke
        DCCM_PORT.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
        // INT1 Flag löschen
        DCCM_PORT.INTFLAGS = Bit(PORT_INT1IF_bp);
        // DCCM_PORT INT1 als HI Level INT
        DCCM_PORT.INTCTRL = (PORTC.INTCTRL & ~PORT_INT1LVL_gm) | PORT_INT1LVL_HI_gc;
        
        // Strom Überwachung und Kurzschlusstimer Init
        dcc_sensors_init();
        
        // Init Startup Timer (Zeit nachdem die Kurzschlusserkennung aktiviert wird)
        g_timer_startup = TIMER_STARTUP;

        // DCC IN Init
        // Status von PC4 (DCCM_IN_b) H?
        if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
            // dann Brücke init mit
            port_setbit(DCCM_PORT, DCCM_IN1_b); // IN1 als H
            port_clrbit(DCCM_PORT, DCCM_IN2_b); // IN2 als L
        } else { // ansonsten
            port_clrbit(DCCM_PORT, DCCM_IN1_b); // IN1 als L
            port_setbit(DCCM_PORT, DCCM_IN2_b); // IN2 als H
        }
        port_setbit(DCCM_PORT, DCCM_EN_b); // Brücke einschalten
    }
}

/* void dcc_notaus(void)
 *
 * was wenn NOTAUS.
 */
void dcc_notaus(void) {
    // Booster aus
    dcc_power_off_all();
    // Flag für Notaus setzen
    setbit(g_booster_flags, BOOSTER_FLG_NOTAUS_b);
    // Notaus LED leuchten
    port_clrbit(LED_PORT, LED_NOTAUS_b);
}

/* Port PC3 (Stromabschaltung)
 * 
 * Kurzschluss LED kurz aufleuchten, Kurzschlusszähler erhöhen.
 */
ISR(PORTC_INT0_vect) { // L6206 current
    // Kurzschluss LED kurz aufleuchten
    port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    
    // ein Kurzschluss ist aufgetreten,
    // Zähler erhöhen
    if (g_dcc_shortcut_cnt <= MAX_SHORTCUT_CNT) {
        g_dcc_shortcut_cnt++;
    }
    // die Maximale Anzahl der Kurzschlüsse erhöhen wenn höher als bisher
    if (g_dcc_shortcut_cnt > g_shortcutnummax ) {
        g_shortcutnummax = g_dcc_shortcut_cnt;
    }
}

/* TCC0.CCB Interrupt
 *
 * alle TIMER_SHORT_CUT (~70ms), ein Kurzschluss wird erkannt wenn es mehr als MAX_SHORTCUT_CNT sind.
 */
ISR(TCC0_CCB_vect) { // shortcut detector
    // CCB erhöhen
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // alle ~70ms
        TCC0.CCB = TCC0.CNT + TIMER_SHORT_CUT;
    }
    // sind es mehr als MAX_SHORTCUT_CNT kleine kurzschlüsse?
    if (g_dcc_shortcut_cnt >= MAX_SHORTCUT_CNT) {
        // ja, dann Gleissignal aus
        dcc_power_off_all();
        // setze "Strom zu hoch" Flag
        setbit(g_booster_flags, BOOSTER_FLG_CUR_OV_b);
        // Kurzschluss LED ein
        port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    } else {
        // falls nicht über Schwellwert -> LED aus Kurzschluss
        port_setbit(LED_PORT, LED_CUR_SHORT_b);
    }
    // Kurzschlusszähler zurücksetzen
    g_dcc_shortcut_cnt = 0;
}

/* void booster_init(void)
 *
 * Booster Init: erstmal alles aus
 */
void booster_init(void) {
    dcc_power_off_all();
}
 
/*  void dcc_signal_disable(void)
 *
 * DCC Signal aus.
 */
static NOINLINE void dcc_signal_disable(void) {
    // Bit DCCM_EN_b (PC2) aus
    port_clrbit(DCCM_PORT, DCCM_EN_b);
    // 2us warten
    _delay_us(2);
}

/* void dcc_signal_enable(void)
 *
 * DCC Signal ein.
 */
static NOINLINE void dcc_signal_enable(void) {
    // 1us warten
    _delay_us(1);
    // Bit DCCM_EN_b (PC2) ein
    port_setbit(DCCM_PORT, DCCM_EN_b);
}

/* DCC Input Signal: PORTC.PC4 port Interrupt
 * PORTC.PC4 reagiert auf beide Flanken.
 *
 */
ISR(PORTC_INT1_vect) { // DCC Input Signal
    // ist PORTC.PC4 gesetzt?
    if (bit_is_set(port_in(DCC_IN_PORT), DCC_IN_b)) {
        // webb H, dann erst disable
        dcc_signal_disable();
        port_clrbit(DCCM_PORT, DCCM_IN1_b); // IN1 L
        port_clrbit(DCCM_PORT, DCCM_IN2_b); // IN2 L
        // dann Flanke wechseln
        _delay_us(0.5);
        port_setbit(DCCM_PORT, DCCM_IN1_b); // IN1 H
        _delay_us(0.5);
        //port_clrbit(DCCM_PORT, DCCM_IN2_b); // IN2 L
        
    } else {
        // wenn L,
        // erst disable
        dcc_signal_disable();
        port_clrbit(DCCM_PORT, DCCM_IN1_b); // IN1 L
        port_clrbit(DCCM_PORT, DCCM_IN2_b); // IN2 L
        
        // dann Flanke wechseln
        _delay_us(0.5);
        //port_clrbit(DCCM_PORT, DCCM_IN1_b); // IN1 L
        port_setbit(DCCM_PORT, DCCM_IN2_b); // IN2 H
        _delay_us(0.5);
        
        //port_clrbit(DCCM_PORT, DCCM_DCCOUT_b);
    }
    // nach dem Flankenwechsel Booster wieder ein
    dcc_signal_enable();
    
    // ist die Zeit für den Startup Timer abgelaufen?
    if (g_timer_startup == 0) {
        // dann Kurzschluss Sensor ein
        dcc_sensors_shortcut_on();
    }
}

/* void read_switches(void)
 *
 * Schalter lesen (NOTAUS)
 */
static void read_switches(void) {
    uint8_t sw;

    sw = 0;
    // ist NOTAUS gedrückt (-> Schalter geschlossen  -> L)
    //volatile uint8_t v = port_in(DCCM_PORT);
    //if (!(v & Bit(DCCM_NOTAUS_b))) {
    if (bit_is_clear(port_in(DCCM_PORT), DCCM_NOTAUS_b)) {
       // dann Flag SWITCH_NOTAUS_b setzen
       setbit(sw, SWITCH_NOTAUS_b);
    }
    // Schalter entprellen, Entprellt -> g_switches
    debounce_keys(&g_switches, &g_switches_t, sw);
}


/* TCC0.CCA 
 * 100Hz Timer,
 * Schalter lesen
 */
// every 10ms ~ 100Hz
ISR(TCC0_CCA_vect) {
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        // CCA um TIMER_PERIOD erhöhen
        TCC0.CCA += TIMER_PERIOD;
    }
    
    // Starttimer runterzählen falls != 0
    if (g_timer_startup) {
        g_timer_startup--;
    }
    // Schalter einlesen
    read_switches();
}

/* void measure_task(void)
 * Durchführung der Strom und Spannungsmessung.
 * PA0, PA2 Strommessung: U = 0.22 * I; 0.22 * 1A = 0.22 V, 0.22 * 2A = 0.44V, 0.22 * 3A = 0.66V: 0,22 * 2.4A= 0,528V
 *     bei Rcl = 10kOhm Stromabschaltung bei ca 2,3 A
 * PA1 Spannungsmessung Vmess = 0.05982 * V bei 15 V -> 0,8974 V
 */
static void measure_task(void) {
    // sind alle ADCA Kanele bereit
    if (ADCA.INTFLAGS == 0x07) {
        int16_t ch0_a;
        int16_t ch1_v;
        int16_t ch2_a;
        // channel 0,1,2 conversion finished
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ch0_a = (int16_t)ADCA.CH0.RES;
            ch1_v = (int16_t)ADCA.CH1.RES;
            ch2_a = (int16_t)ADCA.CH1.RES;
        }
        // Absoluter Wert 
        if (ch0_a < 0)
            ch0_a = 0;
        if (ch1_v < 0)
            ch1_v = 0;
        if (ch2_a < 0)
            ch2_a = 0;
        // in g_advals Speichern
        g_advals[0] = ch0_a;
        g_advals[1] = ch1_v;
        g_advals[2] = ch2_a;
        // Interrupt Flags zurücksetzen
        ADCA.INTFLAGS = 0x07; // clear intflags
        // ADC einschalten und Konvertierung starten
        ADCA.CTRLA |= ADC_ENABLE_bm|Bit(4)|Bit(3)|Bit(2);
    }
}

/* void notaus_task(void)
 *
 * Prüft ob Notaus gedrückt wurde und bei ja, löst notaus aus
 */
static void notaus_task(void) {
    // ist der NOTAUS Schalter gedrückt
    if (bit_is_set(g_switches, SWITCH_NOTAUS_b)) {
        // dann Notaus durchführen
        dcc_notaus();
    }
}

/* uint8_t read_production_signature_row(uint8_t offset)
 *
 * Auslesen eines Werts aus der Chip Kalibrierung: hier ADCACAL0, ADCACAL1
 */
static uint8_t read_production_signature_row(uint8_t offset) {
    uint8_t value;
    NVM.CMD = NVM_CMD_READ_CALIB_ROW_gc;
    value = pgm_read_byte(offset);
    NVM.CMD = NVM_CMD_NO_OPERATION_gc;
    return value;
}

/* void do_init_system(void)
 *
 * Geräte Init.
 */
void do_init_system(void) {
    // LED_PORT auf LEDs off
    port_out(LED_PORT) = 0x0f;
    // LED_PORT auf Ausgabe
    port_dirout(PORTD, 0x0f);
    // LED_PORTs auf totem pole
    PORTCFG_MPCMASK = 0x0f; // PD2..0
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc;
    
    // DCCM_PORT
    // alles OFF vorbereiten
    port_out(DCCM_PORT) = 0;
    // PC0 (IN1), PC1(IN2), PC2 (EN) als Ausgabe
    port_dirout(DCCM_PORT, Bit(DCCM_DCCOUT_b)|(DCCM_EN_b)|Bit(DCCM_IN2_b)|Bit(DCCM_IN1_b));
    // PC3(OV), PC4 (DCCIN), PC5(NOTAUS) als Eingabe
    port_dirin(DCCM_PORT, Bit(DCCM_OV_b)|Bit(DCCM_DCCIN_b)|Bit(DCCM_NOTAUS_b));
    // Ausgabe ports als totem pole
    PORTCFG_MPCMASK = Bit(DCCM_DCCOUT_b)|Bit(DCCM_EN_b)|Bit(DCCM_IN2_b)|Bit(DCCM_IN1_b);
    DCCM_PORT.PIN0CTRL = PORT_OPC_TOTEM_gc;
    // Eingabeports mit Pullups;
    PORTCFG_MPCMASK = Bit(DCCM_OV_b)|Bit(DCCM_DCCIN_b)|Bit(DCCM_NOTAUS_b);
    DCCM_PORT.PIN3CTRL = PORT_OPC_PULLUP_gc;
    
    // ADCA Kanäle als Eingabe
    port_dirin(PORTA, 0xff);
    PORTCFG_MPCMASK = 0xff; // all pins
    // mit Pulldown
    PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
        
    // timer C0 als Normal Timer für 100Hz, und Kurzschluss Timer + Shortcut Timer
    //  CCA für 100Hz
    //  CCB Kurzschlusstimer
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc; // Normal Modus
    TCC0.CTRLC = 0; // kein CMPx
    TCC0.CTRLD = 0; // keine Events
    TCC0.CTRLE = 0; // kein Bytemode
    TCC0.INTCTRLA = 0; // kein ERR oder OVF Interrupt
    TCC0.INTCTRLB = TC_CCAINTLVL_LO_gc; // aber CCA Low Interrupt Level für 100Hz
    TCC0.INTFLAGS = 0xff; // Lösche Interrupt Flags
    TCC0.PER = 0xffff; // Timer auf endlos
    TCC0.CCA = TCC0.CNT + TIMER_PERIOD; // CCA Periode für 100Hz einstellen
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc; // Timer starten mit 1:64

    // ADC: benutze mit Vorzeichen (unsigned mode may be broken) and 1V Interne Referenz
    // - ADC_CONMODE_bp mit Vorzeichen
    // - ADC_FREERUN_bp nein muss angestossen werden
    // - ADC_RESOLUTION_12BIT_gc 12-bit result, right adjusted
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
    
    // Setze ProductID, VendorID, ... im Common Bereich
    g_com.productid = PRODUCT_ID;
    g_com.vendorid = VENDOR_ID;
    g_com.firmware_version = FIRMWARE_VERSION;
    g_com.capabilities = CAP_DCC_BOOSTER;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(DEVICE_DESC);
    // Booster Flags: BOOSTER_FLG*
    g_booster_flags = 0;
    // Booster Init: Booster ausschalten
    booster_init();
}

/* uint8_t do_msg(struct sboxnet_msg_header *pmsg)
 *
 * Gerätnachrichten bearbeiten.
 */
uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    switch (pmsg->cmd) {
        // SBOXNET_CMD_LOCO_POWER: Gerät Power ein
        case SBOXNET_CMD_LOCO_POWER: {
            if (pmsg->opt.len != 1) {
                return SBOXNET_ACKRC_INVALID_ARG;
            }
            // Flags: 0000 000P (POWER)
            uint8_t flags = pmsg->data[0];
            if (flags & 0x01) { // on
                // ist Gerät im NOTAUS?
                if (bit_is_set(g_switches, SWITCH_NOTAUS_b)) {
                    return SBOXNET_ACKRC_LOCO_NOTAUS;
                } else {
                    // nein, NOTAUS flag löschen
                    clrbit(g_booster_flags, BOOSTER_FLG_NOTAUS_b);
                    // NOTAUS Led aus
                    port_setbit(LED_PORT, LED_NOTAUS_b);
                }
                
                // Ist Booster EIN?
                if (bit_is_clear(g_booster_flags, BOOSTER_FLG_ON_b)) {
                    // nein, dann DCC signal auf die Schienen
                    dcc_power_on_track();
                }
                // Booster ist EIN
                setbit(g_booster_flags, BOOSTER_FLG_ON_b);
                port_clrbit(LED_PORT, LED_BO_ON_b);
            } else { // Booster aus
                dcc_power_off_all();
                clrbit(g_booster_flags, BOOSTER_FLG_ON_b);
            }
            pmsg->opt.len = 0;
            return SBOXNET_ACKRC_OK;
        }

    }
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

/* uint8_t do_reg_read(uint16_t reg, uint16_t* pdata)
 *
 * Booster Register lesen.
 */
uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    switch(reg) {
        // Booster Flags
        case R_BOOSTER_FLAGS: *pdata = g_booster_flags; return 0;
        // Anzahl der ADC Kanäle
        case R_ADCVAL_NUM: *pdata = 3; return 0;
        // ADC Kanal 0: Booster Strom 1
        case R_ADCVAL_0: *pdata = g_advals[0]; return 0;
        // ADC Kanal 1: Booster Spannung
        case R_ADCVAL_1: *pdata = g_advals[1]; return 0;
        // ADC Kanal 2: Booster Strom 2
        case R_ADCVAL_2: *pdata = g_advals[2]; return 0;
        // Kurzschluss Maximalzahl
        case 202: *pdata = g_shortcutnummax; g_shortcutnummax = 0; return 0;
    }    
    return SBOXNET_ACKRC_REG_INVALID;
};

/* void do_setup(void)
 * Nichts für den Setup.
 */
void do_setup(void) {
}

/* void do_main(void)
 *
 * Hauptschleife:
 * - measure_task()
 * - notaus_task()
 */
void do_main(void) {
    //port_clrbit(LED_PORT, LED_BO_ON_b);
    //port_clrbit(LED_PORT, LED_CUR_OV_b);
    //port_clrbit(LED_PORT, LED_CUR_SHORT_b);
    //port_clrbit(LED_PORT, LED_NOTAUS_b);
    port_setbit(DCCM_PORT, DCCM_DCCOUT_b);
    measure_task();
    notaus_task();
}

/* void do_before_bldr_activate(void)
 */
void do_before_bldr_activate(void) {
    dcc_power_off_all();
    
    PORTC.INT0MASK = 0;
    PORTC.INT1MASK = 0;
    PORTC.INTFLAGS = 0xff; // clear interrupt flags;
    PORTC.INTCTRL = 0;

    TCC0.INTCTRLA = 0;
    TCC0.INTCTRLB = 0;
    TCC0.CTRLA = 0; // stop timer
    TCC0.INTFLAGS = 0xff; // clear interrupt flags
}
