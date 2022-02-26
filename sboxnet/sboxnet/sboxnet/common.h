
#ifndef _COMMON_H_
#define _COMMON_H_

#if !defined(__AVR_ATxmega32A4__) && !defined(__AVR_ATxmega128A4U__)
# error "please compile for device: ATxmega32A4, ATxmega128A4u"
#endif

#define F_CPU_MHZ 32
#define F_CPU  (F_CPU_MHZ * 1000000)

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

#define SBOXNET_DEBUG_ENABLED 1

#include "avrutilslib/defines.h"
#include "avrutilslib/ringbuffer.h"

#include "sboxnet/sboxnet.h"

#define BLDR_VERSION           0x0004

#define _MAGIC(_a,_b,_c,_d) (((uint32_t)(_d) << 24)|((uint32_t)(_c) << 16)|((uint32_t)(_b) << 8)|(_a))
#define MAGIC_BOOTLOADER    _MAGIC('S','b', 'n', 'B')
#define MAGIC_APPLICATION   _MAGIC('S','b', 'n', 'A')

struct firmware_header {
    uint32_t magic;
    uint16_t productid;
    uint16_t vendorid;
    uint16_t bootloader_version;
    uint16_t app_version;
    uint8_t reserved[20];
};

#define _FIRMWARE_HEADER(_magic, _productid, _vendorid, _bldrversion, _appversion)     __asm__ __volatile__ ( \
        ".long %[magic]\n" \
        ".word %[productid]\n" \
        ".word %[vendorid]\n" \
        ".word %[bldrversion]\n" \
        ".word %[appversion]\n" \
        ".zero 20\n" \
      : \
      : [magic] "i" (_magic), \
        [productid] "i" (_productid), \
        [vendorid] "i" (_vendorid), \
        [bldrversion] "i" (_bldrversion), \
        [appversion] "i" (_appversion) \
    )

#define APP_FIRMWARE_HEADER(_productid, _vendorid, _appversion) \
     __attribute__ ((section(".fwheader"))) __ATTR_NAKED __ATTR_NO_UNUSED_WARN void firmware_header(void) { \
        _FIRMWARE_HEADER(MAGIC_APPLICATION, _productid, _vendorid, BLDR_VERSION, _appversion); \
     }


enum Register_t {
    R_PUID_L = 0,
    R_PUID_H = 1,
    R_PRODUCTID = 2,
    R_VENDORID = 3,
    R_BOOTLOADER_VERSION = 4,
    R_PROTOKOLL_VERSION = 5,
    R_DEV_ADDR = 6,
    R_DEV_STATE = 7,
    R_DEV_ERROR = 8,
    R_DEV_BOOTSTATE = 9,
    R_DEV_NUM_DESCS = 10,
    R_FIRMWARE_VERSION = 11,
    R_CAPABILITIES = 12,
    R_CAP_CLASS = 13,
        
    R_FB_NUM    = 20,
    R_FB_VALUE0 = 21,
    R_FB_VALUE1 = 22,
    R_FB_VALUE2 = 23,
    R_FB_VALUE3 = 24,
    
    R_CNTRL_NUM  = 30,
    R_CNTRL_VALUE0 = 31,
    R_CNTRL_VALUE1 = 32,
    R_CNTRL_VALUE2 = 33,
    R_CNTRL_VALUE3 = 34,
    R_CNTRL_PENDING0 = 35,
    R_CNTRL_PENDING1 = 36,
    R_CNTRL_PENDING2 = 37,
    R_CNTRL_PENDING3 = 38,
    R_CNTRL_TURNOUT_CAP = 39,
    
    R_ADCVAL_NUM = 40,
    R_ADCVAL_0 = 41,
    R_ADCVAL_1 = 42,
    R_ADCVAL_2 = 43,
    R_ADCVAL_3 = 44,
    R_ADCVAL_4 = 45,
    R_ADCVAL_5 = 46,
    R_ADCVAL_6 = 47,
    R_ADCVAL_7 = 48,
            
    ROFFS_TURNOUT_PAR_MINV = 0,
    ROFFS_TURNOUT_PAR_MAXV = 1,
    ROFFS_TURNOUT_PAR_MOVETIME = 2,
    NUM_TURNOUT_PARS = 8,
    NUM_TURNOUT_PARS_MASK = 0x07,
   
