# 1 "test-sboxnet.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "test-sboxnet.c"







# 1 "/usr/lib/gcc/avr/4.8.3/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/4.8.3/include/stdint.h" 3 4
# 1 "/usr/avr/include/stdint.h" 1 3 4
# 121 "/usr/avr/include/stdint.h" 3 4
typedef signed int int8_t __attribute__((__mode__(__QI__)));
typedef unsigned int uint8_t __attribute__((__mode__(__QI__)));
typedef signed int int16_t __attribute__ ((__mode__ (__HI__)));
typedef unsigned int uint16_t __attribute__ ((__mode__ (__HI__)));
typedef signed int int32_t __attribute__ ((__mode__ (__SI__)));
typedef unsigned int uint32_t __attribute__ ((__mode__ (__SI__)));

typedef signed int int64_t __attribute__((__mode__(__DI__)));
typedef unsigned int uint64_t __attribute__((__mode__(__DI__)));
# 142 "/usr/avr/include/stdint.h" 3 4
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;
# 159 "/usr/avr/include/stdint.h" 3 4
typedef int8_t int_least8_t;




typedef uint8_t uint_least8_t;




typedef int16_t int_least16_t;




typedef uint16_t uint_least16_t;




typedef int32_t int_least32_t;




typedef uint32_t uint_least32_t;







typedef int64_t int_least64_t;






typedef uint64_t uint_least64_t;
# 213 "/usr/avr/include/stdint.h" 3 4
typedef int8_t int_fast8_t;




typedef uint8_t uint_fast8_t;




typedef int16_t int_fast16_t;




typedef uint16_t uint_fast16_t;




typedef int32_t int_fast32_t;




typedef uint32_t uint_fast32_t;







typedef int64_t int_fast64_t;






typedef uint64_t uint_fast64_t;
# 273 "/usr/avr/include/stdint.h" 3 4
typedef int64_t intmax_t;




typedef uint64_t uintmax_t;
# 10 "/usr/lib/gcc/avr/4.8.3/include/stdint.h" 2 3 4
# 9 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/io.h" 1 3
# 99 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/sfr_defs.h" 1 3
# 126 "/usr/avr/include/avr/sfr_defs.h" 3
# 1 "/usr/avr/include/inttypes.h" 1 3
# 77 "/usr/avr/include/inttypes.h" 3
typedef int32_t int_farptr_t;



typedef uint32_t uint_farptr_t;
# 127 "/usr/avr/include/avr/sfr_defs.h" 2 3
# 100 "/usr/avr/include/avr/io.h" 2 3
# 378 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/iox16a4.h" 1 3
# 104 "/usr/avr/include/avr/iox16a4.h" 3
typedef volatile uint8_t register8_t;
typedef volatile uint16_t register16_t;
typedef volatile uint32_t register32_t;
# 154 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct OCD_struct
{
    register8_t OCDR0;
    register8_t OCDR1;
} OCD_t;



typedef enum CCP_enum
{
    CCP_SPM_gc = (0x9D<<0),
    CCP_IOREG_gc = (0xD8<<0),
} CCP_t;
# 176 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct CLK_struct
{
    register8_t CTRL;
    register8_t PSCTRL;
    register8_t LOCK;
    register8_t RTCCTRL;
} CLK_t;
# 191 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct PR_struct
{
    register8_t PRGEN;
    register8_t PRPA;
    register8_t PRPB;
    register8_t PRPC;
    register8_t PRPD;
    register8_t PRPE;
    register8_t PRPF;
} PR_t;


typedef enum CLK_SCLKSEL_enum
{
    CLK_SCLKSEL_RC2M_gc = (0x00<<0),
    CLK_SCLKSEL_RC32M_gc = (0x01<<0),
    CLK_SCLKSEL_RC32K_gc = (0x02<<0),
    CLK_SCLKSEL_XOSC_gc = (0x03<<0),
    CLK_SCLKSEL_PLL_gc = (0x04<<0),
} CLK_SCLKSEL_t;


typedef enum CLK_PSADIV_enum
{
    CLK_PSADIV_1_gc = (0x00<<2),
    CLK_PSADIV_2_gc = (0x01<<2),
    CLK_PSADIV_4_gc = (0x03<<2),
    CLK_PSADIV_8_gc = (0x05<<2),
    CLK_PSADIV_16_gc = (0x07<<2),
    CLK_PSADIV_32_gc = (0x09<<2),
    CLK_PSADIV_64_gc = (0x0B<<2),
    CLK_PSADIV_128_gc = (0x0D<<2),
    CLK_PSADIV_256_gc = (0x0F<<2),
    CLK_PSADIV_512_gc = (0x11<<2),
} CLK_PSADIV_t;


typedef enum CLK_PSBCDIV_enum
{
    CLK_PSBCDIV_1_1_gc = (0x00<<0),
    CLK_PSBCDIV_1_2_gc = (0x01<<0),
    CLK_PSBCDIV_4_1_gc = (0x02<<0),
    CLK_PSBCDIV_2_2_gc = (0x03<<0),
} CLK_PSBCDIV_t;


typedef enum CLK_RTCSRC_enum
{
    CLK_RTCSRC_ULP_gc = (0x00<<1),
    CLK_RTCSRC_TOSC_gc = (0x01<<1),
    CLK_RTCSRC_RCOSC_gc = (0x02<<1),
    CLK_RTCSRC_TOSC32_gc = (0x05<<1),
} CLK_RTCSRC_t;
# 253 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct SLEEP_struct
{
    register8_t CTRL;
} SLEEP_t;


typedef enum SLEEP_SMODE_enum
{
    SLEEP_SMODE_IDLE_gc = (0x00<<1),
    SLEEP_SMODE_PDOWN_gc = (0x02<<1),
    SLEEP_SMODE_PSAVE_gc = (0x03<<1),
    SLEEP_SMODE_STDBY_gc = (0x06<<1),
    SLEEP_SMODE_ESTDBY_gc = (0x07<<1),
} SLEEP_SMODE_t;
# 276 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct OSC_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t XOSCCTRL;
    register8_t XOSCFAIL;
    register8_t RC32KCAL;
    register8_t PLLCTRL;
    register8_t DFLLCTRL;
} OSC_t;


typedef enum OSC_FRQRANGE_enum
{
    OSC_FRQRANGE_04TO2_gc = (0x00<<6),
    OSC_FRQRANGE_2TO9_gc = (0x01<<6),
    OSC_FRQRANGE_9TO12_gc = (0x02<<6),
    OSC_FRQRANGE_12TO16_gc = (0x03<<6),
} OSC_FRQRANGE_t;


typedef enum OSC_XOSCSEL_enum
{
    OSC_XOSCSEL_EXTCLK_gc = (0x00<<0),
    OSC_XOSCSEL_32KHz_gc = (0x02<<0),
    OSC_XOSCSEL_XTAL_256CLK_gc = (0x03<<0),
    OSC_XOSCSEL_XTAL_1KCLK_gc = (0x07<<0),
    OSC_XOSCSEL_XTAL_16KCLK_gc = (0x0B<<0),
} OSC_XOSCSEL_t;


typedef enum OSC_PLLSRC_enum
{
    OSC_PLLSRC_RC2M_gc = (0x00<<6),
    OSC_PLLSRC_RC32M_gc = (0x02<<6),
    OSC_PLLSRC_XOSC_gc = (0x03<<6),
} OSC_PLLSRC_t;
# 322 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct DFLL_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t CALA;
    register8_t CALB;
    register8_t COMP0;
    register8_t COMP1;
    register8_t COMP2;
    register8_t reserved_0x07;
} DFLL_t;
# 342 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct RST_struct
{
    register8_t STATUS;
    register8_t CTRL;
} RST_t;
# 356 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct WDT_struct
{
    register8_t CTRL;
    register8_t WINCTRL;
    register8_t STATUS;
} WDT_t;


typedef enum WDT_PER_enum
{
    WDT_PER_8CLK_gc = (0x00<<2),
    WDT_PER_16CLK_gc = (0x01<<2),
    WDT_PER_32CLK_gc = (0x02<<2),
    WDT_PER_64CLK_gc = (0x03<<2),
    WDT_PER_128CLK_gc = (0x04<<2),
    WDT_PER_256CLK_gc = (0x05<<2),
    WDT_PER_512CLK_gc = (0x06<<2),
    WDT_PER_1KCLK_gc = (0x07<<2),
    WDT_PER_2KCLK_gc = (0x08<<2),
    WDT_PER_4KCLK_gc = (0x09<<2),
    WDT_PER_8KCLK_gc = (0x0A<<2),
} WDT_PER_t;


typedef enum WDT_WPER_enum
{
    WDT_WPER_8CLK_gc = (0x00<<2),
    WDT_WPER_16CLK_gc = (0x01<<2),
    WDT_WPER_32CLK_gc = (0x02<<2),
    WDT_WPER_64CLK_gc = (0x03<<2),
    WDT_WPER_128CLK_gc = (0x04<<2),
    WDT_WPER_256CLK_gc = (0x05<<2),
    WDT_WPER_512CLK_gc = (0x06<<2),
    WDT_WPER_1KCLK_gc = (0x07<<2),
    WDT_WPER_2KCLK_gc = (0x08<<2),
    WDT_WPER_4KCLK_gc = (0x09<<2),
    WDT_WPER_8KCLK_gc = (0x0A<<2),
} WDT_WPER_t;
# 403 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct MCU_struct
{
    register8_t DEVID0;
    register8_t DEVID1;
    register8_t DEVID2;
    register8_t REVID;
    register8_t JTAGUID;
    register8_t reserved_0x05;
    register8_t MCUCR;
    register8_t reserved_0x07;
    register8_t EVSYSLOCK;
    register8_t AWEXLOCK;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
} MCU_t;
# 427 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct PMIC_struct
{
    register8_t STATUS;
    register8_t INTPRI;
    register8_t CTRL;
} PMIC_t;
# 442 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct DMA_CH_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t ADDRCTRL;
    register8_t TRIGSRC;
    __extension__ union { register16_t TRFCNT; struct { register8_t TRFCNTL; register8_t TRFCNTH; }; };
    register8_t REPCNT;
    register8_t reserved_0x07;
    register8_t SRCADDR0;
    register8_t SRCADDR1;
    register8_t SRCADDR2;
    register8_t reserved_0x0B;
    register8_t DESTADDR0;
    register8_t DESTADDR1;
    register8_t DESTADDR2;
    register8_t reserved_0x0F;
} DMA_CH_t;
# 468 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct DMA_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t reserved_0x02;
    register8_t INTFLAGS;
    register8_t STATUS;
    register8_t reserved_0x05;
    __extension__ union { register16_t TEMP; struct { register8_t TEMPL; register8_t TEMPH; }; };
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    DMA_CH_t CH0;
    DMA_CH_t CH1;
    DMA_CH_t CH2;
    DMA_CH_t CH3;
} DMA_t;


typedef enum DMA_CH_BURSTLEN_enum
{
    DMA_CH_BURSTLEN_1BYTE_gc = (0x00<<0),
    DMA_CH_BURSTLEN_2BYTE_gc = (0x01<<0),
    DMA_CH_BURSTLEN_4BYTE_gc = (0x02<<0),
    DMA_CH_BURSTLEN_8BYTE_gc = (0x03<<0),
} DMA_CH_BURSTLEN_t;


typedef enum DMA_CH_SRCRELOAD_enum
{
    DMA_CH_SRCRELOAD_NONE_gc = (0x00<<6),
    DMA_CH_SRCRELOAD_BLOCK_gc = (0x01<<6),
    DMA_CH_SRCRELOAD_BURST_gc = (0x02<<6),
    DMA_CH_SRCRELOAD_TRANSACTION_gc = (0x03<<6),
} DMA_CH_SRCRELOAD_t;


typedef enum DMA_CH_SRCDIR_enum
{
    DMA_CH_SRCDIR_FIXED_gc = (0x00<<4),
    DMA_CH_SRCDIR_INC_gc = (0x01<<4),
    DMA_CH_SRCDIR_DEC_gc = (0x02<<4),
} DMA_CH_SRCDIR_t;


typedef enum DMA_CH_DESTRELOAD_enum
{
    DMA_CH_DESTRELOAD_NONE_gc = (0x00<<2),
    DMA_CH_DESTRELOAD_BLOCK_gc = (0x01<<2),
    DMA_CH_DESTRELOAD_BURST_gc = (0x02<<2),
    DMA_CH_DESTRELOAD_TRANSACTION_gc = (0x03<<2),
} DMA_CH_DESTRELOAD_t;


typedef enum DMA_CH_DESTDIR_enum
{
    DMA_CH_DESTDIR_FIXED_gc = (0x00<<0),
    DMA_CH_DESTDIR_INC_gc = (0x01<<0),
    DMA_CH_DESTDIR_DEC_gc = (0x02<<0),
} DMA_CH_DESTDIR_t;


typedef enum DMA_CH_TRIGSRC_enum
{
    DMA_CH_TRIGSRC_OFF_gc = (0x00<<0),
    DMA_CH_TRIGSRC_EVSYS_CH0_gc = (0x01<<0),
    DMA_CH_TRIGSRC_EVSYS_CH1_gc = (0x02<<0),
    DMA_CH_TRIGSRC_EVSYS_CH2_gc = (0x03<<0),
    DMA_CH_TRIGSRC_ADCA_CH0_gc = (0x10<<0),
    DMA_CH_TRIGSRC_ADCA_CH1_gc = (0x11<<0),
    DMA_CH_TRIGSRC_ADCA_CH2_gc = (0x12<<0),
    DMA_CH_TRIGSRC_ADCA_CH3_gc = (0x13<<0),
    DMA_CH_TRIGSRC_ADCA_CH4_gc = (0x14<<0),
    DMA_CH_TRIGSRC_DACA_CH0_gc = (0x15<<0),
    DMA_CH_TRIGSRC_DACA_CH1_gc = (0x16<<0),
    DMA_CH_TRIGSRC_ADCB_CH0_gc = (0x20<<0),
    DMA_CH_TRIGSRC_ADCB_CH1_gc = (0x21<<0),
    DMA_CH_TRIGSRC_ADCB_CH2_gc = (0x22<<0),
    DMA_CH_TRIGSRC_ADCB_CH3_gc = (0x23<<0),
    DMA_CH_TRIGSRC_ADCB_CH4_gc = (0x24<<0),
    DMA_CH_TRIGSRC_DACB_CH0_gc = (0x25<<0),
    DMA_CH_TRIGSRC_DACB_CH1_gc = (0x26<<0),
    DMA_CH_TRIGSRC_TCC0_OVF_gc = (0x40<<0),
    DMA_CH_TRIGSRC_TCC0_ERR_gc = (0x41<<0),
    DMA_CH_TRIGSRC_TCC0_CCA_gc = (0x42<<0),
    DMA_CH_TRIGSRC_TCC0_CCB_gc = (0x43<<0),
    DMA_CH_TRIGSRC_TCC0_CCC_gc = (0x44<<0),
    DMA_CH_TRIGSRC_TCC0_CCD_gc = (0x45<<0),
    DMA_CH_TRIGSRC_TCC1_OVF_gc = (0x46<<0),
    DMA_CH_TRIGSRC_TCC1_ERR_gc = (0x47<<0),
    DMA_CH_TRIGSRC_TCC1_CCA_gc = (0x48<<0),
    DMA_CH_TRIGSRC_TCC1_CCB_gc = (0x49<<0),
    DMA_CH_TRIGSRC_SPIC_gc = (0x4A<<0),
    DMA_CH_TRIGSRC_USARTC0_RXC_gc = (0x4B<<0),
    DMA_CH_TRIGSRC_USARTC0_DRE_gc = (0x4C<<0),
    DMA_CH_TRIGSRC_USARTC1_RXC_gc = (0x4E<<0),
    DMA_CH_TRIGSRC_USARTC1_DRE_gc = (0x4F<<0),
    DMA_CH_TRIGSRC_TCD0_OVF_gc = (0x60<<0),
    DMA_CH_TRIGSRC_TCD0_ERR_gc = (0x61<<0),
    DMA_CH_TRIGSRC_TCD0_CCA_gc = (0x62<<0),
    DMA_CH_TRIGSRC_TCD0_CCB_gc = (0x63<<0),
    DMA_CH_TRIGSRC_TCD0_CCC_gc = (0x64<<0),
    DMA_CH_TRIGSRC_TCD0_CCD_gc = (0x65<<0),
    DMA_CH_TRIGSRC_TCD1_OVF_gc = (0x66<<0),
    DMA_CH_TRIGSRC_TCD1_ERR_gc = (0x67<<0),
    DMA_CH_TRIGSRC_TCD1_CCA_gc = (0x68<<0),
    DMA_CH_TRIGSRC_TCD1_CCB_gc = (0x69<<0),
    DMA_CH_TRIGSRC_SPID_gc = (0x6A<<0),
    DMA_CH_TRIGSRC_USARTD0_RXC_gc = (0x6B<<0),
    DMA_CH_TRIGSRC_USARTD0_DRE_gc = (0x6C<<0),
    DMA_CH_TRIGSRC_USARTD1_RXC_gc = (0x6E<<0),
    DMA_CH_TRIGSRC_USARTD1_DRE_gc = (0x6F<<0),
    DMA_CH_TRIGSRC_TCE0_OVF_gc = (0x80<<0),
    DMA_CH_TRIGSRC_TCE0_ERR_gc = (0x81<<0),
    DMA_CH_TRIGSRC_TCE0_CCA_gc = (0x82<<0),
    DMA_CH_TRIGSRC_TCE0_CCB_gc = (0x83<<0),
    DMA_CH_TRIGSRC_TCE0_CCC_gc = (0x84<<0),
    DMA_CH_TRIGSRC_TCE0_CCD_gc = (0x85<<0),
    DMA_CH_TRIGSRC_TCE1_OVF_gc = (0x86<<0),
    DMA_CH_TRIGSRC_TCE1_ERR_gc = (0x87<<0),
    DMA_CH_TRIGSRC_TCE1_CCA_gc = (0x88<<0),
    DMA_CH_TRIGSRC_TCE1_CCB_gc = (0x89<<0),
    DMA_CH_TRIGSRC_SPIE_gc = (0x8A<<0),
    DMA_CH_TRIGSRC_USARTE0_RXC_gc = (0x8B<<0),
    DMA_CH_TRIGSRC_USARTE0_DRE_gc = (0x8C<<0),
    DMA_CH_TRIGSRC_USARTE1_RXC_gc = (0x8E<<0),
    DMA_CH_TRIGSRC_USARTE1_DRE_gc = (0x8F<<0),
    DMA_CH_TRIGSRC_TCF0_OVF_gc = (0xA0<<0),
    DMA_CH_TRIGSRC_TCF0_ERR_gc = (0xA1<<0),
    DMA_CH_TRIGSRC_TCF0_CCA_gc = (0xA2<<0),
    DMA_CH_TRIGSRC_TCF0_CCB_gc = (0xA3<<0),
    DMA_CH_TRIGSRC_TCF0_CCC_gc = (0xA4<<0),
    DMA_CH_TRIGSRC_TCF0_CCD_gc = (0xA5<<0),
    DMA_CH_TRIGSRC_TCF1_OVF_gc = (0xA6<<0),
    DMA_CH_TRIGSRC_TCF1_ERR_gc = (0xA7<<0),
    DMA_CH_TRIGSRC_TCF1_CCA_gc = (0xA8<<0),
    DMA_CH_TRIGSRC_TCF1_CCB_gc = (0xA9<<0),
    DMA_CH_TRIGSRC_SPIF_gc = (0xAA<<0),
    DMA_CH_TRIGSRC_USARTF0_RXC_gc = (0xAB<<0),
    DMA_CH_TRIGSRC_USARTF0_DRE_gc = (0xAC<<0),
    DMA_CH_TRIGSRC_USARTF1_RXC_gc = (0xAE<<0),
    DMA_CH_TRIGSRC_USARTF1_DRE_gc = (0xAF<<0),
} DMA_CH_TRIGSRC_t;


