	.file	"xmega.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__CCP__ = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	init
	.type	init, @function
init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sts 1536,__zero_reg__
	ldi r30,lo8(-80)
	ldi r31,0
	ldi r25,lo8(-1)
	st Z,r25
	ldi r24,lo8(24)
	sts 1552,r24
	sts 1568,__zero_reg__
	st Z,r25
	sts 1584,r24
	sts 1600,__zero_reg__
	st Z,r25
	sts 1616,r24
	sts 1632,__zero_reg__
	st Z,r25
	sts 1648,r24
	sts 1664,__zero_reg__
	st Z,r25
	sts 1680,r24
	ret
	.size	init, .-init
.global	__vector_14
	.type	__vector_14, @function
__vector_14:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r24,lo8(32)
	sts 1543,r24
/* epilogue start */
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_14, .-__vector_14
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,162
	ori r24,lo8(71)
	ldi r25,lo8(-40)
	out __CCP__,r25
	sts 162,r24
	call init
.L4:
	lds r24,463
	sbrc r24,7
	rjmp .L4
	lds r24,460
	ori r24,lo8(8)
	sts 460,r24
	lds r22,4096
	ldi r24,lo8(1)
	ldi r25,0
	call __eewr_byte_x16a4
	ldi r24,lo8(32)
	sts 1537,r24
	sts 1557,__zero_reg__
	sts 2049,__zero_reg__
	sts 2051,__zero_reg__
	sts 2052,__zero_reg__
	ldi r18,lo8(1)
	sts 2054,r18
	sts 2055,__zero_reg__
	ldi r24,lo8(-1)
	sts 2060,r24
	ldi r24,lo8(-124)
	ldi r25,lo8(30)
	sts 2086,r24
	sts 2086+1,r25
	ldi r24,lo8(5)
	sts 2048,r24
	sts 162,r18
/* #APP */
 ;  111 "xmega.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L5:
	ldi r18,lo8(399999)
	ldi r24,hi8(399999)
	ldi r25,hlo8(399999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(32)
	sts 1543,r24
	rjmp .L5
	.size	main, .-main
	.ident	"GCC: (Gentoo 4.8.3 p1.1, pie-0.5.9) 4.8.3"
