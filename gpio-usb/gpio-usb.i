# 1 "gpio-usb.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "gpio-usb.c"
# 23 "gpio-usb.c"
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
# 24 "gpio-usb.c" 2
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
# 172 "/usr/avr/include/avr/io.h" 3
# 1 "/usr/avr/include/avr/iousb162.h" 1 3
# 39 "/usr/avr/include/avr/iousb162.h" 3
# 1 "/usr/avr/include/avr/iousbxx2.h" 1 3
# 40 "/usr/avr/include/avr/iousb162.h" 2 3
# 173 "/usr/avr/include/avr/io.h" 2 3
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
# 25 "gpio-usb.c" 2
# 1 "/usr/avr/include/avr/interrupt.h" 1 3
# 26 "gpio-usb.c" 2
# 1 "/usr/avr/include/avr/wdt.h" 1 3
# 27 "gpio-usb.c" 2
# 1 "/usr/avr/include/avr/sleep.h" 1 3
# 28 "gpio-usb.c" 2
# 1 "/usr/avr/include/avr/cpufunc.h" 1 3
# 29 "gpio-usb.c" 2
# 1 "/usr/avr/include/avr/eeprom.h" 1 3
# 428 "/usr/avr/include/avr/eeprom.h" 3
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
# 147 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef int ptrdiff_t;
# 212 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef unsigned int size_t;
# 324 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 3 4
typedef int wchar_t;
# 429 "/usr/avr/include/avr/eeprom.h" 2 3
# 515 "/usr/avr/include/avr/eeprom.h" 3
uint8_t __eerd_byte_usb162 (const uint8_t *__p) __attribute__((__pure__));




uint16_t __eerd_word_usb162 (const uint16_t *__p) __attribute__((__pure__));




uint32_t __eerd_dword_usb162 (const uint32_t *__p) __attribute__((__pure__));




float __eerd_float_usb162 (const float *__p) __attribute__((__pure__));





void __eerd_block_usb162 (void *__dst, const void *__src, size_t __n);





void __eewr_byte_usb162 (uint8_t *__p, uint8_t __value);




void __eewr_word_usb162 (uint16_t *__p, uint16_t __value);




void __eewr_dword_usb162 (uint32_t *__p, uint32_t __value);




void __eewr_float_usb162 (float *__p, float __value);





void __eewr_block_usb162 (const void *__src, void *__dst, size_t __n);





void __eeupd_byte_usb162 (uint8_t *__p, uint8_t __value);




void __eeupd_word_usb162 (uint16_t *__p, uint16_t __value);




void __eeupd_dword_usb162 (uint32_t *__p, uint32_t __value);




void __eeupd_float_usb162 (float *__p, float __value);





void __eeupd_block_usb162 (const void *__src, void *__dst, size_t __n);
# 30 "gpio-usb.c" 2
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
# 31 "gpio-usb.c" 2
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
 __tmp = (((8 * 1000000 / 1)) / 1e3) * __ms;
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
 __tmp = (((8 * 1000000 / 1)) / 1e6) * __us;
# 243 "/usr/avr/include/util/delay.h" 3
  __ticks_dc = (uint32_t)(ceil(fabs(__tmp)));


 __builtin_avr_delay_cycles(__ticks_dc);
# 261 "/usr/avr/include/util/delay.h" 3
}
# 32 "gpio-usb.c" 2

# 1 "../avrutilslib/defines.h" 1
# 120 "../avrutilslib/defines.h"
static inline uint8_t rol_byte(uint8_t v) {
    return (v << 1) + (v & 0x80 ? 1 : 0);
}

static inline uint16_t rol_word(uint16_t v) {
    return (v << 1) + (v & 0x8000 ? 1 : 0);
}
# 34 "gpio-usb.c" 2


# 1 "../avrutilslib/usb/usb.c" 1
# 20 "../avrutilslib/usb/usb.c"
# 1 "/usr/avr/include/string.h" 1 3
# 45 "/usr/avr/include/string.h" 3
# 1 "/usr/lib/gcc/avr/4.9.3/include/stddef.h" 1 3 4
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
# 21 "../avrutilslib/usb/usb.c" 2



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
# 25 "../avrutilslib/usb/usb.c" 2

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
struct usb_dev {
    volatile uint8_t state;
    uint8_t cur_config;
    const struct usb_device_desc* pdesc_dev;
    const void* pdesc_config;
    uint8_t desc_config_size;
    const void* const* pdesc_string;
    uint8_t desc_string_num;
    uint8_t desc_dtype;
};

extern struct usb_dev g_usb_dev;







void usb_event_set_configuration(void);
uint8_t usb_event_ep0_request(struct usb_device_request*);



static inline uint8_t usb_ep_isSETUP(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0XE8)))))) & (1 << (3)));
}
static inline void usb_ep_clearSETUP(void) {
    (((*(volatile uint8_t *)(0XE8))) &= ~(1<<(3)));
}
uint8_t inline usb_ep_isIN(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0XE8)))))) & (1 << (0)));
}
static inline void usb_ep_clearIN(void) {
    (((*(volatile uint8_t *)(0XE8))) &= ~(1<<(0)));
}
static inline void usb_ep_clearFIFOCON(void) {
    (((*(volatile uint8_t *)(0XE8))) &= ~(1<<(7)));
}
static inline uint8_t usb_ep_isOUT(void) {
    return ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0XE8)))))) & (1 << (2)));
}
static inline void usb_ep_clearOUT(void) {
    (((*(volatile uint8_t *)(0XE8))) &= ~(1<<(2)));
}
static inline void usb_ep_set_stall(void) {
    (((*(volatile uint8_t *)(0XEB))) |= (1<<(5)));
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
    uint8_t intf = (*(volatile uint8_t *)(0XE1));
    uint8_t inte = (*(volatile uint8_t *)(0XE2));

    uint8_t epnum_save = (*(volatile uint8_t *)(0XE9));

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
        (((*(volatile uint8_t *)(0XE1))) &= ~(1<<(3)));
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (2)))) {
        ;
    }
    if (((*(volatile uint8_t *)(((uint16_t) &(intf)))) & (1 << (0)))) {
        ;
    }
    ;
    (*(volatile uint8_t *)(0XE1)) = 0;
    (*(volatile uint8_t *)(0XE9)) = epnum_save;
}


void usb_ep_read_data(void* buf, uint8_t size) {
    while (size) {
        *(uint8_t*)buf++ = (*(volatile uint8_t *)(0XF1));
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
            return __eerd_byte_usb162((const uint8_t*)psrc);

        case 0:
        default:
            return *(const uint8_t*)psrc;
    }
}