typedef enum DMA_DBUFMODE_enum
{
    DMA_DBUFMODE_DISABLED_gc = (0x00<<2),
    DMA_DBUFMODE_CH01_gc = (0x01<<2),
    DMA_DBUFMODE_CH23_gc = (0x02<<2),
    DMA_DBUFMODE_CH01CH23_gc = (0x03<<2),
} DMA_DBUFMODE_t;


typedef enum DMA_PRIMODE_enum
{
    DMA_PRIMODE_RR0123_gc = (0x00<<0),
    DMA_PRIMODE_CH0RR123_gc = (0x01<<0),
    DMA_PRIMODE_CH01RR23_gc = (0x02<<0),
    DMA_PRIMODE_CH0123_gc = (0x03<<0),
} DMA_PRIMODE_t;


typedef enum DMA_CH_ERRINTLVL_enum
{
    DMA_CH_ERRINTLVL_OFF_gc = (0x00<<2),
    DMA_CH_ERRINTLVL_LO_gc = (0x01<<2),
    DMA_CH_ERRINTLVL_MED_gc = (0x02<<2),
    DMA_CH_ERRINTLVL_HI_gc = (0x03<<2),
} DMA_CH_ERRINTLVL_t;


typedef enum DMA_CH_TRNINTLVL_enum
{
    DMA_CH_TRNINTLVL_OFF_gc = (0x00<<0),
    DMA_CH_TRNINTLVL_LO_gc = (0x01<<0),
    DMA_CH_TRNINTLVL_MED_gc = (0x02<<0),
    DMA_CH_TRNINTLVL_HI_gc = (0x03<<0),
} DMA_CH_TRNINTLVL_t;
# 661 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct EVSYS_struct
{
    register8_t CH0MUX;
    register8_t CH1MUX;
    register8_t CH2MUX;
    register8_t CH3MUX;
    register8_t CH4MUX;
    register8_t CH5MUX;
    register8_t CH6MUX;
    register8_t CH7MUX;
    register8_t CH0CTRL;
    register8_t CH1CTRL;
    register8_t CH2CTRL;
    register8_t CH3CTRL;
    register8_t CH4CTRL;
    register8_t CH5CTRL;
    register8_t CH6CTRL;
    register8_t CH7CTRL;
    register8_t STROBE;
    register8_t DATA;
} EVSYS_t;


typedef enum EVSYS_QDIRM_enum
{
    EVSYS_QDIRM_00_gc = (0x00<<5),
    EVSYS_QDIRM_01_gc = (0x01<<5),
    EVSYS_QDIRM_10_gc = (0x02<<5),
    EVSYS_QDIRM_11_gc = (0x03<<5),
} EVSYS_QDIRM_t;


typedef enum EVSYS_DIGFILT_enum
{
    EVSYS_DIGFILT_1SAMPLE_gc = (0x00<<0),
    EVSYS_DIGFILT_2SAMPLES_gc = (0x01<<0),
    EVSYS_DIGFILT_3SAMPLES_gc = (0x02<<0),
    EVSYS_DIGFILT_4SAMPLES_gc = (0x03<<0),
    EVSYS_DIGFILT_5SAMPLES_gc = (0x04<<0),
    EVSYS_DIGFILT_6SAMPLES_gc = (0x05<<0),
    EVSYS_DIGFILT_7SAMPLES_gc = (0x06<<0),
    EVSYS_DIGFILT_8SAMPLES_gc = (0x07<<0),
} EVSYS_DIGFILT_t;


typedef enum EVSYS_CHMUX_enum
{
    EVSYS_CHMUX_OFF_gc = (0x00<<0),
    EVSYS_CHMUX_RTC_OVF_gc = (0x08<<0),
    EVSYS_CHMUX_RTC_CMP_gc = (0x09<<0),
    EVSYS_CHMUX_ACA_CH0_gc = (0x10<<0),
    EVSYS_CHMUX_ACA_CH1_gc = (0x11<<0),
    EVSYS_CHMUX_ACA_WIN_gc = (0x12<<0),
    EVSYS_CHMUX_ACB_CH0_gc = (0x13<<0),
    EVSYS_CHMUX_ACB_CH1_gc = (0x14<<0),
    EVSYS_CHMUX_ACB_WIN_gc = (0x15<<0),
    EVSYS_CHMUX_ADCA_CH0_gc = (0x20<<0),
    EVSYS_CHMUX_ADCA_CH1_gc = (0x21<<0),
    EVSYS_CHMUX_ADCA_CH2_gc = (0x22<<0),
    EVSYS_CHMUX_ADCA_CH3_gc = (0x23<<0),
    EVSYS_CHMUX_ADCB_CH0_gc = (0x24<<0),
    EVSYS_CHMUX_ADCB_CH1_gc = (0x25<<0),
    EVSYS_CHMUX_ADCB_CH2_gc = (0x26<<0),
    EVSYS_CHMUX_ADCB_CH3_gc = (0x27<<0),
    EVSYS_CHMUX_PORTA_PIN0_gc = (0x50<<0),
    EVSYS_CHMUX_PORTA_PIN1_gc = (0x51<<0),
    EVSYS_CHMUX_PORTA_PIN2_gc = (0x52<<0),
    EVSYS_CHMUX_PORTA_PIN3_gc = (0x53<<0),
    EVSYS_CHMUX_PORTA_PIN4_gc = (0x54<<0),
    EVSYS_CHMUX_PORTA_PIN5_gc = (0x55<<0),
    EVSYS_CHMUX_PORTA_PIN6_gc = (0x56<<0),
    EVSYS_CHMUX_PORTA_PIN7_gc = (0x57<<0),
    EVSYS_CHMUX_PORTB_PIN0_gc = (0x58<<0),
    EVSYS_CHMUX_PORTB_PIN1_gc = (0x59<<0),
    EVSYS_CHMUX_PORTB_PIN2_gc = (0x5A<<0),
    EVSYS_CHMUX_PORTB_PIN3_gc = (0x5B<<0),
    EVSYS_CHMUX_PORTB_PIN4_gc = (0x5C<<0),
    EVSYS_CHMUX_PORTB_PIN5_gc = (0x5D<<0),
    EVSYS_CHMUX_PORTB_PIN6_gc = (0x5E<<0),
    EVSYS_CHMUX_PORTB_PIN7_gc = (0x5F<<0),
    EVSYS_CHMUX_PORTC_PIN0_gc = (0x60<<0),
    EVSYS_CHMUX_PORTC_PIN1_gc = (0x61<<0),
    EVSYS_CHMUX_PORTC_PIN2_gc = (0x62<<0),
    EVSYS_CHMUX_PORTC_PIN3_gc = (0x63<<0),
    EVSYS_CHMUX_PORTC_PIN4_gc = (0x64<<0),
    EVSYS_CHMUX_PORTC_PIN5_gc = (0x65<<0),
    EVSYS_CHMUX_PORTC_PIN6_gc = (0x66<<0),
    EVSYS_CHMUX_PORTC_PIN7_gc = (0x67<<0),
    EVSYS_CHMUX_PORTD_PIN0_gc = (0x68<<0),
    EVSYS_CHMUX_PORTD_PIN1_gc = (0x69<<0),
    EVSYS_CHMUX_PORTD_PIN2_gc = (0x6A<<0),
    EVSYS_CHMUX_PORTD_PIN3_gc = (0x6B<<0),
    EVSYS_CHMUX_PORTD_PIN4_gc = (0x6C<<0),
    EVSYS_CHMUX_PORTD_PIN5_gc = (0x6D<<0),
    EVSYS_CHMUX_PORTD_PIN6_gc = (0x6E<<0),
    EVSYS_CHMUX_PORTD_PIN7_gc = (0x6F<<0),
    EVSYS_CHMUX_PORTE_PIN0_gc = (0x70<<0),
    EVSYS_CHMUX_PORTE_PIN1_gc = (0x71<<0),
    EVSYS_CHMUX_PORTE_PIN2_gc = (0x72<<0),
    EVSYS_CHMUX_PORTE_PIN3_gc = (0x73<<0),
    EVSYS_CHMUX_PORTE_PIN4_gc = (0x74<<0),
    EVSYS_CHMUX_PORTE_PIN5_gc = (0x75<<0),
    EVSYS_CHMUX_PORTE_PIN6_gc = (0x76<<0),
    EVSYS_CHMUX_PORTE_PIN7_gc = (0x77<<0),
    EVSYS_CHMUX_PORTF_PIN0_gc = (0x78<<0),
    EVSYS_CHMUX_PORTF_PIN1_gc = (0x79<<0),
    EVSYS_CHMUX_PORTF_PIN2_gc = (0x7A<<0),
    EVSYS_CHMUX_PORTF_PIN3_gc = (0x7B<<0),
    EVSYS_CHMUX_PORTF_PIN4_gc = (0x7C<<0),
    EVSYS_CHMUX_PORTF_PIN5_gc = (0x7D<<0),
    EVSYS_CHMUX_PORTF_PIN6_gc = (0x7E<<0),
    EVSYS_CHMUX_PORTF_PIN7_gc = (0x7F<<0),
    EVSYS_CHMUX_PRESCALER_1_gc = (0x80<<0),
    EVSYS_CHMUX_PRESCALER_2_gc = (0x81<<0),
    EVSYS_CHMUX_PRESCALER_4_gc = (0x82<<0),
    EVSYS_CHMUX_PRESCALER_8_gc = (0x83<<0),
    EVSYS_CHMUX_PRESCALER_16_gc = (0x84<<0),
    EVSYS_CHMUX_PRESCALER_32_gc = (0x85<<0),
    EVSYS_CHMUX_PRESCALER_64_gc = (0x86<<0),
    EVSYS_CHMUX_PRESCALER_128_gc = (0x87<<0),
    EVSYS_CHMUX_PRESCALER_256_gc = (0x88<<0),
    EVSYS_CHMUX_PRESCALER_512_gc = (0x89<<0),
    EVSYS_CHMUX_PRESCALER_1024_gc = (0x8A<<0),
    EVSYS_CHMUX_PRESCALER_2048_gc = (0x8B<<0),
    EVSYS_CHMUX_PRESCALER_4096_gc = (0x8C<<0),
    EVSYS_CHMUX_PRESCALER_8192_gc = (0x8D<<0),
    EVSYS_CHMUX_PRESCALER_16384_gc = (0x8E<<0),
    EVSYS_CHMUX_PRESCALER_32768_gc = (0x8F<<0),
    EVSYS_CHMUX_TCC0_OVF_gc = (0xC0<<0),
    EVSYS_CHMUX_TCC0_ERR_gc = (0xC1<<0),
    EVSYS_CHMUX_TCC0_CCA_gc = (0xC4<<0),
    EVSYS_CHMUX_TCC0_CCB_gc = (0xC5<<0),
    EVSYS_CHMUX_TCC0_CCC_gc = (0xC6<<0),
    EVSYS_CHMUX_TCC0_CCD_gc = (0xC7<<0),
    EVSYS_CHMUX_TCC1_OVF_gc = (0xC8<<0),
    EVSYS_CHMUX_TCC1_ERR_gc = (0xC9<<0),
    EVSYS_CHMUX_TCC1_CCA_gc = (0xCC<<0),
    EVSYS_CHMUX_TCC1_CCB_gc = (0xCD<<0),
    EVSYS_CHMUX_TCD0_OVF_gc = (0xD0<<0),
    EVSYS_CHMUX_TCD0_ERR_gc = (0xD1<<0),
    EVSYS_CHMUX_TCD0_CCA_gc = (0xD4<<0),
    EVSYS_CHMUX_TCD0_CCB_gc = (0xD5<<0),
    EVSYS_CHMUX_TCD0_CCC_gc = (0xD6<<0),
    EVSYS_CHMUX_TCD0_CCD_gc = (0xD7<<0),
    EVSYS_CHMUX_TCD1_OVF_gc = (0xD8<<0),
    EVSYS_CHMUX_TCD1_ERR_gc = (0xD9<<0),
    EVSYS_CHMUX_TCD1_CCA_gc = (0xDC<<0),
    EVSYS_CHMUX_TCD1_CCB_gc = (0xDD<<0),
    EVSYS_CHMUX_TCE0_OVF_gc = (0xE0<<0),
    EVSYS_CHMUX_TCE0_ERR_gc = (0xE1<<0),
    EVSYS_CHMUX_TCE0_CCA_gc = (0xE4<<0),
    EVSYS_CHMUX_TCE0_CCB_gc = (0xE5<<0),
    EVSYS_CHMUX_TCE0_CCC_gc = (0xE6<<0),
    EVSYS_CHMUX_TCE0_CCD_gc = (0xE7<<0),
    EVSYS_CHMUX_TCE1_OVF_gc = (0xE8<<0),
    EVSYS_CHMUX_TCE1_ERR_gc = (0xE9<<0),
    EVSYS_CHMUX_TCE1_CCA_gc = (0xEC<<0),
    EVSYS_CHMUX_TCE1_CCB_gc = (0xED<<0),
    EVSYS_CHMUX_TCF0_OVF_gc = (0xF0<<0),
    EVSYS_CHMUX_TCF0_ERR_gc = (0xF1<<0),
    EVSYS_CHMUX_TCF0_CCA_gc = (0xF4<<0),
    EVSYS_CHMUX_TCF0_CCB_gc = (0xF5<<0),
    EVSYS_CHMUX_TCF0_CCC_gc = (0xF6<<0),
    EVSYS_CHMUX_TCF0_CCD_gc = (0xF7<<0),
    EVSYS_CHMUX_TCF1_OVF_gc = (0xF8<<0),
    EVSYS_CHMUX_TCF1_ERR_gc = (0xF9<<0),
    EVSYS_CHMUX_TCF1_CCA_gc = (0xFC<<0),
    EVSYS_CHMUX_TCF1_CCB_gc = (0xFD<<0),
} EVSYS_CHMUX_t;
# 839 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct NVM_struct
{
    register8_t ADDR0;
    register8_t ADDR1;
    register8_t ADDR2;
    register8_t reserved_0x03;
    register8_t DATA0;
    register8_t DATA1;
    register8_t DATA2;
    register8_t reserved_0x07;
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t CMD;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t INTCTRL;
    register8_t reserved_0x0E;
    register8_t STATUS;
    register8_t LOCKBITS;
} NVM_t;
# 867 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct NVM_LOCKBITS_struct
{
    register8_t LOCKBITS;
} NVM_LOCKBITS_t;
# 879 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct NVM_FUSES_struct
{
    register8_t FUSEBYTE0;
    register8_t FUSEBYTE1;
    register8_t FUSEBYTE2;
    register8_t reserved_0x03;
    register8_t FUSEBYTE4;
    register8_t FUSEBYTE5;
} NVM_FUSES_t;
# 896 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct NVM_PROD_SIGNATURES_struct
{
    register8_t RCOSC2M;
    register8_t reserved_0x01;
    register8_t RCOSC32K;
    register8_t RCOSC32M;
    register8_t reserved_0x04;
    register8_t reserved_0x05;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    register8_t LOTNUM0;
    register8_t LOTNUM1;
    register8_t LOTNUM2;
    register8_t LOTNUM3;
    register8_t LOTNUM4;
    register8_t LOTNUM5;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t WAFNUM;
    register8_t reserved_0x11;
    register8_t COORDX0;
    register8_t COORDX1;
    register8_t COORDY0;
    register8_t COORDY1;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    register8_t ADCACAL0;
    register8_t ADCACAL1;
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t ADCBCAL0;
    register8_t ADCBCAL1;
    register8_t reserved_0x26;
    register8_t reserved_0x27;
    register8_t reserved_0x28;
    register8_t reserved_0x29;
    register8_t reserved_0x2A;
    register8_t reserved_0x2B;
    register8_t reserved_0x2C;
    register8_t reserved_0x2D;
    register8_t TEMPSENSE0;
    register8_t TEMPSENSE1;
    register8_t DACAOFFCAL;
    register8_t DACAGAINCAL;
    register8_t DACBOFFCAL;
    register8_t DACBGAINCAL;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    register8_t reserved_0x36;
    register8_t reserved_0x37;
    register8_t reserved_0x38;
    register8_t reserved_0x39;
    register8_t reserved_0x3A;
    register8_t reserved_0x3B;
    register8_t reserved_0x3C;
    register8_t reserved_0x3D;
    register8_t reserved_0x3E;
} NVM_PROD_SIGNATURES_t;


typedef enum NVM_CMD_enum
{
    NVM_CMD_NO_OPERATION_gc = (0x00<<0),
    NVM_CMD_READ_CALIB_ROW_gc = (0x02<<0),
    NVM_CMD_READ_USER_SIG_ROW_gc = (0x01<<0),
    NVM_CMD_READ_EEPROM_gc = (0x06<<0),
    NVM_CMD_READ_FUSES_gc = (0x07<<0),
    NVM_CMD_WRITE_LOCK_BITS_gc = (0x08<<0),
    NVM_CMD_ERASE_USER_SIG_ROW_gc = (0x18<<0),
    NVM_CMD_WRITE_USER_SIG_ROW_gc = (0x1A<<0),
    NVM_CMD_ERASE_APP_gc = (0x20<<0),
    NVM_CMD_ERASE_APP_PAGE_gc = (0x22<<0),
    NVM_CMD_LOAD_FLASH_BUFFER_gc = (0x23<<0),
    NVM_CMD_WRITE_APP_PAGE_gc = (0x24<<0),
    NVM_CMD_ERASE_WRITE_APP_PAGE_gc = (0x25<<0),
    NVM_CMD_ERASE_FLASH_BUFFER_gc = (0x26<<0),
    NVM_CMD_ERASE_BOOT_PAGE_gc = (0x2A<<0),
    NVM_CMD_WRITE_BOOT_PAGE_gc = (0x2C<<0),
    NVM_CMD_ERASE_WRITE_BOOT_PAGE_gc = (0x2D<<0),
    NVM_CMD_ERASE_EEPROM_gc = (0x30<<0),
    NVM_CMD_ERASE_EEPROM_PAGE_gc = (0x32<<0),
    NVM_CMD_LOAD_EEPROM_BUFFER_gc = (0x33<<0),
    NVM_CMD_WRITE_EEPROM_PAGE_gc = (0x34<<0),
    NVM_CMD_ERASE_WRITE_EEPROM_PAGE_gc = (0x35<<0),
    NVM_CMD_ERASE_EEPROM_BUFFER_gc = (0x36<<0),
    NVM_CMD_APP_CRC_gc = (0x38<<0),
    NVM_CMD_BOOT_CRC_gc = (0x39<<0),
    NVM_CMD_FLASH_RANGE_CRC_gc = (0x3A<<0),
} NVM_CMD_t;


