
#define F_CPU   (2 * 1000000)

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "avrutilslib/defines.h"

void init(void) {
    PORTA_DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTA_PIN0CTRL = PORT_OPC_PULLUP_gc;
    
    PORTB_DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTB_PIN0CTRL = PORT_OPC_PULLUP_gc;
    
    PORTC_DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTC_PIN0CTRL = PORT_OPC_PULLUP_gc;

    PORTD_DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTD_PIN0CTRL = PORT_OPC_PULLUP_gc;

    PORTE_DIR = 0;
    PORTCFG_MPCMASK = 0xff; // all pins
    PORTE_PIN0CTRL = PORT_OPC_PULLUP_gc;
}

ISR(TCC0_OVF_vect) {
    PORTA_OUTTGL = (1 << 5); // toggle PA5
}

# define e2prom_get_byte(_p) ({ \
    setbit(NVM_CTRLB, NVM_EEMAPEN_bp); \
    *((uint8_t*)0x1000 + (uint16_t)(_p)); })

int main(void) {
   ioreg_ccp(PMIC.CTRL, PMIC.CTRL | Bit(PMIC_IVSEL_bp)|Bit(PMIC_HILVLEN_bp)|Bit(PMIC_MEDLVLEN_bp)|Bit(PMIC_LOLVLEN_bp));
    init();
    
    eeprom_busy_wait();
    
    uint8_t b = e2prom_get_byte(0);
    
    eeprom_write_byte(1, b);
/*
    lcd_ks0070_init();
*/    
    PORTA_DIRSET = (1 << 5); // PA5 output
    PORTA_PIN5CTRL = 0;
    
    TCC0_CTRLB = TC_WGMODE_NORMAL_gc;
    TCC0_CTRLD = 0;
    TCC0_CTRLE = 0;
    TCC0_INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC0_INTCTRLB = 0;
    TCC0_INTFLAGS = 0xff;
    TCC0_PER = 7812;
    TCC0_CTRLA = TC_CLKSEL_DIV64_gc;
    
    PMIC_CTRL = PMIC_LOLVLEN_bm;
    sei();
    
    while (1) {
        _delay_ms(1000);
        PORTA_OUTTGL = (1 << 5); // toggle PA5
    }

    return 0;
}