uint8_t usb_ep_getbufsize(void) {
    static const uint8_t epsizemap[4] __attribute__((__progmem__)) = { 8, 16, 32, 64 };
    return (__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(&epsizemap[((*(volatile uint8_t *)(0XED)) >> 4) & 0x03])); uint8_t __result; __asm__ ( "lpm %0, Z" "\n\t" : "=r" (__result) : "z" (__addr16) ); __result; }));
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

        while (size && ((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0XE8)))))) & (1 << (5)))) {
            uint8_t d = usb_mem_read_byte(psrc++, srctype);
            (*(volatile uint8_t *)(0XF1)) = d;
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
            (*(volatile uint8_t *)(0XF1)) = usb_mem_read_byte(psrc++, srctype);
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
                *(uint8_t*)psrc++ = (*(volatile uint8_t *)(0XF1));
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
            if (idx < dev->desc_string_num) {
                const struct usb_string_desc* paddr;

                usb_ep_clearSETUP();
                switch (dev->desc_dtype) {
                    case 1:
                        paddr = (const struct usb_string_desc*)(__extension__({ uint16_t __addr16 = (uint16_t)((uint16_t)(dev->pdesc_string + idx)); uint16_t __result; __asm__ ( "lpm %A0, Z+" "\n\t" "lpm %B0, Z" "\n\t" : "=r" (__result), "=z" (__addr16) : "1" (__addr16) ); __result; }));
                        break;

                    case 2:
                        paddr = (const struct usb_string_desc*)__eerd_word_usb162((const uint16_t*)dev->pdesc_string + idx);
                        break;

                    default:
                        paddr = (const struct usb_string_desc*)dev->pdesc_string[idx];
                        break;
                }
                usb_ep_send_control_data(paddr, usb_mem_read_byte(paddr, dev->desc_dtype), dev->desc_dtype, req->wLength);
                return 0;
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
        (*(volatile uint8_t *)(0XE3)) = addr;
        usb_ep_send_zlp();
        (((*(volatile uint8_t *)(0XE3))) |= (1<<(7)));
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

                (*(volatile uint8_t *)(0XE9)) = 0;

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

    (*(volatile uint8_t *)(0XE9)) = 0;
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

        (*(volatile uint8_t *)(0XE9)) = 0;

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

    (*(volatile uint8_t *)(0XE9)) = epaddr & 0x07;
    (((*(volatile uint8_t *)(0XEB))) |= (1<<(0)));
    (*(volatile uint8_t *)(0XEC)) = (type << 6)|(((epaddr & 0x80) == 0x80) ? (1<<(0)) : 0);
    (*(volatile uint8_t *)(0XED)) = sz | ((epbanks & 0x01) << 2);

    switch (type) {
# 559 "../avrutilslib/usb/usb.c"
        default:
                (*(volatile uint8_t *)(0XF0)) = 0;
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
    dev->desc_dtype = desc_dtype;

    (*(volatile uint8_t *)(0XD8)) = (0 << (7))|(1 << (5));
    (*(volatile uint8_t *)(0XFB)) = 0;


    (*(volatile uint8_t *)((0x29) + 0x20)) = (0<<2);
    (*(volatile uint8_t *)((0x29) + 0x20)) = (0<<2)|(1 << (1));

    while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0x29) + 0x20)))))) & (1 << (0)))));



    (*(volatile uint8_t *)(0XD8)) = (1 << (7))|(1 << (5));
    (*(volatile uint8_t *)(0XD8)) = (1 << (7))|(0 << (5));


    (*(volatile uint8_t *)(0XE2)) = 0;
    (*(volatile uint8_t *)(0XE1)) = 0;




    (*(volatile uint8_t *)(0XE2)) = (0 << (6))|(0 << (5))|(0 << (4))|(1 << (3))|(0 << (2))|(1 << (0));

    (*(volatile uint8_t *)(0XE0)) = (0 << (2))|(0 << (1))|(0 << (0));

    dev->state = 0x04;

    ;



}
# 37 "gpio-usb.c" 2
# 73 "gpio-usb.c"
enum FLAGS {
    FLG_AVRMODE = 0x0001,
    FLG_AVRPROGMODE = 0x0002,
    FLG_AVRWAIT0 = 0x0004,
    FLG_AVRWAIT1 = 0x0008,
};
enum AvrWait {
    AVRWAIT_MASK = 0x0c,
    AVRWAIT_POLL = 0x00,
    AVRWAIT_8MS = 0x04,
    AVRWAIT_16MS = 0x08,
    AVRWAIT_64MS = 0x0c,
};
enum ERRORFLAGS {
    ERRFLG_AVRCMD = (1<<7),
    ERRFLG_FRAMEERR = (1<<4),
    ERRFLG_DATAOV = (1<<3),
    ERRFLG_PARITYERR = (1<<2),
    ERRFLG_UNKNOWNCMD = (1<<0),
};
enum CMDS {






    CMD_ENTER_AVRMODE = 0x08,
    CMD_LEAVE_AVRMODE = 0x09,




    CMD_AVR_PROGMODE = 0x10,
    CMD_AVR_RAWCMD = 0x11,
    CMD_AVR_READ_PRGMEM = 0x12,
    CMD_AVR_READ_EEPROM = 0x13,
    CMD_AVR_WRITE_PRGMEM_PAGE = 0x14,
    CMD_AVR_WRITE_EEPROM_PAGE = 0x15,
    CMD_AVR_WRITE_EEPROM_BYTE = 0x16,
    CMD_AVR_LOAD_EXT_ADDR = 0x17,
    CMD_AVR_CHIP_ERASE = 0x18,
    CMD_AVR_PROGRAMENABLE = 0x19,
    CMD_AVR_READ_SIGNATURE = 0x1a,
    CMD_AVR_READ_LOCKBITS = 0x1b,
    CMD_AVR_WRITE_LOCKBITS = 0x1c,
    CMD_AVR_READ_FUSEBITS = 0x1d,
    CMD_AVR_WRITE_FUSEBITS = 0x1e,
    CMD_AVR_READ_CALIBR = 0x1f,
    CMD_AVR_GET_STATUS = 0x20,

    CMD_PDI_ENABLE = 0x40,
    CMD_PDI_DISABLE = 0x41,
    CMD_PDI_LDS = 0x42,
    CMD_PDI_STS = 0x43,
    CMD_PDI_LD = 0x44,
    CMD_PDI_ST = 0x45,
    CMD_PDI_LDCS = 0x46,
    CMD_PDI_STCS = 0x47,
    CMD_PDI_REPEAT = 0x48,
    CMD_PDI_KEY = 0x49,
    CMD_PDI_GETSTATUS = 0x4a,
    CMD_ENTER_PDI = 0x4b,
    CMD_LEAVE_PDI = 0x4c,