typedef enum NVM_SPMLVL_enum
{
    NVM_SPMLVL_OFF_gc = (0x00<<2),
    NVM_SPMLVL_LO_gc = (0x01<<2),
    NVM_SPMLVL_MED_gc = (0x02<<2),
    NVM_SPMLVL_HI_gc = (0x03<<2),
} NVM_SPMLVL_t;


typedef enum NVM_EELVL_enum
{
    NVM_EELVL_OFF_gc = (0x00<<0),
    NVM_EELVL_LO_gc = (0x01<<0),
    NVM_EELVL_MED_gc = (0x02<<0),
    NVM_EELVL_HI_gc = (0x03<<0),
} NVM_EELVL_t;


typedef enum NVM_BLBB_enum
{
    NVM_BLBB_NOLOCK_gc = (0x03<<6),
    NVM_BLBB_WLOCK_gc = (0x02<<6),
    NVM_BLBB_RLOCK_gc = (0x01<<6),
    NVM_BLBB_RWLOCK_gc = (0x00<<6),
} NVM_BLBB_t;


typedef enum NVM_BLBA_enum
{
    NVM_BLBA_NOLOCK_gc = (0x03<<4),
    NVM_BLBA_WLOCK_gc = (0x02<<4),
    NVM_BLBA_RLOCK_gc = (0x01<<4),
    NVM_BLBA_RWLOCK_gc = (0x00<<4),
} NVM_BLBA_t;


typedef enum NVM_BLBAT_enum
{
    NVM_BLBAT_NOLOCK_gc = (0x03<<2),
    NVM_BLBAT_WLOCK_gc = (0x02<<2),
    NVM_BLBAT_RLOCK_gc = (0x01<<2),
    NVM_BLBAT_RWLOCK_gc = (0x00<<2),
} NVM_BLBAT_t;


typedef enum NVM_LB_enum
{
    NVM_LB_NOLOCK_gc = (0x03<<0),
    NVM_LB_WLOCK_gc = (0x02<<0),
    NVM_LB_RWLOCK_gc = (0x00<<0),
} NVM_LB_t;


typedef enum BOOTRST_enum
{
    BOOTRST_BOOTLDR_gc = (0x00<<6),
    BOOTRST_APPLICATION_gc = (0x01<<6),
} BOOTRST_t;


typedef enum BOD_enum
{
    BOD_INSAMPLEDMODE_gc = (0x01<<0),
    BOD_CONTINOUSLY_gc = (0x02<<0),
    BOD_DISABLED_gc = (0x03<<0),
} BOD_t;


typedef enum WD_enum
{
    WD_8CLK_gc = (0x00<<4),
    WD_16CLK_gc = (0x01<<4),
    WD_32CLK_gc = (0x02<<4),
    WD_64CLK_gc = (0x03<<4),
    WD_128CLK_gc = (0x04<<4),
    WD_256CLK_gc = (0x05<<4),
    WD_512CLK_gc = (0x06<<4),
    WD_1KCLK_gc = (0x07<<4),
    WD_2KCLK_gc = (0x08<<4),
    WD_4KCLK_gc = (0x09<<4),
    WD_8KCLK_gc = (0x0A<<4),
} WD_t;


typedef enum SUT_enum
{
    SUT_0MS_gc = (0x03<<2),
    SUT_4MS_gc = (0x01<<2),
    SUT_64MS_gc = (0x00<<2),
} SUT_t;


typedef enum BODLVL_enum
{
    BODLVL_1V6_gc = (0x07<<0),
    BODLVL_1V9_gc = (0x06<<0),
    BODLVL_2V1_gc = (0x05<<0),
    BODLVL_2V4_gc = (0x04<<0),
    BODLVL_2V6_gc = (0x03<<0),
    BODLVL_2V9_gc = (0x02<<0),
    BODLVL_3V2_gc = (0x01<<0),
} BODLVL_t;
# 1106 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct AC_struct
{
    register8_t AC0CTRL;
    register8_t AC1CTRL;
    register8_t AC0MUXCTRL;
    register8_t AC1MUXCTRL;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t WINCTRL;
    register8_t STATUS;
} AC_t;


typedef enum AC_INTMODE_enum
{
    AC_INTMODE_BOTHEDGES_gc = (0x00<<6),
    AC_INTMODE_FALLING_gc = (0x02<<6),
    AC_INTMODE_RISING_gc = (0x03<<6),
} AC_INTMODE_t;


typedef enum AC_INTLVL_enum
{
    AC_INTLVL_OFF_gc = (0x00<<4),
    AC_INTLVL_LO_gc = (0x01<<4),
    AC_INTLVL_MED_gc = (0x02<<4),
    AC_INTLVL_HI_gc = (0x03<<4),
} AC_INTLVL_t;


typedef enum AC_HYSMODE_enum
{
    AC_HYSMODE_NO_gc = (0x00<<1),
    AC_HYSMODE_SMALL_gc = (0x01<<1),
    AC_HYSMODE_LARGE_gc = (0x02<<1),
} AC_HYSMODE_t;


typedef enum AC_MUXPOS_enum
{
    AC_MUXPOS_PIN0_gc = (0x00<<3),
    AC_MUXPOS_PIN1_gc = (0x01<<3),
    AC_MUXPOS_PIN2_gc = (0x02<<3),
    AC_MUXPOS_PIN3_gc = (0x03<<3),
    AC_MUXPOS_PIN4_gc = (0x04<<3),
    AC_MUXPOS_PIN5_gc = (0x05<<3),
    AC_MUXPOS_PIN6_gc = (0x06<<3),
    AC_MUXPOS_DAC_gc = (0x07<<3),
} AC_MUXPOS_t;


typedef enum AC_MUXNEG_enum
{
    AC_MUXNEG_PIN0_gc = (0x00<<0),
    AC_MUXNEG_PIN1_gc = (0x01<<0),
    AC_MUXNEG_PIN3_gc = (0x02<<0),
    AC_MUXNEG_PIN5_gc = (0x03<<0),
    AC_MUXNEG_PIN7_gc = (0x04<<0),
    AC_MUXNEG_DAC_gc = (0x05<<0),
    AC_MUXNEG_BANDGAP_gc = (0x06<<0),
    AC_MUXNEG_SCALER_gc = (0x07<<0),
} AC_MUXNEG_t;


typedef enum AC_WINTMODE_enum
{
    AC_WINTMODE_ABOVE_gc = (0x00<<2),
    AC_WINTMODE_INSIDE_gc = (0x01<<2),
    AC_WINTMODE_BELOW_gc = (0x02<<2),
    AC_WINTMODE_OUTSIDE_gc = (0x03<<2),
} AC_WINTMODE_t;


typedef enum AC_WINTLVL_enum
{
    AC_WINTLVL_OFF_gc = (0x00<<0),
    AC_WINTLVL_LO_gc = (0x01<<0),
    AC_WINTLVL_MED_gc = (0x02<<0),
    AC_WINTLVL_HI_gc = (0x03<<0),
} AC_WINTLVL_t;


typedef enum AC_WSTATE_enum
{
    AC_WSTATE_ABOVE_gc = (0x00<<6),
    AC_WSTATE_INSIDE_gc = (0x01<<6),
    AC_WSTATE_BELOW_gc = (0x02<<6),
} AC_WSTATE_t;
# 1203 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct ADC_CH_struct
{
    register8_t CTRL;
    register8_t MUXCTRL;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    __extension__ union { register16_t RES; struct { register8_t RESL; register8_t RESH; }; };
    register8_t reserved_0x6;
    register8_t reserved_0x7;
} ADC_CH_t;
# 1221 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct ADC_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t REFCTRL;
    register8_t EVCTRL;
    register8_t PRESCALER;
    register8_t reserved_0x05;
    register8_t INTFLAGS;
    register8_t reserved_0x07;
    register8_t reserved_0x08;
    register8_t reserved_0x09;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    __extension__ union { register16_t CAL; struct { register8_t CALL; register8_t CALH; }; };
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    __extension__ union { register16_t CH0RES; struct { register8_t CH0RESL; register8_t CH0RESH; }; };
    __extension__ union { register16_t CH1RES; struct { register8_t CH1RESL; register8_t CH1RESH; }; };
    __extension__ union { register16_t CH2RES; struct { register8_t CH2RESL; register8_t CH2RESH; }; };
    __extension__ union { register16_t CH3RES; struct { register8_t CH3RESL; register8_t CH3RESH; }; };
    __extension__ union { register16_t CMP; struct { register8_t CMPL; register8_t CMPH; }; };
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    ADC_CH_t CH0;
    ADC_CH_t CH1;
    ADC_CH_t CH2;
    ADC_CH_t CH3;
} ADC_t;


typedef enum ADC_CH_MUXPOS_enum
{
    ADC_CH_MUXPOS_PIN0_gc = (0x00<<3),
    ADC_CH_MUXPOS_PIN1_gc = (0x01<<3),
    ADC_CH_MUXPOS_PIN2_gc = (0x02<<3),
    ADC_CH_MUXPOS_PIN3_gc = (0x03<<3),
    ADC_CH_MUXPOS_PIN4_gc = (0x04<<3),
    ADC_CH_MUXPOS_PIN5_gc = (0x05<<3),
    ADC_CH_MUXPOS_PIN6_gc = (0x06<<3),
    ADC_CH_MUXPOS_PIN7_gc = (0x07<<3),
} ADC_CH_MUXPOS_t;


typedef enum ADC_CH_MUXINT_enum
{
    ADC_CH_MUXINT_TEMP_gc = (0x00<<3),
    ADC_CH_MUXINT_BANDGAP_gc = (0x01<<3),
    ADC_CH_MUXINT_SCALEDVCC_gc = (0x02<<3),
    ADC_CH_MUXINT_DAC_gc = (0x03<<3),
} ADC_CH_MUXINT_t;


typedef enum ADC_CH_MUXNEG_enum
{
    ADC_CH_MUXNEG_PIN0_gc = (0x00<<0),
    ADC_CH_MUXNEG_PIN1_gc = (0x01<<0),
    ADC_CH_MUXNEG_PIN2_gc = (0x02<<0),
    ADC_CH_MUXNEG_PIN3_gc = (0x03<<0),
    ADC_CH_MUXNEG_PIN4_gc = (0x04<<0),
    ADC_CH_MUXNEG_PIN5_gc = (0x05<<0),
    ADC_CH_MUXNEG_PIN6_gc = (0x06<<0),
    ADC_CH_MUXNEG_PIN7_gc = (0x07<<0),
} ADC_CH_MUXNEG_t;


typedef enum ADC_CH_INPUTMODE_enum
{
    ADC_CH_INPUTMODE_INTERNAL_gc = (0x00<<0),
    ADC_CH_INPUTMODE_SINGLEENDED_gc = (0x01<<0),
    ADC_CH_INPUTMODE_DIFF_gc = (0x02<<0),
    ADC_CH_INPUTMODE_DIFFWGAIN_gc = (0x03<<0),
} ADC_CH_INPUTMODE_t;


typedef enum ADC_CH_GAIN_enum
{
    ADC_CH_GAIN_1X_gc = (0x00<<2),
    ADC_CH_GAIN_2X_gc = (0x01<<2),
    ADC_CH_GAIN_4X_gc = (0x02<<2),
    ADC_CH_GAIN_8X_gc = (0x03<<2),
    ADC_CH_GAIN_16X_gc = (0x04<<2),
    ADC_CH_GAIN_32X_gc = (0x05<<2),
    ADC_CH_GAIN_64X_gc = (0x06<<2),
} ADC_CH_GAIN_t;


typedef enum ADC_RESOLUTION_enum
{
    ADC_RESOLUTION_12BIT_gc = (0x00<<1),
    ADC_RESOLUTION_8BIT_gc = (0x02<<1),
    ADC_RESOLUTION_LEFT12BIT_gc = (0x03<<1),
} ADC_RESOLUTION_t;


typedef enum ADC_REFSEL_enum
{
    ADC_REFSEL_INT1V_gc = (0x00<<4),
    ADC_REFSEL_VCC_gc = (0x01<<4),
    ADC_REFSEL_AREFA_gc = (0x02<<4),
    ADC_REFSEL_AREFB_gc = (0x03<<4),
} ADC_REFSEL_t;


typedef enum ADC_SWEEP_enum
{
    ADC_SWEEP_0_gc = (0x00<<6),
    ADC_SWEEP_01_gc = (0x01<<6),
    ADC_SWEEP_012_gc = (0x02<<6),
    ADC_SWEEP_0123_gc = (0x03<<6),
} ADC_SWEEP_t;


typedef enum ADC_EVSEL_enum
{
    ADC_EVSEL_0123_gc = (0x00<<3),
    ADC_EVSEL_1234_gc = (0x01<<3),
    ADC_EVSEL_2345_gc = (0x02<<3),
    ADC_EVSEL_3456_gc = (0x03<<3),
    ADC_EVSEL_4567_gc = (0x04<<3),
    ADC_EVSEL_567_gc = (0x05<<3),
    ADC_EVSEL_67_gc = (0x06<<3),
    ADC_EVSEL_7_gc = (0x07<<3),
} ADC_EVSEL_t;


typedef enum ADC_EVACT_enum
{
    ADC_EVACT_NONE_gc = (0x00<<0),
    ADC_EVACT_CH0_gc = (0x01<<0),
    ADC_EVACT_CH01_gc = (0x02<<0),
    ADC_EVACT_CH012_gc = (0x03<<0),
    ADC_EVACT_CH0123_gc = (0x04<<0),
    ADC_EVACT_SWEEP_gc = (0x05<<0),
    ADC_EVACT_SYNCHSWEEP_gc = (0x06<<0),
} ADC_EVACT_t;


typedef enum ADC_CH_INTMODE_enum
{
    ADC_CH_INTMODE_COMPLETE_gc = (0x00<<2),
    ADC_CH_INTMODE_BELOW_gc = (0x01<<2),
    ADC_CH_INTMODE_ABOVE_gc = (0x03<<2),
} ADC_CH_INTMODE_t;


typedef enum ADC_CH_INTLVL_enum
{
    ADC_CH_INTLVL_OFF_gc = (0x00<<0),
    ADC_CH_INTLVL_LO_gc = (0x01<<0),
    ADC_CH_INTLVL_MED_gc = (0x02<<0),
    ADC_CH_INTLVL_HI_gc = (0x03<<0),
} ADC_CH_INTLVL_t;


typedef enum ADC_DMASEL_enum
{
    ADC_DMASEL_OFF_gc = (0x00<<6),
    ADC_DMASEL_CH01_gc = (0x01<<6),
    ADC_DMASEL_CH012_gc = (0x02<<6),
    ADC_DMASEL_CH0123_gc = (0x03<<6),
} ADC_DMASEL_t;


typedef enum ADC_PRESCALER_enum
{
    ADC_PRESCALER_DIV4_gc = (0x00<<0),
    ADC_PRESCALER_DIV8_gc = (0x01<<0),
    ADC_PRESCALER_DIV16_gc = (0x02<<0),
    ADC_PRESCALER_DIV32_gc = (0x03<<0),
    ADC_PRESCALER_DIV64_gc = (0x04<<0),
    ADC_PRESCALER_DIV128_gc = (0x05<<0),
    ADC_PRESCALER_DIV256_gc = (0x06<<0),
    ADC_PRESCALER_DIV512_gc = (0x07<<0),
} ADC_PRESCALER_t;
# 1409 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct DAC_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t EVCTRL;
    register8_t TIMCTRL;
    register8_t STATUS;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    register8_t GAINCAL;
    register8_t OFFSETCAL;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    __extension__ union { register16_t CH0DATA; struct { register8_t CH0DATAL; register8_t CH0DATAH; }; };
    __extension__ union { register16_t CH1DATA; struct { register8_t CH1DATAL; register8_t CH1DATAH; }; };
} DAC_t;


typedef enum DAC_CHSEL_enum
{
    DAC_CHSEL_SINGLE_gc = (0x00<<5),
    DAC_CHSEL_DUAL_gc = (0x02<<5),
} DAC_CHSEL_t;


typedef enum DAC_REFSEL_enum
{
    DAC_REFSEL_INT1V_gc = (0x00<<3),
    DAC_REFSEL_AVCC_gc = (0x01<<3),
    DAC_REFSEL_AREFA_gc = (0x02<<3),
    DAC_REFSEL_AREFB_gc = (0x03<<3),
} DAC_REFSEL_t;


typedef enum DAC_EVSEL_enum
{
    DAC_EVSEL_0_gc = (0x00<<0),
    DAC_EVSEL_1_gc = (0x01<<0),
    DAC_EVSEL_2_gc = (0x02<<0),
    DAC_EVSEL_3_gc = (0x03<<0),
    DAC_EVSEL_4_gc = (0x04<<0),
    DAC_EVSEL_5_gc = (0x05<<0),
    DAC_EVSEL_6_gc = (0x06<<0),
    DAC_EVSEL_7_gc = (0x07<<0),
} DAC_EVSEL_t;


typedef enum DAC_CONINTVAL_enum
{
    DAC_CONINTVAL_1CLK_gc = (0x00<<4),
    DAC_CONINTVAL_2CLK_gc = (0x01<<4),
    DAC_CONINTVAL_4CLK_gc = (0x02<<4),
    DAC_CONINTVAL_8CLK_gc = (0x03<<4),
    DAC_CONINTVAL_16CLK_gc = (0x04<<4),
    DAC_CONINTVAL_32CLK_gc = (0x05<<4),
    DAC_CONINTVAL_64CLK_gc = (0x06<<4),
    DAC_CONINTVAL_128CLK_gc = (0x07<<4),
} DAC_CONINTVAL_t;


typedef enum DAC_REFRESH_enum
{
    DAC_REFRESH_16CLK_gc = (0x00<<0),
    DAC_REFRESH_32CLK_gc = (0x01<<0),
    DAC_REFRESH_64CLK_gc = (0x02<<0),
    DAC_REFRESH_128CLK_gc = (0x03<<0),
    DAC_REFRESH_256CLK_gc = (0x04<<0),
    DAC_REFRESH_512CLK_gc = (0x05<<0),
    DAC_REFRESH_1024CLK_gc = (0x06<<0),
    DAC_REFRESH_2048CLK_gc = (0x07<<0),
    DAC_REFRESH_4096CLK_gc = (0x08<<0),
    DAC_REFRESH_8192CLK_gc = (0x09<<0),
    DAC_REFRESH_16384CLK_gc = (0x0A<<0),
    DAC_REFRESH_32768CLK_gc = (0x0B<<0),
    DAC_REFRESH_65536CLK_gc = (0x0C<<0),
    DAC_REFRESH_OFF_gc = (0x0F<<0),
} DAC_REFRESH_t;
# 1508 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct RTC_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t INTCTRL;
    register8_t INTFLAGS;
    register8_t TEMP;
    register8_t reserved_0x05;
    register8_t reserved_0x06;
    register8_t reserved_0x07;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t COMP; struct { register8_t COMPL; register8_t COMPH; }; };
} RTC_t;