    R_TURNOUT_PAR_0 = 64, // must start at a 8 byte boundary
    R_TURNOUT_PAR_1 = R_TURNOUT_PAR_0+1*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_2 = R_TURNOUT_PAR_0+2*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_3 = R_TURNOUT_PAR_0+3*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_4 = R_TURNOUT_PAR_0+4*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_5 = R_TURNOUT_PAR_0+5*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_6 = R_TURNOUT_PAR_0+6*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_7 = R_TURNOUT_PAR_0+7*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_8 = R_TURNOUT_PAR_0+8*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_9 = R_TURNOUT_PAR_0+9*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_10 = R_TURNOUT_PAR_0+10*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_11 = R_TURNOUT_PAR_0+11*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_12 = R_TURNOUT_PAR_0+12*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_13 = R_TURNOUT_PAR_0+13*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_14 = R_TURNOUT_PAR_0+14*NUM_TURNOUT_PARS,
    R_TURNOUT_PAR_15 = R_TURNOUT_PAR_0+15*NUM_TURNOUT_PARS,
    
    R_BOOSTER_FLAGS = 200,
    R_BOOSTER_SHORTCUT_LIMIT = 201,
    R_BOOSTER_SHORTCUT_INTERVAL = 202,
    R_BOOSTER_SHORTCUT_CNT = 203,

    R_DCCGEN_FLAGS = 210,
    R_DCCGEN_NUM_LOCOS = 211,
    R_DCCGEN_LOCOADDR_SCAN_MAX = 212,
    R_DCCGEN_LOCOADDR_SCAN_CUR = 213,
    
    NUM_GBM_PARS = 8,
    NUM_GBM_PARS_MASK = 0x7,
    ROFFS_GBM_HOLDTIME = 0,
    ROFFS_GBM_LOCOADDR = 1,
    
    R_GBM_HOLDTIME = 255,
    R_GBM_PAR_0 = 256,  // must start at a 8 byte boundary
    R_GBM_PAR_1 = R_GBM_PAR_0 + 1*NUM_GBM_PARS,
    R_GBM_PAR_2 = R_GBM_PAR_0 + 2*NUM_GBM_PARS,
    R_GBM_PAR_3 = R_GBM_PAR_0 + 3*NUM_GBM_PARS,
    R_GBM_PAR_4 = R_GBM_PAR_0 + 4*NUM_GBM_PARS,
    R_GBM_PAR_5 = R_GBM_PAR_0 + 5*NUM_GBM_PARS,
    R_GBM_PAR_6 = R_GBM_PAR_0 + 6*NUM_GBM_PARS,
    R_GBM_PAR_7 = R_GBM_PAR_0 + 7*NUM_GBM_PARS,
    R_GBM_PAR_8 = R_GBM_PAR_0 + 8*NUM_GBM_PARS,
    R_GBM_PAR_9 = R_GBM_PAR_0 + 9*NUM_GBM_PARS,
    R_GBM_PAR_10 = R_GBM_PAR_0 + 10*NUM_GBM_PARS,
    R_GBM_PAR_11 = R_GBM_PAR_0 + 11*NUM_GBM_PARS,
    R_GBM_PAR_12 = R_GBM_PAR_0 + 12*NUM_GBM_PARS,
    R_GBM_PAR_13 = R_GBM_PAR_0 + 13*NUM_GBM_PARS,
    R_GBM_PAR_14 = R_GBM_PAR_0 + 14*NUM_GBM_PARS,
    R_GBM_PAR_15 = R_GBM_PAR_0 + 15*NUM_GBM_PARS,

    R_DEBUG_BLDR_RAM_OFFS    = 2000,
    R_DEBUG_COM_RAM_OFFS     = 2001,
    R_DEBUG_SBN_RAM_OFFS     = 2002,
    R_DEBUG_BLDR_EEPROM_OFFS = 2003,
    R_DEBUG_SRAM_SIZE        = 2004,
    R_DEBUG_EEPROM_SIZE      = 2005,
    R_DEBUG_STACK_SIZE       = 2006,
    R_DEBUG_STACK_FREE       = 2007,

    
    R_DEBUG_SBN_0            = 2010,
    NUM_R_DEBUG_SBN          = 32,
    
    R_DEBUG_SBN_STATE        = 2042,
    R_DEBUG_SBN_FLAGS        = 2043,
    
    R_DEBUG_EEPROM_BASE = 2048,
    NUM_R_DEBUG_EEPROM  = 2048,
    R_DEBUG_SRAM_BASE   = 4096,
    NUM_R_DEBUG_SRAM    = 4096,
};

#define CAP_DCC_GENERATOR 0x0001
#define CAP_DCC_BOOSTER   0x0002
#define CAP_FB_GENERIC    0x0004
#define CAP_FB_OCCUPANCY  0x0008
#define CAP_CNTRL_GENERIC 0x0010
#define CAP_CNTRL_TURNOUT 0x0020
#define CAP_TESTER        0x0040

