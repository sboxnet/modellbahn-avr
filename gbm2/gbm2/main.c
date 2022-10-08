/*
 * gbm2.c
 * railcom besetztmelder
 *
 * ADC0..ADC7 Port A PA0..PA7
 * ADC8..ADC11 Port B PB0..PB3
 *
 * Created: 06.10.2022 15:35:09
 * Author : balagi
 */ 

#define F_CPU_MHZ 32
#define F_CPU  (F_CPU_MHZ * 1000000)

#include <avr/io.h>
//#include <avr/iox32a4u.h>, über project included
#include <stddef.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <string.h>
#include <avr/sfr_defs.h>
#include "avrutilslib/defines.h"


static void gbm2_switch_to_32MHz(void) {
	// zuerst auf internen 32 MHz Osz Umschalten
	OSC.CTRL |= OSC_RC32MEN_bm;
	//warte bis der Stabil ist
	while (bit_is_clear(OSC.STATUS,OSC_RC32MRDY_bp)) {
		_NOP();
	}
	// die RC32MEN als Taktquelle einstellen
	ioreg_ccp(&CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
	// den internen 2MHz Takt stoppen
	clrbit(OSC.CTRL, OSC_RC2MEN_bp);
	//auf externen 16Mhz OSC Schalten
	// Frequenzberich 12-16MHz wählen und Startup Zeit 16k
	OSC.XOSCCTRL = OSC_FRQRANGE_12TO16_gc|OSC_XOSCSEL_XTAL_16KCLK_gc;
	// Externen Oszillator einschalten
	setbit(OSC.CTRL, OSC_XOSCEN_bp);
	// warte bis ext. Oszillator stabil ist
	// 200 Mal versuchen umzuschalten
	uint8_t is32 = 0;
	uint8_t cnt = 200;
	while (cnt--) {
		if (bit_is_set(OSC.STATUS, OSC_XOSCRDY_bp)) {
			is32 = 1;
			break;
		}
		_NOP();
		_NOP();
		_NOP();
	}
	// Wenn externer Osc ist stabil?
	if (is32) {
		// dann die PLL auf 2x stellen == 32MHz
		OSC.PLLCTRL = OSC_PLLSRC_XOSC_gc|2; // PLL multiplier: 2x
		// PLL aktivieren
		setbit(OSC.CTRL, OSC_PLLEN_bp);
		// warte bis PLL stabil ist
		while (bit_is_clear(OSC.STATUS, OSC_PLLRDY_bp)){
			_NOP();
		}
		// die PLL als Taktquelle einstellen
		ioreg_ccp(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);
		// den internen 32MHz Takt stoppen
		clrbit(OSC.CTRL, OSC_RC32MRDY_bp);
	}
}

void gbm2_init(void)
{
	// enable app iv and interrupt
	ioreg_ccp(&PMIC.CTRL, PMIC_HILVLEN_bm|PMIC_MEDLVLEN_bm|PMIC_LOLVLEN_bm);
	
	// porta dir IN and pull ups
	PORTA.DIRCLR = 0xff;
	PORTCFG_MPCMASK = 0xff; // all pins
	PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc; // little bias voltage
	// portb dir IN and pull ups
	PORTB.DIRCLR = 0xff;
	PORTCFG_MPCMASK = 0xff;
	PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;

}

int main(void)
{
    /* Replace with your application code */
	gbm2_switch_to_32MHz();
	gbm2_init();
		

}

