# 1 "sboxnet2usb.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "sboxnet2usb.c"
# 39 "sboxnet2usb.c"
# 1 "/usr/lib/gcc/avr/4.9.3/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/avr/4.9.3/include/stdint.h" 3 4
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
# 10 "/usr/lib/gcc/avr/4.9.3/include/stdint.h" 2 3 4
# 40 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/string.h" 1 3
# 45 "/usr/avr/include/string.h" 3
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
# 212 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef unsigned int size_t;
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
# 41 "sboxnet2usb.c" 2
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
# 140 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/iom32u2.h" 1 3
# 141 "/usr/avr/include/avr/io.h" 2 3
# 432 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/portpins.h" 1 3
# 433 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/common.h" 1 3
# 435 "/usr/avr/include/avr/io.h" 2 3

# 1 "/usr/avr/include/avr/version.h" 1 3
# 437 "/usr/avr/include/avr/io.h" 2 3


# 1 "/usr/avr/include/avr/fuse.h" 1 3
# 239 "/usr/avr/include/avr/fuse.h" 3
typedef struct
{
    unsigned char low;
    unsigned char high;
    unsigned char extended;
} __fuse_t;
# 440 "/usr/avr/include/avr/io.h" 2 3


# 1 "/usr/avr/include/avr/lock.h" 1 3
# 443 "/usr/avr/include/avr/io.h" 2 3
# 42 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/interrupt.h" 1 3
# 43 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/wdt.h" 1 3
# 44 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/sleep.h" 1 3
# 45 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/cpufunc.h" 1 3
# 46 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/eeprom.h" 1 3
# 428 "/usr/avr/include/avr/eeprom.h" 3
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 324 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef int wchar_t;
# 429 "/usr/avr/include/avr/eeprom.h" 2 3
# 515 "/usr/avr/include/avr/eeprom.h" 3
uint8_t __eerd_byte_m32u2 (const uint8_t *__p) __attribute__((__pure__));




uint16_t __eerd_word_m32u2 (const uint16_t *__p) __attribute__((__pure__));




uint32_t __eerd_dword_m32u2 (const uint32_t *__p) __attribute__((__pure__));




float __eerd_float_m32u2 (const float *__p) __attribute__((__pure__));





void __eerd_block_m32u2 (void *__dst, const void *__src, size_t __n);





void __eewr_byte_m32u2 (uint8_t *__p, uint8_t __value);




void __eewr_word_m32u2 (uint16_t *__p, uint16_t __value);




void __eewr_dword_m32u2 (uint32_t *__p, uint32_t __value);




void __eewr_float_m32u2 (float *__p, float __value);





void __eewr_block_m32u2 (const void *__src, void *__dst, size_t __n);





void __eeupd_byte_m32u2 (uint8_t *__p, uint8_t __value);




void __eeupd_word_m32u2 (uint16_t *__p, uint16_t __value);




void __eeupd_dword_m32u2 (uint32_t *__p, uint32_t __value);




void __eeupd_float_m32u2 (float *__p, float __value);





void __eeupd_block_m32u2 (const void *__src, void *__dst, size_t __n);
# 47 "sboxnet2usb.c" 2
# 1 "/usr/avr/include/avr/pgmspace.h" 1 3
# 87 "/usr/avr/include/avr/pgmspace.h" 3
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
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
# 48 "sboxnet2usb.c" 2
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
 __tmp = (((16 * 1000000 / 1)) / 1e3) * __ms;
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
 __tmp = (((16 * 1000000 / 1)) / 1e6) * __us;
# 243 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 261 "/usr/avr/include/util/delay.h" 3
}
# 49 "sboxnet2usb.c" 2


# 1 "../avrutilslib/defines.h" 1
# 121 "../avrutilslib/defines.h"
static inline uint8_t rol_byte(uint8_t v) {
    return (v << 1) + (v & 0x80 ? 1 : 0);
}

static inline uint16_t rol_word(uint16_t v) {
    return (v << 1) + (v & 0x8000 ? 1 : 0);
}
# 52 "sboxnet2usb.c" 2

uint8_t g_sboxnet_addr;
struct sboxnet_device g_sboxnet;
# 92 "sboxnet2usb.c"
# 1 "../sboxnet/sboxnet.c" 1
# 22 "../sboxnet/sboxnet.c"
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
# 23 "../sboxnet/sboxnet.c" 2
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
    (*(volatile uint8_t *)((0x3F) + 0x20)) = *__s;
    __asm__ volatile ("" ::: "memory");
}
# 24 "../sboxnet/sboxnet.c" 2
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
# 25 "../sboxnet/sboxnet.c" 2


# 1 "../avrutilslib/random.h" 1
# 25 "../avrutilslib/random.h"
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
# 28 "../sboxnet/sboxnet.c" 2

# 1 "../sboxnet/sboxnet.h" 1
# 49 "../sboxnet/sboxnet.h"
# 1 "../sboxnet/sboxnet-struct.h" 1
# 43 "../sboxnet/sboxnet-struct.h"
struct sboxnet_msg_header {
    uint8_t dstaddr;
    uint8_t srcaddr;
    uint8_t seq;
    struct opt {
        unsigned len : 6;
        unsigned flags : 2;
    } opt;
    uint8_t cmd;
    uint8_t data[0];
} __attribute__ ((packed));







struct sboxnet_msg_max {
    struct sboxnet_msg_header msgh;
    uint8_t data[63 +1];
};


struct sboxnet_debug_info {
    uint16_t recv_byte;
    uint16_t recv_msg_got;
    uint16_t recv_msg_fetched;
    uint16_t tmit_byte;
    uint16_t tmit_msg_tosend;
    uint16_t tmit_msg_sent;
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
# 50 "../sboxnet/sboxnet.h" 2
# 80 "../sboxnet/sboxnet.h"
struct sboxnet_ring_buffer {
    uint8_t rd;
    uint8_t rd_count;
    uint8_t rd_rollback;
    uint8_t wr;
    uint8_t wr_count;
    uint8_t wr_rollback;
    uint8_t count;
    uint8_t size;
    uint8_t buf[0];
};

struct sboxnet_recv_ring_buffer {
    struct sboxnet_ring_buffer rb;
    uint8_t buf[(250)];
};
struct sboxnet_tmit_ring_buffer {
    struct sboxnet_ring_buffer rb;
    uint8_t buf[(250)];
};


struct sboxnet_device {
    uint8_t state;
    uint8_t flags;
    uint8_t retry_counter;
    uint8_t tmit_cnt;
    uint8_t tmit_bytes;
    uint8_t tmit_lastbyte;
    uint8_t backoff_bits;
    uint8_t recv_len;
    uint8_t prng_seed;
    struct sboxnet_recv_ring_buffer recv_buf;
    struct sboxnet_tmit_ring_buffer tmit_buf;


    struct sboxnet_debug_info debug;

};
# 30 "../sboxnet/sboxnet.c" 2




# 1 "../avrutilslib/select-usart.h" 1
# 35 "../sboxnet/sboxnet.c" 2
# 211 "../sboxnet/sboxnet.c"
 static __attribute__((noinline)) void sboxnet_debug_inc_recverrors(void) {
    (&g_sboxnet)->debug.recverrors++;
}

 static __attribute__((noinline)) void sboxnet_debug_inc_recverr_dor(void) {
    (&g_sboxnet)->debug.recverr_dor++;
}

 static __attribute__((noinline)) void sboxnet_debug_inc_recv_byte(void) {
    (&g_sboxnet)->debug.recv_byte++;
}





 static inline uint8_t sboxnet_crc8_init(void) {
    return 0xff;
}

 static uint8_t sboxnet_crc8_add(uint8_t crc, uint8_t v) {
    return _crc_ibutton_update(crc, v);
}

 static inline uint8_t sboxnet_crc8_ok(uint8_t crc) {
    return crc == 0x00;
}