typedef enum RTC_PRESCALER_enum
{
    RTC_PRESCALER_OFF_gc = (0x00<<0),
    RTC_PRESCALER_DIV1_gc = (0x01<<0),
    RTC_PRESCALER_DIV2_gc = (0x02<<0),
    RTC_PRESCALER_DIV8_gc = (0x03<<0),
    RTC_PRESCALER_DIV16_gc = (0x04<<0),
    RTC_PRESCALER_DIV64_gc = (0x05<<0),
    RTC_PRESCALER_DIV256_gc = (0x06<<0),
    RTC_PRESCALER_DIV1024_gc = (0x07<<0),
} RTC_PRESCALER_t;


typedef enum RTC_COMPINTLVL_enum
{
    RTC_COMPINTLVL_OFF_gc = (0x00<<2),
    RTC_COMPINTLVL_LO_gc = (0x01<<2),
    RTC_COMPINTLVL_MED_gc = (0x02<<2),
    RTC_COMPINTLVL_HI_gc = (0x03<<2),
} RTC_COMPINTLVL_t;


typedef enum RTC_OVFINTLVL_enum
{
    RTC_OVFINTLVL_OFF_gc = (0x00<<0),
    RTC_OVFINTLVL_LO_gc = (0x01<<0),
    RTC_OVFINTLVL_MED_gc = (0x02<<0),
    RTC_OVFINTLVL_HI_gc = (0x03<<0),
} RTC_OVFINTLVL_t;
# 1562 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct EBI_CS_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    __extension__ union { register16_t BASEADDR; struct { register8_t BASEADDRL; register8_t BASEADDRH; }; };
} EBI_CS_t;
# 1576 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct EBI_struct
{
    register8_t CTRL;
    register8_t SDRAMCTRLA;
    register8_t reserved_0x02;
    register8_t reserved_0x03;
    __extension__ union { register16_t REFRESH; struct { register8_t REFRESHL; register8_t REFRESHH; }; };
    __extension__ union { register16_t INITDLY; struct { register8_t INITDLYL; register8_t INITDLYH; }; };
    register8_t SDRAMCTRLB;
    register8_t SDRAMCTRLC;
    register8_t reserved_0x0A;
    register8_t reserved_0x0B;
    register8_t reserved_0x0C;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    EBI_CS_t CS0;
    EBI_CS_t CS1;
    EBI_CS_t CS2;
    EBI_CS_t CS3;
} EBI_t;


typedef enum EBI_CS_ASIZE_enum
{
    EBI_CS_ASIZE_256B_gc = (0x00<<2),
    EBI_CS_ASIZE_512B_gc = (0x01<<2),
    EBI_CS_ASIZE_1KB_gc = (0x02<<2),
    EBI_CS_ASIZE_2KB_gc = (0x03<<2),
    EBI_CS_ASIZE_4KB_gc = (0x04<<2),
    EBI_CS_ASIZE_8KB_gc = (0x05<<2),
    EBI_CS_ASIZE_16KB_gc = (0x06<<2),
    EBI_CS_ASIZE_32KB_gc = (0x07<<2),
    EBI_CS_ASIZE_64KB_gc = (0x08<<2),
    EBI_CS_ASIZE_128KB_gc = (0x09<<2),
    EBI_CS_ASIZE_256KB_gc = (0x0A<<2),
    EBI_CS_ASIZE_512KB_gc = (0x0B<<2),
    EBI_CS_ASIZE_1MB_gc = (0x0C<<2),
    EBI_CS_ASIZE_2MB_gc = (0x0D<<2),
    EBI_CS_ASIZE_4MB_gc = (0x0E<<2),
    EBI_CS_ASIZE_8MB_gc = (0x0F<<2),
    EBI_CS_ASIZE_16M_gc = (0x10<<2),
} EBI_CS_ASIZE_t;


typedef enum EBI_CS_SRWS_enum
{
    EBI_CS_SRWS_0CLK_gc = (0x00<<0),
    EBI_CS_SRWS_1CLK_gc = (0x01<<0),
    EBI_CS_SRWS_2CLK_gc = (0x02<<0),
    EBI_CS_SRWS_3CLK_gc = (0x03<<0),
    EBI_CS_SRWS_4CLK_gc = (0x04<<0),
    EBI_CS_SRWS_5CLK_gc = (0x05<<0),
    EBI_CS_SRWS_6CLK_gc = (0x06<<0),
    EBI_CS_SRWS_7CLK_gc = (0x07<<0),
} EBI_CS_SRWS_t;


typedef enum EBI_CS_MODE_enum
{
    EBI_CS_MODE_DISABLED_gc = (0x00<<0),
    EBI_CS_MODE_SRAM_gc = (0x01<<0),
    EBI_CS_MODE_LPC_gc = (0x02<<0),
    EBI_CS_MODE_SDRAM_gc = (0x03<<0),
} EBI_CS_MODE_t;


typedef enum EBI_CS_SDMODE_enum
{
    EBI_CS_SDMODE_NORMAL_gc = (0x00<<0),
    EBI_CS_SDMODE_LOAD_gc = (0x01<<0),
} EBI_CS_SDMODE_t;


typedef enum EBI_SDDATAW_enum
{
    EBI_SDDATAW_4BIT_gc = (0x00<<6),
    EBI_SDDATAW_8BIT_gc = (0x01<<6),
} EBI_SDDATAW_t;


typedef enum EBI_LPCMODE_enum
{
    EBI_LPCMODE_ALE1_gc = (0x00<<4),
    EBI_LPCMODE_ALE12_gc = (0x02<<4),
} EBI_LPCMODE_t;


typedef enum EBI_SRMODE_enum
{
    EBI_SRMODE_ALE1_gc = (0x00<<2),
    EBI_SRMODE_ALE2_gc = (0x01<<2),
    EBI_SRMODE_ALE12_gc = (0x02<<2),
    EBI_SRMODE_NOALE_gc = (0x03<<2),
} EBI_SRMODE_t;


typedef enum EBI_IFMODE_enum
{
    EBI_IFMODE_DISABLED_gc = (0x00<<0),
    EBI_IFMODE_3PORT_gc = (0x01<<0),
    EBI_IFMODE_4PORT_gc = (0x02<<0),
    EBI_IFMODE_2PORT_gc = (0x03<<0),
} EBI_IFMODE_t;


typedef enum EBI_SDCOL_enum
{
    EBI_SDCOL_8BIT_gc = (0x00<<0),
    EBI_SDCOL_9BIT_gc = (0x01<<0),
    EBI_SDCOL_10BIT_gc = (0x02<<0),
    EBI_SDCOL_11BIT_gc = (0x03<<0),
} EBI_SDCOL_t;


typedef enum EBI_MRDLY_enum
{
    EBI_MRDLY_0CLK_gc = (0x00<<6),
    EBI_MRDLY_1CLK_gc = (0x01<<6),
    EBI_MRDLY_2CLK_gc = (0x02<<6),
    EBI_MRDLY_3CLK_gc = (0x03<<6),
} EBI_MRDLY_t;


typedef enum EBI_ROWCYCDLY_enum
{
    EBI_ROWCYCDLY_0CLK_gc = (0x00<<3),
    EBI_ROWCYCDLY_1CLK_gc = (0x01<<3),
    EBI_ROWCYCDLY_2CLK_gc = (0x02<<3),
    EBI_ROWCYCDLY_3CLK_gc = (0x03<<3),
    EBI_ROWCYCDLY_4CLK_gc = (0x04<<3),
    EBI_ROWCYCDLY_5CLK_gc = (0x05<<3),
    EBI_ROWCYCDLY_6CLK_gc = (0x06<<3),
    EBI_ROWCYCDLY_7CLK_gc = (0x07<<3),
} EBI_ROWCYCDLY_t;


typedef enum EBI_RPDLY_enum
{
    EBI_RPDLY_0CLK_gc = (0x00<<0),
    EBI_RPDLY_1CLK_gc = (0x01<<0),
    EBI_RPDLY_2CLK_gc = (0x02<<0),
    EBI_RPDLY_3CLK_gc = (0x03<<0),
    EBI_RPDLY_4CLK_gc = (0x04<<0),
    EBI_RPDLY_5CLK_gc = (0x05<<0),
    EBI_RPDLY_6CLK_gc = (0x06<<0),
    EBI_RPDLY_7CLK_gc = (0x07<<0),
} EBI_RPDLY_t;


typedef enum EBI_WRDLY_enum
{
    EBI_WRDLY_0CLK_gc = (0x00<<6),
    EBI_WRDLY_1CLK_gc = (0x01<<6),
    EBI_WRDLY_2CLK_gc = (0x02<<6),
    EBI_WRDLY_3CLK_gc = (0x03<<6),
} EBI_WRDLY_t;


typedef enum EBI_ESRDLY_enum
{
    EBI_ESRDLY_0CLK_gc = (0x00<<3),
    EBI_ESRDLY_1CLK_gc = (0x01<<3),
    EBI_ESRDLY_2CLK_gc = (0x02<<3),
    EBI_ESRDLY_3CLK_gc = (0x03<<3),
    EBI_ESRDLY_4CLK_gc = (0x04<<3),
    EBI_ESRDLY_5CLK_gc = (0x05<<3),
    EBI_ESRDLY_6CLK_gc = (0x06<<3),
    EBI_ESRDLY_7CLK_gc = (0x07<<3),
} EBI_ESRDLY_t;


typedef enum EBI_ROWCOLDLY_enum
{
    EBI_ROWCOLDLY_0CLK_gc = (0x00<<0),
    EBI_ROWCOLDLY_1CLK_gc = (0x01<<0),
    EBI_ROWCOLDLY_2CLK_gc = (0x02<<0),
    EBI_ROWCOLDLY_3CLK_gc = (0x03<<0),
    EBI_ROWCOLDLY_4CLK_gc = (0x04<<0),
    EBI_ROWCOLDLY_5CLK_gc = (0x05<<0),
    EBI_ROWCOLDLY_6CLK_gc = (0x06<<0),
    EBI_ROWCOLDLY_7CLK_gc = (0x07<<0),
} EBI_ROWCOLDLY_t;
# 1768 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct TWI_MASTER_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t STATUS;
    register8_t BAUD;
    register8_t ADDR;
    register8_t DATA;
} TWI_MASTER_t;
# 1786 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct TWI_SLAVE_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t STATUS;
    register8_t ADDR;
    register8_t DATA;
    register8_t ADDRMASK;
} TWI_SLAVE_t;
# 1803 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct TWI_struct
{
    register8_t CTRL;
    TWI_MASTER_t MASTER;
    TWI_SLAVE_t SLAVE;
} TWI_t;


typedef enum TWI_MASTER_INTLVL_enum
{
    TWI_MASTER_INTLVL_OFF_gc = (0x00<<6),
    TWI_MASTER_INTLVL_LO_gc = (0x01<<6),
    TWI_MASTER_INTLVL_MED_gc = (0x02<<6),
    TWI_MASTER_INTLVL_HI_gc = (0x03<<6),
} TWI_MASTER_INTLVL_t;


typedef enum TWI_MASTER_TIMEOUT_enum
{
    TWI_MASTER_TIMEOUT_DISABLED_gc = (0x00<<2),
    TWI_MASTER_TIMEOUT_50US_gc = (0x01<<2),
    TWI_MASTER_TIMEOUT_100US_gc = (0x02<<2),
    TWI_MASTER_TIMEOUT_200US_gc = (0x03<<2),
} TWI_MASTER_TIMEOUT_t;


typedef enum TWI_MASTER_CMD_enum
{
    TWI_MASTER_CMD_NOACT_gc = (0x00<<0),
    TWI_MASTER_CMD_REPSTART_gc = (0x01<<0),
    TWI_MASTER_CMD_RECVTRANS_gc = (0x02<<0),
    TWI_MASTER_CMD_STOP_gc = (0x03<<0),
} TWI_MASTER_CMD_t;


typedef enum TWI_MASTER_BUSSTATE_enum
{
    TWI_MASTER_BUSSTATE_UNKNOWN_gc = (0x00<<0),
    TWI_MASTER_BUSSTATE_IDLE_gc = (0x01<<0),
    TWI_MASTER_BUSSTATE_OWNER_gc = (0x02<<0),
    TWI_MASTER_BUSSTATE_BUSY_gc = (0x03<<0),
} TWI_MASTER_BUSSTATE_t;


typedef enum TWI_SLAVE_INTLVL_enum
{
    TWI_SLAVE_INTLVL_OFF_gc = (0x00<<6),
    TWI_SLAVE_INTLVL_LO_gc = (0x01<<6),
    TWI_SLAVE_INTLVL_MED_gc = (0x02<<6),
    TWI_SLAVE_INTLVL_HI_gc = (0x03<<6),
} TWI_SLAVE_INTLVL_t;


typedef enum TWI_SLAVE_CMD_enum
{
    TWI_SLAVE_CMD_NOACT_gc = (0x00<<0),
    TWI_SLAVE_CMD_COMPTRANS_gc = (0x02<<0),
    TWI_SLAVE_CMD_RESPONSE_gc = (0x03<<0),
} TWI_SLAVE_CMD_t;
# 1871 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct PORTCFG_struct
{
    register8_t MPCMASK;
    register8_t reserved_0x01;
    register8_t VPCTRLA;
    register8_t VPCTRLB;
    register8_t CLKEVOUT;
} PORTCFG_t;
# 1887 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct VPORT_struct
{
    register8_t DIR;
    register8_t OUT;
    register8_t IN;
    register8_t INTFLAGS;
} VPORT_t;
# 1902 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct PORT_struct
{
    register8_t DIR;
    register8_t DIRSET;
    register8_t DIRCLR;
    register8_t DIRTGL;
    register8_t OUT;
    register8_t OUTSET;
    register8_t OUTCLR;
    register8_t OUTTGL;
    register8_t IN;
    register8_t INTCTRL;
    register8_t INT0MASK;
    register8_t INT1MASK;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t reserved_0x0F;
    register8_t PIN0CTRL;
    register8_t PIN1CTRL;
    register8_t PIN2CTRL;
    register8_t PIN3CTRL;
    register8_t PIN4CTRL;
    register8_t PIN5CTRL;
    register8_t PIN6CTRL;
    register8_t PIN7CTRL;
} PORT_t;


typedef enum PORTCFG_VP0MAP_enum
{
    PORTCFG_VP0MAP_PORTA_gc = (0x00<<0),
    PORTCFG_VP0MAP_PORTB_gc = (0x01<<0),
    PORTCFG_VP0MAP_PORTC_gc = (0x02<<0),
    PORTCFG_VP0MAP_PORTD_gc = (0x03<<0),
    PORTCFG_VP0MAP_PORTE_gc = (0x04<<0),
    PORTCFG_VP0MAP_PORTF_gc = (0x05<<0),
    PORTCFG_VP0MAP_PORTG_gc = (0x06<<0),
    PORTCFG_VP0MAP_PORTH_gc = (0x07<<0),
    PORTCFG_VP0MAP_PORTJ_gc = (0x08<<0),
    PORTCFG_VP0MAP_PORTK_gc = (0x09<<0),
    PORTCFG_VP0MAP_PORTL_gc = (0x0A<<0),
    PORTCFG_VP0MAP_PORTM_gc = (0x0B<<0),
    PORTCFG_VP0MAP_PORTN_gc = (0x0C<<0),
    PORTCFG_VP0MAP_PORTP_gc = (0x0D<<0),
    PORTCFG_VP0MAP_PORTQ_gc = (0x0E<<0),
    PORTCFG_VP0MAP_PORTR_gc = (0x0F<<0),
} PORTCFG_VP0MAP_t;


typedef enum PORTCFG_VP1MAP_enum
{
    PORTCFG_VP1MAP_PORTA_gc = (0x00<<4),
    PORTCFG_VP1MAP_PORTB_gc = (0x01<<4),
    PORTCFG_VP1MAP_PORTC_gc = (0x02<<4),
    PORTCFG_VP1MAP_PORTD_gc = (0x03<<4),
    PORTCFG_VP1MAP_PORTE_gc = (0x04<<4),
    PORTCFG_VP1MAP_PORTF_gc = (0x05<<4),
    PORTCFG_VP1MAP_PORTG_gc = (0x06<<4),
    PORTCFG_VP1MAP_PORTH_gc = (0x07<<4),
    PORTCFG_VP1MAP_PORTJ_gc = (0x08<<4),
    PORTCFG_VP1MAP_PORTK_gc = (0x09<<4),
    PORTCFG_VP1MAP_PORTL_gc = (0x0A<<4),
    PORTCFG_VP1MAP_PORTM_gc = (0x0B<<4),
    PORTCFG_VP1MAP_PORTN_gc = (0x0C<<4),
    PORTCFG_VP1MAP_PORTP_gc = (0x0D<<4),
    PORTCFG_VP1MAP_PORTQ_gc = (0x0E<<4),
    PORTCFG_VP1MAP_PORTR_gc = (0x0F<<4),
} PORTCFG_VP1MAP_t;


typedef enum PORTCFG_VP2MAP_enum
{
    PORTCFG_VP2MAP_PORTA_gc = (0x00<<0),
    PORTCFG_VP2MAP_PORTB_gc = (0x01<<0),
    PORTCFG_VP2MAP_PORTC_gc = (0x02<<0),
    PORTCFG_VP2MAP_PORTD_gc = (0x03<<0),
    PORTCFG_VP2MAP_PORTE_gc = (0x04<<0),
    PORTCFG_VP2MAP_PORTF_gc = (0x05<<0),
    PORTCFG_VP2MAP_PORTG_gc = (0x06<<0),
    PORTCFG_VP2MAP_PORTH_gc = (0x07<<0),
    PORTCFG_VP2MAP_PORTJ_gc = (0x08<<0),
    PORTCFG_VP2MAP_PORTK_gc = (0x09<<0),
    PORTCFG_VP2MAP_PORTL_gc = (0x0A<<0),
    PORTCFG_VP2MAP_PORTM_gc = (0x0B<<0),
    PORTCFG_VP2MAP_PORTN_gc = (0x0C<<0),
    PORTCFG_VP2MAP_PORTP_gc = (0x0D<<0),
    PORTCFG_VP2MAP_PORTQ_gc = (0x0E<<0),
    PORTCFG_VP2MAP_PORTR_gc = (0x0F<<0),
} PORTCFG_VP2MAP_t;


typedef enum PORTCFG_VP3MAP_enum
{
    PORTCFG_VP3MAP_PORTA_gc = (0x00<<4),
    PORTCFG_VP3MAP_PORTB_gc = (0x01<<4),
    PORTCFG_VP3MAP_PORTC_gc = (0x02<<4),
    PORTCFG_VP3MAP_PORTD_gc = (0x03<<4),
    PORTCFG_VP3MAP_PORTE_gc = (0x04<<4),
    PORTCFG_VP3MAP_PORTF_gc = (0x05<<4),
    PORTCFG_VP3MAP_PORTG_gc = (0x06<<4),
    PORTCFG_VP3MAP_PORTH_gc = (0x07<<4),
    PORTCFG_VP3MAP_PORTJ_gc = (0x08<<4),
    PORTCFG_VP3MAP_PORTK_gc = (0x09<<4),
    PORTCFG_VP3MAP_PORTL_gc = (0x0A<<4),
    PORTCFG_VP3MAP_PORTM_gc = (0x0B<<4),
    PORTCFG_VP3MAP_PORTN_gc = (0x0C<<4),
    PORTCFG_VP3MAP_PORTP_gc = (0x0D<<4),
    PORTCFG_VP3MAP_PORTQ_gc = (0x0E<<4),
    PORTCFG_VP3MAP_PORTR_gc = (0x0F<<4),
} PORTCFG_VP3MAP_t;


