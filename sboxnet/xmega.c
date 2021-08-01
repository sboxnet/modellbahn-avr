

#include "common.h"

static volatile uint8_t g_timer = 0;
static uint8_t g_wait_timer = 0;
static uint8_t g_lauflicht = 0;

void do_init_system(void) {
    // timer
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC0.CTRLD = 0;
    TCC0.CTRLE = 0;
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC0.INTCTRLB = 0;
    TCC0.INTFLAGS = 0xff;
    TCC0.PER = (2048-1);
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
    
    port_dirout(PORTD, 0xff);
    g_lauflicht = 0x01;
    port_out(PORTD) = g_lauflicht;

    g_v.led_rt = 0x33;
}

// every 4.096ms
ISR(TCC0_OVF_vect) {
    g_timer++;
    
    if (g_wait_timer) {
        g_wait_timer--;
    }
}


uint8_t do_msg(struct sboxnet_msg_max *pmsg) {    
    return 0xff;
}

void do_setup(void) {
}

void do_main(void) {
    if (g_wait_timer == 0) {
        
        g_lauflicht = rol_byte(g_lauflicht);
        port_out(PORTD) = g_lauflicht;
        
        g_wait_timer = 200;
    }
}