#define DEV_DESC_MAX_SIZE   32
#define DEV_NUM_DESCS       3

struct bldr_eeprom_t {
    uint8_t     firmware_update;
    uint32_t    puid;
    uint16_t    productid;
    uint16_t    vendorid;
    uint8_t     reserved1[64-9];
    // align on eeprom page size (32bytes)
    uint8_t     dev_desc[DEV_NUM_DESCS][DEV_DESC_MAX_SIZE];
};

#define BLDR_EEPROM_ADDR  0x300
#define bldr_eeprom  (*(struct bldr_eeprom_t*)BLDR_EEPROM_ADDR)

#define WATCHDOG_VAL   128 // 0.5s

    // device state flags
#define DEV_STATE_FLG_BOOTLOADER_b         0
#define DEV_STATE_FLG_FWUP_BOOTLOADER_b    1
#define DEV_STATE_FLG_FWUP_APPLICATION_b   2
#define DEV_STATE_FLG_FWUP_MASK            0x06
#define DEV_STATE_FLG_FWUP_BOOTLOADER      0x02
#define DEV_STATE_FLG_FWUP_APPLICATION     0x04
#define DEV_STATE_FLG_REQ_ADDR_b           3
#define DEV_STATE_FLG_IDENTIFY_b           4
#define DEV_STATE_FLG_WATCHDOG_b           5
#define DEV_STATE_FLG_RESET_b           6

// bit manipulation with setbit/clrbit result in atomic sbi/cbi asm instructions !
//#define g_dev_state    GPIO_GPIOR0
extern uint8_t g_dev_state;

// device error flags
#define DEV_ERR_FLG_ERROR_b      0
#define DEV_ERR_FLG_SBOXNETCRC_b 1
#define DEV_ERR_FLG_TEST_b 4
#define DEV_ERR_FLG_32MHZOSC_b   7

// values for g_v.module
#define MODULE_TESTER 1
#define MODULE_GBM    2
#define MODULE_WS     3
#define MODULE_DCC    4
#define MODULE_BOOSTER 5
#define MODULE_OTHER  0xe

// bit manipulation with setbit/clrbit result in atomic sbi/cbi asm instructions !
//#define g_dev_errflags GPIO_GPIOR1
extern uint8_t g_dev_errflags;

// GBM Sensor
struct gbm_sensor_t {
	// 4 Bytes
	uint16_t holdtime;
	uint16_t reserved;
};
// GBM EEPROM
struct gbm_eeprom_t {
	//2+16*4=66 Bytes 
	uint16_t  holdtime;
	struct gbm_sensor_t sensors[16];
};
struct ws_Servo_eeprom_t {
	// 8 Bytes
	uint16_t minv; // Wert fuer Min Ausschlag in Prozent * 10
	uint16_t maxv; // Wert fuer Max Ausschlag in Prozent * 10
	uint16_t movetime; // Bewegungszeit in us
	uint16_t reserved;
};
struct ws_eeprom_t {
	// 8x8=64 Bytes
	struct ws_Servo_eeprom_t servo[8];
};
struct dcc_eeprom_t {
	// 2 Bytes
	uint16_t locoaddr_scan_max;
};
struct booster_eeprom_t {
	// 4 Bytes
	uint16_t shortcut_limit;
	uint16_t shortcut_interval;
};
// Struktur im EEPROM
// EEPROM atxmega128a4u bei 0x1000
struct Eeprom_t {
	// +0 0x1000+0
	struct gbm_eeprom_t gbm;
	// +66 0x1000+0x42
	struct ws_eeprom_t ws;
	// +130 0x1000+0x82
	struct dcc_eeprom_t dcc;
	// +132 0x1000+0x84
	struct booster_eeprom_t booster;
	// +136 0x1000+0x88
};
#define EEPROM_OFFS_GBM 0x0	// 0
#define EEPROM_OFFS_WS offsetof(struct Eeprom_t, ws) // 66
#define EEPROM_OFFS_DCC offsetof(struct Eeprom_t, dcc) // 130
#define EEPROM_OFFS_BOOSTER offsetof(struct Eeprom_t, booster) // 132


//forward decls
void com_sched_init_system(void);
uint8_t com_sched_do_msg(struct sboxnet_msg_header *pmsg);
void com_sched_do_setup();
void com_sched_do_main(void);
void com_sched_do_before_bldr_activate(void);
uint8_t com_sched_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t com_sched_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);