typedef enum PORTCFG_CLKOUT_enum
{
    PORTCFG_CLKOUT_OFF_gc = (0x00<<0),
    PORTCFG_CLKOUT_PC7_gc = (0x01<<0),
    PORTCFG_CLKOUT_PD7_gc = (0x02<<0),
    PORTCFG_CLKOUT_PE7_gc = (0x03<<0),
} PORTCFG_CLKOUT_t;


typedef enum PORTCFG_EVOUT_enum
{
    PORTCFG_EVOUT_OFF_gc = (0x00<<4),
    PORTCFG_EVOUT_PC7_gc = (0x01<<4),
    PORTCFG_EVOUT_PD7_gc = (0x02<<4),
    PORTCFG_EVOUT_PE7_gc = (0x03<<4),
} PORTCFG_EVOUT_t;


typedef enum PORT_INT0LVL_enum
{
    PORT_INT0LVL_OFF_gc = (0x00<<0),
    PORT_INT0LVL_LO_gc = (0x01<<0),
    PORT_INT0LVL_MED_gc = (0x02<<0),
    PORT_INT0LVL_HI_gc = (0x03<<0),
} PORT_INT0LVL_t;


typedef enum PORT_INT1LVL_enum
{
    PORT_INT1LVL_OFF_gc = (0x00<<2),
    PORT_INT1LVL_LO_gc = (0x01<<2),
    PORT_INT1LVL_MED_gc = (0x02<<2),
    PORT_INT1LVL_HI_gc = (0x03<<2),
} PORT_INT1LVL_t;


typedef enum PORT_OPC_enum
{
    PORT_OPC_TOTEM_gc = (0x00<<3),
    PORT_OPC_BUSKEEPER_gc = (0x01<<3),
    PORT_OPC_PULLDOWN_gc = (0x02<<3),
    PORT_OPC_PULLUP_gc = (0x03<<3),
    PORT_OPC_WIREDOR_gc = (0x04<<3),
    PORT_OPC_WIREDAND_gc = (0x05<<3),
    PORT_OPC_WIREDORPULL_gc = (0x06<<3),
    PORT_OPC_WIREDANDPULL_gc = (0x07<<3),
} PORT_OPC_t;


typedef enum PORT_ISC_enum
{
    PORT_ISC_BOTHEDGES_gc = (0x00<<0),
    PORT_ISC_RISING_gc = (0x01<<0),
    PORT_ISC_FALLING_gc = (0x02<<0),
    PORT_ISC_LEVEL_gc = (0x03<<0),
    PORT_ISC_INPUT_DISABLE_gc = (0x07<<0),
} PORT_ISC_t;
# 2081 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct TC0_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLE;
    register8_t reserved_0x05;
    register8_t INTCTRLA;
    register8_t INTCTRLB;
    register8_t CTRLFCLR;
    register8_t CTRLFSET;
    register8_t CTRLGCLR;
    register8_t CTRLGSET;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t TEMP;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t reserved_0x24;
    register8_t reserved_0x25;
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CCA; struct { register8_t CCAL; register8_t CCAH; }; };
    __extension__ union { register16_t CCB; struct { register8_t CCBL; register8_t CCBH; }; };
    __extension__ union { register16_t CCC; struct { register8_t CCCL; register8_t CCCH; }; };
    __extension__ union { register16_t CCD; struct { register8_t CCDL; register8_t CCDH; }; };
    register8_t reserved_0x30;
    register8_t reserved_0x31;
    register8_t reserved_0x32;
    register8_t reserved_0x33;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    __extension__ union { register16_t PERBUF; struct { register8_t PERBUFL; register8_t PERBUFH; }; };
    __extension__ union { register16_t CCABUF; struct { register8_t CCABUFL; register8_t CCABUFH; }; };
    __extension__ union { register16_t CCBBUF; struct { register8_t CCBBUFL; register8_t CCBBUFH; }; };
    __extension__ union { register16_t CCCBUF; struct { register8_t CCCBUFL; register8_t CCCBUFH; }; };
    __extension__ union { register16_t CCDBUF; struct { register8_t CCDBUFL; register8_t CCDBUFH; }; };
} TC0_t;
# 2145 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct TC1_struct
{
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t CTRLD;
    register8_t CTRLE;
    register8_t reserved_0x05;
    register8_t INTCTRLA;
    register8_t INTCTRLB;
    register8_t CTRLFCLR;
    register8_t CTRLFSET;
    register8_t CTRLGCLR;
    register8_t CTRLGSET;
    register8_t INTFLAGS;
    register8_t reserved_0x0D;
    register8_t reserved_0x0E;
    register8_t TEMP;
    register8_t reserved_0x10;
    register8_t reserved_0x11;
    register8_t reserved_0x12;
    register8_t reserved_0x13;
    register8_t reserved_0x14;
    register8_t reserved_0x15;
    register8_t reserved_0x16;
    register8_t reserved_0x17;
    register8_t reserved_0x18;
    register8_t reserved_0x19;
    register8_t reserved_0x1A;
    register8_t reserved_0x1B;
    register8_t reserved_0x1C;
    register8_t reserved_0x1D;
    register8_t reserved_0x1E;
    register8_t reserved_0x1F;
    __extension__ union { register16_t CNT; struct { register8_t CNTL; register8_t CNTH; }; };
    register8_t reserved_0x22;
    register8_t reserved_0x23;
    register8_t reserved_0x24;
    register8_t reserved_0x25;
    __extension__ union { register16_t PER; struct { register8_t PERL; register8_t PERH; }; };
    __extension__ union { register16_t CCA; struct { register8_t CCAL; register8_t CCAH; }; };
    __extension__ union { register16_t CCB; struct { register8_t CCBL; register8_t CCBH; }; };
    register8_t reserved_0x2C;
    register8_t reserved_0x2D;
    register8_t reserved_0x2E;
    register8_t reserved_0x2F;
    register8_t reserved_0x30;
    register8_t reserved_0x31;
    register8_t reserved_0x32;
    register8_t reserved_0x33;
    register8_t reserved_0x34;
    register8_t reserved_0x35;
    __extension__ union { register16_t PERBUF; struct { register8_t PERBUFL; register8_t PERBUFH; }; };
    __extension__ union { register16_t CCABUF; struct { register8_t CCABUFL; register8_t CCABUFH; }; };
    __extension__ union { register16_t CCBBUF; struct { register8_t CCBBUFL; register8_t CCBBUFH; }; };
} TC1_t;
# 2209 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct AWEX_struct
{
    register8_t CTRL;
    register8_t reserved_0x01;
    register8_t FDEMASK;
    register8_t FDCTRL;
    register8_t STATUS;
    register8_t reserved_0x05;
    register8_t DTBOTH;
    register8_t DTBOTHBUF;
    register8_t DTLS;
    register8_t DTHS;
    register8_t DTLSBUF;
    register8_t DTHSBUF;
    register8_t OUTOVEN;
} AWEX_t;
# 2233 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct HIRES_struct
{
    register8_t CTRLA;
} HIRES_t;


typedef enum TC_CLKSEL_enum
{
    TC_CLKSEL_OFF_gc = (0x00<<0),
    TC_CLKSEL_DIV1_gc = (0x01<<0),
    TC_CLKSEL_DIV2_gc = (0x02<<0),
    TC_CLKSEL_DIV4_gc = (0x03<<0),
    TC_CLKSEL_DIV8_gc = (0x04<<0),
    TC_CLKSEL_DIV64_gc = (0x05<<0),
    TC_CLKSEL_DIV256_gc = (0x06<<0),
    TC_CLKSEL_DIV1024_gc = (0x07<<0),
    TC_CLKSEL_EVCH0_gc = (0x08<<0),
    TC_CLKSEL_EVCH1_gc = (0x09<<0),
    TC_CLKSEL_EVCH2_gc = (0x0A<<0),
    TC_CLKSEL_EVCH3_gc = (0x0B<<0),
    TC_CLKSEL_EVCH4_gc = (0x0C<<0),
    TC_CLKSEL_EVCH5_gc = (0x0D<<0),
    TC_CLKSEL_EVCH6_gc = (0x0E<<0),
    TC_CLKSEL_EVCH7_gc = (0x0F<<0),
} TC_CLKSEL_t;


typedef enum TC_WGMODE_enum
{
    TC_WGMODE_NORMAL_gc = (0x00<<0),
    TC_WGMODE_FRQ_gc = (0x01<<0),
    TC_WGMODE_SS_gc = (0x03<<0),
    TC_WGMODE_DS_T_gc = (0x05<<0),
    TC_WGMODE_DS_TB_gc = (0x06<<0),
    TC_WGMODE_DS_B_gc = (0x07<<0),
} TC_WGMODE_t;


typedef enum TC_EVACT_enum
{
    TC_EVACT_OFF_gc = (0x00<<5),
    TC_EVACT_CAPT_gc = (0x01<<5),
    TC_EVACT_UPDOWN_gc = (0x02<<5),
    TC_EVACT_QDEC_gc = (0x03<<5),
    TC_EVACT_RESTART_gc = (0x04<<5),
    TC_EVACT_FRW_gc = (0x05<<5),
    TC_EVACT_PW_gc = (0x06<<5),
} TC_EVACT_t;


typedef enum TC_EVSEL_enum
{
    TC_EVSEL_OFF_gc = (0x00<<0),
    TC_EVSEL_CH0_gc = (0x08<<0),
    TC_EVSEL_CH1_gc = (0x09<<0),
    TC_EVSEL_CH2_gc = (0x0A<<0),
    TC_EVSEL_CH3_gc = (0x0B<<0),
    TC_EVSEL_CH4_gc = (0x0C<<0),
    TC_EVSEL_CH5_gc = (0x0D<<0),
    TC_EVSEL_CH6_gc = (0x0E<<0),
    TC_EVSEL_CH7_gc = (0x0F<<0),
} TC_EVSEL_t;


typedef enum TC_ERRINTLVL_enum
{
    TC_ERRINTLVL_OFF_gc = (0x00<<2),
    TC_ERRINTLVL_LO_gc = (0x01<<2),
    TC_ERRINTLVL_MED_gc = (0x02<<2),
    TC_ERRINTLVL_HI_gc = (0x03<<2),
} TC_ERRINTLVL_t;


typedef enum TC_OVFINTLVL_enum
{
    TC_OVFINTLVL_OFF_gc = (0x00<<0),
    TC_OVFINTLVL_LO_gc = (0x01<<0),
    TC_OVFINTLVL_MED_gc = (0x02<<0),
    TC_OVFINTLVL_HI_gc = (0x03<<0),
} TC_OVFINTLVL_t;


typedef enum TC_CCDINTLVL_enum
{
    TC_CCDINTLVL_OFF_gc = (0x00<<6),
    TC_CCDINTLVL_LO_gc = (0x01<<6),
    TC_CCDINTLVL_MED_gc = (0x02<<6),
    TC_CCDINTLVL_HI_gc = (0x03<<6),
} TC_CCDINTLVL_t;


typedef enum TC_CCCINTLVL_enum
{
    TC_CCCINTLVL_OFF_gc = (0x00<<4),
    TC_CCCINTLVL_LO_gc = (0x01<<4),
    TC_CCCINTLVL_MED_gc = (0x02<<4),
    TC_CCCINTLVL_HI_gc = (0x03<<4),
} TC_CCCINTLVL_t;


typedef enum TC_CCBINTLVL_enum
{
    TC_CCBINTLVL_OFF_gc = (0x00<<2),
    TC_CCBINTLVL_LO_gc = (0x01<<2),
    TC_CCBINTLVL_MED_gc = (0x02<<2),
    TC_CCBINTLVL_HI_gc = (0x03<<2),
} TC_CCBINTLVL_t;


typedef enum TC_CCAINTLVL_enum
{
    TC_CCAINTLVL_OFF_gc = (0x00<<0),
    TC_CCAINTLVL_LO_gc = (0x01<<0),
    TC_CCAINTLVL_MED_gc = (0x02<<0),
    TC_CCAINTLVL_HI_gc = (0x03<<0),
} TC_CCAINTLVL_t;


typedef enum TC_CMD_enum
{
    TC_CMD_NONE_gc = (0x00<<2),
    TC_CMD_UPDATE_gc = (0x01<<2),
    TC_CMD_RESTART_gc = (0x02<<2),
    TC_CMD_RESET_gc = (0x03<<2),
} TC_CMD_t;


typedef enum AWEX_FDACT_enum
{
    AWEX_FDACT_NONE_gc = (0x00<<0),
    AWEX_FDACT_CLEAROE_gc = (0x01<<0),
    AWEX_FDACT_CLEARDIR_gc = (0x03<<0),
} AWEX_FDACT_t;


typedef enum HIRES_HREN_enum
{
    HIRES_HREN_NONE_gc = (0x00<<0),
    HIRES_HREN_TC0_gc = (0x01<<0),
    HIRES_HREN_TC1_gc = (0x02<<0),
    HIRES_HREN_BOTH_gc = (0x03<<0),
} HIRES_HREN_t;
# 2384 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct USART_struct
{
    register8_t DATA;
    register8_t STATUS;
    register8_t reserved_0x02;
    register8_t CTRLA;
    register8_t CTRLB;
    register8_t CTRLC;
    register8_t BAUDCTRLA;
    register8_t BAUDCTRLB;
} USART_t;


typedef enum USART_RXCINTLVL_enum
{
    USART_RXCINTLVL_OFF_gc = (0x00<<4),
    USART_RXCINTLVL_LO_gc = (0x01<<4),
    USART_RXCINTLVL_MED_gc = (0x02<<4),
    USART_RXCINTLVL_HI_gc = (0x03<<4),
} USART_RXCINTLVL_t;


typedef enum USART_TXCINTLVL_enum
{
    USART_TXCINTLVL_OFF_gc = (0x00<<2),
    USART_TXCINTLVL_LO_gc = (0x01<<2),
    USART_TXCINTLVL_MED_gc = (0x02<<2),
    USART_TXCINTLVL_HI_gc = (0x03<<2),
} USART_TXCINTLVL_t;


typedef enum USART_DREINTLVL_enum
{
    USART_DREINTLVL_OFF_gc = (0x00<<0),
    USART_DREINTLVL_LO_gc = (0x01<<0),
    USART_DREINTLVL_MED_gc = (0x02<<0),
    USART_DREINTLVL_HI_gc = (0x03<<0),
} USART_DREINTLVL_t;


typedef enum USART_CHSIZE_enum
{
    USART_CHSIZE_5BIT_gc = (0x00<<0),
    USART_CHSIZE_6BIT_gc = (0x01<<0),
    USART_CHSIZE_7BIT_gc = (0x02<<0),
    USART_CHSIZE_8BIT_gc = (0x03<<0),
    USART_CHSIZE_9BIT_gc = (0x07<<0),
} USART_CHSIZE_t;


typedef enum USART_CMODE_enum
{
    USART_CMODE_ASYNCHRONOUS_gc = (0x00<<6),
    USART_CMODE_SYNCHRONOUS_gc = (0x01<<6),
    USART_CMODE_IRDA_gc = (0x02<<6),
    USART_CMODE_MSPI_gc = (0x03<<6),
} USART_CMODE_t;


typedef enum USART_PMODE_enum
{
    USART_PMODE_DISABLED_gc = (0x00<<4),
    USART_PMODE_EVEN_gc = (0x02<<4),
    USART_PMODE_ODD_gc = (0x03<<4),
} USART_PMODE_t;
# 2458 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct SPI_struct
{
    register8_t CTRL;
    register8_t INTCTRL;
    register8_t STATUS;
    register8_t DATA;
} SPI_t;


typedef enum SPI_MODE_enum
{
    SPI_MODE_0_gc = (0x00<<2),
    SPI_MODE_1_gc = (0x01<<2),
    SPI_MODE_2_gc = (0x02<<2),
    SPI_MODE_3_gc = (0x03<<2),
} SPI_MODE_t;


typedef enum SPI_PRESCALER_enum
{
    SPI_PRESCALER_DIV4_gc = (0x00<<0),
    SPI_PRESCALER_DIV16_gc = (0x01<<0),
    SPI_PRESCALER_DIV64_gc = (0x02<<0),
    SPI_PRESCALER_DIV128_gc = (0x03<<0),
} SPI_PRESCALER_t;


typedef enum SPI_INTLVL_enum
{
    SPI_INTLVL_OFF_gc = (0x00<<0),
    SPI_INTLVL_LO_gc = (0x01<<0),
    SPI_INTLVL_MED_gc = (0x02<<0),
    SPI_INTLVL_HI_gc = (0x03<<0),
} SPI_INTLVL_t;
# 2501 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct IRCOM_struct
{
    register8_t CTRL;
    register8_t TXPLCTRL;
    register8_t RXPLCTRL;
} IRCOM_t;


typedef enum IRDA_EVSEL_enum
{
    IRDA_EVSEL_OFF_gc = (0x00<<0),
    IRDA_EVSEL_0_gc = (0x08<<0),
    IRDA_EVSEL_1_gc = (0x09<<0),
    IRDA_EVSEL_2_gc = (0x0A<<0),
    IRDA_EVSEL_3_gc = (0x0B<<0),
    IRDA_EVSEL_4_gc = (0x0C<<0),
    IRDA_EVSEL_5_gc = (0x0D<<0),
    IRDA_EVSEL_6_gc = (0x0E<<0),
    IRDA_EVSEL_7_gc = (0x0F<<0),
} IRDA_EVSEL_t;
# 2530 "/usr/avr/include/avr/iox16a4.h" 3
typedef struct AES_struct
{
    register8_t CTRL;
    register8_t STATUS;
    register8_t STATE;
    register8_t KEY;
    register8_t INTCTRL;
} AES_t;


typedef enum AES_INTLVL_enum
{
    AES_INTLVL_OFF_gc = (0x00<<0),
    AES_INTLVL_LO_gc = (0x01<<0),
    AES_INTLVL_MED_gc = (0x02<<0),
    AES_INTLVL_HI_gc = (0x03<<0),
} AES_INTLVL_t;
# 379 "/usr/avr/include/avr/io.h" 2 3
# 432 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/portpins.h" 1 3
# 433 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/common.h" 1 3
# 435 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/version.h" 1 3
# 437 "/usr/avr/include/avr/io.h" 2 3


# 1 "/usr/avr/include/avr/fuse.h" 1 3
# 231 "/usr/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char byte[6];
} __fuse_t;
# 440 "/usr/avr/include/avr/io.h" 2 3