 static uint8_t sboxnet_prandom(void) {
    (&g_sboxnet)->prng_seed = prng_xorshift8((&g_sboxnet)->prng_seed);
    return (&g_sboxnet)->prng_seed;
}




 static void sboxnet_rb_init(struct sboxnet_ring_buffer* rb, uint8_t size) {
    rb->rd = 0;
    rb->rd_count = 0;
    rb->rd_rollback = 0;
    rb->wr = 0;
    rb->wr_count = 0;
    rb->wr_rollback = 0;
    rb->count = 0;
    rb->size = size;
}


 static inline uint8_t sboxnet_rb_getcount(struct sboxnet_ring_buffer* rb) {
    return rb->count;
}
 static inline uint8_t sboxnet_rb_getfree(struct sboxnet_ring_buffer* rb) {
    return rb->size - rb->count - rb->wr_count;
}

 static inline void sboxnet_rb_write_commit(struct sboxnet_ring_buffer* rb) {

    if ((rb->count + rb->wr_count) > rb->size) {
        (((&g_sboxnet)->flags) |= (1<<(2)));
    }

    rb->count += rb->wr_count;
    rb->wr_count = 0;
    rb->wr_rollback = rb->wr;
}

 static void sboxnet_rb_write_rollback(struct sboxnet_ring_buffer* rb) {
    rb->wr_count = 0;
    rb->wr = rb->wr_rollback;
}

 static uint8_t sboxnet_rb_write(struct sboxnet_ring_buffer* rb, uint8_t a_v) {
    uint8_t rc = 0;
    if ((uint8_t)(rb->count + rb->wr_count) < rb->size) {
        rb->wr_count++;
        rb->buf[rb->wr] = a_v;
        rb->wr++;
        if (rb->wr >= rb->size) {
            rb->wr = 0;
        }
        rc = 1;
    } else {

        (((&g_sboxnet)->flags) |= (1<<(2)));

        rc = 0;
    }
    return rc;
}

 static void sboxnet_rb_put_first(struct sboxnet_ring_buffer* rb, uint8_t v) {
    rb->buf[rb->wr_rollback] = v;
}

 static inline uint8_t sboxnet_rb_write_count(struct sboxnet_ring_buffer* rb) {
    return rb->wr_count;
}

 static void sboxnet_rb_read_commit(struct sboxnet_ring_buffer* rb) {

    if (rb->count < rb->rd_count) {
        (((&g_sboxnet)->flags) |= (1<<(1)));
    }

    rb->count -= rb->rd_count;
    rb->rd_count = 0;
    rb->rd_rollback = rb->rd;
}

 static void sboxnet_rb_read_rollback(struct sboxnet_ring_buffer* rb) {
    rb->rd_count = 0;
    rb->rd = rb->rd_rollback;
}

 static uint8_t sboxnet_rb_read(struct sboxnet_ring_buffer* rb) {
    uint8_t v = 0;
    if (rb->count > rb->rd_count) {
        rb->rd_count++;
        v = rb->buf[rb->rd];
        rb->rd++;
        if (rb->rd >= rb->size) {
            rb->rd = 0;
        }
    } else {

        (((&g_sboxnet)->flags) |= (1<<(1)));

    }
    return v;
}

 static uint8_t sboxnet_rb_read_getone(struct sboxnet_ring_buffer* rb) {
    uint8_t v = 0;
    if (rb->count > rb->rd_count) {
        v = rb->buf[rb->rd];
    } else {

        (((&g_sboxnet)->flags) |= (1<<(1)));

    }
    return v;
}

 static inline uint8_t sboxnet_rb_can_read_bytes(struct sboxnet_ring_buffer* rb) {
    return rb->count - rb->rd_count;
}

 __attribute__((noinline)) __attribute__((used)) static void sboxnet_receiver_enable(void) {




    (((*(volatile uint8_t *)(0xC8))) |= (1<<(0)));
    (*(volatile uint8_t *)(0xC9)) |= ((1<<(4))|(1<<(7)));

}



 __attribute__((noinline)) __attribute__((used)) static void sboxnet_init(void) {
    struct sboxnet_device *dev = (&g_sboxnet);
    memset(dev, 0, sizeof(struct sboxnet_device));
    dev->state = 0;
    dev->prng_seed = (*(volatile uint8_t *)(0x66));
    dev->retry_counter = 25;

    sboxnet_rb_init(&dev->recv_buf.rb, sizeof(dev->recv_buf.buf));
    sboxnet_rb_init(&dev->tmit_buf.rb, sizeof(dev->tmit_buf.buf));

    *(&((*(volatile uint8_t *)((0x0B) + 0x20))) - 1) &= ~((1<<(2)));
    ((*(volatile uint8_t *)((0x0B) + 0x20))) |= ((1<<(2)));

    ((*(volatile uint8_t *)((0x0B) + 0x20))) |= ((1<<(3)));
    *(&((*(volatile uint8_t *)((0x0B) + 0x20))) - 1) |= ((1<<(3)));
# 409 "../sboxnet/sboxnet.c"
    (((*(volatile uint8_t *)((0x1D) + 0x20))) &= ~(1<<(2)));
    (((*(volatile uint8_t *)(0x69))) &= ~(1<<(5)));
    (((*(volatile uint8_t *)(0x69))) |= (1<<(4)));
    (*(volatile uint8_t *)((0x1C) + 0x20)) = (1<<(2));

    (*(volatile uint8_t *)(0xCD)) = ((uint8_t)(((((16 * 1000000 / 1) / 16 / 250000) - 1)) >> 8)) & 0x7f;
    (*(volatile uint8_t *)(0xCC)) = ((uint8_t)((((16 * 1000000 / 1) / 16 / 250000) - 1)));
    (*(volatile uint8_t *)(0xC8)) = (1 << (6))|(0 << (1))|(0 << (0));
    (*(volatile uint8_t *)(0xCA)) =



            (0 << (6))|(1 << (5))|(0 << (4))|(0 << (3))|(1 << (2))|(1 << (1));

    (*(volatile uint8_t *)(0xC9)) = (0 << (7))|(0 << (6))|(0 << (5))|(0 << (4))|(0 << (3))|(1 << (2))|(0 << (0));


    sboxnet_receiver_enable();
}



