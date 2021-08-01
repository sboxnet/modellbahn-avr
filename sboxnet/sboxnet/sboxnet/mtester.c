
#include "common.h"
#include "sboxnet-struct.h"

#define mt_DEVICE_DESC "ModulTester"
#define mt_PRODUCT_ID   0xa123
#define mt_VENDOR_ID    0x5566
#define mt_FIRMWARE_VERSION 0x0200
/*

APP_FIRMWARE_HEADER(PRODUCT_ID, VENDOR_ID, FIRMWARE_VERSION)
*/

/* timer TCC1
 */

#define MT_TIMER_PERIOD (32000000 / 1024 -1) // = 31250-1

#define NUM_STEPS 3
// wird bei jedem TCC0 OV um eins verringert
volatile int8_t step = NUM_STEPS-1;

// TCC0 overflow 
ISR(TCC1_OVF_vect) {
    step--;
    if (step < 0) {
        step = NUM_STEPS-1;
    }
}


// do_init_system: device system init
// return none
void mtester_do_init_system(void) {
    // alle Ports auf Output
    port_out(PORTA) = 0x0;
    port_dirout(PORTA, 0xff); // PORTA as output
    PORTCFG_MPCMASK = 0xff;
    PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTA Pins to TOTEM 
    
	// PB3 as output
    port_out(PORTB) = 0x0;
    port_dirout(PORTB, 0x8); // PB3 as output
    PORTCFG_MPCMASK = 0xff; // all pin
    PORTB.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTB Pins to TOTEM 

    port_out(PORTC) = 0x0;
    port_dirout(PORTC, 0xff); // PORTC as output
    PORTCFG_MPCMASK = 0xff; // all pin
    PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTC Pins to TOTEM 
    
    port_out(PORTD) = 0x0;
    port_dirout(PORTD, 0xff); // PORTD as output
    PORTCFG_MPCMASK = 0xff; // all pin
    PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTD Pins to TOTEM
    
    //port_set(PORTD, Bit(5));
    //port_set(PORTD, Bit(4));
    
    // set paramters written to EEPROM from common area g_com
    // set Product ID
    g_com.productid = mt_PRODUCT_ID;
    // set VENDOR_ID
    g_com.vendorid = mt_VENDOR_ID;
    // set Firmware Version
    g_com.firmware_version = mt_FIRMWARE_VERSION;
    // Fähigkeiten
    g_com.capabilities = CAP_TESTER;
    // Fähigkeit.Klasse
    g_com.cap_class = 0;
    // Gerätebeschreibung
    g_com.dev_desc_P = PSTR(mt_DEVICE_DESC);
    
    // timer 16bit TCC0
    TCC1.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC1.CTRLD = 0;
    TCC1.CTRLE = 0;
    // timer overflow 
    TCC1.INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC1.INTCTRLB = 0;
     // clear all interupts
    TCC1.INTFLAGS = 0xff;
    // periode
    TCC1.PER = MT_TIMER_PERIOD; 
    
    // enable with /1024
    TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
    
    port_set(PORTD, 0x30);
    port_clr(PORTD, 0xc0);
    
    // enable clkout pd7
    //port_dirout(PORTD, Bit(7));
    //PORTCFG.CLKEVOUT = PORTCFG_CLKOUT_PD7_gc;
    
    return;
}

// do_reg_read: read a register (reg: 16bit register, pdata: pointer to data)
// return a SBOXNET_ACKRC_* retcode
uint8_t mtester_do_reg_read(uint16_t reg, uint16_t* pdata) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_reg_write: write a register (reg: 16 bit register, data: 16 bit data, mask: 16 bit maske)
// return a SBOXNET_ACKRC_* retcode
uint8_t mtester_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_msg: process a message pmsg: pointer to a struct sboxnet_msg_header
// return a SBOXNET_ACKRC_* retcode
uint8_t mtester_do_msg(struct sboxnet_msg_header *pmsg) {
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

// do_setup: init the device user defined
// return none
void mtester_do_setup(void) {
    sei();
    return;
}

void mtester_exec_step0();
void mtester_exec_step1();
void mtester_exec_step2();


// do_main: Hauptroutine
// wird periodisch aufgerufen
// return none

void mtester_do_main(void) {
    sei();
    
    //port_out(PORTA) = 0xff;
    
    //PORTA = 

    //exec_step0();
    //exec_step1();
    //exec_step2();
    //step=3;
    
    
    switch(step)
    {
        case 0:
        {
            mtester_exec_step0();
            //step++;
            break;
        }
        case 1:
        {
            mtester_exec_step1();
            //step++;
            break;
        }
        case 2:
        {
            mtester_exec_step2();
            //step++
            break;
        }
        default:
            break;
    }
    
    return;
}

// do_before_bldr_activate: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
// return none
void mtester_do_before_bldr_activate(void) {
    // nothing
    return;
}


void mtester_exec_step0() {
    // first porta auf alle HIGH
    port_out(PORTA) = 0xff;
    // PC* auf LOW
    port_out(PORTC) = 0;
    
    port_out(PORTB) = 0xff; // PB all 1
    port_clr(PORTD, 0x0f); // PD0..3 auf LOW
    
    port_set(PORTD, 0x30); // PD4..5 H
    port_clr(PORTD, 0xc0); // PD6..7 L
}

void mtester_exec_step1() {
    // first porta auf alle L
    port_out(PORTA) = 0;
    // PC* auf H
    port_out(PORTC) = 0xff;
    
    port_out(PORTB) = 0; // PB all 0
    port_set(PORTD, 0x0f); // PD0..3 auf H
    
    port_clr(PORTD, 0x30); // PD4..5 L
    port_set(PORTD, 0xc0); // PD6..7 H
}

void mtester_exec_step2() {
    // first porta auf  00001111
    port_out(PORTA) = 0x0f;
    // PC* auf 11110000
    port_out(PORTC) = 0xf0;
    
    port_out(PORTB) = 0; // PB all 0
    port_set(PORTD, 0x0f); // PD0..3 auf H
    
    port_clr(PORTD, 0x30); // PD4..5 L
    port_set(PORTD, 0xc0); // PD6..7 H
    /*port_clr(PORTD, Bit(5));
    port_clr(PORTD, Bit(4));
    */
}