# 1 "/usr/avr/include/avr/lock.h" 1 3
# 443 "/usr/avr/include/avr/io.h" 2 3
# 10 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/interrupt.h" 1 3
# 11 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/wdt.h" 1 3
# 12 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/sleep.h" 1 3
# 13 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/cpufunc.h" 1 3
# 14 "test-sboxnet.c" 2
# 1 "/usr/avr/include/avr/eeprom.h" 1 3
# 428 "/usr/avr/include/avr/eeprom.h" 3
# 1 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 212 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 324 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 3 4
typedef int wchar_t;
# 429 "/usr/avr/include/avr/eeprom.h" 2 3
# 515 "/usr/avr/include/avr/eeprom.h" 3
uint8_t __eerd_byte_x16a4 (const uint8_t *__p) __attribute__((__pure__));




uint16_t __eerd_word_x16a4 (const uint16_t *__p) __attribute__((__pure__));




uint32_t __eerd_dword_x16a4 (const uint32_t *__p) __attribute__((__pure__));




float __eerd_float_x16a4 (const float *__p) __attribute__((__pure__));





void __eerd_block_x16a4 (void *__dst, const void *__src, size_t __n);





void __eewr_byte_x16a4 (uint8_t *__p, uint8_t __value);




void __eewr_word_x16a4 (uint16_t *__p, uint16_t __value);




void __eewr_dword_x16a4 (uint32_t *__p, uint32_t __value);




void __eewr_float_x16a4 (float *__p, float __value);





void __eewr_block_x16a4 (const void *__src, void *__dst, size_t __n);





void __eeupd_byte_x16a4 (uint8_t *__p, uint8_t __value);




void __eeupd_word_x16a4 (uint16_t *__p, uint16_t __value);




void __eeupd_dword_x16a4 (uint32_t *__p, uint32_t __value);




void __eeupd_float_x16a4 (float *__p, float __value);





void __eeupd_block_x16a4 (const void *__src, void *__dst, size_t __n);
# 15 "test-sboxnet.c" 2
# 38 "test-sboxnet.c"
# 1 "../../avrutilslib/lcd.c" 1
# 19 "../../avrutilslib/lcd.c"
# 1 "../../avrutilslib/defines.h" 1
# 63 "../../avrutilslib/defines.h"
static inline void _port_ctrl_opc(PORT_t* p, uint8_t bv, uint8_t opc) {
    if (bv & 0x01)
        p->PIN0CTRL = (p->PIN0CTRL & ~0x38) | opc;
    if (bv & 0x02)
        p->PIN1CTRL = (p->PIN1CTRL & ~0x38) | opc;
    if (bv & 0x04)
        p->PIN2CTRL = (p->PIN2CTRL & ~0x38) | opc;
    if (bv & 0x08)
        p->PIN3CTRL = (p->PIN3CTRL & ~0x38) | opc;
    if (bv & 0x10)
        p->PIN4CTRL = (p->PIN4CTRL & ~0x38) | opc;
    if (bv & 0x20)
        p->PIN5CTRL = (p->PIN5CTRL & ~0x38) | opc;
    if (bv & 0x40)
        p->PIN6CTRL = (p->PIN6CTRL & ~0x38) | opc;
    if (bv & 0x80)
        p->PIN7CTRL = (p->PIN7CTRL & ~0x38) | opc;
}
# 20 "../../avrutilslib/lcd.c" 2
# 1 "../../avrutilslib/lcd.h" 1
# 65 "../../avrutilslib/lcd.h"
# 1 "/usr/avr/include/avr/pgmspace.h" 1 3
# 87 "/usr/avr/include/avr/pgmspace.h" 3
# 1 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 1 3 4
# 88 "/usr/avr/include/avr/pgmspace.h" 2 3
# 1037 "/usr/avr/include/avr/pgmspace.h" 3
extern const void * memchr_P(const void *, int __val, size_t __len) __attribute__((__const__));
extern int memcmp_P(const void *, const void *, size_t) __attribute__((__pure__));
extern void *memccpy_P(void *, const void *, int __val, size_t);
extern void *memcpy_P(void *, const void *, size_t);
extern void *memmem_P(const void *, size_t, const void *, size_t) __attribute__((__pure__));
extern const void * memrchr_P(const void *, int __val, size_t __len) __attribute__((__const__));
extern char *strcat_P(char *, const char *);
extern const char * strchr_P(const char *, int __val) __attribute__((__const__));
extern const char * strchrnul_P(const char *, int __val) __attribute__((__const__));
extern int strcmp_P(const char *, const char *) __attribute__((__pure__));
extern char *strcpy_P(char *, const char *);
extern int strcasecmp_P(const char *, const char *) __attribute__((__pure__));
extern char *strcasestr_P(const char *, const char *) __attribute__((__pure__));
extern size_t strcspn_P(const char *__s, const char * __reject) __attribute__((__pure__));
extern size_t strlcat_P (char *, const char *, size_t );
extern size_t strlcpy_P (char *, const char *, size_t );
extern size_t __strlen_P(const char *) __attribute__((__const__));
extern size_t strnlen_P(const char *, size_t) __attribute__((__const__));
extern int strncmp_P(const char *, const char *, size_t) __attribute__((__pure__));
extern int strncasecmp_P(const char *, const char *, size_t) __attribute__((__pure__));
extern char *strncat_P(char *, const char *, size_t);
extern char *strncpy_P(char *, const char *, size_t);
extern char *strpbrk_P(const char *__s, const char * __accept) __attribute__((__pure__));
extern const char * strrchr_P(const char *, int __val) __attribute__((__const__));
extern char *strsep_P(char **__sp, const char * __delim);
extern size_t strspn_P(const char *__s, const char * __accept) __attribute__((__pure__));
extern char *strstr_P(const char *, const char *) __attribute__((__pure__));
extern char *strtok_P(char *__s, const char * __delim);
extern char *strtok_rP(char *__s, const char * __delim, char **__last);

extern size_t strlen_PF (uint_farptr_t src) __attribute__((__const__));
extern size_t strnlen_PF (uint_farptr_t src, size_t len) __attribute__((__const__));
extern void *memcpy_PF (void *dest, uint_farptr_t src, size_t len);
extern char *strcpy_PF (char *dest, uint_farptr_t src);
extern char *strncpy_PF (char *dest, uint_farptr_t src, size_t len);
extern char *strcat_PF (char *dest, uint_farptr_t src);
extern size_t strlcat_PF (char *dst, uint_farptr_t src, size_t siz);
extern char *strncat_PF (char *dest, uint_farptr_t src, size_t len);
extern int strcmp_PF (const char *s1, uint_farptr_t s2) __attribute__((__pure__));
extern int strncmp_PF (const char *s1, uint_farptr_t s2, size_t n) __attribute__((__pure__));
extern int strcasecmp_PF (const char *s1, uint_farptr_t s2) __attribute__((__pure__));
extern int strncasecmp_PF (const char *s1, uint_farptr_t s2, size_t n) __attribute__((__pure__));
extern char *strstr_PF (const char *s1, uint_farptr_t s2);
extern size_t strlcpy_PF (char *dst, uint_farptr_t src, size_t siz);
extern int memcmp_PF(const void *, uint_farptr_t, size_t) __attribute__((__pure__));


__attribute__((__always_inline__)) static inline size_t strlen_P(const char * s);
static inline size_t strlen_P(const char *s) {
  return __builtin_constant_p(__builtin_strlen(s))
     ? __builtin_strlen(s) : __strlen_P(s);
}
# 66 "../../avrutilslib/lcd.h" 2





static __attribute__((noinline)) void lcd_ks0070_init(void);
static __attribute__((noinline)) void lcd_ks0070_display_clear(void);
static __attribute__((noinline)) void lcd_ks0070_display_return_home(void);

static inline void lcd_display_clear(void) {
    lcd_ks0070_display_clear();
}

static inline void lcd_display_return_home(void) {
    lcd_ks0070_display_return_home();
}





static __attribute__((unused)) void lcd_ks0070_set_cursor_addr(uint8_t addr);

static __attribute__((unused)) void lcd_set_cursor(uint8_t x, uint8_t y);



static __attribute__((unused)) void lcd_move_cursor(uint8_t dir);
static __attribute__((unused)) void lcd_shift_display(uint8_t dir);






static __attribute__((unused)) void lcd_ks0070_set_display(uint8_t onoff);





static __attribute__((unused)) void lcd_ks0070_set_entry_mode(uint8_t mode);

static __attribute__((unused)) void lcd_char(char ch);
static __attribute__((unused)) __attribute__((noinline)) void lcd_char_at(uint8_t x, uint8_t y, char ch);
static __attribute__((unused)) __attribute__((noinline)) void lcd_str(const char* str);
static __attribute__((unused)) __attribute__((noinline)) void lcd_str_at(uint8_t x, uint8_t y, const char* str);
static __attribute__((unused)) __attribute__((noinline)) void lcd_str_P(const char * str);
static __attribute__((unused)) __attribute__((noinline)) void lcd_str_at_P(uint8_t x, uint8_t y, const char * str);
static __attribute__((unused)) __attribute__((noinline)) void lcd_itoa(int i);
static __attribute__((unused)) __attribute__((noinline)) void lcd_hex(uint16_t i, uint8_t ndigits);
# 21 "../../avrutilslib/lcd.c" 2

# 1 "/usr/avr/include/util/delay.h" 1 3
# 43 "/usr/avr/include/util/delay.h" 3
# 1 "/usr/avr/include/util/delay_basic.h" 1 3
# 40 "/usr/avr/include/util/delay_basic.h" 3
static inline void _delay_loop_1(uint8_t __count) __attribute__((always_inline));
static inline void _delay_loop_2(uint16_t __count) __attribute__((always_inline));
# 80 "/usr/avr/include/util/delay_basic.h" 3
void
_delay_loop_1(uint8_t __count)
{
 __asm__ volatile (
  "1: dec %0" "\n\t"
  "brne 1b"
  : "=r" (__count)
  : "0" (__count)
 );
}
# 102 "/usr/avr/include/util/delay_basic.h" 3
void
_delay_loop_2(uint16_t __count)
{
 __asm__ volatile (
  "1: sbiw %0,1" "\n\t"
  "brne 1b"
  : "=w" (__count)
  : "0" (__count)
 );
}
# 44 "/usr/avr/include/util/delay.h" 2 3
# 1 "/usr/avr/include/math.h" 1 3
# 127 "/usr/avr/include/math.h" 3
extern double cos(double __x) __attribute__((__const__));





extern double sin(double __x) __attribute__((__const__));





extern double tan(double __x) __attribute__((__const__));






extern double fabs(double __x) __attribute__((__const__));






extern double fmod(double __x, double __y) __attribute__((__const__));
# 168 "/usr/avr/include/math.h" 3
extern double modf(double __x, double *__iptr);



extern float modff (float __x, float *__iptr);




extern double sqrt(double __x) __attribute__((__const__));





extern double cbrt(double __x) __attribute__((__const__));
# 194 "/usr/avr/include/math.h" 3
extern double hypot (double __x, double __y) __attribute__((__const__));







extern double square(double __x) __attribute__((__const__));






extern double floor(double __x) __attribute__((__const__));






extern double ceil(double __x) __attribute__((__const__));
# 234 "/usr/avr/include/math.h" 3
extern double frexp(double __x, int *__pexp);







extern double ldexp(double __x, int __exp) __attribute__((__const__));





extern double exp(double __x) __attribute__((__const__));





extern double cosh(double __x) __attribute__((__const__));





extern double sinh(double __x) __attribute__((__const__));





extern double tanh(double __x) __attribute__((__const__));







extern double acos(double __x) __attribute__((__const__));







extern double asin(double __x) __attribute__((__const__));






extern double atan(double __x) __attribute__((__const__));
# 298 "/usr/avr/include/math.h" 3
extern double atan2(double __y, double __x) __attribute__((__const__));





extern double log(double __x) __attribute__((__const__));





extern double log10(double __x) __attribute__((__const__));





extern double pow(double __x, double __y) __attribute__((__const__));






extern int isnan(double __x) __attribute__((__const__));
# 333 "/usr/avr/include/math.h" 3
extern int isinf(double __x) __attribute__((__const__));






__attribute__((__const__)) static inline int isfinite (double __x)
{
    unsigned char __exp;
    __asm__ (
 "mov	%0, %C1		\n\t"
 "lsl	%0		\n\t"
 "mov	%0, %D1		\n\t"
 "rol	%0		"
 : "=r" (__exp)
 : "r" (__x) );
    return __exp != 0xff;
}






__attribute__((__const__)) static inline double copysign (double __x, double __y)
{
    __asm__ (
 "bst	%D2, 7	\n\t"
 "bld	%D0, 7	"
 : "=r" (__x)
 : "0" (__x), "r" (__y) );
    return __x;
}
# 376 "/usr/avr/include/math.h" 3
extern int signbit (double __x) __attribute__((__const__));






extern double fdim (double __x, double __y) __attribute__((__const__));
# 392 "/usr/avr/include/math.h" 3
extern double fma (double __x, double __y, double __z) __attribute__((__const__));







extern double fmax (double __x, double __y) __attribute__((__const__));







extern double fmin (double __x, double __y) __attribute__((__const__));






extern double trunc (double __x) __attribute__((__const__));
# 426 "/usr/avr/include/math.h" 3
extern double round (double __x) __attribute__((__const__));
# 439 "/usr/avr/include/math.h" 3
extern long lround (double __x) __attribute__((__const__));
# 453 "/usr/avr/include/math.h" 3
extern long lrint (double __x) __attribute__((__const__));
# 45 "/usr/avr/include/util/delay.h" 2 3
# 84 "/usr/avr/include/util/delay.h" 3
static inline void _delay_us(double __us) __attribute__((always_inline));
static inline void _delay_ms(double __ms) __attribute__((always_inline));
# 141 "/usr/avr/include/util/delay.h" 3
void
_delay_ms(double __ms)
{
 uint16_t __ticks;
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((32000000UL) / 1e3) * __ms;
# 161 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 186 "/usr/avr/include/util/delay.h" 3
}
# 223 "/usr/avr/include/util/delay.h" 3
void
_delay_us(double __us)
{
 uint8_t __ticks;
 double __tmp ;



 uint32_t __ticks_dc;
 extern void __builtin_avr_delay_cycles(unsigned long);
 __tmp = ((32000000UL) / 1e6) * __us;
# 243 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 261 "/usr/avr/include/util/delay.h" 3
}
# 23 "../../avrutilslib/lcd.c" 2
# 1 "/usr/avr/include/stdlib.h" 1 3
# 47 "/usr/avr/include/stdlib.h" 3
# 1 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 1 3 4
# 48 "/usr/avr/include/stdlib.h" 2 3
# 68 "/usr/avr/include/stdlib.h" 3
typedef struct {
 int quot;
 int rem;
} div_t;


typedef struct {
 long quot;
 long rem;
} ldiv_t;


typedef int (*__compar_fn_t)(const void *, const void *);
# 106 "/usr/avr/include/stdlib.h" 3
extern void abort(void) __attribute__((__noreturn__));




extern int abs(int __i) __attribute__((__const__));
# 120 "/usr/avr/include/stdlib.h" 3
extern long labs(long __i) __attribute__((__const__));
# 143 "/usr/avr/include/stdlib.h" 3
extern void *bsearch(const void *__key, const void *__base, size_t __nmemb,
       size_t __size, int (*__compar)(const void *, const void *));







extern div_t div(int __num, int __denom) __asm__("__divmodhi4") __attribute__((__const__));





extern ldiv_t ldiv(long __num, long __denom) __asm__("__divmodsi4") __attribute__((__const__));
# 175 "/usr/avr/include/stdlib.h" 3
extern void qsort(void *__base, size_t __nmemb, size_t __size,
    __compar_fn_t __compar);
# 208 "/usr/avr/include/stdlib.h" 3
extern long strtol(const char *__nptr, char **__endptr, int __base);
# 242 "/usr/avr/include/stdlib.h" 3
extern unsigned long strtoul(const char *__nptr, char **__endptr, int __base);
# 254 "/usr/avr/include/stdlib.h" 3
extern long atol(const char *__s) __attribute__((__pure__));
# 266 "/usr/avr/include/stdlib.h" 3
extern int atoi(const char *__s) __attribute__((__pure__));
# 278 "/usr/avr/include/stdlib.h" 3
extern void exit(int __status) __attribute__((__noreturn__));
# 290 "/usr/avr/include/stdlib.h" 3
extern void *malloc(size_t __size) __attribute__((__malloc__));






extern void free(void *__ptr);




extern size_t __malloc_margin;




extern char *__malloc_heap_start;




extern char *__malloc_heap_end;






extern void *calloc(size_t __nele, size_t __size) __attribute__((__malloc__));
# 338 "/usr/avr/include/stdlib.h" 3
extern void *realloc(void *__ptr, size_t __size) __attribute__((__malloc__));

extern double strtod(const char *__nptr, char **__endptr);

extern double atof(const char *__nptr);
# 364 "/usr/avr/include/stdlib.h" 3
extern int rand(void);



extern void srand(unsigned int __seed);






extern int rand_r(unsigned long *__ctx);
# 406 "/usr/avr/include/stdlib.h" 3
extern char *itoa(int __val, char *__s, int __radix);
# 434 "/usr/avr/include/stdlib.h" 3
extern char *ltoa(long int __val, char *__s, int __radix);
# 460 "/usr/avr/include/stdlib.h" 3
extern char *utoa(unsigned int __val, char *__s, int __radix);
# 486 "/usr/avr/include/stdlib.h" 3
extern char *ultoa(unsigned long int __val, char *__s, int __radix);
# 504 "/usr/avr/include/stdlib.h" 3
extern long random(void);




extern void srandom(unsigned long __seed);







extern long random_r(unsigned long *__ctx);
# 563 "/usr/avr/include/stdlib.h" 3
extern char *dtostre(double __val, char *__s, unsigned char __prec,
       unsigned char __flags);
# 580 "/usr/avr/include/stdlib.h" 3
extern char *dtostrf(double __val, signed char __width,
                     unsigned char __prec, char *__s);
# 24 "../../avrutilslib/lcd.c" 2

static __attribute__((noinline)) void lcd_ks0070_write4(uint8_t rs, uint8_t value) {
    ((*(PORT_t *) 0x0660)).OUT = (((*(PORT_t *) 0x0660)).OUT & ~(0x0f<<3))
                | ((value & 0x0f) << 3);
    if (rs) {
        ((*(PORT_t *) 0x0660)).OUTSET = ((1<<(7)));
    } else {
        ((*(PORT_t *) 0x0660)).OUTCLR = ((1<<(7)));
    }




    ((*(PORT_t *) 0x0680)).OUTSET = ((1<<(0)));
    _delay_us(1);
    ((*(PORT_t *) 0x0680)).OUTCLR = ((1<<(0)));
}
# 73 "../../avrutilslib/lcd.c"
static __attribute__((noinline)) void lcd_ks0070_write(uint8_t rs, uint8_t value) {
    lcd_ks0070_write4(rs, value >> 4);
    lcd_ks0070_write4(rs, value);
}

static __attribute__((noinline)) void lcd_ks0070_command(uint8_t cmd) {




    lcd_ks0070_write(0, cmd);
    _delay_us(45);

}

static __attribute__((noinline)) void lcd_ks0070_data(uint8_t data) {




    lcd_ks0070_write(1, data);
    _delay_us(50);

}


