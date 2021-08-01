
// do_init_system: device system init
// return none
void do_init_system(void) {
    return;
}

// do_reg_read: read a register (reg: 16bit register, pdata: pointer to data)
// return a SBOXNET_ACKRC_* retcode
uint8_t do_reg_read(uint16_t reg, uint16_t* pdata) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_reg_write: write a register (reg: 16 bit register, data: 16 bit data, mask: 16 bit maske)
// return a SBOXNET_ACKRC_* retcode
uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask) {
    return SBOXNET_ACKRC_REG_INVALID;
}

// do_msg: process a message pmsg: pointer to a struct sboxnet_msg_header
// return a SBOXNET_ACKRC_* retcode
uint8_t do_msg(struct sboxnet_msg_header *pmsg) {
    return SBOXNET_ACKRC_SEND_NO_ANSWER;
}

// do_setup: init the device user defined
// return none
void do_setup(void) {
    return;
}

// do_main: Hauptroutine
// wird periodisch aufgerufen
// return none
void do_main(void) {
    return;
}

// do_before_bldr_activate: Statements die vor der Aktivierung des Bootloaders ausgeführt werden sollen
// return none
void do_before_bldr_activate(void) {
    return;
}