 __attribute__((unused))
static void sboxnet_deinit(void) {
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        (&g_sboxnet)->state = 0;




        (*(volatile uint8_t *)(0xC9)) = (0 << (7))|(0 << (6))|(0 << (5))|(0 << (4))|(0 << (3))|(1 << (2))|(0 << (0));


        *(&((*(volatile uint8_t *)((0x0B) + 0x20))) - 1) &= ~((1<<(2)));
        *(&((*(volatile uint8_t *)((0x0B) + 0x20))) - 1) &= ~((1<<(3)));
        ((*(volatile uint8_t *)((0x0B) + 0x20))) |= ((1<<(3)));







        (((*(volatile uint8_t *)((0x1D) + 0x20))) &= ~(1<<(2)));


    }
}

 static void sboxnet_set_timer(uint16_t offset) {
# 471 "../sboxnet/sboxnet.c"
    register uint16_t ocr = (*(volatile uint16_t *)(0x84));
    (*(volatile uint16_t *)(0x8A)) = (ocr + offset) & 0xffff;
    (*(volatile uint8_t *)((0x16) + 0x20)) = (1<<(2));
    (((*(volatile uint8_t *)(0x6F))) |= (1<<(2)));

}

 static inline void sboxnet_clear_line_sensor(void) {






    (*(volatile uint8_t *)((0x1C) + 0x20)) = (1<<(2));


}

 static inline uint8_t sboxnet_line_sensor_changed(void) {






    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0x1C) + 0x20)))))) & (1 << (2)));


}

 static void sboxnet_set_backoff_with_offset(uint16_t offset) {
    sboxnet_clear_line_sensor();
    (&g_sboxnet)->state = 1;
    sboxnet_set_timer(8 * 15 + offset);
}


 static void sboxnet_intr_usart_rx(void) {
    uint8_t rxb8, rxb, rflags;
    struct sboxnet_device *dev = (&g_sboxnet);
    __asm__ __volatile__ ("" : "+r"(dev) ::);







    rflags = (*(volatile uint8_t *)(0xC8));
    rxb8 = ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC9)))))) & (1 << (1)));
    rxb = (*(volatile uint8_t *)(0xCE));
    rflags &= ((1<<(4))|(1<<(3))|(1<<(2)));

    if (rflags) {
        sboxnet_debug_inc_recverrors();





        if (rflags & (1<<(4))) { dev->debug.recverr_fe++; }
        if (rflags & (1<<(3))) { sboxnet_debug_inc_recverr_dor(); }
        if (rflags & (1<<(2))) { dev->debug.recverr_pe++; }

        goto receive_reset;

    } else if (rxb8) {
        if (sboxnet_rb_write_count(&dev->recv_buf.rb) != 0) {
            dev->debug.recverr_proto++;
            sboxnet_debug_inc_recverrors();
            sboxnet_rb_write_rollback(&dev->recv_buf.rb);
        }
        if (rxb == g_sboxnet_addr || rxb == 255

            || ((*(volatile uint8_t *)(((uint16_t) &(dev->flags)))) & (1 << (0)))

                                                ) {



            (((*(volatile uint8_t *)(0xC8))) &= ~(1<<(0)));

            if (!sboxnet_rb_write(&dev->recv_buf.rb, 0)) {
                goto rec_dor;
            }
            if (sboxnet_rb_write(&dev->recv_buf.rb, rxb)) {
                sboxnet_debug_inc_recv_byte();
                dev->recv_len = (5 +1);
            } else {
              rec_dor:
                sboxnet_debug_inc_recverr_dor();
                goto receive_error;
            }
        } else {

            goto receive_reset;
        }

    } else {
        uint8_t writecount = sboxnet_rb_write_count(&dev->recv_buf.rb);
        if (writecount > dev->recv_len) {
            dev->debug.recverr_size++;
            goto receive_error;
        } else {
            if (writecount == (__builtin_offsetof (struct sboxnet_msg_header, opt)+1)) {
                dev->recv_len = (5 +1) + (rxb & 0x3f);
            }
            if (!sboxnet_rb_write(&dev->recv_buf.rb, rxb)) {
                sboxnet_debug_inc_recverr_dor();
                goto receive_error;
            }
            sboxnet_debug_inc_recv_byte();
            if (sboxnet_rb_write_count(&dev->recv_buf.rb) == dev->recv_len+1) {
                sboxnet_rb_put_first(&dev->recv_buf.rb, dev->recv_len);
                dev->recv_len = 0;



                (((*(volatile uint8_t *)(0xC8))) |= (1<<(0)));

                sboxnet_rb_write_commit(&dev->recv_buf.rb);
                (&g_sboxnet)->debug.recv_msg_got++;
            }
        }
    }
    return;

receive_error:
    sboxnet_debug_inc_recverrors();