    CMD_NVMC_ENABLE = 0x50,
    CMD_NVMC_DISABLE = 0x51,
    CMD_NVMC_FLASH_READ = 0x52,
    CMD_NVMC_FLASH_WRITE_PAGE = 0x53,
    CMD_NVMC_EEPROM_READ = 0x54,
    CMD_NVMC_EEPROM_WRITE = 0x55,
    CMD_NVMC_FUSE_READ = 0x56,
    CMD_NVMC_FUSE_WRITE = 0x57,
    CMD_NVMC_SIGNROW_READ = 0x58,
    CMD_NVMC_NVM_READ = 0x59,
    CMD_NVMC_GET_DEVID = 0x60,
    CMD_NVMC_ERASE_CHIP = 0x61,
};
# 181 "gpio-usb.c"
uint8_t g_avr_flags = 0;
uint8_t g_avr_error = 0;
uint8_t g_avr_clk_wait = 0;
volatile uint8_t g_avr_timer = 0;

volatile uint8_t g_pdi_timer = 0;
volatile uint8_t g_nvmc_timer = 0;
uint8_t g_pdi_last_rc = 0;




uint8_t g_pdi_flags = 0;






void __vector_15 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_15 (void) {
    (*(volatile uint16_t *)(0x88)) += ((8 * 1000000 / 1) / (8 * 500) - 1);
    if (g_avr_timer) {
        g_avr_timer--;
    }
    if (g_pdi_timer) {
        g_pdi_timer--;
    }
    if (g_nvmc_timer) {
        g_nvmc_timer--;
    }
}


void avr_clkwait(void) {
    uint8_t wait = g_avr_clk_wait;
    do {
        _delay_us(5);
    } while (wait--);
}

uint8_t avr_sendbyte(uint8_t b) {
    if ((!((*(volatile uint8_t *)(((uint16_t) &(g_avr_flags)))) & (1 << (1)))))
        return 0;

    uint8_t r = 0;
    (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(1)));
    for (uint8_t i = 0; i < 8; i++) {
        if (b & 0x80) {
            (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(2)));
        } else {
            (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(2)));
        }
        b <<= 1;

        avr_clkwait();

        r <<= 1;
        if (((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)((0X03) + 0x20)))))) & (1 << (3)))) {
            r |= 0x01;
        }

        (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(1)));
        avr_clkwait();
        (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(1)));
    }
    return r;
}

void cmd_avr_init(void) {
    (*(volatile uint8_t *)((0x04) + 0x20)) &= ~((1<<(1))|(1<<(3))|(1<<(2))|(1<<(0))|(1<<(4)));
    (*(volatile uint8_t *)((0x05) + 0x20)) &= ~((1<<(1))|(1<<(3))|(1<<(2))|(1<<(0)));
    (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(4)));
    (*(volatile uint8_t *)((0x04) + 0x20)) |= ((1<<(1))|(1<<(2))|(1<<(0))|(1<<(4)));

    ((g_avr_flags) |= (1<<(0)));
    ((g_avr_flags) &= ~(1<<(1)));
    ((g_avr_flags) &= ~(1<<(2)));
    ((g_avr_flags) &= ~(1<<(3)));

    usb_ep_send_zlp();
}

void cmd_avr_exit(void) {
    (*(volatile uint8_t *)((0x04) + 0x20)) &= ~((1<<(1))|(1<<(3))|(1<<(2))|(1<<(0))|(1<<(4)));
    (*(volatile uint8_t *)((0x05) + 0x20)) |= ((1<<(1))|(1<<(3))|(1<<(2))|(1<<(0))|(1<<(4)));

    g_avr_flags &= ~((1<<(0))|(1<<(1))|(1<<(2))|(1<<(3)));

    usb_ep_send_zlp();
}

void cmd_avr_progmode(uint8_t flags, uint8_t sclk_period, uint16_t wLength) {
    if (((*(volatile uint8_t *)(((uint16_t) &(g_avr_flags)))) & (1 << (0)))) {
        if (((*(volatile uint8_t *)(((uint16_t) &(flags)))) & (1 << (1)))) {
            g_avr_clk_wait = sclk_period;
            g_avr_flags = (g_avr_flags & ~((1<<(1))|(1<<(2))|(1<<(3)))) | (flags & ((1<<(1))|(1<<(2))|(1<<(3))));
            (*(volatile uint8_t *)((0x05) + 0x20)) &= ~((1<<(1))|(1<<(2))|(1<<(0)));
            (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(4)));
            _delay_us(10);
            (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(4)));
            goto do_answer;

        } else {
            (((*(volatile uint8_t *)((0x05) + 0x20))) |= (1<<(4)));
            (*(volatile uint8_t *)((0x05) + 0x20)) &= ~((1<<(1))|(1<<(2))|(1<<(0)));
        }
    }
    g_avr_flags &= ~((1<<(1))|(1<<(2))|(1<<(3)));
    g_avr_clk_wait = 0;

do_answer:
    usb_ep_send_control_data(&g_avr_flags, 1, 0, wLength);
}

void cmd_avr_rawcmd(uint16_t w1, uint16_t w2, uint16_t wLength) {
    uint8_t ret[3];
    avr_sendbyte(w1 >> 8);
    ret[0] = avr_sendbyte(w1);
    ret[1] = avr_sendbyte(w2 >> 8);
    ret[2] = avr_sendbyte(w2);
    usb_ep_send_control_data(ret, 3, 0, wLength);
}

uint8_t avr_sendcmd(uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4) {
    avr_sendbyte(b1);
    uint8_t r1 = avr_sendbyte(b2);
    uint8_t r2 = avr_sendbyte(b3);
    uint8_t r3 = avr_sendbyte(b4);
    if (b1 != r1 || b2 != r2) {
        g_avr_error |= 0x01;
    }
    return r3;
}

void cmd_avr_readflash(uint16_t addr, uint16_t numbytes) {
    while (numbytes > 0) {
        uint8_t buf[2];
        buf[0] = avr_sendcmd(0x20, addr >> 8, addr, 0);
        buf[1] = avr_sendcmd(0x28, addr >> 8, addr, 0);
        if (usb_ep_send_control_data_chunk(buf, 2, 0) != 0) {
            return;
        }
        addr++;
        numbytes -= 2;
    }
    usb_ep_send_control_data_finish();
}

void cmd_avr_readeeprom(uint16_t addr, uint16_t numbytes) {
    while (numbytes > 0) {
        uint8_t b = avr_sendcmd(0xa0, addr >> 8, addr, 0);
        if (usb_ep_send_control_data_chunk(&b, 1, 0) != 0) {
            return;
        }
        addr++;
        numbytes--;
    }
    usb_ep_send_control_data_finish();
}