// do_init_system
void mtester_do_init_system(void);
void gbm_do_init_system(void);
void ws_do_init_system(void);
void dg_do_init_system(void);
void bo_do_init_system(void);
// do_msg
uint8_t mtester_do_msg(struct sboxnet_msg_header *pmsg);
uint8_t gbm_do_msg(struct sboxnet_msg_header *pmsg);
uint8_t ws_do_msg(struct sboxnet_msg_header *pmsg);
uint8_t dg_do_msg(struct sboxnet_msg_header *pmsg);
uint8_t bo_do_msg(struct sboxnet_msg_header *pmsg);
// do_setup
void mtester_do_setup(void);
void gbm_do_setup(void);
void ws_do_setup(void);
void dg_do_setup(void);
void bo_do_setup(void);
// do_main
void mtester_do_main(void);
void gbm_do_main(void);
void ws_do_main(void);
void dg_do_main(void);
void bo_do_main(void);
// do_before_bldr_activate(void)
void mtester_do_before_bldr_activate(void);
void gbm_do_before_bldr_activate(void);
void ws_do_before_bldr_activate(void);
void dg_do_before_bldr_activate(void);
void bo_do_before_bldr_activate(void);
// uint8_t do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t mtester_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t gbm_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t ws_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t dg_do_reg_read(uint16_t reg, uint16_t* pdata);
uint8_t bo_do_reg_read(uint16_t reg, uint16_t* pdata);
//  uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
uint8_t mtester_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
uint8_t gbm_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
uint8_t ws_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
uint8_t dg_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
uint8_t bo_do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);


#define setbit_atomic(_reg, _bitnr) \
    ({ if (&(_reg) <= (uint8_t*)31) { \
           setbit(_reg, _bitnr); \
       } else { \
           ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { setbit(_reg, _bitnr); } \
       } \
    })
#define clrbit_atomic(_reg, _bitnr) \
    ({ if (&(_reg) <= (uint8_t*)31) { \
           clrbit(_reg, _bitnr); \
       } else { \
           ATOMIC_BLOCK(ATOMIC_RESTORESTATE) { clrbit(_reg, _bitnr); } \
       } \
    })
    
struct bldr_ram {
    uint8_t dev_addr;

    uint32_t dev_puid;
    
    volatile uint8_t  timer;
    volatile uint8_t  timer_logon;
    volatile uint8_t  timer_watchdog;
    volatile uint8_t  timer_leds;
    volatile uint8_t  timer_keys;
    
    uint8_t  led_gn;
    uint8_t  led_rt;
    volatile uint8_t dev_old_state;
    
    uint8_t  key_id;
    uint8_t  key_id_t;
    uint8_t  key_id_state;
    
    uint16_t fwup_pageaddr;
    uint16_t fwup_pageoffs;
    uint16_t fwup_size;
    uint16_t fwup_crc;
    
    uint16_t prandom;
	uint16_t is_a4u;
	
	uint16_t module; 
    
    struct sboxnet_device sboxnet;
};


struct timer {
    struct timer* next;
    int8_t        value;
};

struct com_ram {
    uint16_t  productid;
    uint16_t  vendorid;
    uint16_t  firmware_version;
    uint16_t  capabilities;
    uint16_t  cap_class;
    PGM_P     dev_desc_P;
    
    uint8_t   timer;
    struct timer *timer_1ms;
    struct timer *timer_16ms;
};

extern struct com_ram g_com;

#define BLDR_RAM_START (INTERNAL_SRAM_END-1 )//(INTERNAL_SRAM_END - sizeof(struct bldr_ram) - 1)
#define BLDR_STACK_TOP (BLDR_RAM_START - 1)
extern struct bldr_ram g_v;
//#define g_v (*(struct bldr_ram*)BLDR_RAM_START)

#define nvmc_wait_busy()  ({ while (bit_is_set(NVM_STATUS, NVM_NVMBUSY_bp)); })

#define e2prom_wait_busy()  nvmc_wait_busy()

#define e2prom_get_byte(_p) ({ \
    e2prom_wait_busy(); \
    setbit(NVM_CTRLB, NVM_EEMAPEN_bp); \
    *((uint8_t*)(MAPPED_EEPROM_START + (uint16_t)(_p))); })
    
#define e2prom_get_word(_p) ({ \
    e2prom_wait_busy(); \
    setbit(NVM_CTRLB, NVM_EEMAPEN_bp); \
    *((uint16_t*)(MAPPED_EEPROM_START + (uint16_t)(_p))); })

