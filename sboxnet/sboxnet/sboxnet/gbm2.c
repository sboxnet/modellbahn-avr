/*
 * gbm2.c: railcom gbm
 * Um eine '0' zu übertragen muss der Decoder einen Strom I von 30+4/-6 mA liefern
 * bei einem Spannungsabfall am Gleis von bis zu 2.2V.
 * Bei einer '1' darf der Strom I höchstens +/- 0.1mA betragen.
 *
 * Created: 10.08.2022 16:15:49
 *  Author: balagi
 */ 

#include "common.h"

#define g2_PRODUCT_ID   0x0600
#define g2_VENDOR_ID    0x1234
#define g2_FIRMWARE_VERSION 0x0103
#define g2_DEVICE_DESC  "GBM2:1"

void g2_do_init_system() {
	
}

uint8_t g2_do_msg(struct sboxnet_msg_header *pmsg) {
	return SBOXNET_ACKRC_CMD_UNKNOWN;
}

void g2_do_setup() {
}

void g2_do_main() {
}

void g2_do_before_bldr_activate(void) {
}

uint8_t g2_do_reg_read(uint16_t reg, uint16_t* pdata) {
	return SBOXNET_ACKRC_REG_INVALID;
}

uint8_t g2_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
	return SBOXNET_ACKRC_REG_INVALID;
}
