/*
 * g2gbm.c
 *
 * Created: 16.12.2023 17:53:21
 *  Author: vm
 */ 

#include "dcc_dec.h"
#include "common.h"
#include "sboxnet-struct.h"

#define g2_DEVICE_DESC "Railcom Gleisbesetztmelder"
#define g2_PRODUCT_ID   0x0501
#define g2_VENDOR_ID    0x1234
#define g2_FIRMWARE_VERSION 0x0100


struct g2_sensor {
	uint8_t  timer;
	uint16_t locoaddr;
	uint8_t  last_seq;
	uint8_t  holdtime;
	struct bits {
		unsigned notack:1;
		unsigned on:1;
		unsigned holdtime_changed:1;
	}        flags;
	uint8_t  retry_timer;
};

#define g2_DEFAULT_HOLDTIME 200

#define g2_NUM_SENSORS   10
struct g2_gbm2_pipe {
	pipe_t pipe;
	uint8_t buf[64];
};

struct g2_v_t {
	uint8_t g_holdtime;
	uint8_t g_old_holdtime;
	uint16_t g_sensor_bits;
	uint16_t g_sensor_bits_1;
	struct g2_sensor g_sensors[g2_NUM_SENSORS];
	uint8_t  g_led_counter;
	uint8_t  g_transmit_seq;
	struct g2_gbm2_pipe g_locoaddr_pipe;
	//struct timer g_led_timer;
	struct timer g_power_on_timer;
	uint8_t g_power_on;
	uint16_t g_dec_lastaddr;
	struct dcc_dec_t dccdec;
};

struct g2_v_t g2_v = { 0 };  // Speicher g2 Variablen

//APP_FIRMWARE_HEADER(mt_PRODUCT_ID, mt_VENDOR_ID, mt_FIRMWARE_VERSION)


/* timer TCC1
 */

//#define MT_TIMER_PERIOD (32000000 / 1024 -1) // = 31250-1

//#define NUM_STEPS 3
// wird bei jedem TCC0 OV um eins verringert
//volatile int8_t step = NUM_STEPS-1;

// TCC1 overflow 
/*
ISR(TCC1_OVF_vect) {
    step--;
    if (step < 0) {
        step = NUM_STEPS-1;
    }
}
*/

// do_init_system: device system init
// return none
void g2_do_init_system(void) {
    // alle Ports auf Output
    //port_out(PORTA) = 0x0;
    //port_dirout(PORTA, 0xff); // PORTA as output
    //PORTCFG_MPCMASK = 0xff;
    //PORTA.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTA Pins to TOTEM 
    
	// PB3 as output
    //port_out(PORTB) = 0x0;
    //port_dirin(PORTB, 0x8); // PB3 as output
    //PORTCFG_MPCMASK = 0xff; // all pin
    //PORTB.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTB Pins to TOTEM 

    //port_out(PORTC) = 0x0;
    //port_dirout(PORTC, 0xff); // PORTC as output
    //PORTCFG_MPCMASK = 0xff; // all pin
    //PORTC.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTC Pins to TOTEM 
    
    //port_out(PORTD) = 0x0;
    //port_dirout(PORTD, 0xff); // PORTD as output
    //PORTCFG_MPCMASK = 0xff; // all pin
    //PORTD.PIN0CTRL = PORT_OPC_TOTEM_gc; // all PORTD Pins to TOTEM
    
    //port_set(PORTD, Bit(5));
    //port_set(PORTD, Bit(4));
    
    // set paramters written to EEPROM from common area g_com
    // set Product ID
    g_com.productid = g2_PRODUCT_ID;
    // set VENDOR_ID
    g_com.vendorid = g2_VENDOR_ID;
    // set Firmware Version
    g_com.firmware_version = g2_FIRMWARE_VERSION;
    // Fähigkeiten
    g_com.capabilities = CAP_FB_OCCUPANCY_RAILCOM;
    // Fähigkeit.Klasse
    g_com.cap_class = 0;
    // Gerätebeschreibung
    g_com.dev_desc_P = PSTR(g2_DEVICE_DESC);
    return;
}

// do_reg_read: read a register (reg: 16bit register, pdata: pointer to data)
// return a SBOXNET_ACKRC_* retcode
uint8_t g2_do_reg_read(uint16_t reg, uint16_t* pdata) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_reg_write: write a register (reg: 16 bit register, data: 16 bit data, mask: 16 bit maske)
// return a SBOXNET_ACKRC_* retcode
uint8_t g2_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_msg: process a message pmsg: pointer to a struct sboxnet_msg_header
// return a SBOXNET_ACKRC_* retcode
uint8_t g2_do_msg(struct sboxnet_msg_header *pmsg) {
    return SBOXNET_ACKRC_CMD_UNKNOWN;
}

// do_setup: init the device user defined
// return none
void g2_do_setup(void) {
    sei();
    return;
}

// do_main: Hauptroutine
// wird periodisch aufgerufen
// return none

void g2_do_main(void) {          
    return;
}

// do_before_bldr_activate: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
// return none
void g2_do_before_bldr_activate(void) {
    // nothing
    return;
}