receive_reset:
    dev->recv_len = 0;



    (((*(volatile uint8_t *)(0xC8))) |= (1<<(0)));

    sboxnet_rb_write_rollback(&dev->recv_buf.rb);
}


 static void sboxnet_check_try_transmit(void) {
    struct sboxnet_device *dev = (&g_sboxnet);
    __asm__ __volatile__ ("" : "+r"(dev) ::);

    if (dev->tmit_bytes == 0 && sboxnet_rb_getcount(&dev->tmit_buf.rb) > (5 +1)) {
        dev->tmit_cnt = 0;
        dev->tmit_bytes = sboxnet_rb_read(&dev->tmit_buf.rb);
    }
    if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
        if (dev->state == 0) {
            uint16_t offset = sboxnet_prandom() & 0x3f;
            sboxnet_set_backoff_with_offset((uint16_t)dev->retry_counter * (8 / 4) + 8*2 + offset);
        }
    }
}

 static void sboxnet_intr_usart_tx(void) {
    struct sboxnet_device *dev = (&g_sboxnet);
    __asm__ __volatile__ ("" : "+r"(dev) ::);
# 639 "../sboxnet/sboxnet.c"
    uint8_t ucsra = (*(volatile uint8_t *)(0xC8));
    uint8_t crxc = !(ucsra & (1<<(7)));
    uint8_t cfe = (ucsra & ((1<<(4))|(1<<(2))));
    uint8_t cudr = ((*(volatile uint8_t *)(0xCE)) != dev->tmit_lastbyte);

    if (crxc || cfe || cudr) {
        if (crxc) { dev->debug.coll_rxc++; }
        if (cfe) { dev->debug.coll_fe++; }
        if (cudr) { dev->debug.coll_udr++; }

        ((*(volatile uint8_t *)((0x0B) + 0x20))) &= ~((1<<(3)));




        (*(volatile uint8_t *)(0xC9)) &= ~((1<<(6))|(1<<(3))|(1<<(4)));

        dev->state = 2;
        dev->debug.collisions++;
        if (dev->retry_counter > 0) {
            dev->retry_counter--;
            sboxnet_rb_read_rollback(&dev->tmit_buf.rb);

            dev->debug.retries++;
        } else {
            while (dev->tmit_cnt < dev->tmit_bytes) {
                sboxnet_rb_read(&dev->tmit_buf.rb);
                dev->tmit_cnt++;
            }
            dev->tmit_cnt = dev->tmit_bytes = 0;
            dev->retry_counter = 25;
            sboxnet_rb_read_commit(&dev->tmit_buf.rb);
            dev->debug.tmiterrors++;
        }
        dev->tmit_cnt = dev->tmit_bytes = 0;

        sboxnet_set_timer(8 * 16);
        return;
    }





    (*(volatile uint8_t *)(0xC9)) &= ~((1<<(7))|(1<<(4)));
    (*(volatile uint8_t *)(0xC9)) |= (1<<(4));


    if (dev->tmit_cnt < dev->tmit_bytes) {
        uint8_t v = sboxnet_rb_read(&dev->tmit_buf.rb);
        dev->tmit_cnt++;
        dev->tmit_lastbyte = v;




        (((*(volatile uint8_t *)(0xC9))) &= ~(1<<(0)));
        (*(volatile uint8_t *)(0xCE)) = v;

    } else {



        (((*(volatile uint8_t *)(0xC9))) &= ~(1<<(6)));

        dev->tmit_bytes = 0;
        dev->tmit_cnt = 0;
        sboxnet_rb_read_commit(&dev->tmit_buf.rb);
        (&g_sboxnet)->debug.tmit_msg_sent++;

        sboxnet_receiver_enable();
        if (dev->state == 3) {
            dev->state = 0;
        }
        dev->retry_counter = 25;
        sboxnet_check_try_transmit();
    }
    dev->debug.tmit_byte++;
}
# 730 "../sboxnet/sboxnet.c"
 void __vector_23 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_23 (void) {
    sboxnet_intr_usart_rx();
}

 void __vector_25 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_25 (void) {
    sboxnet_intr_usart_tx();
}



 static void sboxnet_timer_bit(void) {
    struct sboxnet_device *dev = (&g_sboxnet);




    (((*(volatile uint8_t *)(0x6F))) &= ~(1<<(2)));

    if (dev->state == 1) {
        if (dev->tmit_bytes > 0 && dev->tmit_cnt == 0) {
            uint8_t d = sboxnet_rb_read_getone(&dev->tmit_buf.rb);

            ((*(volatile uint8_t *)((0x0B) + 0x20))) |= ((1<<(3)));




            (*(volatile uint8_t *)(0xC9)) |= (1<<(3))|(1<<(0));

            if (sboxnet_line_sensor_changed() || (!((*(volatile uint8_t *)(((uint16_t) &((*(&(*(volatile uint8_t *)((0x0B) + 0x20)) - 2)))))) & (1 << (2))))) {
                uint16_t offset = sboxnet_prandom() & 0x3f;
                sboxnet_set_backoff_with_offset((uint16_t)dev->retry_counter * (8 / 4) + offset);

            } else {
# 772 "../sboxnet/sboxnet.c"
                (*(volatile uint8_t *)(0xC9)) &= ~((1<<(4))|(1<<(7)));
                (*(volatile uint8_t *)(0xC9)) |= (1<<(4))|(1<<(6));
                (*(volatile uint8_t *)(0xCE)) = d;
                (((*(volatile uint8_t *)(0xC8))) &= ~(1<<(0)));

                sboxnet_rb_read(&dev->tmit_buf.rb);
                dev->tmit_lastbyte = d;
                dev->tmit_cnt++;
                dev->state = 3;
            }
        } else {
            dev->state = 0;
        }
        return;
    }

    if (dev->state == 2) {
        ((*(volatile uint8_t *)((0x0B) + 0x20))) |= ((1<<(3)));
        dev->state = 4;
        sboxnet_set_timer(8 * 4);
        sboxnet_clear_line_sensor();
        return;
    }

    if (dev->state == 4) {
        if (!sboxnet_line_sensor_changed() && ((*(volatile uint8_t *)(((uint16_t) &((*(&(*(volatile uint8_t *)((0x0B) + 0x20)) - 2)))))) & (1 << (2)))) {
            sboxnet_receiver_enable();
            dev->state = 0;
            sboxnet_check_try_transmit();
        } else {
            sboxnet_clear_line_sensor();
            sboxnet_set_timer(8 * 4);
        }
        return;
    }
}



 __attribute__((noinline)) __attribute__((used)) static int8_t sboxnet_receive_msg(struct sboxnet_msg_header* pmsg, uint8_t maxmsglen) {
    if (maxmsglen < (5 +1))
        return (-3);

    struct sboxnet_device *dev = (&g_sboxnet);
    uint8_t msglen = 0;
    uint8_t maxb = (5 +1);
    uint8_t avail;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        avail = sboxnet_rb_can_read_bytes(&dev->recv_buf.rb);
        if (avail > (5 +1))
            msglen = sboxnet_rb_read_getone(&dev->recv_buf.rb);
    }
    if (msglen < maxb ) {
        return (-1);
    }
    if ((msglen+1) > avail) {
        return (-1);
    }
    if (msglen > maxmsglen) {
        return (-3);
    }
    if (msglen > (63 +(5 +1))) {
        return (-4);
    }
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        sboxnet_rb_read(&dev->recv_buf.rb);
    }
    uint8_t* p = (uint8_t*)pmsg;
    uint8_t crc = sboxnet_crc8_init();
    for (uint8_t i = 0; i < msglen; i++) {
        uint8_t v;
        for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
            v = sboxnet_rb_read(&dev->recv_buf.rb);
        }
        if (i == __builtin_offsetof (struct sboxnet_msg_header, opt)) {
            maxb += (v & 0x3f);
        }
        crc = sboxnet_crc8_add(crc, v);
        *p++ = v;
    }
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        sboxnet_rb_read_commit(&dev->recv_buf.rb);
    }

    if (!sboxnet_crc8_ok(crc)) {
        sboxnet_debug_inc_recverrors();
        (&g_sboxnet)->debug.crcerrors++;
        return (-2);
    } else {
       (&g_sboxnet)->debug.recv_msg_fetched++;
    }
    return msglen;
}



 __attribute__((noinline)) __attribute__((used)) static uint8_t sboxnet_send_msg(struct sboxnet_msg_header* pmsg) {
    struct sboxnet_device *dev = (&g_sboxnet);
    uint8_t msglen = (5 +1) + pmsg->opt.len;

    uint8_t free;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        free = sboxnet_rb_getfree(&dev->tmit_buf.rb);
    }
    if (!(free > msglen)) {
        return 1;
    }

    uint8_t rc;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, msglen);
    }
    if (!rc) {
        goto send_error;
    }
    uint8_t crc = sboxnet_crc8_init();
    uint8_t i;
    for (i = 0; i < (msglen-1); i++) {
        uint8_t d;
        if (i == 1) {
            d = g_sboxnet_addr;
        } else {
            d = ((uint8_t*)pmsg)[i];
        }
        crc = sboxnet_crc8_add(crc, d);
        for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
            rc = sboxnet_rb_write(&dev->tmit_buf.rb, d);
        }
        if (!rc) {
            goto send_error;
        }
    }
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        rc = sboxnet_rb_write(&dev->tmit_buf.rb, crc);
    }
    if (!rc) {
 send_error:
        for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
            sboxnet_rb_write_rollback(&dev->tmit_buf.rb);
        }
        return 1;
    }
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        sboxnet_rb_write_commit(&dev->tmit_buf.rb);
        sboxnet_check_try_transmit();
    }
    (&g_sboxnet)->debug.tmit_msg_tosend++;
    return 0;
}

 __attribute__((used)) static uint8_t sboxnet_can_send_msg(struct sboxnet_msg_header* pmsg) {
    uint8_t msglen = (5 +1) + pmsg->opt.len;
    uint8_t free;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        free = sboxnet_rb_getfree(&(&g_sboxnet)->tmit_buf.rb);
    }
    return (free > msglen);
}
 __attribute__((used)) static uint8_t sboxnet_all_sent(void) {
    return sboxnet_rb_getcount(&(&g_sboxnet)->tmit_buf.rb) == 0;
}
 __attribute__((used)) static uint8_t sboxnet_can_read(void) {
    uint8_t b;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        b = sboxnet_rb_can_read_bytes(&(&g_sboxnet)->recv_buf.rb);
    }
    return (b >= (5 +1));
}
 __attribute__((used)) static uint8_t sboxnet_can_send(void) {
    uint8_t free;
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        free = sboxnet_rb_getfree(&(&g_sboxnet)->tmit_buf.rb);
    }
    return (free > (5 +1));
}

 static void sboxnet_set_sniffer_mode(uint8_t on) {
    if (on) {
        (((&g_sboxnet)->flags) |= (1<<(0)));
    } else {
        (((&g_sboxnet)->flags) &= ~(1<<(0)));
    }
}




# 1 "../avrutilslib/select-usart.h" 1
# 960 "../sboxnet/sboxnet.c" 2
# 93 "sboxnet2usb.c" 2

 void __vector_16 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_16 (void) {
    sboxnet_timer_bit();
}

enum CMDS {
    CMD_SBOXNET_SET_MODE = 0x30,
    CMD_SBOXNET_GET_STATUS = 0x31,
    CMD_SBOXNET_RECEIVE = 0x32,
    CMD_SBOXNET_TRANSMIT = 0x33,
    CMD_SBOXNET_GET_DBGINFO = 0x3a,
    CMD_SBOXNET_GET_DBGSTATE = 0x3b,
    CMD_SBOXNET_GET_DBGRECVBUF = 0x3c,
    CMD_SBOXNET_GET_DBGTMITBUF = 0x3d,

    CMD_DBG_STACK = 0x40,
    CMD_SET_SERIALNUMBER = 0x51,




};





uint8_t g_sboxnet_flags = 0;
# 129 "sboxnet2usb.c"
uint8_t g_sboxnet_status = 0;

uint8_t g_timer = 0;
uint8_t g_timer_led_msg_read = 0;
uint8_t g_timer_led_msg_write = 0;





static void trigger_led_msg_read(void) {
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        g_timer_led_msg_read = 100/33;
        (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(6)));
    }
}
static void trigger_led_msg_write(void) {
    for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
        g_timer_led_msg_write = 100/33;
        (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(7)));
    }
}



extern uint8_t _end;
extern uint8_t __stack;



static uint16_t stack_free(void) {
    uint8_t* p = &_end;
    while (p < &__stack) {
        if (*p != 0xa5) {
            break;
        }
        p++;
    }
    return (uint16_t)(p - &_end);
}

static uint16_t stack_size(void) {
    return (uint16_t)(&__stack - &_end);
}