void __attribute__((noinline)) lcd_ks0070_init(void) {
    ((*(PORT_t *) 0x0660)).OUTCLR = (0x0f<<3);
    ((*(PORT_t *) 0x0660)).DIRSET = (0x0f<<3);
    ((*(PORT_t *) 0x0660)).OUTCLR = ((1<<(7)));
    ((*(PORT_t *) 0x0660)).DIRSET = ((1<<(7)));
    ((*(PORT_t *) 0x0680)).OUTCLR = ((1<<(0)));
    ((*(PORT_t *) 0x0680)).DIRSET = ((1<<(0)));






    lcd_ks0070_write4(0, 0x03);
    _delay_ms(5);
    lcd_ks0070_write4(0, 0x03);
    _delay_us(120);
    lcd_ks0070_write4(0, 0x03);
    _delay_us(120);


    lcd_ks0070_write4(0, 0x02);
    _delay_us(120);





    lcd_ks0070_command(0x28);


    lcd_ks0070_set_display(0x04);
    lcd_ks0070_display_clear();

    lcd_ks0070_set_entry_mode(0x02|0x00);
}


static __attribute__((noinline)) void lcd_ks0070_display_clear(void) {
    lcd_ks0070_command(0x01);

    _delay_ms(2);

}

static __attribute__((noinline)) void lcd_ks0070_display_return_home(void) {
    lcd_ks0070_command(0x02);

    _delay_ms(2);

}

static void lcd_ks0070_set_cursor_addr(uint8_t addr) {
    lcd_ks0070_command(0x80|(addr & 0x7f));
}

static void lcd_set_cursor(uint8_t x, uint8_t y) {
    uint8_t addr;
    switch (y) {
        case 1: addr = 0x40; break;
        case 2: addr = 0x14; break;
        case 3: addr = 0x54; break;
        default: addr = 0x00; break;
    }
    lcd_ks0070_set_cursor_addr(addr + x);
}

static void lcd_move_cursor(uint8_t dir) {
    lcd_ks0070_command(0x10|(dir ? 0x04:0x00));
}

static void lcd_shift_display(uint8_t dir) {
    lcd_ks0070_command(0x18|(dir ? 0x04:0x00));
}

static void lcd_ks0070_set_display(uint8_t onoff) {
    lcd_ks0070_command(0x08|(onoff & 0x07));
}

static void lcd_ks0070_set_entry_mode(uint8_t mode) {
    lcd_ks0070_command(0x04|(mode & 0x03));
}


static void lcd_char(char ch) {
    lcd_ks0070_data(ch);
}

static __attribute__((noinline)) void lcd_char_at(uint8_t x, uint8_t y, char ch) {
    lcd_set_cursor(x, y);
    lcd_char(ch);
}

static __attribute__((noinline)) void lcd_str(const char* str) {
    while (*str) {
        lcd_char(*str);
        str++;
    }
}

static __attribute__((noinline)) void lcd_str_at(uint8_t x, uint8_t y, const char* str) {
    lcd_set_cursor(x, y);
    lcd_str(str);
}

static __attribute__((noinline)) void lcd_str_P(const char * str) {
    uint8_t c = (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(str)); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }));
    while (c != 0) {
        lcd_char(c);
        c = (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(++str)); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }));
    }
}

static __attribute__((noinline)) void lcd_str_at_P(uint8_t x, uint8_t y, const char * str) {
    lcd_set_cursor(x, y);
    lcd_str_P(str);
}

static __attribute__((noinline)) void lcd_itoa(int i) {
    char buf[7];
    itoa(i, buf, 10);
    lcd_str(buf);
}

static __attribute__((noinline)) void lcd_hex(uint16_t i, uint8_t ndigits) {
    const char * pc = (__extension__({static const char __c[] __attribute__((__progmem__)) = ("0123456789abcdef"); &__c[0];}));
    for (uint8_t k = 4; k; k--) {
        if (k <= ndigits) {
            lcd_char((__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(pc + ((i >> 12) & 0x0f))); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; })));
        }
        i <<= 4;
    }
}
# 39 "test-sboxnet.c" 2

# 1 "../../avrutilslib/random.h" 1
# 25 "../../avrutilslib/random.h"
static inline uint8_t prng_xorshift8(uint8_t x) {
    x ^= (x << 1);
    x ^= (x >> 1);
    x ^= (x * 4);
    return x;
}

static inline uint16_t prng_xorshift16(uint16_t x) {
    x ^= (x << 1);
    x ^= (x >> 5);
    x ^= (x << 2);
    return x;
}
# 41 "test-sboxnet.c" 2


static uint8_t g_dev_addr;
# 97 "test-sboxnet.c"
# 1 "../../avrutilslib/sboxnet.c" 1
# 20 "../../avrutilslib/sboxnet.c"
# 1 "/usr/avr/include/string.h" 1 3
# 45 "/usr/avr/include/string.h" 3
# 1 "/usr/lib/gcc/avr/4.8.3/include/stddef.h" 1 3 4
# 46 "/usr/avr/include/string.h" 2 3
# 111 "/usr/avr/include/string.h" 3
extern int ffs (int __val) __attribute__((__const__));
extern int ffsl (long __val) __attribute__((__const__));
extern int ffsll (long long __val) __attribute__((__const__));
extern void *memccpy(void *, const void *, int, size_t);
extern void *memchr(const void *, int, size_t) __attribute__((__pure__));
extern int memcmp(const void *, const void *, size_t) __attribute__((__pure__));
extern void *memcpy(void *, const void *, size_t);
extern void *memmem(const void *, size_t, const void *, size_t) __attribute__((__pure__));
extern void *memmove(void *, const void *, size_t);
extern void *memrchr(const void *, int, size_t) __attribute__((__pure__));
extern void *memset(void *, int, size_t);
extern char *strcat(char *, const char *);
extern char *strchr(const char *, int) __attribute__((__pure__));
extern char *strchrnul(const char *, int) __attribute__((__pure__));
extern int strcmp(const char *, const char *) __attribute__((__pure__));
extern char *strcpy(char *, const char *);
extern int strcasecmp(const char *, const char *) __attribute__((__pure__));
extern char *strcasestr(const char *, const char *) __attribute__((__pure__));
extern size_t strcspn(const char *__s, const char *__reject) __attribute__((__pure__));
extern char *strdup(const char *s1);
extern size_t strlcat(char *, const char *, size_t);
extern size_t strlcpy(char *, const char *, size_t);
extern size_t strlen(const char *) __attribute__((__pure__));
extern char *strlwr(char *);
extern char *strncat(char *, const char *, size_t);
extern int strncmp(const char *, const char *, size_t) __attribute__((__pure__));
extern char *strncpy(char *, const char *, size_t);
extern int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));
extern size_t strnlen(const char *, size_t) __attribute__((__pure__));
extern char *strpbrk(const char *__s, const char *__accept) __attribute__((__pure__));
extern char *strrchr(const char *, int) __attribute__((__pure__));
extern char *strrev(char *);
extern char *strsep(char **, const char *);
extern size_t strspn(const char *__s, const char *__accept) __attribute__((__pure__));
extern char *strstr(const char *, const char *) __attribute__((__pure__));
extern char *strtok(char *, const char *);
extern char *strtok_r(char *, const char *, char **);
extern char *strupr(char *);
# 21 "../../avrutilslib/sboxnet.c" 2

# 1 "/usr/avr/include/util/crc16.h" 1 3
# 110 "/usr/avr/include/util/crc16.h" 3
static __inline__ uint16_t
_crc16_update(uint16_t __crc, uint8_t __data)
{
 uint8_t __tmp;
 uint16_t __ret;

 __asm__ __volatile__ (
  "eor %A0,%2" "\n\t"
  "mov %1,%A0" "\n\t"
  "swap %1" "\n\t"
  "eor %1,%A0" "\n\t"
  "mov __tmp_reg__,%1" "\n\t"
  "lsr %1" "\n\t"
  "lsr %1" "\n\t"
  "eor %1,__tmp_reg__" "\n\t"
  "mov __tmp_reg__,%1" "\n\t"
  "lsr %1" "\n\t"
  "eor %1,__tmp_reg__" "\n\t"
  "andi %1,0x07" "\n\t"
  "mov __tmp_reg__,%A0" "\n\t"
  "mov %A0,%B0" "\n\t"
  "lsr %1" "\n\t"
  "ror __tmp_reg__" "\n\t"
  "ror %1" "\n\t"
  "mov %B0,__tmp_reg__" "\n\t"
  "eor %A0,%1" "\n\t"
  "lsr __tmp_reg__" "\n\t"
  "ror %1" "\n\t"
  "eor %B0,__tmp_reg__" "\n\t"
  "eor %A0,%1"
  : "=r" (__ret), "=d" (__tmp)
  : "r" (__data), "0" (__crc)
  : "r0"
 );
 return __ret;
}
# 176 "/usr/avr/include/util/crc16.h" 3
static __inline__ uint16_t
_crc_xmodem_update(uint16_t __crc, uint8_t __data)
{
    uint16_t __ret;
    uint8_t __tmp1;
    uint8_t __tmp2;


    __asm__ __volatile__ (
        "eor    %B0,%3" "\n\t"
        "mov    __tmp_reg__,%B0" "\n\t"
        "swap   __tmp_reg__" "\n\t"


        "mov    %1,__tmp_reg__" "\n\t"
        "andi   %1,0x0f" "\n\t"
        "eor    %1,%B0" "\n\t"
        "mov    %2,%B0" "\n\t"
        "eor    %2,__tmp_reg__" "\n\t"
        "lsl    %2" "\n\t"
        "andi   %2,0xe0" "\n\t"
        "eor    %1,%2" "\n\t"


        "mov    %2,__tmp_reg__" "\n\t"
        "eor    %2,%B0" "\n\t"
        "andi   %2,0xf0" "\n\t"
        "lsr    %2" "\n\t"
        "mov    __tmp_reg__,%B0" "\n\t"
        "lsl    __tmp_reg__" "\n\t"
        "rol    %2" "\n\t"
        "lsr    %B0" "\n\t"
        "lsr    %B0" "\n\t"
        "lsr    %B0" "\n\t"
        "andi   %B0,0x1f" "\n\t"
        "eor    %B0,%2" "\n\t"
        "eor    %B0,%A0" "\n\t"
        "mov    %A0,%1" "\n\t"
        : "=d" (__ret), "=d" (__tmp1), "=d" (__tmp2)
        : "r" (__data), "0" (__crc)
        : "r0"
    );
    return __ret;
}
# 250 "/usr/avr/include/util/crc16.h" 3
static __inline__ uint16_t
_crc_ccitt_update (uint16_t __crc, uint8_t __data)
{
    uint16_t __ret;

    __asm__ __volatile__ (
        "eor    %A0,%1" "\n\t"

        "mov    __tmp_reg__,%A0" "\n\t"
        "swap   %A0" "\n\t"
        "andi   %A0,0xf0" "\n\t"
        "eor    %A0,__tmp_reg__" "\n\t"

        "mov    __tmp_reg__,%B0" "\n\t"

        "mov    %B0,%A0" "\n\t"

        "swap   %A0" "\n\t"
        "andi   %A0,0x0f" "\n\t"
        "eor    __tmp_reg__,%A0" "\n\t"

        "lsr    %A0" "\n\t"
        "eor    %B0,%A0" "\n\t"

        "eor    %A0,%B0" "\n\t"
        "lsl    %A0" "\n\t"
        "lsl    %A0" "\n\t"
        "lsl    %A0" "\n\t"
        "eor    %A0,__tmp_reg__"

        : "=d" (__ret)
        : "r" (__data), "0" (__crc)
        : "r0"
    );
    return __ret;
}
# 317 "/usr/avr/include/util/crc16.h" 3
static __inline__ uint8_t
_crc_ibutton_update(uint8_t __crc, uint8_t __data)
{
 uint8_t __i, __pattern;
 __asm__ __volatile__ (
  "	eor	%0, %4" "\n\t"
  "	ldi	%1, 8" "\n\t"
  "	ldi	%2, 0x8C" "\n\t"
  "1:	lsr	%0" "\n\t"
  "	brcc	2f" "\n\t"
  "	eor	%0, %2" "\n\t"
  "2:	dec	%1" "\n\t"
  "	brne	1b" "\n\t"
  : "=r" (__crc), "=d" (__i), "=d" (__pattern)
  : "0" (__crc), "r" (__data));
 return __crc;
}
# 23 "../../avrutilslib/sboxnet.c" 2
# 1 "/usr/avr/include/util/atomic.h" 1 3
# 42 "/usr/avr/include/util/atomic.h" 3
static __inline__ uint8_t __iSeiRetVal(void)
{
    __asm__ __volatile__ ("sei" ::: "memory");
    return 1;
}

static __inline__ uint8_t __iCliRetVal(void)
{
    __asm__ __volatile__ ("cli" ::: "memory");
    return 1;
}

static __inline__ void __iSeiParam(const uint8_t *__s)
{
    __asm__ __volatile__ ("sei" ::: "memory");
    __asm__ volatile ("" ::: "memory");
    (void)__s;
}

static __inline__ void __iCliParam(const uint8_t *__s)
{
    __asm__ __volatile__ ("cli" ::: "memory");
    __asm__ volatile ("" ::: "memory");
    (void)__s;
}

static __inline__ void __iRestore(const uint8_t *__s)
{
    (*(volatile uint8_t *)(0x003F)) = *__s;
    __asm__ volatile ("" ::: "memory");
}
# 24 "../../avrutilslib/sboxnet.c" 2


# 1 "../../avrutilslib/sboxnet.h" 1
# 221 "../../avrutilslib/sboxnet.h"
# 1 "../../avrutilslib/sboxnet-struct.h" 1
# 40 "../../avrutilslib/sboxnet-struct.h"
struct sboxnet_msg_header {
    uint8_t dstaddr;
    uint8_t srcaddr;
    struct len_seq {
        unsigned len : 4;
        unsigned seq : 4;
    } opt;
    uint8_t cmd;
    uint8_t crc;
    uint8_t data[0];
} __attribute__ ((packed));




struct sboxnet_msg_max {
    struct sboxnet_msg_header msg;
    uint8_t data[20 -sizeof(struct sboxnet_msg_header)];
};


struct sboxnet_debug_info {
    uint16_t recv_byte;
    uint16_t recv_msg;
    uint16_t tmit_byte;
    uint16_t tmit_msg;
    uint16_t collisions;
    uint16_t coll_rxc;
    uint16_t coll_fe;
    uint16_t coll_udr;
    uint16_t retries;
    uint16_t recverrors;
    uint16_t recverr_fe;
    uint16_t recverr_pe;
    uint16_t recverr_dor;
    uint16_t recverr_proto;
    uint16_t recverr_size;
    uint16_t crcerrors;
    uint16_t tmiterrors;
};
# 222 "../../avrutilslib/sboxnet.h" 2
# 238 "../../avrutilslib/sboxnet.h"
struct sboxnet_ring_buffer {
    uint8_t rd;
    uint8_t wr;
    uint8_t wr_count;
    uint8_t wr_rollback;
    uint8_t count;
    uint8_t buf[(8*20)];
};


struct sboxnet_device {
    uint8_t state;
    uint8_t flags;
    uint8_t retry_counter;
    uint8_t tmit_cnt;
    uint8_t tmit_bytes;
    uint8_t backoff_bits;
    uint8_t recv_len;
    uint8_t prng_seed;
    struct sboxnet_ring_buffer recv_buf;
    uint8_t tmit_buf[(20)];


    struct sboxnet_debug_info debug;

};



extern struct sboxnet_device g_sboxnet_dev_def;

 void sboxnet_init(void);

 static void sboxnet_timer_bit(void);

 uint8_t sboxnet_receive_msg(struct sboxnet_msg_max* pmsg);

 void sboxnet_send_msg(struct sboxnet_msg_max* pmsg);

 static inline uint8_t sboxnet_can_send_msg(void) {
    return (*(volatile uint8_t*)&g_sboxnet_dev_def.tmit_bytes == 0);
}
# 27 "../../avrutilslib/sboxnet.c" 2
# 35 "../../avrutilslib/sboxnet.c"
struct sboxnet_device g_sboxnet_dev_def;


 static inline uint8_t sboxnet_crc8_init(void) {
    return 0xff;
}

 static uint8_t sboxnet_crc8_add(uint8_t crc, uint8_t v) {
    return _crc_ibutton_update(crc, v);
}

 static inline uint8_t sboxnet_crc8_ok(uint8_t crc) {
    return crc == 0x00;
}

 uint8_t sboxnet_prandom(void) {
    g_sboxnet_dev_def.prng_seed = prng_xorshift8(g_sboxnet_dev_def.prng_seed);
    return g_sboxnet_dev_def.prng_seed;
}



 static uint8_t sboxnet_recv_buf_read_msg(uint8_t* p) {
    struct sboxnet_ring_buffer *rb = &g_sboxnet_dev_def.recv_buf;
    uint8_t idx = 0;
    uint8_t maxb = sizeof(struct sboxnet_msg_header);
    while (rb->count > 0 && idx < maxb) {
        rb->count--;
        uint8_t v = rb->buf[rb->rd];
        if (idx == 2) {
            maxb += (v & 0x0f);
        }
        *p++ = v;
        rb->rd++;
        if (rb->rd >= (8*20)) {
            rb->rd = 0;
        }
        idx++;
    }
    return idx;
}

 static inline uint8_t sboxnet_get_recv_bytes(void) {
    return *(volatile uint8_t*)&g_sboxnet_dev_def.recv_buf.count;
}

 static inline void sboxnet_recv_buf_commit() {
    struct sboxnet_ring_buffer *rb = &g_sboxnet_dev_def.recv_buf;
    rb->count += rb->wr_count;
    rb->wr_count = 0;
    rb->wr_rollback = rb->wr;
}

 static void sboxnet_recv_buf_rollback() {
    struct sboxnet_ring_buffer *rb = &g_sboxnet_dev_def.recv_buf;
    __asm__ __volatile__ ("" : "+r"(rb) ::);
    rb->wr_count = 0;
    rb->wr = rb->wr_rollback;
}

 static uint8_t sboxnet_recv_buf_write(uint8_t a_v) {
    struct sboxnet_ring_buffer *rb = &g_sboxnet_dev_def.recv_buf;


    if ((uint8_t)(rb->count + rb->wr_count) < (uint8_t)(8*20)) {
        rb->wr_count++;
        rb->buf[rb->wr] = a_v;
        rb->wr++;
        if (rb->wr >= (8*20)) {
            rb->wr = 0;
        }
        return 1;
    }
    return 0;
}




 __attribute__((noinline)) void sboxnet_init(void) {
    struct sboxnet_device *dev = &g_sboxnet_dev_def;
    memset(dev, 0, sizeof(g_sboxnet_dev_def));
    dev->state = 0;
    dev->prng_seed = (*(volatile uint8_t *)(0x0054));

    ((*(PORT_t *) 0x0640)).DIRCLR = ((1<<(2)));
    _port_ctrl_opc(&((*(PORT_t *) 0x0640)), (1<<(2)), PORT_OPC_PULLUP_gc);

    ((*(PORT_t *) 0x0640)).OUTSET = ((1<<(3)));
    ((*(PORT_t *) 0x0640)).DIRSET = ((1<<(3)));



    (*(PORT_t *) 0x0640).INTCTRL &= ~0x03;
    (*(PORT_t *) 0x0640).INT0MASK = (1<<(2));
    (*(PORT_t *) 0x0640).INTFLAGS = (1<<(0));

    (*(USART_t *) 0x08A0).BAUDCTRLB = ((uint8_t)((((32000000UL / 16 / 125000) - 1)) >> 8)) & 0x0f;
    (*(USART_t *) 0x08A0).BAUDCTRLA = ((uint8_t)(((32000000UL / 16 / 125000) - 1)));

    (*(USART_t *) 0x08A0).STATUS = (1<<(6));
    (*(USART_t *) 0x08A0).CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;
    (*(USART_t *) 0x08A0).CTRLB = (0 << (4))|(0 << (3))|(0 << (2))|(1 << (1));
    (*(USART_t *) 0x08A0).CTRLC = USART_CMODE_ASYNCHRONOUS_gc|USART_PMODE_EVEN_gc|(0 << (3))|USART_CHSIZE_9BIT_gc;
# 163 "../../avrutilslib/sboxnet.c"
}


