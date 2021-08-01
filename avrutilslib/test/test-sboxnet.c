
TODO Umstellung auf 250kBit

#ifdef __AVR_XMEGA__
#define F_CPU 32000000UL
#else
#define F_CPU 16000000UL
#endif

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <avr/eeprom.h>

#ifdef __AVR_XMEGA__
# define LCD_KS0070_DB_PORT  PORTD
# define LCD_KS0070_DB_SHIFT 3
# define LCD_KS0070_RS_PORT  PORTD
# define LCD_KS0070_RS_BIT   7
# define LCD_KS0070_E_PORT   PORTE
# define LCD_KS0070_E_BIT    0
# define LCD_KS0070_RW_PORT  
# define LCD_KS0070_RW_BIT   
# define LCD_KS0070_USE_BUSY 0
#else
# define LCD_KS0070_DB_PORT  PORTC
# define LCD_KS0070_DB_SHIFT 0
# define LCD_KS0070_RS_PORT  PORTC
# define LCD_KS0070_RS_BIT   PC5
# define LCD_KS0070_E_PORT   PORTC
# define LCD_KS0070_E_BIT    PC4
# define LCD_KS0070_RW_PORT  PORTD
# define LCD_KS0070_RW_BIT   PD3
# define LCD_KS0070_USE_BUSY 1
#endif
#define LCD_KS0070_NUM_LINES 2
#include "avrutilslib/lcd.c"

#include "avrutilslib/random.h"

// device UXNET address
static uint8_t g_dev_addr;


#define SBOXNET_DEBUG_ENABLED 1
#define SBOXNET_DEVICE_ADDR    g_dev_addr
//#define SBOXNET_SECTION        BOOTLOADER_SECTION

#ifdef __AVR_XMEGA__
# define SBOXNET_RxD_PORT              PORTC
# define SBOXNET_RxD_BIT               2
# define SBOXNET_TxD_PORT              PORTC
# define SBOXNET_TxD_BIT               3
# define SBOXNET_USART_RxC_vect        USARTC0_RXC_vect
# define SBOXNET_USART_TxC_vect        USARTC0_TXC_vect
# define SBOXNET_TIMER                 TCC0
# define SBOXNET_TIMER_CC              CCA
# define SBOXNET_TIMER_CCIF            TC0_CCAIF_bp
# define SBOXNET_TIMER_CCINTLVL_MASK   TC0_CCAINTLVL_gm
# define SBOXNET_TIMER_CCINTLVL        TC_CCAINTLVL_LO_gc
# define SBOXNET_TIMER_BIT_TICKS       4
# define SBOXNET_TIMER_PERIOD_MASK     0xff
# define SBOXNET_TIMER_RANDOM_MASK     0x0f
# define SBOXNET_HW_SENSOR_INTLVL_MASK PORT_INT0LVL_gm
# define SBOXNET_HW_SENSOR_INTMASK     INT0MASK
# define SBOXNET_HW_SENSOR_INTF_BIT    PORT_INT0IF_bp
# define SBOXNET_USART_ID              USARTC0
# define SBOXNET_USART_DEF_INTR        1
# define SBOXNET_PRNG_SEED             OSC_RC32KCAL
#else
# define SBOXNET_RxD_PORT PORTD
# define SBOXNET_RxD_BIT  PD0
# define SBOXNET_TxD_PORT PORTD
# define SBOXNET_TxD_BIT  PD1
# define SBOXNET_TIMER_TIMSK   TIMSK2  // use timer2 for UX bit timing
# define SBOXNET_TIMER_TIFR    TIFR2
# define SBOXNET_TIMER_OCIE    OCIE2A
# define SBOXNET_TIMER_OCF     OCF2A
# define SBOXNET_TIMER_TCNT    TCNT2
# define SBOXNET_TIMER_OCR     OCR2A
# define SBOXNET_TIMER_BIT_TICKS 4
# define SBOXNET_TIMER_PERIOD_MASK 0xff
# define SBOXNET_TIMER_RANDOM_MASK 0x0f
# define SBOXNET_HW_SENSOR_PCICR      PCICR
# define SBOXNET_HW_SENSOR_PCIE_BIT   PCIE2
# define SBOXNET_HW_SENSOR_PCMSK      PCMSK2
# define SBOXNET_HW_SENSOR_PCINT_BIT  PCINT16   // PD0=RxD pin is PCINT16
# define SBOXNET_HW_SENSOR_PCIFR      PCIFR
# define SBOXNET_HW_SENSOR_PCIF_BIT   PCIF2
# define SBOXNET_USE_HW_SENSOR_PINCH  1
# define SBOXNET_USE_HW_SENSOR_EXTINT 0
# define SBOXNET_USART_ID       0
# define SBOXNET_USART_DEF_INTR 1
# define SBOXNET_PRNG_SEED      (OSCCAL)
#endif
#include "avrutilslib/sboxnet.c"