__attribute__ ((section(".init8"))) __attribute__((naked))
void stack_init8(void) {
    uint8_t* p = &_end;
    while (p < &__stack) {
        *p++ = 0xa5;
    }
}



# 1 "../avrutilslib/usb/usb.c" 1
# 26 "../avrutilslib/usb/usb.c"
# 1 "../avrutilslib/logger-usart.h" 1
# 27 "../avrutilslib/usb/usb.c" 2
# 1 "../avrutilslib/ringbuffer.h" 1
# 29 "../avrutilslib/ringbuffer.h"
struct ring_buffer {
    uint8_t rd;
    uint8_t wr;
    uint8_t size;
    uint8_t count;
    uint8_t buf[0];
};


void ringbuf_init(struct ring_buffer* a_rb, uint8_t a_size);

uint8_t ringbuf_read(struct ring_buffer* a_rb);

void ringbuf_write(struct ring_buffer* a_rb, uint8_t a_v);

static inline uint8_t ringbuf_isempty(struct ring_buffer* a_rb) {
    return a_rb->count == 0;
}

static inline uint8_t ringbuf_isfull(struct ring_buffer* a_rb) {
    return a_rb->count == a_rb->size;
}

static inline uint8_t ringbuf_getfree(struct ring_buffer* a_rb) {
    return a_rb->size - a_rb->count;
}

static inline uint8_t ringbuf_count(struct ring_buffer* a_rb) {
    return a_rb->count;
}
# 28 "../avrutilslib/usb/usb.c" 2
# 1 "../avrutilslib/usb/usb.h" 1
# 44 "../avrutilslib/usb/usb.h"
struct usb_device_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
};

struct usb_device_qualifier {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint8_t bNumConfigurations;
    uint8_t bReserved;
};





struct usb_configuration_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
};

struct usb_other_speed_config_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
};

struct usb_interface_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocoll;
    uint8_t iInterface;
};
# 129 "../avrutilslib/usb/usb.h"
struct usb_endpoint_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
};

struct usb_string_desc {
    uint8_t bLength;
    uint8_t bDescriptorType;
};
# 181 "../avrutilslib/usb/usb.h"
struct usb_device_request {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};
# 211 "../avrutilslib/usb/usb.h"
typedef uint8_t (*get_string_func_t)(struct usb_device_request*, uint8_t);

struct usb_dev {
    volatile uint8_t state;
    uint8_t cur_config;
    const struct usb_device_desc* pdesc_dev;
    const void* pdesc_config;
    uint8_t desc_config_size;
    const void* const* pdesc_string;
    uint8_t desc_string_num;
    get_string_func_t get_string_func;
    uint8_t desc_dtype;
};

extern struct usb_dev g_usb_dev;







void usb_event_set_configuration(void);
uint8_t usb_event_ep0_request(struct usb_device_request*);



static inline uint8_t usb_ep_isSETUP(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xE8)))))) & (1 << (3)));
}
static inline void usb_ep_clearSETUP(void) {
    (((*(volatile uint8_t *)(0xE8))) &= ~(1<<(3)));
}
uint8_t inline usb_ep_isIN(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xE8)))))) & (1 << (0)));
}
static inline void usb_ep_clearIN(void) {
    (((*(volatile uint8_t *)(0xE8))) &= ~(1<<(0)));
}
static inline void usb_ep_clearFIFOCON(void) {
    (((*(volatile uint8_t *)(0xE8))) &= ~(1<<(7)));
}
static inline uint8_t usb_ep_isOUT(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xE8)))))) & (1 << (2)));
}
static inline void usb_ep_clearOUT(void) {
    (((*(volatile uint8_t *)(0xE8))) &= ~(1<<(2)));
}
static inline void usb_ep_set_stall(void) {
    (((*(volatile uint8_t *)(0xEB))) |= (1<<(5)));
}

static inline uint8_t usb_req_isOUT(struct usb_device_request* ur) {
    return (ur->bmRequestType & 0x80) == 0x00;
}
static inline uint8_t usb_req_isIN(struct usb_device_request* ur) {
    return (ur->bmRequestType & 0x80) == (1<<7);
}


void usb_ep_read_data(void* buf, uint8_t size);
uint8_t usb_ep_waitIN(void);
uint8_t usb_ep_send_zlp(void);
uint8_t usb_mem_read_byte(const void* psrc, uint8_t srctype);
uint8_t usb_ep_getbufsize(void);
uint8_t usb_ep_send_in_data(const void* psrc, uint16_t size, uint8_t srctype);
uint8_t usb_ep_send_control_data_chunk(const void* psrc, uint16_t size, uint8_t srctype);
uint8_t usb_ep_send_control_data_finish(void);
uint8_t usb_ep_send_control_data(const void* psrc, uint16_t size, uint8_t srctype, uint16_t reqsize);
uint8_t usb_ep_recv_control_data_chunk(void* psrc, uint16_t* psize, uint16_t size);
uint8_t usb_ep_recv_control_data_finish(void);
uint8_t usb_ep_recv_control_data(void* psrc, uint16_t* psize, uint16_t reqsize);
void usb_ep0_task(void);
# 29 "../avrutilslib/usb/usb.c" 2
# 40 "../avrutilslib/usb/usb.c"
struct usb_dev g_usb_dev;
# 72 "../avrutilslib/usb/usb.c"
void usb_intr_endofreset(void);


void __vector_11 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_11 (void) {
    ;
    uint8_t intf = (*(volatile uint8_t *)(0xE1));
    uint8_t inte = (*(volatile uint8_t *)(0xE2));

    uint8_t epnum_save = (*(volatile uint8_t *)(0xE9));

    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (6)))) {
        ;
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (5)))) {
        ;
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (4)))) {
        ;
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (3)))) {
        ;
        if (((*(volatile uint8_t *)(((uint16_t) &(inte)))) & (1 << (3)))) {
            usb_intr_endofreset();
        }
        (((*(volatile uint8_t *)(0xE1))) &= ~(1<<(3)));
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (2)))) {
        ;
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (0)))) {
        ;
    }
    ;
    (*(volatile uint8_t *)(0xE1)) = 0;
    (*(volatile uint8_t *)(0xE9)) = epnum_save;
}


void usb_ep_read_data(void* buf, uint8_t size) {
    while (size) {
        *(uint8_t*)buf++ = (*(volatile uint8_t *)(0xF1));
        size--;
    }
}

uint8_t usb_ep_waitIN(void) {
    while (!usb_ep_isIN()) {
        if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
            return 3;
        }
    }
    return 0;
}

uint8_t usb_ep_send_zlp(void) {
    uint8_t rc = usb_ep_waitIN();
    if (!rc) {
        usb_ep_clearIN();
        rc = usb_ep_waitIN();
    }
    return rc;
}


uint8_t usb_mem_read_byte(const void* psrc, uint8_t srctype) {
    switch (srctype) {
        case 1:
            return (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)((const uint8_t*)psrc)); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }));

        case 2:
            return __eerd_byte_m32u2((const uint8_t*)psrc);

        case 0:
        default:
            return *(const uint8_t*)psrc;
    }
}

uint8_t usb_ep_getbufsize(void) {
    static const uint8_t epsizemap[4] __attribute__((__progmem__)) = { 8, 16, 32, 64 };
    return (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(&epsizemap[((*(volatile uint8_t *)(0xED)) >> 4) & 0x03])); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }));
}



uint8_t usb_ep_send_in_data(const void* psrc, uint16_t size, uint8_t srctype) {
    ;
    ;
    ;
    ;
    ;

    for (;;) {
        if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
            return 3;
        } else if (usb_ep_isSETUP()) {
            ;
            return 1;
        } else if (usb_ep_isOUT()) {
            usb_ep_clearOUT();
            ;
            return 2;
        } else if (!usb_ep_isIN()) {
            ;
            continue;
        }

        while (size && ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xE8)))))) & (1 << (5)))) {
            uint8_t d = usb_mem_read_byte(psrc++, srctype);
            (*(volatile uint8_t *)(0xF1)) = d;
            size--;
        }
        ;
        usb_ep_clearIN();
        usb_ep_clearFIFOCON();

        if (size == 0) {
            break;
        }
    }
    ;
    return 0;
}