void avr_poll_and_return(void) {
    switch (g_avr_flags & 0x0c) {
        case 0x00: {

            g_avr_timer = ((24)/2);
            do {
                uint8_t r = avr_sendcmd(0xf0, 0, 0, 0);
                if ((r & 0x01) == 0) {
                    return;
                }
            } while (*(volatile uint8_t*)&g_avr_timer);
            g_avr_error |= 0x02;
            return;
        }
        case 0x04: {
            g_avr_timer = ((10)/2);
            break;
        }
        case 0x08: {
            g_avr_timer = ((18)/2);
            break;
        }
        case 0x0c:
        default: {
            g_avr_timer = ((66)/2);
            break;
        }
    }
    while (*(volatile uint8_t*)&g_avr_timer) {
        do { __asm__ __volatile__ ( "sleep" "\n\t" :: ); } while(0);
    }
}

void cmd_avr_writeflashpage(uint16_t addr, uint16_t numbytes) {
    uint16_t pageoff = 0;
    while (numbytes > 1) {
        uint8_t buf[2];
        uint16_t readb;
        if (usb_ep_recv_control_data_chunk(buf, &readb, 2) != 0) {
            return;
        }
        avr_sendcmd(0x40, pageoff >> 8, pageoff, buf[0]);
        avr_sendcmd(0x48, pageoff >> 8, pageoff, buf[1]);
        numbytes -= 2;
        pageoff++;
    }

    avr_sendcmd(0x4c, addr >> 8, addr, 0);
    avr_poll_and_return();
    usb_ep_recv_control_data_finish();
}

void cmd_avr_writeeeprompage(uint16_t addr, uint16_t numbytes) {
    uint16_t pageoff = 0;
    while (numbytes > 0) {
        uint8_t buf[1];
        uint16_t readb;
        if (usb_ep_recv_control_data_chunk(buf, &readb, 1) != 0) {
            return;
        }
        avr_sendcmd(0xc1, pageoff >> 8, pageoff, buf[0]);
        numbytes--;
        pageoff++;
    }

    avr_sendcmd(0xc2, addr >> 8, addr, 0);
    avr_poll_and_return();
    usb_ep_recv_control_data_finish();
}
# 600 "gpio-usb.c"
enum EraseMode {
    EraseMode_CHIP = 0,
    EraseMode_APP_SECTION = 1,
    EraseMode_BOOT_SECTION = 2,
    EraseMode_EEPROM = 3,
};


void pdi_off(void) {
    (*(volatile uint8_t *)(0xC8)) = 0;
    (*(volatile uint8_t *)(0XC9)) = 0;
    (*(volatile uint8_t *)(0xCA)) = 0;
    (*(volatile uint8_t *)(0xCB)) = 0;
    (((*(volatile uint8_t *)((0x0A) + 0x20))) &= ~(1<<(2)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) &= ~(1<<(2)));
    (((*(volatile uint8_t *)((0x0A) + 0x20))) |= (1<<(3)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) &= ~(1<<(3)));
    (((*(volatile uint8_t *)((0x0A) + 0x20))) |= (1<<(5)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) |= (1<<(5)));
}

void pdi_enter(void) {
    pdi_off();
}

void pdi_leave(void) {
    (*(volatile uint8_t *)(0xC8)) = 0;
    (*(volatile uint8_t *)(0XC9)) = 0;
    (*(volatile uint8_t *)(0xCA)) = 0;
    (*(volatile uint8_t *)(0xCB)) = 0;
    (((*(volatile uint8_t *)((0x0A) + 0x20))) &= ~(1<<(5)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) |= (1<<(5)));
    (((*(volatile uint8_t *)((0x0A) + 0x20))) &= ~(1<<(2)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) |= (1<<(2)));
    (((*(volatile uint8_t *)((0x0A) + 0x20))) &= ~(1<<(3)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) |= (1<<(3)));
}

void pdi_start(void) {
    pdi_off();

    (((*(volatile uint8_t *)((0x0B) + 0x20))) |= (1<<(3)));

    _delay_us(50);


    (*(volatile uint16_t *)(0xCC)) = 39;
    (*(volatile uint8_t *)(0xC8)) = 0;
    (*(volatile uint8_t *)(0XC9)) = (0 << (7))|(0 << (6))|(0 << (5))|(0 << (4))
            |(0 << (3))|(0 << (2));
    (*(volatile uint8_t *)(0xCA)) = (0 << (7))|(1 << (6))|(1 << (5))|(0 << (4))
            |(1 << (3))|(1 << (2))|(1 << (1))|(1 << (0));
    (*(volatile uint8_t *)(0xCB)) = 0;

    (((*(volatile uint8_t *)(0XC9))) |= (1<<(4)));

    _delay_us(200);
}


uint8_t pdi_write(uint8_t* buf, uint16_t count0) {
    uint8_t x;
    uint8_t rc = 0;


    while (((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC8)))))) & (1 << (7)))) {
        x = (*(volatile uint8_t *)(0XCE));
    }

    (((*(volatile uint8_t *)(0XC9))) |= (1<<(3)));

    while (1) {

        while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC8)))))) & (1 << (5)))));
        (((*(volatile uint8_t *)(0xC8))) |= (1<<(6)));

        uint8_t b;
        if (buf) {
            b = *buf++;
        } else {
            uint16_t readb;
            if (usb_ep_recv_control_data_chunk(&b, &readb, 1) != 0) {
                rc = 21;
                break;
            }
            if (readb != 1) {
                rc = 21;
                break;
            }
        }
        (*(volatile uint8_t *)(0XCE)) = b;

        while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC8)))))) & (1 << (6)))));
        (((*(volatile uint8_t *)(0xC8))) |= (1<<(6)));

        g_pdi_timer = ((10)/2);
        while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC8)))))) & (1 << (7))))) {
            if (g_pdi_timer == 0) {
                rc = 10;
                break;
            }
        }
        if (rc == 0) {
            uint8_t flags = (*(volatile uint8_t *)(0xC8));
            x = (*(volatile uint8_t *)(0XCE));
            if (x != b || (flags & ((1<<(4))|(1<<(3))|(1<<(2))))) {
                rc = 4;
            }
        }
        if (count0 == 0 || rc) {
            break;
        }
        count0--;
    }

    (((*(volatile uint8_t *)(0XC9))) &= ~(1<<(3)));
    (((*(volatile uint8_t *)((0x0A) + 0x20))) &= ~(1<<(3)));
    (((*(volatile uint8_t *)((0x0B) + 0x20))) &= ~(1<<(3)));

    return rc;
}

uint8_t pdi_read(uint8_t* data) {
    g_pdi_timer = ((10)/2);
    while ((!((*(volatile uint8_t *)(((uint16_t) &((*(volatile uint8_t *)(0xC8)))))) & (1 << (7))))) {
        if (g_pdi_timer == 0) {
            return 11;
        }
    }

    uint8_t flags = (*(volatile uint8_t *)(0xC8));
    uint8_t b = (*(volatile uint8_t *)(0XCE));

    if (flags & ((1<<(4))|(1<<(3))|(1<<(2)))) {
        return 5;
    }
    *data = b;
    return 0;
}