#ifdef __AVR_XMEGA__
#define TIMER_OVINT_vect   TCC0_OVF_vect
#define TIMER_COMPINT_vect TCC0_CCA_vect
#define LED_PORT PORTA
#define LED_BIT  5
#else
#define TIMER_OVINT_vect   TIMER2_OVF_vect
#define TIMER_COMPINT_vect TIMER2_COMPA_vect
#define LED_PORT PORTB
#define LED_BIT  0
#endif

volatile uint16_t g_lcd_timer = 0;
volatile uint8_t  g_lcd_timedout = 0;
volatile uint16_t g_led_timer = 0;

ISR(TIMER_OVINT_vect) { // ca. alle 500 us
    g_lcd_timer++;
    if (g_lcd_timer >= 400) {
        g_lcd_timer = 0;
        g_lcd_timedout = 1;
    }
    g_led_timer++;
    if (g_led_timer >= 1000) {
        g_led_timer = 0;

        port_tglbit(LED_PORT, LED_BIT);
    }
}

ISR(TIMER_COMPINT_vect) {
    sboxnet_timer_bit();
}

void init(void) {
#ifdef __AVR_XMEGA__
    wdt_reset();
   
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
    
    // switch to 32Mhz internal osc
    setbit(OSC.CTRL, OSC_RC32MEN_bp);
    while (bit_is_clear(OSC.STATUS, OSC_RC32MRDY_bp));
    CPU_CCP = CCP_IOREG_gc;
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
    
    TCC0.CTRLB = TC_WGMODE_NORMAL_gc;
    TCC0.CTRLD = 0;
    TCC0.CTRLE = 0;
    TCC0.INTCTRLA = TC_OVFINTLVL_LO_gc;
    TCC0.INTCTRLB = 0;
    TCC0.INTFLAGS = 0xff;
    TCC0.PER = 255;
    TCC0.CTRLA = TC_CLKSEL_DIV64_gc;
    
    PMIC_CTRL = PMIC_LOLVLEN_bm;

#else
    MCUSR = 0;
    wdt_reset();
    wdt_disable();
    
    PORTB = 0b11111111;
    DDRB  = 0b00000000;
    PORTC = 0b11111111;
    DDRC  = 0b00000000; // PC input with pullups
    PORTD = 0b11111111;
    DDRD  = 0b00000000; // PD input with pullups
    
    // Analog comparator control and status: disable analog comparator
    ACSR = Bit(ACD);
    
    // configure sleep mode: idle sleep mode, sleep mode allowed
    SMCR = Bsv(SM2,0)|Bsv(SM1,0)|Bsv(SM0,0)|Bsv(SE,1);

    // timer for ux
    TCCR2A = Bsv(WGM21,0)|Bsv(WGM20,0);      // normal mode, prescaler 1/32
    TCCR2B = Bsv(WGM22,0)|Bsv(CS22,0)|Bsv(CS21,1)|Bsv(CS20,1); // start timer with prescaler 1/32
    TIMSK2 = Bit(TOIE2);
#endif
    
    port_dirout(LED_PORT, Bit(LED_BIT));
    port_clrbit(LED_PORT, LED_BIT);
}