uint8_t usb_ep_send_control_data_chunk(const void* psrc, uint16_t size, uint8_t srctype) {
    uint8_t epsize = usb_ep_getbufsize();

    ;
    ;
    ;
    ;
    ;

    for (;;) {
        if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
            return 3;
        } else if (usb_ep_isSETUP()) {
            ;
            return 1;
        } else if (usb_ep_isOUT()) {
            usb_ep_clearOUT();
            ;
            return 2;
        } else if (!usb_ep_isIN()) {
            ;
            continue;
        }

        uint8_t written = (*(volatile uint8_t *)(0xF2));
        while (size && written < epsize) {
            (*(volatile uint8_t *)(0xF1)) = usb_mem_read_byte(psrc++, srctype);
            size--;
            written++;
        }
        if (written == epsize) {
            ;
            usb_ep_clearIN();
        }
        if (size == 0) {
            break;
        }
    }
    return 0;
}

uint8_t usb_ep_send_control_data_finish(void) {
    for (;;) {
        if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
            return 3;
        } else if (usb_ep_isSETUP()) {
            ;
            return 1;
        } else if (usb_ep_isOUT()) {
            usb_ep_clearOUT();
            ;
            return 2;
        } else if (!usb_ep_isIN()) {
            ;
            continue;
        }
        ;
        usb_ep_clearIN();
        break;
    }
    ;
    while (!usb_ep_isOUT()) {
        if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
            return 3;
        } else if (usb_ep_isSETUP()) {
            ;
            return 1;
        }
    }
    usb_ep_clearOUT();
    ;
    return 0;
}

uint8_t usb_ep_send_control_data(const void* psrc, uint16_t size, uint8_t srctype, uint16_t reqsize) {
    uint8_t r = usb_ep_send_control_data_chunk(psrc, ((size) < (reqsize) ? (size) : (reqsize)), srctype);
    if (r == 0) {
        r = usb_ep_send_control_data_finish();
    }
    return r;
}



uint8_t usb_ep_recv_control_data_chunk(void* psrc, uint16_t* psize, uint16_t size) {
    uint16_t readb = 0;
    *psize = 0;
    if (size) {
        for (;;) {
            if (!(g_usb_dev.state & (0x08|0x10|0x04))) {
                return 3;
            } else if (usb_ep_isSETUP()) {
                return 1;
            } else if (!usb_ep_isOUT()) {
                continue;
            }

            while ((*(volatile uint8_t *)(0xF2)) && readb < size) {
                *(uint8_t*)psrc++ = (*(volatile uint8_t *)(0xF1));
                readb++;
            }
            if ((*(volatile uint8_t *)(0xF2)) == 0) {
                usb_ep_clearOUT();
            }
            if (readb >= size) {
                *psize = readb;
                break;
            }
        }
    }
    return 0;
}

uint8_t usb_ep_recv_control_data_finish(void) {
    usb_ep_clearOUT();
    return usb_ep_send_zlp();
}

uint8_t usb_ep_recv_control_data(void* psrc, uint16_t* psize, uint16_t reqsize) {
    uint8_t r = usb_ep_recv_control_data_chunk(psrc, psize, reqsize);
    if (r == 0) {
        r = usb_ep_recv_control_data_finish();
    }
    return r;
}



static uint8_t usb_ep0_get_configuration(void) {
    struct usb_dev* dev = &g_usb_dev;
    if (dev->state & (0x08|0x10)) {
        usb_ep_clearSETUP();
        usb_ep_send_control_data(&dev->cur_config, 1, 0, 1);
        return 0;
    }
    return 1;
}

static uint8_t usb_ep0_get_descriptor(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;
    switch (((uint8_t)((req->wValue) >> 8))) {
        case 1:
        {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(dev->pdesc_dev, sizeof(struct usb_device_desc), dev->desc_dtype, req->wLength);
            return 0;
        }

        case 2:
        {
            uint8_t idx = ((uint8_t)(req->wValue));
            if (idx == 0) {
                usb_ep_clearSETUP();
                usb_ep_send_control_data(dev->pdesc_config, dev->desc_config_size, dev->desc_dtype, req->wLength);
                return 0;
            }
            break;
        }

        case 3:
        {
            uint8_t idx = ((uint8_t)(req->wValue));
            if (dev->get_string_func != ((void *)0) && (*dev->get_string_func)(req, idx) == 0) {
                return 0;
            }
            if (idx < dev->desc_string_num) {
                const struct usb_string_desc* paddr;

                switch (dev->desc_dtype) {
                    case 1:
                        paddr = (const struct usb_string_desc*)(__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(dev->pdesc_string + idx)); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; }));
                        break;

                    case 2:
                        paddr = (const struct usb_string_desc*)__eerd_word_m32u2((const uint16_t*)dev->pdesc_string + idx);
                        break;

                    default:
                        paddr = (const struct usb_string_desc*)dev->pdesc_string[idx];
                        break;
                }
                if (paddr != ((void *)0)) {
                    usb_ep_clearSETUP();
                    usb_ep_send_control_data(paddr, usb_mem_read_byte(paddr, dev->desc_dtype), dev->desc_dtype, req->wLength);
                    return 0;
                }
            }
            break;
        }

        default:
            break;
    }
    return 1;
}

static uint8_t usb_ep0_set_address(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;

    if (dev->state & (0x04|0x08)) {
        uint8_t addr = req->wValue & 0x007f;
        usb_ep_clearSETUP();
        (*(volatile uint8_t *)(0xE3)) = addr;
        usb_ep_send_zlp();
        (((*(volatile uint8_t *)(0xE3))) |= (1<<(7)));
        if (addr) {
            dev->state = 0x08;
        } else {
            dev->state = 0x04;
        }
        return 0;
    }
    return 1;
}

static uint8_t usb_ep0_set_configuration(struct usb_device_request* req) {
    struct usb_dev* dev = &g_usb_dev;
    if (dev->state & (0x08|0x10)) {
        uint8_t confnr = ((uint8_t)(req->wValue));
        if (confnr == 0) {
            dev->state = 0x08;
            dev->cur_config = 0;

        } else {
            uint8_t cnr = usb_mem_read_byte(&((struct usb_configuration_desc*)dev->pdesc_config)->bConfigurationValue, dev->desc_dtype);
            if (confnr == cnr) {
                dev->state = 0x10;
                dev->cur_config = confnr;

                usb_event_set_configuration();

                (*(volatile uint8_t *)(0xE9)) = 0;

            } else {
                return 1;
            }
        }
        usb_ep_clearSETUP();
        usb_ep_send_zlp();
        return 0;
    }
    return 1;
}


void usb_ep0_task(void) {
    struct usb_device_request req;

    (*(volatile uint8_t *)(0xE9)) = 0;
    if (usb_ep_isSETUP()) {

        usb_ep_read_data(&req, sizeof(req));

        uint8_t rc = 1;

        ;
        ;
        ;
        ; ;
        ; ;
        ; ;
        if ((req.bmRequestType & 0x60) == (0<<5)) {
            switch(req.bRequest) {
                case 1:

                    break;

                case 8:
                    if (req.bmRequestType == ((1<<7)|(0<<5)|0)) {
                        rc = usb_ep0_get_configuration();
                    }
                    break;

                case 6:
                    if (req.bmRequestType == ((1<<7)|(0<<5)|0)) {
                        rc = usb_ep0_get_descriptor(&req);
                    }
                    break;

                case 10:

                    break;

                case 0:

                    break;

                case 5:
                    if (req.bmRequestType == (0x00|(0<<5)|0)) {
                        rc = usb_ep0_set_address(&req);
                    }
                    break;

                case 9:
                    if (req.bmRequestType == (0x00|(0<<5)|0)) {
                        rc = usb_ep0_set_configuration(&req);
                    }
                    break;

                case 7:

                    break;

                case 3:

                    break;

                case 11:

                    break;

                case 12:

                    break;

                default:
                    break;
            }
        }
        if (rc != 0) {
            rc = usb_event_ep0_request(&req);
        }

        (*(volatile uint8_t *)(0xE9)) = 0;

        if (rc != 0) {
            usb_ep_clearSETUP();
            usb_ep_set_stall();
            ;
        }
        ;
    }
}