uint8_t pdi_cmd(uint8_t cmdb[], uint8_t len, uint8_t pout[], uint8_t* plen) {
    if (len == 0)
        return 1;
    uint8_t cmd = cmdb[0];
    uint8_t rc = 0;
    if (plen)
        *plen = 0;
    switch (cmd & 0xe0) {
        case 0x00: {
            uint8_t sza = ((cmd & 0x0c) >> 2) + 1;
            uint8_t szb = (cmd & 0x03) + 1;
            if (sza + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            for (uint8_t i = 0; i < szb; i++) {
                rc = pdi_read(pout + i);
                if (rc) {
                    return rc;
                }
            }
            if (plen)
                *plen = szb;
            break;
        }
        case 0x40: {
            uint8_t sza = ((cmd & 0x0c) >> 2) + 1;
            uint8_t szb = (cmd & 0x03) + 1;
            if (sza + szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case 0x20: {
            uint8_t szb = (cmd & 0x03) + 1;
            if (1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            for (uint8_t i = 0; i < szb; i++) {
                rc = pdi_read(pout + i);
                if (rc) {
                    return rc;
                }
            }
            if (plen)
                *plen = szb;
            break;
        }
        case 0x60: {
            uint8_t szb = (cmd & 0x03) + 1;
            if (szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case 0x80: {
            if (1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            rc = pdi_read(pout);
            if (rc) {
                return rc;
            }
            *plen = 1;
            break;
        }
        case 0xC0: {
            if (2 != len) {
                return 3;
            }
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case 0xA0: {
            uint8_t szb = (cmd & 0x03) + 1;
            if (szb + 1 != len)
                return 3;
            rc = pdi_write(cmdb, len-1);
            if (rc) {
                return rc;
            }
            break;
        }
        case 0xE0: {
            if (9 != len)
                return 3;
            rc = pdi_write(cmdb, 9-1);
            if (rc) {
                return rc;
            }
            break;
        }
        default: return 2;
    }
    return 0;
}

uint8_t pdi_cmd_lds_byte(uint32_t addr, uint8_t* b) {
    uint8_t buf[6];
    buf[0] = 0x00 | 0x0c | 0x00;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    return pdi_cmd(buf, 5, b, ((void *)0));
}

uint8_t pdi_cmd_sts_byte(uint32_t addr, uint8_t b) {
    uint8_t buf[6];
    buf[0] = 0x40 | 0x0c | 0x00;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    buf[5] = b;
    return pdi_cmd(buf, 6, ((void *)0), ((void *)0));
}

uint8_t pdi_cmd_st_ptr(uint32_t addr) {
    uint8_t buf[5];
    buf[0] = 0x60 | 0x08 | 0x03;
    buf[1] = addr & 0xff;
    buf[2] = addr >> 8;
    buf[3] = addr >> 16;
    buf[4] = addr >> 24;
    return pdi_cmd(buf, 5, ((void *)0), ((void *)0));
}

uint8_t pdi_cmd_stcs(uint8_t addr, uint8_t b) {
    uint8_t buf[2];
    buf[0] = 0xC0 | (addr & 0x0f);
    buf[1] = b;
    return pdi_cmd(buf, 2, ((void *)0), ((void *)0));
}

uint8_t pdi_cmd_ldcs(uint8_t addr, uint8_t* b) {
    uint8_t buf[2];
    buf[0] = 0x80 | (addr & 0x0f);
    return pdi_cmd(buf, 1, b, ((void *)0));
}

uint8_t pdi_cmd_key_nvm(void) {
    uint8_t buf[9];
    buf[0] = 0xE0;
    buf[1] = 0xFF;
    buf[2] = 0x88;
    buf[3] = 0xD8;
    buf[4] = 0xCD;
    buf[5] = 0x45;
    buf[6] = 0xAB;
    buf[7] = 0x89;
    buf[8] = 0x12;
    return pdi_cmd(buf, 9, ((void *)0), ((void *)0));
}

uint8_t pdi_cmd_repeat(uint16_t count) {
    uint8_t buf[3];
    buf[0] = 0xA0 | 0x01;
    buf[1] = count & 0xff;
    buf[2] = count >> 8;
    return pdi_cmd(buf, 3, ((void *)0), ((void *)0));
}

uint8_t pdi_cmd_load(uint32_t addr, uint8_t* pdata, uint16_t count0) {
    uint8_t rc;
    uint8_t buf[2];

    rc = pdi_cmd_st_ptr(addr);
    if (rc)
        return rc;

    rc = pdi_cmd_repeat(count0);
    if (rc)
        return rc;

    buf[0] = 0x20 | 0x04 | 0x00;
    rc = pdi_write(buf, 0);
    if (rc)
        return rc;

    while (1) {
        if (pdata) {
            rc = pdi_read(pdata);
            pdata++;
        } else {
            rc = pdi_read(buf);
            if (!rc) {
                if (usb_ep_send_control_data_chunk(buf, 1, 0) != 0)
                    rc = 20;
            }
        }
        if (rc)
            return rc;
        if (count0 == 0)
            break;
        count0--;
    }
    return 0;
}

uint8_t pdi_cmd_store(uint32_t addr, uint8_t* pdata, uint16_t count0) {
    uint8_t rc;
    uint8_t buf[2];

    rc = pdi_cmd_st_ptr(addr);
    if (rc)
        return rc;

    rc = pdi_cmd_repeat(count0);
    if (rc)
        return rc;

    buf[0] = 0x60 | 0x04 | 0x00;
    rc = pdi_write(buf, 0);
    if (rc)
        return rc;

    return pdi_write(pdata, count0);
}

uint8_t nvmc_init(void) {
    uint8_t rc;
    uint8_t b;

    rc = pdi_cmd_stcs(1, 0x59);
    if (rc)
        goto error;

    _delay_us(10);
    rc = pdi_cmd_ldcs(1, &b);
    if (rc)
        goto error;
    if ((!((*(volatile uint8_t *)(((uint16_t) &(b)))) & (1 << (0))))) {
        rc = 99;
        goto error;
    }

    rc = pdi_cmd_key_nvm();
    if (rc)
        goto error;

    _delay_ms(1);

    g_nvmc_timer = ((200)/2);
    while (1) {
        rc = pdi_cmd_ldcs(0, &b);
        if (rc)
            goto error;
        if (((*(volatile uint8_t *)(((uint16_t) &(b)))) & (1 << (1)))) {
            break;
        }
        if (g_nvmc_timer == 0) {
            rc = 12;
            goto error;
        }
    }

    return 0;

error:
    pdi_off();
    return rc;
}

uint8_t nvmc_exit(void) {
    uint8_t rc;

    pdi_cmd_stcs(0, 0);
    rc = pdi_cmd_stcs(1, 0);

    return rc;
}

uint8_t nvmc_set_cmd(uint8_t cmd) {
    return pdi_cmd_sts_byte(0x1000000 + (0x01C0 +0x0A), cmd);
}
uint8_t nvmc_trigger_cmdex(void) {
    return pdi_cmd_sts_byte(0x1000000 + (0x01C0 +0x0B), (1<<(0)));
}
uint8_t nvmc_nvmbusy_wait(void) {
    uint8_t rc;
    uint8_t status;
    g_nvmc_timer = ((20)/2);
    while (1) {
        _delay_us(100);
        rc = pdi_cmd_lds_byte(0x1000000 + (0x01C0 +0x0F), &status);
        if (rc)
            return rc;
        if ((!((*(volatile uint8_t *)(((uint16_t) &(status)))) & (1 << (7)))))
            break;
        if (g_nvmc_timer == 0)
            return 14;
    }
    return 0;
}



uint8_t nvmc_read_nvm(uint32_t addr, uint16_t count0) {
    uint8_t rc = nvmc_set_cmd(0x43);
    if (rc == 0) {
        rc = pdi_cmd_load(addr, ((void *)0), count0);
    }
    return rc;
}

uint8_t nvmc_erase_chip(uint8_t mode) {
    uint8_t rc;
    uint8_t cmd;
    switch (mode) {
        case EraseMode_APP_SECTION:
            cmd = 0x40;
            break;
        case EraseMode_BOOT_SECTION:
            cmd = 0x68;
            break;
        case EraseMode_EEPROM:
            cmd = 0x30;
            break;
        case EraseMode_CHIP:
            cmd = 0x40;
            break;
        default:
            return 15;
    }
    rc = nvmc_set_cmd(cmd);
    if (rc)
        return rc;
    rc = nvmc_trigger_cmdex();
    if (rc)
        return rc;
    if (mode == EraseMode_CHIP) {
        g_nvmc_timer = ((100)/2);
        while (1) {
            uint8_t b;
            _delay_us(500);
            rc = pdi_cmd_ldcs(0, &b);
            if (rc)
                return rc;
            if (((*(volatile uint8_t *)(((uint16_t) &(b)))) & (1 << (1)))) {
                break;
            }
            if (g_nvmc_timer == 0) {
                return 13;
            }
        }
    } else {
        return nvmc_nvmbusy_wait();
    }
    return 0;
}



uint8_t nvmc_flash_write_page(uint32_t addr, uint16_t count0) {
    uint8_t rc = 0;

    addr = 0x0800000 + addr*2;


    rc = nvmc_set_cmd(0x26); if (rc) { return rc; };
    rc = nvmc_trigger_cmdex(); if (rc) { return rc; };
    rc = nvmc_nvmbusy_wait(); if (rc) { return rc; };


    rc = nvmc_set_cmd(0x23); if (rc) { return rc; };
    rc = pdi_cmd_store(addr, ((void *)0), count0); if (rc) { return rc; };


    rc = nvmc_set_cmd(0x2F); if (rc) { return rc; };
    rc = pdi_cmd_sts_byte(addr, 0x55); if (rc) { return rc; };
    rc = nvmc_nvmbusy_wait(); if (rc) { return rc; };

    return rc;
}

uint8_t nvmc_flash_read(uint32_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr*2 + 0x0800000, count0);
}

uint8_t nvmc_eeprom_write(uint32_t addr, uint16_t count0) {
    uint8_t rc = 0;

    addr = 0x08C0000 + addr;


    rc = nvmc_set_cmd(0x36); if (rc) { return rc; };
    rc = nvmc_trigger_cmdex(); if (rc) { return rc; };
    rc = nvmc_nvmbusy_wait(); if (rc) { return rc; };


    rc = nvmc_set_cmd(0x33); if (rc) { return rc; };
    rc = pdi_cmd_store(addr, ((void *)0), count0); if (rc) { return rc; };


    rc = nvmc_set_cmd(0x35); if (rc) { return rc; };
    rc = pdi_cmd_sts_byte(addr, 0x55); if (rc) { return rc; };
    rc = nvmc_nvmbusy_wait(); if (rc) { return rc; };

    return rc;
}

uint8_t nvmc_eeprom_read(uint32_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr + 0x08C0000, count0);
}

uint8_t nvmc_fuse_write(uint16_t addr, uint8_t b) {
    uint8_t rc = 0;

    rc = nvmc_set_cmd(0x4C); if (rc) { return rc; };
    rc = pdi_cmd_sts_byte(0x08F0020 + addr, b); if (rc) { return rc; };
    rc = nvmc_nvmbusy_wait(); if (rc) { return rc; };

    return rc;
}

uint8_t nvmc_fuse_read(uint16_t addr) {
    return nvmc_read_nvm(addr + 0x08F0020, 0);
}

uint8_t nvmc_signaturerow_read(uint16_t addr, uint16_t count0) {
    return nvmc_read_nvm(addr + 0x008E0200, count0);
}

uint8_t nvmc_read_dev_sign(uint16_t count0) {
    return pdi_cmd_load(0x1000000 + (0x90 +0), ((void *)0), count0);
}

void pdi_exec(struct usb_device_request* req) {
    uint8_t buf[12];
    uint8_t bout[6];
    uint8_t outlen;
    uint16_t wValue = req->wValue;
    uint16_t wIndex = req->wIndex;
    uint16_t wLength = req->wLength;

    g_pdi_last_rc = 1;

    switch(req->bRequest) {
        case CMD_PDI_LDS: {
            uint8_t len = ((wLength) < (4) ? (wLength) : (4));
            len = ((len) > (1) ? (len) : (1));
            buf[0] = 0x0c | 0x0c | ((len-1) & 0x03);
            buf[1] = wIndex & 0xff;
            buf[2] = (wIndex >> 8) & 0xff;
            buf[3] = wValue & 0xff;
            buf[4] = (wValue >> 8) & 0xff;
            g_pdi_last_rc = pdi_cmd(buf, 5, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, 0, len);
            break;
        }
        case CMD_PDI_STS: {
            uint16_t recvb = 0;
            uint8_t len = ((wLength) < (4) ? (wLength) : (4));
            len = ((len) > (1) ? (len) : (1));
            buf[0] = 0x40 | 0x0c | ((len-1) & 0x03);
            buf[1] = wIndex & 0xff;
            buf[2] = (wIndex >> 8) & 0xff;
            buf[3] = wValue & 0xff;
            buf[4] = (wValue >> 8) & 0xff;
            usb_ep_recv_control_data(buf+5, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 5+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_LD: {
            uint8_t len = ((wLength) < (4) ? (wLength) : (4));
            len = ((len) > (1) ? (len) : (1));
            buf[0] = 0x20 | (wValue & 0x0c) | ((len-1) & 0x03);
            pdi_cmd(buf, 1, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, 0, len);
            break;
        }
        case CMD_PDI_ST: {
            uint16_t recvb = 0;
            uint8_t len = ((wLength) < (4) ? (wLength) : (4));
            len = ((len) > (1) ? (len) : (1));
            buf[0] = 0x60 | (wValue & 0x0c) | ((len-1) & 0x03);
            usb_ep_recv_control_data(buf+1, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_LDCS: {
            buf[0] = 0x80 | (wValue & 0x0f);
            g_pdi_last_rc = pdi_cmd(buf, 1, bout, &outlen);
            usb_ep_send_control_data(bout, outlen, 0, wLength);
            break;
        }
        case CMD_PDI_STCS: {
            buf[0] = 0xc0 | (wValue & 0x0f);
            buf[1] = wIndex & 0x0ff;
            g_pdi_last_rc = pdi_cmd(buf, 2, bout, &outlen);
            usb_ep_send_zlp();
            break;
        }
        case CMD_PDI_REPEAT: {
            uint16_t recvb = 0;
            uint8_t len = ((wLength) < (4) ? (wLength) : (4));
            len = ((len) > (1) ? (len) : (1));
            buf[0] = 0xa0 | ((len-1) & 0x03);
            usb_ep_recv_control_data(buf+1, &recvb, len);
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }
        case CMD_PDI_KEY: {
            uint16_t recvb = 0;
            buf[0] = 0xe0;
            usb_ep_recv_control_data(buf+1, &recvb, ((wLength) < (8) ? (wLength) : (8)));
            g_pdi_last_rc = pdi_cmd(buf, 1+recvb, bout, &outlen);
            break;
        }

        case CMD_NVMC_NVM_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_read_nvm(((uint32_t)wValue << 16)|wIndex, wLength-1);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FLASH_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_flash_read(((uint32_t)wValue << 16)|wIndex, wLength-1);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_EEPROM_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_eeprom_read(((uint32_t)wValue << 16)|wIndex, wLength-1);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FUSE_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_fuse_read(wValue);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_SIGNROW_READ: {
            if (wLength)
                g_pdi_last_rc = nvmc_signaturerow_read(wValue, wLength-1);
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_FLASH_WRITE_PAGE: {
            if (wLength) {
                g_pdi_last_rc = nvmc_flash_write_page(((uint32_t)wValue << 16)|wIndex, wLength-1);
            }
            usb_ep_recv_control_data_finish();
            break;
        }
        case CMD_NVMC_EEPROM_WRITE: {
            if (wLength) {
                g_pdi_last_rc = nvmc_eeprom_write(((uint32_t)wValue << 16)|wIndex, wLength-1);
            }
            usb_ep_recv_control_data_finish();
            break;
        }
        case CMD_NVMC_FUSE_WRITE: {
            g_pdi_last_rc = nvmc_fuse_write(wValue, wIndex);
            usb_ep_send_zlp();
            break;
        }
        case CMD_NVMC_GET_DEVID: {
            if (wLength) {
                g_pdi_last_rc = nvmc_read_dev_sign(((wLength-1) < (3) ? (wLength-1) : (3)));
            }
            usb_ep_send_control_data_finish();
            break;
        }
        case CMD_NVMC_ERASE_CHIP: {
            g_pdi_last_rc = nvmc_erase_chip(wValue);
            usb_ep_send_zlp();
            break;
        }
    }
}
# 1340 "gpio-usb.c"
const struct usb_device_desc g_device_desc_P __attribute__((__progmem__)) = {
    .bLength = sizeof(struct usb_device_desc),
    .bDescriptorType = 1,
    .bcdUSB = 0x0110,
    .bDeviceClass = 0xff,
    .bDeviceSubClass = 0xff,
    .bDeviceProtocol = 0xff,
    .bMaxPacketSize0 = 64,
    .idVendor = 0xf123,
    .idProduct = 0xa001,
    .bcdDevice = 0x0100,
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
const struct { struct usb_string_desc d; wchar_t b[]; } g_string2_P __attribute__((__progmem__)) = {{sizeof(struct usb_string_desc)+(sizeof(L"gpio-usb")-2), 3}, L"gpio-usb" };;
const struct { struct usb_string_desc d; wchar_t b[]; } g_string3_P __attribute__((__progmem__)) = {{sizeof(struct usb_string_desc)+(sizeof(L"V 1.2")-2), 3}, L"V 1.2" };;


const void * const g_strings_P[4] __attribute__((__progmem__)) = {
    &g_string0_P,
    &g_string1_P,
    &g_string2_P,
    &g_string3_P
};


void usb_event_set_configuration(void) {
}


uint8_t usb_event_ep0_request(struct usb_device_request* req) {
    if ( (req->bmRequestType & 0x7f) != ((2<<5)|1) )
        return 1;

    if (usb_req_isOUT(req)) {
        switch(req->bRequest) {
            case CMD_ENTER_AVRMODE: {
                usb_ep_clearSETUP();
                cmd_avr_init();
                return 0;
            }
            case CMD_LEAVE_AVRMODE: {
                usb_ep_clearSETUP();
                cmd_avr_exit();
                return 0;
            }
            case CMD_AVR_WRITE_PRGMEM_PAGE: {
                usb_ep_clearSETUP();
                cmd_avr_writeflashpage(req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_WRITE_EEPROM_PAGE: {
                usb_ep_clearSETUP();
                cmd_avr_writeeeprompage(req->wValue, req->wLength);
                return 0;
            }
            case CMD_ENTER_PDI: {
                usb_ep_clearSETUP();
                if (1) {
                    pdi_enter();
                    ((g_pdi_flags) |= (1<<(0)));
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 200;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_LEAVE_PDI: {
                usb_ep_clearSETUP();
                g_pdi_flags = 0;
                pdi_leave();
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_PDI_ENABLE: {
                usb_ep_clearSETUP();
                if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (0)))) {
                    if ((!((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (1))))) {
                        pdi_start();
                        ((g_pdi_flags) |= (1<<(1)));
                    }
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_NVMC_ENABLE: {
                usb_ep_clearSETUP();
                if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (0)))) {
                    g_pdi_last_rc = 0;
                    if ((!((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (2))))) {
                        if ((!((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (1))))) {
                            pdi_start();
                            ((g_pdi_flags) |= (1<<(1)));
                        }
                        g_pdi_last_rc = nvmc_init();
                        if (g_pdi_last_rc == 0)
                            ((g_pdi_flags) |= (1<<(2)));
                    }
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_NVMC_DISABLE:
            case CMD_PDI_DISABLE: {
                usb_ep_clearSETUP();
                if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (0)))) {
                    nvmc_exit();
                    pdi_off();
                    ((g_pdi_flags) &= ~(1<<(2)));
                    ((g_pdi_flags) &= ~(1<<(1)));
                    g_pdi_last_rc = 0;
                } else {
                    g_pdi_last_rc = 100;
                }
                usb_ep_send_zlp();
                return 0;
            }
            case CMD_PDI_STS:
            case CMD_PDI_ST:
            case CMD_PDI_STCS:
            case CMD_PDI_REPEAT:
            case CMD_PDI_KEY:
            case CMD_NVMC_FLASH_WRITE_PAGE:
            case CMD_NVMC_EEPROM_WRITE:
            case CMD_NVMC_FUSE_WRITE:
            case CMD_NVMC_ERASE_CHIP:
            {
                usb_ep_clearSETUP();
                if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (0)))) {
                    if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (1)))) {
                        pdi_exec(req);
                    } else {
                        g_pdi_last_rc = 101;
                        usb_ep_send_zlp();
                    }
                } else {
                    g_pdi_last_rc = 100;
                    usb_ep_send_zlp();
                }
                return 0;
            }
        }
    } else {
        switch(req->bRequest) {
            case CMD_AVR_PROGMODE: {
                usb_ep_clearSETUP();
                cmd_avr_progmode(req->wValue >> 8, req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_RAWCMD: {
                usb_ep_clearSETUP();
                cmd_avr_rawcmd(req->wValue, req->wIndex, req->wLength);
                return 0;
            }
            case CMD_AVR_READ_PRGMEM: {
                usb_ep_clearSETUP();
                cmd_avr_readflash(req->wValue, req->wLength);
                return 0;
            }
           case CMD_AVR_READ_EEPROM: {
                usb_ep_clearSETUP();
                cmd_avr_readeeprom(req->wValue, req->wLength);
                return 0;
            }
            case CMD_AVR_GET_STATUS: {
                usb_ep_clearSETUP();
                uint8_t ret[2];
                ret[0] = g_avr_flags;
                ret[1] = g_avr_error;
                g_avr_error = 0;
                usb_ep_send_control_data(ret, 2, 0, req->wLength);
                return 0;
            }
            case CMD_PDI_GETSTATUS: {
                usb_ep_clearSETUP();
                uint8_t ret[1];
                ret[0] = g_pdi_last_rc;
                g_pdi_last_rc = 0;
                usb_ep_send_control_data(ret, 1, 0, req->wLength);
                return 0;
            }
            case CMD_PDI_LDS:
            case CMD_PDI_LD:
            case CMD_PDI_LDCS:
            case CMD_NVMC_FLASH_READ:
            case CMD_NVMC_EEPROM_READ:
            case CMD_NVMC_FUSE_READ:
            case CMD_NVMC_SIGNROW_READ:
            case CMD_NVMC_NVM_READ:
            case CMD_NVMC_GET_DEVID:
            {
                usb_ep_clearSETUP();
                if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (0)))) {
                    if (((*(volatile uint8_t *)(((uint16_t) &(g_pdi_flags)))) & (1 << (1)))) {
                        pdi_exec(req);
                    } else {
                        g_pdi_last_rc = 101;
                        usb_ep_send_control_data_finish();
                    }
                } else {
                    g_pdi_last_rc = 100;
                    usb_ep_send_control_data_finish();
                }
                return 0;
            }
        }
    }
    return 1;
}


static uint8_t g_led_counter = 0;

void __vector_18 (void) __attribute__ ((signal,used, externally_visible)) ; void __vector_18 (void) {
    g_led_counter++;
    if (g_led_counter >= 10) {
        (*(volatile uint8_t *)((0x05) + 0x20)) ^= (1<<(5));
        g_led_counter = 0;
    }
}


static __attribute__((noinline)) void init_system(void) {
    __asm__ __volatile__ ("wdr");
    __asm__ __volatile__ ( "in __tmp_reg__, __SREG__" "\n\t" "cli" "\n\t" "sts %0, %1" "\n\t" "sts %0, __zero_reg__" "\n\t" "out __SREG__,__tmp_reg__" "\n\t" : : "M" (((uint16_t) &((*(volatile uint8_t *)(0x60))))), "r" ((uint8_t)((1 << (4)) | (1 << (3)))) : "r0" );

    (*(volatile uint8_t *)((0x05) + 0x20)) = 0b11111111;
    (*(volatile uint8_t *)((0x08) + 0x20)) = 0b11111111;
    (*(volatile uint8_t *)((0x0B) + 0x20)) = 0b11111111;
    (*(volatile uint8_t *)((0x04) + 0x20)) = 0;
    (*(volatile uint8_t *)((0x07) + 0x20)) = 0;
    (*(volatile uint8_t *)((0x0A) + 0x20)) = 0;


    (*(volatile uint8_t *)(0x61)) = (1 << (7));
    (*(volatile uint8_t *)(0x61)) = 0x00;

    (((*(volatile uint8_t *)((0x04) + 0x20))) |= (1<<(5)));
    (((*(volatile uint8_t *)((0x05) + 0x20))) &= ~(1<<(5)));


    (*(volatile uint16_t *)(0x88)) = (*(volatile uint16_t *)(0x84)) + ((8 * 1000000 / 1) / (8 * 500) - 1);
    (*(volatile uint8_t *)(0x82)) = 0;
    (*(volatile uint8_t *)(0x80)) = (0 << (1))|(0 << (0));
    (*(volatile uint8_t *)(0x81)) = (0 << (4))|(0 << (3))|(0 << (2))|(1 << (1))|(0 << (0));
    (*(volatile uint8_t *)((0x16) + 0x20)) = 0xff;
    (*(volatile uint8_t *)(0x6F)) = (1 << (1))|(1 << (0));
}

int main(void) {
    __asm__ __volatile__ ("cli" ::: "memory");
    init_system();

    usb_init(&g_device_desc_P,
             &g_config_1_P, sizeof(g_config_1_P),
             g_strings_P, 4,
             1);

    do { (*(volatile uint8_t *)((0x33) + 0x20)) = (((*(volatile uint8_t *)((0x33) + 0x20)) & ~((1 << (1)) | (1 << (2)) | (1 << (3)))) | ((0))); } while(0);
    do { (*(volatile uint8_t *)((0x33) + 0x20)) |= (uint8_t)(1 << (0)); } while(0);
    __asm__ __volatile__ ("sei" ::: "memory");

    while(1) {
        usb_ep0_task();
    }

    return 0;
}
