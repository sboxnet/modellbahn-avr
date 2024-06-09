
/*
 * gbm2.c
 *
 * Created: 04.10.2023 16:05:08
 *  Author: balagi tom@tom.ist-im-web.de
 */ 

#include "dcc_dec.h"
#include "common.h"

#define g2_PRODUCT_ID   0x0600
#define g2_VENDOR_ID    0x1234
#define g2_FIRMWARE_VERSION 0x0103
#define g2_DEVICE_DESC  "GBRailcom:1"

#define g2_RM_0_7_PORT PORTA

struct g2_v_t {
	uint8_t g_power_on;
	uint16_t g_dec_lastaddr;
	struct timer g_power_on_timer;
	struct dcc_dec_t dccdec;
};

struct g2_v_t g2_v = { 0 };  // Speicher g2 Variablen

void g2_do_init_system(void) {
	// PA0..7 analog input railcom sensors 0..7
	port_dirin(g2_RM_0_7_PORT, 0xff); // all PORTA input
	PORTCFG_MPCMASK = 0xff; // all pins
	g2_RM_0_7_PORT.PIN0CTRL = PORT_OPC_WIREDAND_gc; // all wired without resistor
	port_out(g2_RM_0_7_PORT) = 0xff; // let ports float, OUT 0xff
	
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SLEEP.CTRL = SLEEP_SMODE_IDLE_gc|Bit(SLEEP_SEN_bp);
    // power reduction
    PR.PRPA = Bit(PR_DAC_bp)|Bit(PR_AC_bp);
    PR.PRPB = Bit(PR_DAC_bp)|Bit(PR_AC_bp);
    PR.PRPC = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    PR.PRPD = Bit(PR_TWI_bp)|Bit(PR_USART1_bp)|Bit(PR_USART0_bp)|Bit(PR_SPI_bp)|Bit(PR_HIRES_bp); // PR_TC1_bp PR_TC0_bp;
    
    g_com.productid = g2_PRODUCT_ID;
    g_com.vendorid = g2_VENDOR_ID;
    g_com.firmware_version = g2_FIRMWARE_VERSION;
    g_com.capabilities = CAP_FB_OCCUPANCY_RAILCOM;
    g_com.cap_class = 0;
    g_com.dev_desc_P = PSTR(g2_DEVICE_DESC);
}

uint8_t g2_do_msg(struct sboxnet_msg_header *pmsg) {
	if (pmsg->dstaddr == SBOXNET_ADDR_BROADCAST) {
	    return SBOXNET_ACKRC_CMD_UNKNOWN;
	}
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

void g2_do_setup(void) {
	ADCA.CTRLA = 0;
	ADCA.CTRLB = Bsv(ADC_IMPMODE_bp,1)/*highimp mode*/|Bsv(ADC_CURRLIMIT0_bp,0)/*no current limit*/|Bsv(ADC_CONMODE_bp,1) /*signed*/|Bsv(ADC_FREERUN_bp,0)/*no freerun*/|Bsv(ADC_RESOLUTION0_bp,ADC_RESOLUTION_12BIT_gc); // 12bit resolution
}

void g2_do_main(void) {
	
}

void g2_do_before_bldr_activate(void) {
	
}

uint8_t g2_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

uint8_t g2_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}