void usb_ep_setup(uint8_t epaddr, uint8_t epattr, uint8_t epsize, uint8_t epbanks) {

    uint8_t sz;
    uint8_t type;
    switch (epsize) {
        case 8: sz = (0 << 4)|(1 << (1)); break;
        case 16: sz = (1 << 4)|(1 << (1)); break;
        case 32: sz = (2 << 4)|(1 << (1)); break;
        default: sz = (3 << 4)|(1 << (1)); break;
    }
    type = (epattr & 0x03);

    (*(volatile uint8_t *)(0xE9)) = epaddr & 0x07;
    (((*(volatile uint8_t *)(0xEB))) |= (1<<(0)));
    (*(volatile uint8_t *)(0xEC)) = (type << 6)|(((epaddr & 0x80) == 0x80) ? (1<<(0)) : 0);
    (*(volatile uint8_t *)(0xED)) = sz | ((epbanks & 0x01) << 2);

    switch (type) {
# 564 "../avrutilslib/usb/usb.c"
        default:
                (*(volatile uint8_t *)(0xF0)) = 0;
                break;
    }

    ; ; ; ;
}

void usb_ep_setup_control(void) {
    struct usb_dev* dev = &g_usb_dev;
    uint8_t size = usb_mem_read_byte(&dev->pdesc_dev->bMaxPacketSize0, dev->desc_dtype);
    usb_ep_setup(0x00|0, 0, size, 0);
}

void usb_intr_endofreset(void) {
    usb_ep_setup_control();

    g_usb_dev.state = 0x04;
    g_usb_dev.cur_config = 0;
}

void usb_init(const struct usb_device_desc* pdevdesc,
              const void* pconfig, uint8_t configsize,
              const void* const* pstrings, uint8_t numstrings,
              get_string_func_t getstringfunc,
              uint8_t desc_dtype) {
    ;

    struct usb_dev* dev = &g_usb_dev;
    memset(dev, 0, sizeof(struct usb_dev));
    dev->state = 0x00;
    dev->cur_config = 0;
    dev->pdesc_dev = pdevdesc;
    dev->pdesc_config = pconfig;
    dev->desc_config_size = configsize;
    dev->pdesc_string = pstrings;
    dev->desc_string_num = numstrings;
    dev->get_string_func = getstringfunc;
    dev->desc_dtype = desc_dtype;

    (*(volatile uint8_t *)(0xD8)) = (0 << (7))|(1 << (5));
    (*(volatile uint8_t *)(0xFB)) = 0;


    (*(volatile uint8_t *)((0x29) + 0x20)) = (1<<2);
    (*(volatile uint8_t *)((0x29) + 0x20)) = (1<<2)|(1 << (1));

    while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0x29) + 0x20)))))) & (1 << (0)))));



    (*(volatile uint8_t *)(0xD8)) = (1 << (7))|(1 << (5));
    (*(volatile uint8_t *)(0xD8)) = (1 << (7))|(0 << (5));


    (*(volatile uint8_t *)(0xE2)) = 0;
    (*(volatile uint8_t *)(0xE1)) = 0;




    (*(volatile uint8_t *)(0xE2)) = (0 << (6))|(0 << (5))|(0 << (4))|(1 << (3))|(0 << (2))|(1 << (0));

    (*(volatile uint8_t *)(0xE0)) = (0 << (2))|(0 << (1))|(0 << (0));

    dev->state = 0x04;

    ;



}
# 185 "sboxnet2usb.c" 2


struct Eeprom {
    struct serial {
        struct usb_string_desc desc;
        wchar_t str[32];
    } serialnumber;
};
struct Eeprom eeprom __attribute__((section(".eeprom")));




const struct usb_device_desc g_device_desc_P __attribute__((__progmem__)) = {
    .bLength = sizeof(struct usb_device_desc),
    .bDescriptorType = 1,
    .bcdUSB = 0x0110,
    .bDeviceClass = 0xff,
    .bDeviceSubClass = 0xff,
    .bDeviceProtocol = 0xff,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xf123,
    .idProduct = 0xa002,
    .bcdDevice = 0x0101,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 3,
    .bNumConfigurations = 1
};
const struct config_1 {
    struct usb_configuration_desc config;
    struct usb_interface_desc interface;
} g_config_1_P __attribute__((__progmem__)) = {
    .config = {
        .bLength = sizeof(struct usb_configuration_desc),
        .bDescriptorType = 2,
        .wTotalLength = sizeof(struct config_1),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = (0x80 | 0),
        .bMaxPower = 25
    },
    .interface = {
        .bLength = sizeof(struct usb_interface_desc),
        .bDescriptorType = 4,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = 0xff,
        .bInterfaceSubClass = 0xff,
        .bInterfaceProtocoll = 0xff,
        .iInterface = 0
    }
};


const struct { struct usb_string_desc d; wchar_t b[]; } g_string0_P __attribute__((__progmem__)) = {{sizeof(struct usb_string_desc)+(2), 3}, {0x0409} };;
const struct { struct usb_string_desc d; wchar_t b[]; } g_string1_P __attribute__((__progmem__)) = {{sizeof(struct usb_string_desc)+(sizeof(L"Thomas Maier <balagi@justmail.de>")-2), 3}, L"Thomas Maier <balagi@justmail.de>" };;
const struct { struct usb_string_desc d; wchar_t b[]; } g_string2_P __attribute__((__progmem__)) = {{sizeof(struct usb_string_desc)+(sizeof(L"sboxnet2usb")-2), 3}, L"sboxnet2usb" };;


const void * const g_strings_P[3] __attribute__((__progmem__)) = {
    &g_string0_P,
    &g_string1_P,
    &g_string2_P,
};

uint8_t get_string(struct usb_device_request* req, uint8_t idx) {
    if (idx == 3) {
        usb_ep_clearSETUP();
        usb_ep_send_control_data(&eeprom.serialnumber.desc, usb_mem_read_byte(&eeprom.serialnumber.desc.bLength, 2),
                                 2, req->wLength);
        return 0;
    }
    return 1;
}

void usb_event_set_configuration(void) {
}

static uint8_t get_status(void) {
    uint16_t st = g_sboxnet_status;
    g_sboxnet_status = 0;
    if (sboxnet_can_read()) {
        st |= 0x02;
    }
    if (sboxnet_can_send()) {
        st |= 0x10;
    }
    return st;
}