#define e2prom_get_long(_p) ({ \
    e2prom_wait_busy(); \
    setbit(NVM_CTRLB, NVM_EEMAPEN_bp); \
    *((uint32_t*)(MAPPED_EEPROM_START + (uint16_t)(_p))); })


#define TIMER_RESOLUTION_1MS  0
#define TIMER_RESOLUTION_16MS 1

static INLINE void timer_register(struct timer* t, uint8_t resolution) {
    if (resolution == TIMER_RESOLUTION_16MS) {
        t->next = g_com.timer_16ms;
        g_com.timer_16ms = t;
    } else {
        t->next = g_com.timer_1ms;
        g_com.timer_1ms = t;
    }
    t->value = 0;
}

static INLINE uint8_t timer_timedout(struct timer* t) {
    return t->value == 0;
}

static INLINE void timer_set(struct timer* t, int8_t value) {
    t->value = value;
}




struct pipe {
    struct ring_buffer rb;
};
typedef struct pipe pipe_t;

static INLINE void pipe_init(pipe_t* p, uint8_t size) {
    ringbuf_init(&p->rb, size);
}

static INLINE uint8_t pipe_count(pipe_t* p) {
    return ringbuf_count(&p->rb);
}

static INLINE uint8_t pipe_can_read(pipe_t* p) {
    return !ringbuf_isempty(&p->rb);
}

static INLINE uint8_t pipe_read(pipe_t* p) {
    return ringbuf_read(&p->rb);
}

static INLINE uint8_t pipe_can_write(pipe_t* p) {
    return !ringbuf_isfull(&p->rb);
}

static INLINE uint8_t pipe_getfree(pipe_t* p) {
    return ringbuf_getfree(&p->rb);
}

static INLINE void pipe_write(pipe_t* p, uint8_t v) {
    ringbuf_write(&p->rb, v);
}


static INLINE void debounce_8(uint8_t* k, uint8_t* k_t, uint8_t newk) {
    uint8_t newk_save = newk;
    
    uint8_t changed = newk ^ *k_t;      // XOR: 1 means: changed since last query
    
    *k &= changed;        // transfer unchanged button bits into keys: clear unchanged button bits
    *k |= (newk_save & ~changed);
    
    *k_t = newk_save;   // time shift
}

static INLINE void debounce_16(uint16_t* k, uint16_t* k_t, uint16_t newk) {
    uint16_t newk_save = newk;
    
    uint16_t changed = newk ^ *k_t;      // XOR: 1 means: changed since last query
    
    *k &= changed;        // transfer unchanged button bits into keys: clear unchanged button bits
    *k |= (newk_save & ~changed);
    
    *k_t = newk_save;   // time shift
}


static INLINE void debounce_keys(uint8_t* k, uint8_t* k_t, uint8_t newk) {
    debounce_8(k, k_t, newk);
}

extern void bldr_init_modules(void);
extern void bldr_init_vars(void);
extern void bldr_activate(void);
extern uint8_t bldr_task(struct sboxnet_msg_max* pmsg);
extern uint8_t bldr_reg_read(uint16_t reg, uint16_t *pdata);
extern uint8_t bldr_process_basic_msg(struct sboxnet_msg_header *pmsg);
extern void sbldr_e2prom_write(uint8_t* addr, uint8_t v);

#ifndef BOOTLOADER
extern void sbn_init(void);
extern uint8_t sbn_all_sent(void);
extern uint8_t sbn_send_msg(struct sboxnet_msg_header* pmsg);
extern int8_t sbn_receive_msg(struct sboxnet_msg_header* pmsg, uint8_t maxmsglen);
static INLINE void sboxnet_init(void) { sbn_init(); }
static INLINE uint8_t sboxnet_all_sent(void) { return sbn_all_sent(); }
static INLINE uint8_t sboxnet_send_msg(struct sboxnet_msg_header* pmsg) { return sbn_send_msg(pmsg); }
static INLINE int8_t sboxnet_receive_msg(struct sboxnet_msg_header* pmsg, uint8_t maxmsglen) { return sbn_receive_msg(pmsg, maxmsglen); }

extern uint8_t bitmask(uint8_t bit);

extern void do_init_system(void);
extern uint8_t do_msg(struct sboxnet_msg_header *pmsg);
extern void do_setup(void);
extern void do_main(void);
extern void do_before_bldr_activate(void);
extern uint8_t do_reg_read(uint16_t reg, uint16_t* pdata);
extern uint8_t do_reg_write(uint16_t reg, uint16_t data, uint16_t mask);
#endif // BOOTLOADER

#endif //_COMMON_H_