int main(void) {
    struct sboxnet_msg_max msg;

    init();
    
    g_dev_addr = eeprom_read_byte(0);
    
    lcd_ks0070_init();
    
    sboxnet_init();
    
    sboxnet_receiver_enable();
    
    sei();

    _delay_ms(100);
    
    uint8_t sidetimer = 0;
    uint8_t side = 0;
    while(1) {
        static uint8_t buf[16] = "nixda";
        
        uint8_t rc;
        do {
            rc = sboxnet_receive_msg(&msg);
            if (rc == 0) {
                memcpy(buf, msg.data, 15);
            }
        } while (rc != 1);
        
        if (sboxnet_can_send_msg()) {
            msg.msg.dstaddr = (g_dev_addr == 11 ? 10 : 11);
            msg.msg.opt.seq = 0;
            msg.msg.opt.len = 5;
            msg.msg.cmd = 0x5a;
            msg.data[0] = 'H';
            msg.data[1] = 'e';
            msg.data[2] = 'l';
            msg.data[3] = 'l';
            msg.data[4] = 'o';
            sboxnet_send_msg(&msg);
        }
/*
        memset(&msg, 0, sizeof(msg));
        if (sboxnet_receive_msg(&msg) == 0) {
            memcpy(buf, msg.data, 15);
            msg.msg.dstaddr = (g_dev_addr == 11 ? 10 : 11);
            sboxnet_send_msg(&msg);
        }
*/
        if (g_lcd_timedout) {
            struct sboxnet_device *g_sboxnet_dev = &g_sboxnet_dev_def;
            //FORCEPTR(g_sboxnet_dev);
            
            g_lcd_timedout = 0;

            sidetimer++;
            if (sidetimer >= 10) {
                side = !side;
                sidetimer = 0;
                lcd_display_clear();
            }
//side = 0;
            if (side) {
                lcd_set_cursor(0, 0);
                lcd_str_P(PSTR("       "));
                lcd_set_cursor(0, 0);
                lcd_char(buf[0]);
                lcd_char(buf[1]);
                lcd_char(buf[2]);
                lcd_char(buf[3]);
                lcd_char(buf[4]);
                
            } else {
                
                lcd_set_cursor(0, 0);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(0, 0);
                lcd_itoa(g_sboxnet_dev->debug.recv_msg & 0x3f);
                lcd_set_cursor(2, 0);
                lcd_itoa(g_sboxnet_dev->debug.recv_byte & 0x3f);

                lcd_set_cursor(0, 1);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(0, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmit_msg & 0x3f);
                lcd_set_cursor(2, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmit_byte & 0x3f);

                lcd_set_cursor(5, 0);
                lcd_str_P(PSTR("   "));
                lcd_set_cursor(5, 0);
                lcd_itoa(g_sboxnet_dev->debug.collisions & 0x3f);

                lcd_set_cursor(5, 1);
                lcd_str_P(PSTR("   "));
                lcd_set_cursor(5, 1);
                lcd_itoa(g_sboxnet_dev->debug.retries & 0x3f);

                lcd_set_cursor(8, 0);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(8, 0);
                lcd_itoa(g_sboxnet_dev->debug.recverrors & 0x3f);
                lcd_set_cursor(10, 0);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(10, 0);
                lcd_itoa(g_sboxnet_dev->debug.crcerrors & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_fe & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_pe & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_dor & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_proto & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_size & 7);
                
                lcd_set_cursor(8, 1);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(8, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmiterrors & 0xff);
                
                lcd_set_cursor(13, 1);
                lcd_str_P(PSTR("    "));
                lcd_set_cursor(13, 1);
                lcd_itoa(g_sboxnet_dev->debug.coll_rxc & 7);
                lcd_itoa(g_sboxnet_dev->debug.coll_fe  & 7);
                lcd_itoa(g_sboxnet_dev->debug.coll_udr & 7);
            }
            
        }
    }
    return 0;
}