uint8_t usb_event_ep0_request(struct usb_device_request* req) {
    if ( (req->bmRequestType & 0x7f) != ((2<<5)|1) )
        return 1;

    switch(req->bRequest) {
        case CMD_SBOXNET_SET_MODE: {
            usb_ep_clearSETUP();
            for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = (*(volatile uint8_t *)((0x3F) + 0x20)), __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 ) {
                if (req->wValue & 0x80) {
                    __asm__ __volatile__ ("wdr");



                    (*(volatile uint8_t *)(0x60)) = (1<<(4))|(1<<(3));
                    (*(volatile uint8_t *)(0x60)) = (1<<(2))|(1<<(3));

                } else if (req->wValue & 0x01) {
                    g_sboxnet_addr = req->wIndex;
                    if (!(g_sboxnet_flags & 0x01)) {
                        sboxnet_init();
                        g_sboxnet_flags |= 0x01;
                    }
                    sboxnet_set_sniffer_mode(req->wValue & 0x02);
                } else {
                    if (g_sboxnet_flags & 0x01) {
                        sboxnet_deinit();
                        g_sboxnet_flags &= ~0x01;
                    }
                }
            }
            usb_ep_send_zlp();
            return 0;
        }

        case CMD_SBOXNET_GET_STATUS: {
            usb_ep_clearSETUP();
            uint8_t st = get_status();
            usb_ep_send_control_data(&st, 1, 0, req->wLength);
            return 0;
        }

        case CMD_SBOXNET_RECEIVE: {
            usb_ep_clearSETUP();

            uint16_t wlen = req->wLength;
            uint8_t usbrc = 0;
            struct {
                uint8_t status;
            } buf;
            if (wlen >= sizeof(buf)) {
                wlen -= sizeof(buf);
                buf.status = get_status();
                usbrc = usb_ep_send_control_data_chunk(&buf, sizeof(buf), 0);
            }

            if (usbrc == 0) {
                while (wlen > 1) {
                    struct {
                        int8_t msglen;
                        struct sboxnet_msg_max msg;
                    } buf;
                    uint8_t maxs = sizeof(buf.msg);
                    if (wlen < 257) {
                        maxs = (uint8_t)(wlen - 1);
                    }
                    buf.msglen = sboxnet_receive_msg(&buf.msg.msgh, maxs);
                    if (buf.msglen > 0) {
                        wlen -= (buf.msglen + 1);
                        trigger_led_msg_read();
                        usbrc = usb_ep_send_control_data_chunk(&buf, buf.msglen + 1, 0);
                    } else {
                        usbrc = usb_ep_send_control_data_chunk(&buf.msglen, 1, 0);
                        break;
                    }
                    if (usbrc != 0) {
                        break;
                    }
                }
            }
            if (usbrc == 0) {
                usb_ep_send_control_data_finish();
            }
            return 0;
        }

        case CMD_SBOXNET_TRANSMIT: {
            usb_ep_clearSETUP();
            struct sboxnet_msg_max msg;
            uint16_t readb;
            usb_ep_recv_control_data(&msg, &readb, ((req->wLength) < (sizeof(msg)) ? (req->wLength) : (sizeof(msg))));
            if (readb >= 5) {
                if (sboxnet_can_send_msg(&msg.msgh)) {
                    trigger_led_msg_write();
                    if (sboxnet_send_msg(&msg.msgh) != 0)
                        g_sboxnet_status |= 0x80;
                } else {
                    g_sboxnet_status |= 0x40;
                }
            } else {
                g_sboxnet_status |= 0x20;
            }
            return 0;
        }

        case CMD_SBOXNET_GET_DBGINFO: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.debug, sizeof(struct sboxnet_debug_info), 0, req->wLength);
            return 0;
        }

        case CMD_SBOXNET_GET_DBGSTATE: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet, 9, 0, req->wLength);
            return 0;
        }

        case CMD_SBOXNET_GET_DBGRECVBUF: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.recv_buf, sizeof(g_sboxnet.recv_buf), 0, req->wLength);
            return 0;
        }
        case CMD_SBOXNET_GET_DBGTMITBUF: {
            usb_ep_clearSETUP();
            usb_ep_send_control_data(&g_sboxnet.tmit_buf, sizeof(g_sboxnet.tmit_buf), 0, req->wLength);
            return 0;
        }

        case CMD_DBG_STACK: {
            usb_ep_clearSETUP();
            struct {
                uint16_t size;
                uint16_t free;
            } buf;
            buf.size = stack_size();
            buf.free = stack_free();
            usb_ep_send_control_data(&buf, sizeof(buf), 0, req->wLength);
            return 0;
        }

        case CMD_SET_SERIALNUMBER: {
            usb_ep_clearSETUP();
            struct {
                struct usb_string_desc desc;
                wchar_t str[32];
            } buf;
            memset(&buf, 0xff, sizeof(buf));
            uint16_t readb;
            usb_ep_recv_control_data(&buf.str, &readb, ((req->wLength) < (sizeof(buf.str)) ? (req->wLength) : (sizeof(buf.str))));
            buf.desc.bLength = sizeof(struct usb_string_desc)+(uint8_t)readb;
            buf.desc.bDescriptorType = 3;
            __eeupd_block_m32u2(&buf, &eeprom.serialnumber, sizeof(struct usb_string_desc)+readb);
            return 0;
        }
# 441 "sboxnet2usb.c"
        default: break;
    }
    return 1;
}


void __vector_18 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_18 (void) {

    g_timer++;

    if ((g_timer & 0x0f) == 0) {
        (*(volatile uint8_t *)((0x05) + 0x20)) ^= (1<<(5));
    }

    if (g_timer_led_msg_read) {
        g_timer_led_msg_read--;
        if (g_timer_led_msg_read == 0)
            (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(6)));
    }

    if (g_timer_led_msg_write) {
        g_timer_led_msg_write--;
        if (g_timer_led_msg_write == 0)
            (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(7)));
    }
}


static __attribute__((noinline)) void init_system(void) {
    __asm__ __volatile__ ("wdr");
    uint8_t mcusr = (*(volatile uint8_t *)((0x34) + 0x20));
    (*(volatile uint8_t *)((0x34) + 0x20)) = 0;
    __asm__ __volatile__ ( "in __tmp_reg__, __SREG__" "\n\t" "cli" "\n\t" "sts %0, %1" "\n\t" "sts %0, __zero_reg__" "\n\t" "out __SREG__,__tmp_reg__" "\n\t" : : "M" (((uint16_t) &((*(volatile uint8_t *)(0x60))))), "r" ((uint8_t)((1 << (4)) | (1 << (3)))) : "r0" );
    if (((*(volatile uint8_t *)(((uint16_t) &(mcusr)))) & (1 << (3)))) {

        __asm__ __volatile__ (
            "jmp  0x3000\n"
        );
    }

    (*(volatile uint8_t *)((0x05) + 0x20)) = 0b00011111;
    (*(volatile uint8_t *)((0x08) + 0x20)) = 0b11111111;
    (*(volatile uint8_t *)((0x0B) + 0x20)) = 0b01111111;
    (*(volatile uint8_t *)((0x04) + 0x20)) = 0b11100000;
    (*(volatile uint8_t *)((0x07) + 0x20)) = 0;
    (*(volatile uint8_t *)((0x0A) + 0x20)) = 0;


    (*(volatile uint8_t *)(0x61)) = (1 << (7));



    (*(volatile uint8_t *)(0x61)) = 0x00;



    (*(volatile uint8_t *)(0x80)) = (0 << (1))|(0 << (0));
    (*(volatile uint8_t *)(0x81)) = (0 << (3))|((0 << (2))|(1 << (1))|(0 << (0)));
    (*(volatile uint8_t *)((0x16) + 0x20)) = 0xff;
    (*(volatile uint8_t *)(0x6F)) = (1 << (0));

    if (__eerd_word_m32u2((const uint16_t *)&eeprom.serialnumber.desc) == 0xffff) {

        __eeupd_byte_m32u2(&eeprom.serialnumber.desc.bLength, sizeof(struct usb_string_desc)+2);
        __eeupd_byte_m32u2(&eeprom.serialnumber.desc.bDescriptorType, 3);
        __eeupd_word_m32u2((uint16_t*)&eeprom.serialnumber.str[0], 'x');
    }
}

int main(void) {
    __asm__ __volatile__ ("cli" ::: "memory");
    init_system();

    usb_init(&g_device_desc_P,
             &g_config_1_P, sizeof(g_config_1_P),
             g_strings_P, 3, &get_string,
             1);

    do { (*(volatile uint8_t *)((0x33) + 0x20)) = (((*(volatile uint8_t *)((0x33) + 0x20)) & ~((1 << (1)) | (1 << (2)) | (1 << (3)))) | ((0))); } while(0);
    do { (*(volatile uint8_t *)((0x33) + 0x20)) |= (uint8_t)(1 << (0)); } while(0);
    __asm__ __volatile__ ("sei" ::: "memory");

    while(1) {
        usb_ep0_task();
    }

    return 0;
}