static __attribute__((unused)) __attribute__((noinline)) void sboxnet_deinit(void) {
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)(0x003F)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        g_sboxnet_dev_def.state = 0;

        (*(USART_t *) 0x08A0).CTRLB = (0 << (4))|(0 << (3))|(0 << (2))|(0 << (1));
        (*(USART_t *) 0x08A0).CTRLA = USART_RXCINTLVL_OFF_gc|USART_TXCINTLVL_OFF_gc|USART_DREINTLVL_OFF_gc;




        ((*(PORT_t *) 0x0640)).DIRCLR = ((1<<(2)));
        ((*(PORT_t *) 0x0640)).DIRCLR = ((1<<(3)));
        _port_ctrl_opc(&((*(PORT_t *) 0x0640)), (1<<(3)), PORT_OPC_PULLUP_gc);

        (*(PORT_t *) 0x0640).INTFLAGS = (1<<(0));
        (*(PORT_t *) 0x0640).INT0MASK = 0;







    }
}


 __attribute__((noinline)) void sboxnet_receiver_enable(void) {
    sboxnet_recv_buf_rollback();

    (*(USART_t *) 0x08A0).CTRLA = ((*(USART_t *) 0x08A0).CTRLA & ~0x30) | USART_RXCINTLVL_LO_gc;
    (((*(USART_t *) 0x08A0).CTRLB) |= (0x10|0x02));




}


 static void sboxnet_set_timer(uint16_t offset) {

    register uint16_t ocr = (*(TC0_t *) 0x0800).CNT;
    (*(TC0_t *) 0x0800).CCA = (ocr + offset) & 0xff;
    (*(TC0_t *) 0x0800).INTFLAGS = (1<<(4));
    (*(TC0_t *) 0x0800).INTCTRLB = ((*(TC0_t *) 0x0800).INTCTRLB & ~0x03) | TC_CCAINTLVL_LO_gc;






}

 static inline void sboxnet_clear_line_sensor(void) {

    (*(PORT_t *) 0x0640).INTFLAGS = (1<<(0));







}

 static inline uint8_t sboxnet_line_sensor_changed(void) {

    return ((*(volatile uint8_t *)(((uint16_t) &((*(PORT_t *) 0x0640).INTFLAGS)))) & (1 << (0)));







}

 static void sboxnet_set_backoff_with_offset(uint8_t offset) {
    sboxnet_clear_line_sensor();
    g_sboxnet_dev_def.state = 1;
    sboxnet_set_timer(4 * 15 + offset);
}


 static void sboxnet_intr_usart_rx(void) {
    uint8_t rxb8, rxb, rflags;
    struct sboxnet_device *dev = &g_sboxnet_dev_def;
    __asm__ __volatile__ ("" : "+r"(dev) ::);


    rflags = (*(USART_t *) 0x08A0).STATUS;
    rxb8 = ((*(volatile uint8_t *)(((uint16_t) &(rflags)))) & (1 << (0)));
    rxb = (*(USART_t *) 0x08A0).DATA;
    rflags &= (0x10|0x08|0x04);
# 270 "../../avrutilslib/sboxnet.c"
    if (rflags) {
        dev->debug.recverrors++;

        if (rflags & 0x10) { dev->debug.recverr_fe++; }
        if (rflags & 0x08) { dev->debug.recverr_dor++; }
        if (rflags & 0x04) { dev->debug.recverr_pe++; }





        goto receive_reset;

    } else if (rxb8) {
        if (dev->recv_buf.wr_count != 0) {
            dev->debug.recverr_proto++;
            dev->debug.recverrors++;
            sboxnet_recv_buf_rollback();
        }
        if (rxb == g_dev_addr || rxb == 255



                                                ) {

            (((*(USART_t *) 0x08A0).CTRLB) &= ~(1<<(1)));



            if (sboxnet_recv_buf_write(rxb)) {
                dev->debug.recv_byte++;
                dev->recv_len = 5;
            } else {
                dev->debug.recverr_dor++;
                goto receive_error;
            }
        } else {

            goto receive_reset;
        }

    } else {
        if (dev->recv_buf.wr_count >= dev->recv_len) {
            dev->debug.recverr_size++;
            goto receive_error;
        } else {
            if (dev->recv_buf.wr_count == 2) {
                dev->recv_len = 5 + (rxb & 0x0f);
            }
            if (!sboxnet_recv_buf_write(rxb)) {
                dev->debug.recverr_dor++;
                goto receive_error;
            }
            dev->debug.recv_byte++;
            if (dev->recv_buf.wr_count == dev->recv_len) {
                dev->recv_len = 0;

                (((*(USART_t *) 0x08A0).CTRLB) |= (1<<(1)));



                sboxnet_recv_buf_commit();
            }
        }
    }
    return;

receive_error:
    dev->debug.recverrors++;
receive_reset:
    dev->recv_len = 0;

    (((*(USART_t *) 0x08A0).CTRLB) |= (1<<(1)));



    sboxnet_recv_buf_rollback();
}


 static void sboxnet_intr_usart_tx(void) {
    struct sboxnet_device *dev = &g_sboxnet_dev_def;
    __asm__ __volatile__ ("" : "+r"(dev) ::);



    uint8_t ucsra = (*(USART_t *) 0x08A0).STATUS;
    uint8_t crxc = !(ucsra & 0x80);
    uint8_t cfe = (ucsra & (0x10|0x04));
    uint8_t cudr = ((*(USART_t *) 0x08A0).DATA != dev->tmit_buf[dev->tmit_cnt]);






    if (crxc || cfe || cudr) {
        if (crxc) { dev->debug.coll_rxc++; }
        if (cfe) { dev->debug.coll_fe++; }
        if (cudr) { dev->debug.coll_udr++; }

        ((*(PORT_t *) 0x0640)).OUTCLR = ((1<<(3)));

        (*(USART_t *) 0x08A0).CTRLB &= ~(0x08|0x10);
        (*(USART_t *) 0x08A0).CTRLA &= ~(0x0C);



        dev->state = 2;
        dev->debug.collisions++;
        if (dev->retry_counter > 0) {
            dev->retry_counter--;
            dev->tmit_cnt = dev->tmit_bytes;

            dev->debug.retries++;
        } else {
            dev->tmit_cnt = dev->tmit_bytes = 0;
            dev->debug.tmiterrors++;
        }

        sboxnet_set_timer(4 * 16);
        return;
    }

     (((*(USART_t *) 0x08A0).CTRLB) &= ~(1<<(4)));
     (*(USART_t *) 0x08A0).CTRLA &= ~(0x30);
     (((*(USART_t *) 0x08A0).CTRLB) |= (1<<(4)));




    if (dev->tmit_cnt) {
        dev->tmit_cnt--;

        (((*(USART_t *) 0x08A0).CTRLB) &= ~(1<<(0)));
        (*(USART_t *) 0x08A0).DATA = dev->tmit_buf[dev->tmit_cnt];




    } else {

        (*(USART_t *) 0x08A0).CTRLA &= ~(0x0C);



        dev->tmit_bytes = 0;
        sboxnet_receiver_enable();
        if (dev->state == 3) {
            dev->state = 0;
        }
    }
    dev->debug.tmit_byte++;
}




 void __vector_25 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_25 (void) {
    sboxnet_intr_usart_rx();
}

 void __vector_27 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_27 (void) {
    sboxnet_intr_usart_tx();
}
# 447 "../../avrutilslib/sboxnet.c"
 static void sboxnet_check_try_transmit(void) {
    struct sboxnet_device *dev = &g_sboxnet_dev_def;
    __asm__ __volatile__ ("" : "+r"(dev) ::);

    if (dev->tmit_bytes > 0 && dev->tmit_cnt == dev->tmit_bytes) {
        if (dev->state == 0) {
            uint8_t offset = sboxnet_prandom() & 0x0f;
            sboxnet_set_backoff_with_offset(4*5 + offset);
        }
    }
}

 static __attribute__((noinline)) void sboxnet_timer_bit(void) {
    struct sboxnet_device *dev = &g_sboxnet_dev_def;


    (*(TC0_t *) 0x0800).INTCTRLB &= ~0x03;



    if (dev->state == 1) {
        if (dev->tmit_bytes > 0 && dev->tmit_cnt == dev->tmit_bytes) {
            uint8_t d = dev->tmit_buf[dev->tmit_cnt - 1];

            ((*(PORT_t *) 0x0640)).OUTSET = ((1<<(3)));


            (((*(USART_t *) 0x08A0).CTRLB) |= (0x08|0x01));



            if (sboxnet_line_sensor_changed() || (!((*(volatile uint8_t *)(((uint16_t) &(((*(PORT_t *) 0x0640)).IN)))) & (1 << (2))))) {
                uint8_t offset = sboxnet_prandom() & 0x0f;
                sboxnet_set_backoff_with_offset(offset);

            } else {

                (((*(USART_t *) 0x08A0).CTRLB) &= ~(1<<(4)));
                (*(USART_t *) 0x08A0).CTRLA &= ~(0x30|0x0C);
                (((*(USART_t *) 0x08A0).CTRLB) |= (1<<(4)));
                (*(USART_t *) 0x08A0).CTRLA |= USART_TXCINTLVL_LO_gc;
                (*(USART_t *) 0x08A0).DATA = d;
                (((*(USART_t *) 0x08A0).CTRLB) &= ~(1<<(1)));






                dev->tmit_cnt--;
                dev->state = 3;
            }
        } else {
            dev->state = 0;
        }
        return;
    }

    if (dev->state == 2) {
        sboxnet_clear_line_sensor();
        ((*(PORT_t *) 0x0640)).OUTSET = ((1<<(3)));
        dev->state = 4;
        sboxnet_set_timer(4 * 4);
        return;
    }

    if (dev->state == 4) {
        if (sboxnet_line_sensor_changed() && ((*(volatile uint8_t *)(((uint16_t) &(((*(PORT_t *) 0x0640)).IN)))) & (1 << (2)))) {
            sboxnet_receiver_enable();
            dev->state = 0;
            sboxnet_check_try_transmit();
        } else {
            sboxnet_clear_line_sensor();
            sboxnet_set_timer(4 * 4);
        }
        return;
    }
}

 __attribute__((noinline)) uint8_t sboxnet_receive_msg(struct sboxnet_msg_max* pmsg) {
    uint8_t msglen = 0;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)(0x003F)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        if (sboxnet_get_recv_bytes() >= sizeof(struct sboxnet_msg_header)) {
            msglen = sboxnet_recv_buf_read_msg((uint8_t*)pmsg);
        }
    }
    if (msglen == 0) {
        return 1;
    }

    uint8_t i;
    uint8_t crc = sboxnet_crc8_init();
    for (i = 0; i < msglen; i++) {
        if (i != 4) {
            crc = sboxnet_crc8_add(crc, ((uint8_t*)pmsg)[i]);
        }
    }
    crc = sboxnet_crc8_add(crc, pmsg->msg.crc);

    if (!sboxnet_crc8_ok(crc)) {
        g_sboxnet_dev_def.debug.recverrors++;
        g_sboxnet_dev_def.debug.crcerrors++;
        return 2;
    }
    g_sboxnet_dev_def.debug.recv_msg++;
    return 0;
}



 __attribute__((noinline)) void sboxnet_send_msg(struct sboxnet_msg_max* pmsg) {
    while (!sboxnet_can_send_msg())
        ;

    pmsg->msg.srcaddr = g_dev_addr;

    uint8_t msglen = sizeof(struct sboxnet_msg_header) + pmsg->msg.opt.len;
    uint8_t crc = sboxnet_crc8_init();
    uint8_t i;
    struct sboxnet_device *dev = &g_sboxnet_dev_def;
    uint8_t* p = dev->tmit_buf + msglen;
    for (i = 0; i < msglen; i++) {
        uint8_t d = ((uint8_t*)pmsg)[i];
        if (i != 4) {
            crc = sboxnet_crc8_add(crc, d);
        }
        *(--p) = d;
    }
    dev->tmit_buf[msglen - __builtin_offsetof (struct sboxnet_msg_header, crc) - 1] = crc;

    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)(0x003F)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        dev->tmit_cnt = dev->tmit_bytes = msglen;
        dev->retry_counter = 25;
        sboxnet_check_try_transmit();
    }
    g_sboxnet_dev_def.debug.tmit_msg++;
}
# 98 "test-sboxnet.c" 2
# 111 "test-sboxnet.c"
volatile uint16_t g_lcd_timer = 0;
volatile uint8_t g_lcd_timedout = 0;
volatile uint16_t g_led_timer = 0;

void __vector_14 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_14 (void) {
    g_lcd_timer++;
    if (g_lcd_timer >= 400) {
        g_lcd_timer = 0;
        g_lcd_timedout = 1;
    }
    g_led_timer++;
    if (g_led_timer >= 1000) {
        g_led_timer = 0;

        ((*(PORT_t *) 0x0600)).OUTTGL = ((1<<(5)));
    }
}

void __vector_16 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_16 (void) {
    sboxnet_timer_bit();
}

void init(void) {

    __asm__ __volatile__ ("wdr");

    (*(volatile uint8_t *)(0x0600)) = 0;
    (*(volatile uint8_t *)(0x00B0)) = 0xff;
    (*(volatile uint8_t *)(0x0610)) = PORT_OPC_PULLUP_gc;

    (*(volatile uint8_t *)(0x0620)) = 0;
    (*(volatile uint8_t *)(0x00B0)) = 0xff;
    (*(volatile uint8_t *)(0x0630)) = PORT_OPC_PULLUP_gc;

    (*(volatile uint8_t *)(0x0640)) = 0;
    (*(volatile uint8_t *)(0x00B0)) = 0xff;
    (*(volatile uint8_t *)(0x0650)) = PORT_OPC_PULLUP_gc;

    (*(volatile uint8_t *)(0x0660)) = 0;
    (*(volatile uint8_t *)(0x00B0)) = 0xff;
    (*(volatile uint8_t *)(0x0670)) = PORT_OPC_PULLUP_gc;

    (*(volatile uint8_t *)(0x0680)) = 0;
    (*(volatile uint8_t *)(0x00B0)) = 0xff;
    (*(volatile uint8_t *)(0x0690)) = PORT_OPC_PULLUP_gc;


    (((*(OSC_t *) 0x0050).CTRL) |= (1<<(1)));
    while ((!((*(volatile uint8_t *)(((uint16_t) &((*(OSC_t *) 0x0050).STATUS)))) & (1 << (1)))));
    (*(volatile uint8_t *)(0x0034)) = CCP_IOREG_gc;
    (*(CLK_t *) 0x0040).CTRL = CLK_SCLKSEL_RC32M_gc;

    (*(TC0_t *) 0x0800).CTRLB = TC_WGMODE_NORMAL_gc;
    (*(TC0_t *) 0x0800).CTRLD = 0;
    (*(TC0_t *) 0x0800).CTRLE = 0;
    (*(TC0_t *) 0x0800).INTCTRLA = TC_OVFINTLVL_LO_gc;
    (*(TC0_t *) 0x0800).INTCTRLB = 0;
    (*(TC0_t *) 0x0800).INTFLAGS = 0xff;
    (*(TC0_t *) 0x0800).PER = 255;
    (*(TC0_t *) 0x0800).CTRLA = TC_CLKSEL_DIV64_gc;

    (*(volatile uint8_t *)(0x00A2)) = 0x01;
# 198 "test-sboxnet.c"
    ((*(PORT_t *) 0x0600)).DIRSET = ((1<<(5)));
    ((*(PORT_t *) 0x0600)).OUTCLR = ((1<<(5)));
}

int main(void) {
    struct sboxnet_msg_max msg;

    init();

    g_dev_addr = __eerd_byte_x16a4(0);

    lcd_ks0070_init();

    sboxnet_init();

    sboxnet_receiver_enable();

    __asm__ __volatile__ ("sei" ::: "memory");

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
# 252 "test-sboxnet.c"
        if (g_lcd_timedout) {
            struct sboxnet_device *g_sboxnet_dev = &g_sboxnet_dev_def;


            g_lcd_timedout = 0;

            sidetimer++;
            if (sidetimer >= 10) {
                side = !side;
                sidetimer = 0;
                lcd_display_clear();
            }

            if (side) {
                lcd_set_cursor(0, 0);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("       "); &__c[0];})));
                lcd_set_cursor(0, 0);
                lcd_char(buf[0]);
                lcd_char(buf[1]);
                lcd_char(buf[2]);
                lcd_char(buf[3]);
                lcd_char(buf[4]);

            } else {

                lcd_set_cursor(0, 0);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(0, 0);
                lcd_itoa(g_sboxnet_dev->debug.recv_msg & 0x3f);
                lcd_set_cursor(2, 0);
                lcd_itoa(g_sboxnet_dev->debug.recv_byte & 0x3f);

                lcd_set_cursor(0, 1);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(0, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmit_msg & 0x3f);
                lcd_set_cursor(2, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmit_byte & 0x3f);

                lcd_set_cursor(5, 0);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("   "); &__c[0];})));
                lcd_set_cursor(5, 0);
                lcd_itoa(g_sboxnet_dev->debug.collisions & 0x3f);

                lcd_set_cursor(5, 1);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("   "); &__c[0];})));
                lcd_set_cursor(5, 1);
                lcd_itoa(g_sboxnet_dev->debug.retries & 0x3f);

                lcd_set_cursor(8, 0);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(8, 0);
                lcd_itoa(g_sboxnet_dev->debug.recverrors & 0x3f);
                lcd_set_cursor(10, 0);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(10, 0);
                lcd_itoa(g_sboxnet_dev->debug.crcerrors & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_fe & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_pe & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_dor & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_proto & 7);
                lcd_itoa(g_sboxnet_dev->debug.recverr_size & 7);

                lcd_set_cursor(8, 1);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(8, 1);
                lcd_itoa(g_sboxnet_dev->debug.tmiterrors & 0xff);

                lcd_set_cursor(13, 1);
                lcd_str_P((__extension__({static const char __c[] __attribute__((__progmem__)) = ("    "); &__c[0];})));
                lcd_set_cursor(13, 1);
                lcd_itoa(g_sboxnet_dev->debug.coll_rxc & 7);
                lcd_itoa(g_sboxnet_dev->debug.coll_fe & 7);
                lcd_itoa(g_sboxnet_dev->debug.coll_udr & 7);
            }

        }
    }
    return 0;
}
