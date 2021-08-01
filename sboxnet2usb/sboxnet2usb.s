	.file	"sboxnet2usb.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.type	sboxnet_debug_inc_recverrors, @function
sboxnet_debug_inc_recverrors:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet+547)
	ldi r31,hi8(g_sboxnet+547)
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	ret
	.size	sboxnet_debug_inc_recverrors, .-sboxnet_debug_inc_recverrors
	.type	sboxnet_debug_inc_recverr_dor, @function
sboxnet_debug_inc_recverr_dor:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet+553)
	ldi r31,hi8(g_sboxnet+553)
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	ret
	.size	sboxnet_debug_inc_recverr_dor, .-sboxnet_debug_inc_recverr_dor
	.type	sboxnet_debug_inc_recv_byte, @function
sboxnet_debug_inc_recv_byte:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet+525)
	ldi r31,hi8(g_sboxnet+525)
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	ret
	.size	sboxnet_debug_inc_recv_byte, .-sboxnet_debug_inc_recv_byte
	.type	sboxnet_prandom, @function
sboxnet_prandom:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet)
	ldi r31,hi8(g_sboxnet)
	ldd r24,Z+8
	mov r25,r24
	lsl r25
	eor r24,r25
	mov r25,r24
	lsr r25
	eor r24,r25
	mov r25,r24
	lsl r25
	lsl r25
	eor r24,r25
	std Z+8,r24
	ret
	.size	sboxnet_prandom, .-sboxnet_prandom
	.type	sboxnet_rb_write_commit, @function
sboxnet_rb_write_commit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r30,r24
	ldd r24,Z+6
	ldd r22,Z+4
	mov r20,r22
	ldi r21,0
	add r20,r24
	adc r21,__zero_reg__
	ldd r18,Z+7
	ldi r19,0
	cp r18,r20
	cpc r19,r21
	brge .L6
	lds r25,g_sboxnet+1
	ori r25,lo8(4)
	sts g_sboxnet+1,r25
.L6:
	add r24,r22
	std Z+6,r24
	std Z+4,__zero_reg__
	ldd r24,Z+3
	std Z+5,r24
	ret
	.size	sboxnet_rb_write_commit, .-sboxnet_rb_write_commit
	.type	sboxnet_rb_write, @function
sboxnet_rb_write:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r30,r24
	ldd r25,Z+6
	ldd r24,Z+4
	add r25,r24
	ldd r18,Z+7
	cp r25,r18
	brsh .L9
	subi r24,lo8(-(1))
	std Z+4,r24
	ldd r24,Z+3
	movw r28,r30
	add r28,r24
	adc r29,__zero_reg__
	std Y+8,r22
	subi r24,lo8(-(1))
	cp r24,r18
	brsh .L10
	std Z+3,r24
	rjmp .L12
.L10:
	std Z+3,__zero_reg__
.L12:
	ldi r24,lo8(1)
	rjmp .L11
.L9:
	lds r24,g_sboxnet+1
	ori r24,lo8(4)
	sts g_sboxnet+1,r24
	ldi r24,0
.L11:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	sboxnet_rb_write, .-sboxnet_rb_write
	.type	sboxnet_rb_read_commit, @function
sboxnet_rb_read_commit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r30,r24
	ldd r24,Z+6
	ldd r25,Z+1
	cp r24,r25
	brsh .L14
	lds r25,g_sboxnet+1
	ori r25,lo8(2)
	sts g_sboxnet+1,r25
.L14:
	ldd r25,Z+1
	sub r24,r25
	std Z+6,r24
	std Z+1,__zero_reg__
	ld r24,Z
	std Z+2,r24
	ret
	.size	sboxnet_rb_read_commit, .-sboxnet_rb_read_commit
	.type	sboxnet_rb_read, @function
sboxnet_rb_read:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r30,r24
	ldd r25,Z+6
	ldd r24,Z+1
	cp r24,r25
	brsh .L16
	subi r24,lo8(-(1))
	std Z+1,r24
	ld r25,Z
	movw r28,r30
	add r28,r25
	adc r29,__zero_reg__
	ldd r24,Y+8
	subi r25,lo8(-(1))
	st Z,r25
	ldd r18,Z+7
	cp r25,r18
	brlo .L17
	st Z,__zero_reg__
	rjmp .L17
.L16:
	lds r24,g_sboxnet+1
	ori r24,lo8(2)
	sts g_sboxnet+1,r24
	ldi r24,0
.L17:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	sboxnet_rb_read, .-sboxnet_rb_read
	.type	sboxnet_set_timer, @function
sboxnet_set_timer:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r18,132
	lds r19,132+1
	add r24,r18
	adc r25,r19
	sts 138+1,r25
	sts 138,r24
	ldi r24,lo8(4)
	out 0x16,r24
	ldi r30,lo8(111)
	ldi r31,0
	ld r24,Z
	ori r24,lo8(4)
	st Z,r24
	ret
	.size	sboxnet_set_timer, .-sboxnet_set_timer
	.type	sboxnet_check_try_transmit, @function
sboxnet_check_try_transmit:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	ldi r28,lo8(g_sboxnet)
	ldi r29,hi8(g_sboxnet)
	ldd r24,Y+4
	cpse r24,__zero_reg__
	rjmp .L20
	movw r30,r28
	subi r30,-17
	sbci r31,-2
	ld r24,Z
	cpi r24,lo8(7)
	brlo .L20
	std Y+3,__zero_reg__
	movw r24,r28
	subi r24,-11
	sbci r25,-2
	call sboxnet_rb_read
	std Y+4,r24
.L20:
	ldd r24,Y+4
	tst r24
	breq .L19
	ldd r24,Y+3
	cpse r24,__zero_reg__
	rjmp .L19
	ld r24,Y
	cpse r24,__zero_reg__
	rjmp .L19
	call sboxnet_prandom
	mov r18,r24
	andi r18,lo8(63)
	ldd r25,Y+2
	mov r24,r25
	ldi r25,0
	lsl r24
	rol r25
	subi r24,120
	sbci r25,-1
	ldi r19,lo8(4)
	out 0x1c,r19
	ldi r19,lo8(1)
	sts g_sboxnet,r19
	add r24,r18
	adc r25,__zero_reg__
/* epilogue start */
	pop r29
	pop r28
	jmp sboxnet_set_timer
.L19:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	sboxnet_check_try_transmit, .-sboxnet_check_try_transmit
	.type	usb_ep_clearSETUP, @function
usb_ep_clearSETUP:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(-24)
	ldi r31,0
	ld r24,Z
	andi r24,lo8(-9)
	st Z,r24
	ret
	.size	usb_ep_clearSETUP, .-usb_ep_clearSETUP
	.type	usb_ep_clearIN, @function
usb_ep_clearIN:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(-24)
	ldi r31,0
	ld r24,Z
	andi r24,lo8(-2)
	st Z,r24
	ret
	.size	usb_ep_clearIN, .-usb_ep_clearIN
	.type	usb_ep_clearOUT, @function
usb_ep_clearOUT:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(-24)
	ldi r31,0
	ld r24,Z
	andi r24,lo8(-5)
	st Z,r24
	ret
	.size	usb_ep_clearOUT, .-usb_ep_clearOUT
	.type	init_system, @function
init_system:
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 3 */
.L__stack_usage = 3
/* #APP */
 ;  470 "sboxnet2usb.c" 1
	wdr
 ;  0 "" 2
/* #NOAPP */
	in r24,0x34
	std Y+1,r24
	out 0x34,__zero_reg__
	ldi r24,lo8(24)
/* #APP */
 ;  473 "sboxnet2usb.c" 1
	in __tmp_reg__, __SREG__
	cli
	sts 96, r24
	sts 96, __zero_reg__
	out __SREG__,__tmp_reg__
	
 ;  0 "" 2
/* #NOAPP */
	ldd r24,Y+1
	sbrs r24,3
	rjmp .L29
/* #APP */
 ;  476 "sboxnet2usb.c" 1
	jmp  0x3000

 ;  0 "" 2
/* #NOAPP */
.L29:
	ldi r24,lo8(31)
	out 0x5,r24
	ldi r24,lo8(-1)
	out 0x8,r24
	ldi r25,lo8(127)
	out 0xb,r25
	ldi r25,lo8(-32)
	out 0x4,r25
	out 0x7,__zero_reg__
	out 0xa,__zero_reg__
	ldi r25,lo8(-128)
	sts 97,r25
	sts 97,__zero_reg__
	sts 128,__zero_reg__
	ldi r25,lo8(2)
	sts 129,r25
	out 0x16,r24
	ldi r24,lo8(1)
	sts 111,r24
	ldi r24,lo8(eeprom)
	ldi r25,hi8(eeprom)
	call __eerd_word_m32u2
	adiw r24,1
	brne .L28
	ldi r22,lo8(4)
	ldi r24,lo8(eeprom)
	ldi r25,hi8(eeprom)
	call __eeupd_byte_m32u2
	ldi r22,lo8(3)
	ldi r24,lo8(eeprom+1)
	ldi r25,hi8(eeprom+1)
	call __eeupd_byte_m32u2
	ldi r22,lo8(120)
	ldi r23,0
	ldi r24,lo8(eeprom+2)
	ldi r25,hi8(eeprom+2)
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	jmp __eeupd_word_m32u2
.L28:
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	init_system, .-init_system
	.type	sboxnet_receiver_enable, @function
sboxnet_receiver_enable:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(-56)
	ldi r31,0
	ld r24,Z
	ori r24,lo8(1)
	st Z,r24
	ldi r30,lo8(-55)
	ldi r31,0
	ld r24,Z
	ori r24,lo8(-112)
	st Z,r24
	ret
	.size	sboxnet_receiver_enable, .-sboxnet_receiver_enable
	.type	sboxnet_init, @function
sboxnet_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet)
	ldi r31,hi8(g_sboxnet)
	ldi r24,lo8(51)
	ldi r25,lo8(2)
	movw r26,r30
	movw r18,r24
	0:
	st X+,__zero_reg__
	subi r18,1
	sbci r19,0
	brne 0b
	lds r24,102
	std Z+8,r24
	ldi r24,lo8(25)
	std Z+2,r24
	ldi r24,lo8(-6)
	std Z+16,r24
	sts g_sboxnet+274,r24
	cbi 0xa,2
	sbi 0xb,2
	sbi 0xb,3
	sbi 0xa,3
	cbi 0x1d,2
	ldi r30,lo8(105)
	ldi r31,0
	ld r24,Z
	andi r24,lo8(-33)
	st Z,r24
	ld r24,Z
	ori r24,lo8(16)
	st Z,r24
	ldi r24,lo8(4)
	out 0x1c,r24
	sts 205,__zero_reg__
	ldi r25,lo8(3)
	sts 204,r25
	ldi r25,lo8(64)
	sts 200,r25
	ldi r25,lo8(38)
	sts 202,r25
	sts 201,r24
	jmp sboxnet_receiver_enable
	.size	sboxnet_init, .-sboxnet_init
.global	__vector_23
	.type	__vector_23, @function
__vector_23:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r14
	push r15
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 21 */
.L__stack_usage = 21
	ldi r28,lo8(g_sboxnet)
	ldi r29,hi8(g_sboxnet)
	lds r16,200
	lds r25,201
	lds r17,206
	mov r24,r16
	andi r24,lo8(28)
	breq .L37
	call sboxnet_debug_inc_recverrors
	sbrs r16,4
	rjmp .L38
	movw r30,r28
	subi r30,-37
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
.L38:
	sbrc r16,3
	call sboxnet_debug_inc_recverr_dor
.L39:
	sbrs r16,2
	rjmp .L41
	movw r30,r28
	subi r30,-39
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	rjmp .L41
.L37:
	ldd r24,Y+13
	sbrs r25,1
	rjmp .L42
	tst r24
	breq .L43
	movw r30,r28
	subi r30,-43
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	call sboxnet_debug_inc_recverrors
	std Y+13,__zero_reg__
	ldd r24,Y+14
	std Y+12,r24
.L43:
	lds r24,g_sboxnet_addr
	cp r17,r24
	breq .L44
	cpi r17,lo8(-1)
	breq .L44
	ldd r24,Y+1
	sbrs r24,0
	rjmp .L41
.L44:
	lds r24,200
	andi r24,lo8(-2)
	sts 200,r24
	movw r14,r28
	ldi r24,9
	add r14,r24
	adc r15,__zero_reg__
	ldi r22,0
	movw r24,r14
	call sboxnet_rb_write
	tst r24
	breq .L73
	mov r22,r17
	movw r24,r14
	call sboxnet_rb_write
	tst r24
	breq .L73
	call sboxnet_debug_inc_recv_byte
	ldi r24,lo8(6)
	std Y+7,r24
	rjmp .L36
.L42:
	ldd r25,Y+7
	cp r25,r24
	brsh .L49
	movw r30,r28
	subi r30,-45
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	rjmp .L48
.L49:
	cpi r24,lo8(4)
	brne .L50
	mov r24,r17
	andi r24,lo8(63)
	subi r24,lo8(-(6))
	std Y+7,r24
.L50:
	movw r14,r28
	ldi r24,9
	add r14,r24
	adc r15,__zero_reg__
	mov r22,r17
	movw r24,r14
	call sboxnet_rb_write
	cpse r24,__zero_reg__
	rjmp .L51
.L73:
	call sboxnet_debug_inc_recverr_dor
	rjmp .L48
.L51:
	call sboxnet_debug_inc_recv_byte
	ldd r24,Y+13
	ldi r25,0
	ldd r20,Y+7
	mov r18,r20
	ldi r19,0
	subi r18,-1
	sbci r19,-1
	cp r24,r18
	cpc r25,r19
	brne .L36
	ldd r24,Y+14
	movw r30,r28
	add r30,r24
	adc r31,__zero_reg__
	std Z+17,r20
	std Y+7,__zero_reg__
	lds r25,200
	ori r25,lo8(1)
	sts 200,r25
	movw r24,r14
	call sboxnet_rb_write_commit
	lds r24,g_sboxnet+527
	lds r25,g_sboxnet+527+1
	adiw r24,1
	sts g_sboxnet+527+1,r25
	sts g_sboxnet+527,r24
	rjmp .L36
.L48:
	call sboxnet_debug_inc_recverrors
.L41:
	std Y+7,__zero_reg__
	lds r24,200
	ori r24,lo8(1)
	sts 200,r24
	std Y+13,__zero_reg__
	ldd r24,Y+14
	std Y+12,r24
.L36:
/* epilogue start */
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r15
	pop r14
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_23, .-__vector_23
.global	__vector_25
	.type	__vector_25, @function
__vector_25:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r16
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 19 */
.L__stack_usage = 19
	ldi r28,lo8(g_sboxnet)
	ldi r29,hi8(g_sboxnet)
	lds r18,200
	mov r25,r18
	andi r25,lo8(20)
	lds r20,206
	ldd r19,Y+5
	ldi r24,lo8(1)
	cpse r20,r19
	rjmp .L75
	ldi r24,0
.L75:
	movw r16,r28
	subi r16,-11
	sbci r17,-2
	sbrs r18,7
	rjmp .L76
	cpse r25,__zero_reg__
	rjmp .L77
	cpse r24,__zero_reg__
	rjmp .L78
	rjmp .L99
.L76:
	movw r30,r28
	subi r30,-27
	sbci r31,-3
	ld r18,Z
	ldd r19,Z+1
	subi r18,-1
	sbci r19,-1
	std Z+1,r19
	st Z,r18
	tst r25
	breq .L80
.L77:
	movw r30,r28
	subi r30,-29
	sbci r31,-3
	ld r18,Z
	ldd r19,Z+1
	subi r18,-1
	sbci r19,-1
	std Z+1,r19
	st Z,r18
.L80:
	tst r24
	breq .L81
.L78:
	movw r30,r28
	subi r30,-31
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
.L81:
	cbi 0xb,3
	lds r24,201
	andi r24,lo8(-89)
	sts 201,r24
	ldi r24,lo8(2)
	st Y,r24
	movw r30,r28
	subi r30,-25
	sbci r31,-3
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	ldd r24,Y+2
	tst r24
	breq .L82
	subi r24,lo8(-(-1))
	std Y+2,r24
	movw r30,r16
	std Z+1,__zero_reg__
	ldd r24,Z+2
	st Z,r24
	movw r30,r28
	subi r30,-33
	sbci r31,-3
	rjmp .L98
.L82:
	ldd r25,Y+3
	ldd r24,Y+4
	cp r25,r24
	brsh .L100
	movw r24,r16
	call sboxnet_rb_read
	ldd r24,Y+3
	subi r24,lo8(-(1))
	std Y+3,r24
	rjmp .L82
.L100:
	std Y+4,__zero_reg__
	std Y+3,__zero_reg__
	ldi r24,lo8(25)
	std Y+2,r24
	movw r24,r16
	call sboxnet_rb_read_commit
	movw r30,r28
	subi r30,-49
	sbci r31,-3
.L98:
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	std Z+1,r25
	st Z,r24
	std Y+4,__zero_reg__
	std Y+3,__zero_reg__
	ldi r24,lo8(-128)
	ldi r25,0
	call sboxnet_set_timer
	rjmp .L74
.L99:
	lds r24,201
	andi r24,lo8(111)
	sts 201,r24
	lds r24,201
	ori r24,lo8(16)
	sts 201,r24
	ldd r25,Y+3
	ldd r24,Y+4
	cp r25,r24
	brsh .L86
	movw r24,r16
	call sboxnet_rb_read
	ldd r25,Y+3
	subi r25,lo8(-(1))
	std Y+3,r25
	std Y+5,r24
	lds r25,201
	andi r25,lo8(-2)
	sts 201,r25
	sts 206,r24
	rjmp .L87
.L86:
	lds r24,201
	andi r24,lo8(-65)
	sts 201,r24
	std Y+4,__zero_reg__
	std Y+3,__zero_reg__
	movw r24,r16
	call sboxnet_rb_read_commit
	lds r24,g_sboxnet+535
	lds r25,g_sboxnet+535+1
	adiw r24,1
	sts g_sboxnet+535+1,r25
	sts g_sboxnet+535,r24
	call sboxnet_receiver_enable
	ld r24,Y
	cpi r24,lo8(3)
	brne .L88
	st Y,__zero_reg__
.L88:
	ldi r24,lo8(25)
	std Y+2,r24
	call sboxnet_check_try_transmit
.L87:
	subi r28,-19
	sbci r29,-3
	ld r24,Y
	ldd r25,Y+1
	adiw r24,1
	std Y+1,r25
	st Y,r24
.L74:
/* epilogue start */
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r16
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_25, .-__vector_25
	.type	sboxnet_receive_msg, @function
sboxnet_receive_msg:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	movw r16,r24
	cpi r22,lo8(6)
	brsh .L102
.L106:
	ldi r24,lo8(-3)
	rjmp .L103
.L102:
	in r19,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r25,g_sboxnet+10
	lds r18,g_sboxnet+15
	mov r24,r18
	sub r24,r25
	cpi r24,lo8(7)
	brlo .L109
	cp r25,r18
	brsh .L105
	lds r30,g_sboxnet+9
	ldi r31,0
	subi r30,lo8(-(g_sboxnet+8))
	sbci r31,hi8(-(g_sboxnet+8))
	ldd r28,Z+9
	rjmp .L104
.L105:
	lds r25,g_sboxnet+1
	ori r25,lo8(2)
	sts g_sboxnet+1,r25
.L109:
	ldi r28,0
.L104:
	out __SREG__,r19
	cpi r28,lo8(6)
	brsh .+2
	rjmp .L111
	mov r18,r28
	ldi r19,0
	ldi r25,0
	cp r18,r24
	cpc r19,r25
	brlt .+2
	rjmp .L111
	cp r22,r28
	brlo .L106
	cpi r28,lo8(70)
	brlo .+2
	rjmp .L112
	in r29,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(g_sboxnet+9)
	ldi r25,hi8(g_sboxnet+9)
	call sboxnet_rb_read
	out __SREG__,r29
	mov r15,__zero_reg__
	ldi r29,lo8(-1)
.L107:
	in r14,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(g_sboxnet+9)
	ldi r25,hi8(g_sboxnet+9)
	call sboxnet_rb_read
	out __SREG__,r14
/* #APP */
 ;  321 "/usr/avr/include/util/crc16.h" 1
		eor	r29, r24
		ldi	r25, 8
		ldi	r18, 0x8C
	1:	lsr	r29
		brcc	2f
		eor	r29, r18
	2:	dec	r25
		brne	1b
	
 ;  0 "" 2
/* #NOAPP */
	movw r30,r16
	st Z,r24
	inc r15
	subi r16,-1
	sbci r17,-1
	cp r15,r28
	brlo .L107
	in r17,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(g_sboxnet+9)
	ldi r25,hi8(g_sboxnet+9)
	call sboxnet_rb_read_commit
	out __SREG__,r17
	tst r29
	breq .L108
	call sboxnet_debug_inc_recverrors
	lds r24,g_sboxnet+559
	lds r25,g_sboxnet+559+1
	adiw r24,1
	sts g_sboxnet+559+1,r25
	sts g_sboxnet+559,r24
	ldi r24,lo8(-2)
	rjmp .L103
.L108:
	lds r24,g_sboxnet+529
	lds r25,g_sboxnet+529+1
	adiw r24,1
	sts g_sboxnet+529+1,r25
	sts g_sboxnet+529,r24
	mov r24,r28
	rjmp .L103
.L111:
	ldi r24,lo8(-1)
	rjmp .L103
.L112:
	ldi r24,lo8(-4)
.L103:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	sboxnet_receive_msg, .-sboxnet_receive_msg
	.type	sboxnet_send_msg, @function
sboxnet_send_msg:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	movw r30,r24
	ldd r28,Z+3
	andi r28,lo8(63)
	subi r28,lo8(-(6))
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r25,g_sboxnet+274
	lds r18,g_sboxnet+273
	sub r25,r18
	lds r18,g_sboxnet+271
	sub r25,r18
	out __SREG__,r24
	cp r28,r25
	brlo .+2
	rjmp .L120
	movw r14,r30
	in r29,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	mov r22,r28
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	call sboxnet_rb_write
	out __SREG__,r29
	cpse r24,__zero_reg__
	rjmp .L121
.L118:
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	sts g_sboxnet+271,__zero_reg__
	lds r25,g_sboxnet+272
	sts g_sboxnet+270,r25
	out __SREG__,r24
	rjmp .L120
.L121:
	ldi r17,0
	ldi r29,lo8(-1)
.L115:
	mov r18,r17
	ldi r19,0
	mov r24,r28
	ldi r25,0
	sbiw r24,1
	cp r18,r24
	cpc r19,r25
	brge .L128
	cpi r17,lo8(1)
	brne .L116
	lds r22,g_sboxnet_addr
	rjmp .L117
.L116:
	movw r30,r14
	add r30,r18
	adc r31,r19
	ld r22,Z
.L117:
/* #APP */
 ;  321 "/usr/avr/include/util/crc16.h" 1
		eor	r29, r22
		ldi	r24, 8
		ldi	r25, 0x8C
	1:	lsr	r29
		brcc	2f
		eor	r29, r25
	2:	dec	r24
		brne	1b
	
 ;  0 "" 2
/* #NOAPP */
	in r16,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	call sboxnet_rb_write
	out __SREG__,r16
	tst r24
	breq .L118
	subi r17,lo8(-(1))
	rjmp .L115
.L128:
	in r28,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	mov r22,r29
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	call sboxnet_rb_write
	out __SREG__,r28
	tst r24
	brne .+2
	rjmp .L118
	in r28,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	call sboxnet_rb_write_commit
	call sboxnet_check_try_transmit
	out __SREG__,r28
	lds r24,g_sboxnet+533
	lds r25,g_sboxnet+533+1
	adiw r24,1
	sts g_sboxnet+533+1,r25
	sts g_sboxnet+533,r24
	ldi r24,0
	rjmp .L114
.L120:
	ldi r24,lo8(1)
.L114:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	sboxnet_send_msg, .-sboxnet_send_msg
	.type	sboxnet_can_send_msg, @function
sboxnet_can_send_msg:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r30,r24
	ldd r25,Z+3
	andi r25,lo8(63)
	subi r25,lo8(-(6))
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r18,g_sboxnet+274
	lds r19,g_sboxnet+273
	sub r18,r19
	lds r19,g_sboxnet+271
	sub r18,r19
	out __SREG__,r24
	ldi r24,lo8(1)
	cp r25,r18
	brlo .L130
	ldi r24,0
.L130:
	ret
	.size	sboxnet_can_send_msg, .-sboxnet_can_send_msg
	.type	sboxnet_all_sent, @function
sboxnet_all_sent:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r25,g_sboxnet+273
	ldi r24,lo8(1)
	cpse r25,__zero_reg__
	ldi r24,0
.L132:
	ret
	.size	sboxnet_all_sent, .-sboxnet_all_sent
	.type	sboxnet_can_read, @function
sboxnet_can_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r18,g_sboxnet+10
	lds r25,g_sboxnet+15
	sub r25,r18
	out __SREG__,r24
	ldi r24,lo8(1)
	cpi r25,lo8(6)
	brsh .L137
	ldi r24,0
.L137:
	ret
	.size	sboxnet_can_read, .-sboxnet_can_read
	.type	sboxnet_can_send, @function
sboxnet_can_send:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r25,g_sboxnet+274
	lds r18,g_sboxnet+273
	sub r25,r18
	lds r18,g_sboxnet+271
	sub r25,r18
	out __SREG__,r24
	ldi r24,lo8(1)
	cpi r25,lo8(7)
	brsh .L139
	ldi r24,0
.L139:
	ret
	.size	sboxnet_can_send, .-sboxnet_can_send
	.type	get_status, @function
get_status:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	lds r28,g_sboxnet_status
	sts g_sboxnet_status,__zero_reg__
	call sboxnet_can_read
	cpse r24,__zero_reg__
	ori r28,lo8(2)
.L147:
	ldi r29,0
	call sboxnet_can_send
	cpse r24,__zero_reg__
	ori r28,16
.L143:
	mov r24,r28
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	get_status, .-get_status
.global	__vector_16
	.type	__vector_16, @function
__vector_16:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 16 */
.L__stack_usage = 16
	lds r24,111
	andi r24,lo8(-5)
	sts 111,r24
	lds r24,g_sboxnet
	cpi r24,lo8(1)
	breq .+2
	rjmp .L149
	lds r24,g_sboxnet+4
	tst r24
	brne .+2
	rjmp .L150
	lds r24,g_sboxnet+3
	cpse r24,__zero_reg__
	rjmp .L150
	lds r25,g_sboxnet+273
	lds r24,g_sboxnet+268
	cp r24,r25
	brsh .L151
	lds r30,g_sboxnet+267
	ldi r31,0
	subi r30,lo8(-(g_sboxnet+275))
	sbci r31,hi8(-(g_sboxnet+275))
	ld r28,Z
	rjmp .L152
.L151:
	lds r24,g_sboxnet+1
	ori r24,lo8(2)
	sts g_sboxnet+1,r24
	ldi r28,0
.L152:
	sbi 0xb,3
	lds r24,201
	ori r24,lo8(9)
	sts 201,r24
	sbic 0x1c,2
	rjmp .L153
	sbic 0x9,2
	rjmp .L154
.L153:
	call sboxnet_prandom
	mov r20,r24
	andi r20,lo8(63)
	lds r18,g_sboxnet+2
	ldi r19,0
	lsl r18
	rol r19
	movw r24,r18
	subi r24,-120
	sbci r25,-1
	ldi r18,lo8(4)
	out 0x1c,r18
	ldi r18,lo8(1)
	sts g_sboxnet,r18
	add r24,r20
	adc r25,__zero_reg__
	rjmp .L164
.L154:
	lds r24,201
	andi r24,lo8(111)
	sts 201,r24
	lds r24,201
	ori r24,lo8(80)
	sts 201,r24
	sts 206,r28
	lds r24,200
	andi r24,lo8(-2)
	sts 200,r24
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	call sboxnet_rb_read
	sts g_sboxnet+5,r28
	lds r24,g_sboxnet+3
	subi r24,lo8(-(1))
	sts g_sboxnet+3,r24
	ldi r24,lo8(3)
	sts g_sboxnet,r24
	rjmp .L148
.L150:
	sts g_sboxnet,__zero_reg__
	rjmp .L148
.L149:
	cpi r24,lo8(2)
	brne .L156
	sbi 0xb,3
	ldi r28,lo8(4)
	sts g_sboxnet,r28
	ldi r24,lo8(32)
	ldi r25,0
	call sboxnet_set_timer
	out 0x1c,r28
	rjmp .L148
.L156:
	cpi r24,lo8(4)
	brne .L148
	sbic 0x1c,2
	rjmp .L157
	sbis 0x9,2
	rjmp .L157
	call sboxnet_receiver_enable
	sts g_sboxnet,__zero_reg__
	call sboxnet_check_try_transmit
	rjmp .L148
.L157:
	ldi r24,lo8(4)
	out 0x1c,r24
	ldi r24,lo8(32)
	ldi r25,0
.L164:
	call sboxnet_set_timer
.L148:
/* epilogue start */
	pop r31
	pop r30
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_16, .-__vector_16
	.section	.init8,"ax",@progbits
.global	stack_init8
	.type	stack_init8, @function
stack_init8:
/* prologue: naked */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(_end)
	ldi r31,hi8(_end)
	rjmp .L166
.L167:
	ldi r24,lo8(-91)
	st Z,r24
	adiw r30,1
.L166:
	ldi r24,hi8(__stack)
	cpi r30,lo8(__stack)
	cpc r31,r24
	brlo .L167
/* epilogue start */
	.size	stack_init8, .-stack_init8
	.text
.global	usb_ep_read_data
	.type	usb_ep_read_data, @function
usb_ep_read_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L169:
	tst r22
	breq .L171
	lds r18,241
	movw r30,r24
	st Z,r18
	subi r22,lo8(-(-1))
	adiw r24,1
	rjmp .L169
.L171:
/* epilogue start */
	ret
	.size	usb_ep_read_data, .-usb_ep_read_data
.global	usb_ep_waitIN
	.type	usb_ep_waitIN, @function
usb_ep_waitIN:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L173:
	lds r24,232
	sbrc r24,0
	rjmp .L177
	lds r24,g_usb_dev
	andi r24,lo8(28)
	brne .L173
	ldi r24,lo8(3)
	ret
.L177:
	ldi r24,0
	ret
	.size	usb_ep_waitIN, .-usb_ep_waitIN
.global	usb_ep_send_zlp
	.type	usb_ep_send_zlp, @function
usb_ep_send_zlp:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call usb_ep_waitIN
	cpse r24,__zero_reg__
	rjmp .L179
	call usb_ep_clearIN
	jmp usb_ep_waitIN
.L179:
	ret
	.size	usb_ep_send_zlp, .-usb_ep_send_zlp
.global	usb_mem_read_byte
	.type	usb_mem_read_byte, @function
usb_mem_read_byte:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cpi r22,lo8(1)
	breq .L182
	cpi r22,lo8(2)
	brne .L186
	jmp __eerd_byte_m32u2
.L182:
	movw r30,r24
/* #APP */
 ;  139 "../avrutilslib/usb/usb.c" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	ret
.L186:
	movw r30,r24
	ld r24,Z
	ret
	.size	usb_mem_read_byte, .-usb_mem_read_byte
.global	usb_ep_getbufsize
	.type	usb_ep_getbufsize, @function
usb_ep_getbufsize:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r30,237
	swap r30
	andi r30,lo8(3)
	ldi r31,0
	subi r30,lo8(-(epsizemap.2616))
	sbci r31,hi8(-(epsizemap.2616))
/* #APP */
 ;  152 "../avrutilslib/usb/usb.c" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	ret
	.size	usb_ep_getbufsize, .-usb_ep_getbufsize
.global	usb_ep_send_in_data
	.type	usb_ep_send_in_data, @function
usb_ep_send_in_data:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 8 */
.L__stack_usage = 8
	movw r14,r24
	movw r16,r22
	mov r29,r20
.L191:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L196
	lds r24,232
	sbrc r24,3
	rjmp .L197
	lds r24,232
	mov r28,r24
	andi r28,lo8(4)
	sbrs r24,2
	rjmp .L190
	call usb_ep_clearOUT
	ldi r28,lo8(2)
	rjmp .L189
.L190:
	lds r24,232
	sbrs r24,0
	rjmp .L191
.L192:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L193
	lds r24,232
	sbrs r24,5
	rjmp .L193
	movw r12,r14
	ldi r24,-1
	sub r12,r24
	sbc r13,r24
	mov r22,r29
	movw r24,r14
	call usb_mem_read_byte
	sts 241,r24
	subi r16,1
	sbc r17,__zero_reg__
	movw r14,r12
	rjmp .L192
.L193:
	call usb_ep_clearIN
	lds r25,232
	andi r25,lo8(127)
	sts 232,r25
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .L191
	rjmp .L189
.L196:
	ldi r28,lo8(3)
	rjmp .L189
.L197:
	ldi r28,lo8(1)
.L189:
	mov r24,r28
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	usb_ep_send_in_data, .-usb_ep_send_in_data
.global	usb_ep_send_control_data_chunk
	.type	usb_ep_send_control_data_chunk, @function
usb_ep_send_control_data_chunk:
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 10 */
.L__stack_usage = 10
	movw r12,r24
	movw r16,r22
	mov r14,r20
	call usb_ep_getbufsize
	mov r15,r24
.L206:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L214
	lds r24,232
	sbrc r24,3
	rjmp .L215
	lds r24,232
	mov r29,r24
	andi r29,lo8(4)
	sbrs r24,2
	rjmp .L205
	call usb_ep_clearOUT
	ldi r29,lo8(2)
	rjmp .L217
.L205:
	lds r24,232
	sbrs r24,0
	rjmp .L206
	lds r28,242
.L207:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L208
	cp r28,r15
	brsh .L219
	movw r10,r12
	ldi r24,-1
	sub r10,r24
	sbc r11,r24
	mov r22,r14
	movw r24,r12
	call usb_mem_read_byte
	sts 241,r24
	subi r16,1
	sbc r17,__zero_reg__
	subi r28,lo8(-(1))
	movw r12,r10
	rjmp .L207
.L214:
	ldi r29,lo8(3)
	rjmp .L217
.L215:
	ldi r29,lo8(1)
	rjmp .L217
.L208:
	cpse r28,r15
	rjmp .L217
.L212:
	call usb_ep_clearIN
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .L206
	rjmp .L217
.L219:
	cpse r28,r15
	rjmp .L206
	rjmp .L212
.L217:
	mov r24,r29
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	ret
	.size	usb_ep_send_control_data_chunk, .-usb_ep_send_control_data_chunk
.global	usb_ep_send_control_data_finish
	.type	usb_ep_send_control_data_finish, @function
usb_ep_send_control_data_finish:
	push r28
/* prologue: function */
/* frame size = 0 */
/* stack size = 1 */
.L__stack_usage = 1
.L225:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	brne .L221
.L227:
	ldi r28,lo8(3)
	rjmp .L222
.L221:
	lds r24,232
	sbrs r24,3
	rjmp .L223
.L228:
	ldi r28,lo8(1)
	rjmp .L222
.L223:
	lds r24,232
	mov r28,r24
	andi r28,lo8(4)
	sbrs r24,2
	rjmp .L224
	call usb_ep_clearOUT
	ldi r28,lo8(2)
	rjmp .L222
.L224:
	lds r24,232
	sbrs r24,0
	rjmp .L225
	call usb_ep_clearIN
.L226:
	lds r24,232
	sbrc r24,2
	rjmp .L234
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L227
	lds r24,232
	sbrs r24,3
	rjmp .L226
	rjmp .L228
.L234:
	call usb_ep_clearOUT
.L222:
	mov r24,r28
/* epilogue start */
	pop r28
	ret
	.size	usb_ep_send_control_data_finish, .-usb_ep_send_control_data_finish
.global	usb_ep_send_control_data
	.type	usb_ep_send_control_data, @function
usb_ep_send_control_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cp r22,r18
	cpc r23,r19
	brsh .L236
	movw r18,r22
.L236:
	movw r22,r18
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L237
	jmp usb_ep_send_control_data_finish
.L237:
	ret
	.size	usb_ep_send_control_data, .-usb_ep_send_control_data
.global	get_string
	.type	get_string, @function
get_string:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	cpi r22,lo8(3)
	brne .L240
	movw r28,r24
	call usb_ep_clearSETUP
	ldd __tmp_reg__,Y+6
	ldd r29,Y+7
	mov r28,__tmp_reg__
	ldi r22,lo8(2)
	ldi r24,lo8(eeprom)
	ldi r25,hi8(eeprom)
	call usb_mem_read_byte
	mov r22,r24
	ldi r23,0
	movw r18,r28
	ldi r20,lo8(2)
	ldi r24,lo8(eeprom)
	ldi r25,hi8(eeprom)
	call usb_ep_send_control_data
	ldi r24,0
	rjmp .L239
.L240:
	ldi r24,lo8(1)
.L239:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	get_string, .-get_string
.global	usb_ep_recv_control_data_chunk
	.type	usb_ep_recv_control_data_chunk, @function
usb_ep_recv_control_data_chunk:
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 9 */
.L__stack_usage = 9
	movw r12,r24
	movw r10,r22
	movw r14,r20
	movw r30,r22
	std Z+1,__zero_reg__
	st Z,__zero_reg__
	or r20,r21
	breq .L249
	ldi r28,0
	ldi r29,0
.L243:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L250
	lds r24,232
	mov r17,r24
	andi r17,lo8(8)
	sbrc r24,3
	rjmp .L251
	lds r24,232
	sbrs r24,2
	rjmp .L243
.L244:
	lds r24,242
	tst r24
	breq .L245
	cp r28,r14
	cpc r29,r15
	brsh .L245
	lds r24,241
	movw r30,r12
	st Z,r24
	adiw r28,1
	movw r24,r12
	adiw r24,1
	movw r12,r24
	rjmp .L244
.L245:
	lds r24,242
	cpse r24,__zero_reg__
	rjmp .L247
	call usb_ep_clearOUT
.L247:
	cp r28,r14
	cpc r29,r15
	brlo .L243
	movw r30,r10
	std Z+1,r29
	st Z,r28
	rjmp .L242
.L249:
	ldi r17,0
	rjmp .L242
.L250:
	ldi r17,lo8(3)
	rjmp .L242
.L251:
	ldi r17,lo8(1)
.L242:
	mov r24,r17
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	ret
	.size	usb_ep_recv_control_data_chunk, .-usb_ep_recv_control_data_chunk
.global	usb_ep_recv_control_data_finish
	.type	usb_ep_recv_control_data_finish, @function
usb_ep_recv_control_data_finish:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call usb_ep_clearOUT
	jmp usb_ep_send_zlp
	.size	usb_ep_recv_control_data_finish, .-usb_ep_recv_control_data_finish
.global	usb_ep_recv_control_data
	.type	usb_ep_recv_control_data, @function
usb_ep_recv_control_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call usb_ep_recv_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L258
	jmp usb_ep_recv_control_data_finish
.L258:
	ret
	.size	usb_ep_recv_control_data, .-usb_ep_recv_control_data
.global	usb_ep_setup
	.type	usb_ep_setup, @function
usb_ep_setup:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	subi r20,lo8(-(-8))
	cpi r20,lo8(25)
	brsh .L261
	mov r30,r20
	ldi r31,0
	subi r30,lo8(-(CSWTCH.146))
	sbci r31,hi8(-(CSWTCH.146))
	ld r19,Z
	rjmp .L260
.L261:
	ldi r19,lo8(50)
.L260:
	mov r25,r24
	andi r25,lo8(7)
	sts 233,r25
	lds r25,235
	ori r25,lo8(1)
	sts 235,r25
	swap r22
	lsl r22
	lsl r22
	andi r22,lo8(-64)
	rol r24
	clr r24
	rol r24
	or r24,r22
	sts 236,r24
	mov r26,r18
	andi r26,1
	ldi r27,0
	lsl r26
	rol r27
	lsl r26
	rol r27
	or r26,r19
	sts 237,r26
	sts 240,__zero_reg__
	ret
	.size	usb_ep_setup, .-usb_ep_setup
.global	usb_ep_setup_control
	.type	usb_ep_setup_control, @function
usb_ep_setup_control:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_usb_dev)
	ldi r31,hi8(g_usb_dev)
	ldd r22,Z+12
	ldd r24,Z+2
	ldd r25,Z+3
	adiw r24,7
	call usb_mem_read_byte
	ldi r18,0
	mov r20,r24
	ldi r22,0
	ldi r24,0
	jmp usb_ep_setup
	.size	usb_ep_setup_control, .-usb_ep_setup_control
.global	usb_intr_endofreset
	.type	usb_intr_endofreset, @function
usb_intr_endofreset:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call usb_ep_setup_control
	ldi r30,lo8(g_usb_dev)
	ldi r31,hi8(g_usb_dev)
	ldi r24,lo8(4)
	st Z,r24
	std Z+1,__zero_reg__
	ret
	.size	usb_intr_endofreset, .-usb_intr_endofreset
.global	__vector_11
	.type	__vector_11, @function
__vector_11:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r17
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: Signal */
/* frame size = 2 */
/* stack size = 20 */
.L__stack_usage = 20
	lds r24,225
	std Y+2,r24
	lds r24,226
	std Y+1,r24
	lds r17,233
	ldd r24,Y+2
	ldd r24,Y+2
	ldd r24,Y+2
	ldd r24,Y+2
	sbrs r24,3
	rjmp .L265
	ldd r24,Y+1
	sbrc r24,3
	call usb_intr_endofreset
.L266:
	lds r24,225
	andi r24,lo8(-9)
	sts 225,r24
.L265:
	ldd r24,Y+2
	ldd r24,Y+2
	sts 225,__zero_reg__
	sts 233,r17
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r17
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_11, .-__vector_11
.global	usb_init
	.type	usb_init, @function
usb_init:
	push r12
	push r14
	push r15
	push r16
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r21,lo8(13)
	ldi r30,lo8(g_usb_dev)
	ldi r31,hi8(g_usb_dev)
	movw r26,r30
	0:
	st X+,__zero_reg__
	dec r21
	brne 0b
	sts g_usb_dev,__zero_reg__
	sts g_usb_dev+2+1,r25
	sts g_usb_dev+2,r24
	sts g_usb_dev+4+1,r23
	sts g_usb_dev+4,r22
	sts g_usb_dev+6,r20
	sts g_usb_dev+7+1,r19
	sts g_usb_dev+7,r18
	sts g_usb_dev+9,r16
	sts g_usb_dev+10+1,r15
	sts g_usb_dev+10,r14
	sts g_usb_dev+12,r12
	ldi r24,lo8(32)
	sts 216,r24
	sts 251,__zero_reg__
	ldi r24,lo8(4)
	out 0x29,r24
	ldi r24,lo8(6)
	out 0x29,r24
.L274:
	in __tmp_reg__,0x29
	sbrs __tmp_reg__,0
	rjmp .L274
	ldi r24,lo8(-96)
	sts 216,r24
	ldi r24,lo8(-128)
	sts 216,r24
	sts 226,__zero_reg__
	sts 225,__zero_reg__
	ldi r24,lo8(9)
	sts 226,r24
	sts 224,__zero_reg__
	ldi r24,lo8(4)
	sts g_usb_dev,r24
/* epilogue start */
	pop r16
	pop r15
	pop r14
	pop r12
	ret
	.size	usb_init, .-usb_init
.global	usb_event_set_configuration
	.type	usb_event_set_configuration, @function
usb_event_set_configuration:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ret
	.size	usb_event_set_configuration, .-usb_event_set_configuration
.global	usb_event_ep0_request
	.type	usb_event_ep0_request, @function
usb_event_ep0_request:
	push r15
	push r16
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	subi r28,72
	sbc r29,__zero_reg__
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 72 */
/* stack size = 77 */
.L__stack_usage = 77
	movw r30,r24
	ld r25,Z
	andi r25,lo8(127)
	cpi r25,lo8(65)
	breq .+2
	rjmp .L317
	movw r16,r30
	ldd r24,Z+1
	cpi r24,lo8(58)
	brne .+2
	rjmp .L279
	brsh .L280
	cpi r24,lo8(49)
	brne .+2
	rjmp .L281
	brsh .L282
	cpi r24,lo8(48)
	breq .L283
	rjmp .L317
.L282:
	cpi r24,lo8(50)
	brne .+2
	rjmp .L284
	cpi r24,lo8(51)
	brne .+2
	rjmp .L285
	rjmp .L317
.L280:
	cpi r24,lo8(61)
	brne .+2
	rjmp .L286
	brsh .L287
	cpi r24,lo8(59)
	brne .+2
	rjmp .L288
	cpi r24,lo8(60)
	breq .+2
	rjmp .L317
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(2)
	ldi r23,lo8(1)
	ldi r24,lo8(g_sboxnet+9)
	ldi r25,hi8(g_sboxnet+9)
	rjmp .L331
.L287:
	cpi r24,lo8(64)
	brne .+2
	rjmp .L290
	cpi r24,lo8(81)
	brne .+2
	rjmp .L291
	rjmp .L317
.L283:
	call usb_ep_clearSETUP
	in r15,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	movw r30,r16
	ldd r25,Z+2
	sbrs r25,7
	rjmp .L292
/* #APP */
 ;  287 "sboxnet2usb.c" 1
	wdr
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(24)
	sts 96,r24
	ldi r24,lo8(12)
	sts 96,r24
	rjmp .L293
.L292:
	lds r24,g_sboxnet_flags
	sbrs r25,0
	rjmp .L294
	movw r30,r16
	ldd r25,Z+4
	sts g_sboxnet_addr,r25
	sbrc r24,0
	rjmp .L295
	call sboxnet_init
	lds r24,g_sboxnet_flags
	ori r24,lo8(1)
	sts g_sboxnet_flags,r24
.L295:
	movw r30,r16
	ldd r25,Z+2
	lds r24,g_sboxnet+1
	sbrs r25,1
	rjmp .L296
	ori r24,lo8(1)
	rjmp .L328
.L296:
	andi r24,lo8(-2)
.L328:
	sts g_sboxnet+1,r24
	rjmp .L293
.L294:
	sbrs r24,0
	rjmp .L293
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	sts g_sboxnet,__zero_reg__
	ldi r25,lo8(4)
	sts 201,r25
	cbi 0xa,2
	cbi 0xa,3
	sbi 0xb,3
	cbi 0x1d,2
	out __SREG__,r24
	lds r24,g_sboxnet_flags
	andi r24,lo8(-2)
	sts g_sboxnet_flags,r24
.L293:
	out __SREG__,r15
	call usb_ep_send_zlp
	rjmp .L330
.L281:
	call usb_ep_clearSETUP
	call get_status
	std Y+1,r24
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	rjmp .L332
.L284:
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r16,Z+6
	ldd r17,Z+7
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L300
	call get_status
	adiw r28,71-63
	std Y+63,r24
	sbiw r28,71-63
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	movw r24,r28
	subi r24,-71
	sbci r25,-1
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L330
	subi r16,1
	sbc r17,__zero_reg__
	rjmp .L300
.L306:
	cpi r16,1
	ldi r31,1
	cpc r17,r31
	brsh .L318
	ldi r22,lo8(-1)
	add r22,r16
.L301:
	movw r24,r28
	adiw r24,2
	call sboxnet_receive_msg
	std Y+1,r24
	cp __zero_reg__,r24
	brge .L302
	sub r16,r24
	sbc r17,__zero_reg__
	sbrc r24,7
	inc r17
	subi r16,1
	sbc r17,__zero_reg__
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r25,lo8(3)
	sts g_timer_led_msg_read,r25
	sbi 0x5,6
	out __SREG__,r24
	ldd r22,Y+1
	clr r23
	sbrc r22,7
	com r23
	subi r22,-1
	sbci r23,-1
	ldi r20,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L303
.L300:
	cpi r16,2
	cpc r17,__zero_reg__
	brsh .L306
	rjmp .L307
.L318:
	ldi r22,lo8(69)
	rjmp .L301
.L302:
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data_chunk
.L303:
	cpse r24,__zero_reg__
	rjmp .L330
.L307:
	call usb_ep_send_control_data_finish
	rjmp .L330
.L285:
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r20,Z+6
	ldd r21,Z+7
	cpi r20,70
	cpc r21,__zero_reg__
	brlo .L308
	ldi r20,lo8(69)
	ldi r21,0
.L308:
	movw r22,r28
	subi r22,-71
	sbci r23,-1
	movw r24,r28
	adiw r24,1
	call usb_ep_recv_control_data
	adiw r28,71-62
	ldd r24,Y+62
	ldd r25,Y+63
	sbiw r28,71-62
	sbiw r24,5
	brlo .L309
	movw r24,r28
	adiw r24,1
	call sboxnet_can_send_msg
	tst r24
	breq .L310
	in r24,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	ldi r25,lo8(3)
	sts g_timer_led_msg_write,r25
	sbi 0x5,7
	out __SREG__,r24
	movw r24,r28
	adiw r24,1
	call sboxnet_send_msg
	tst r24
	brne .+2
	rjmp .L330
	lds r24,g_sboxnet_status
	ori r24,lo8(-128)
	rjmp .L329
.L310:
	lds r24,g_sboxnet_status
	ori r24,lo8(64)
	rjmp .L329
.L309:
	lds r24,g_sboxnet_status
	ori r24,lo8(32)
.L329:
	sts g_sboxnet_status,r24
	rjmp .L330
.L279:
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(38)
	ldi r23,0
	ldi r24,lo8(g_sboxnet+525)
	ldi r25,hi8(g_sboxnet+525)
	rjmp .L331
.L288:
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(9)
	ldi r23,0
	ldi r24,lo8(g_sboxnet)
	ldi r25,hi8(g_sboxnet)
	rjmp .L331
.L286:
	call usb_ep_clearSETUP
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(2)
	ldi r23,lo8(1)
	ldi r24,lo8(g_sboxnet+267)
	ldi r25,hi8(g_sboxnet+267)
	rjmp .L331
.L290:
	call usb_ep_clearSETUP
	ldi r24,lo8(__stack)
	ldi r25,hi8(__stack)
	subi r24,lo8(_end)
	sbci r25,hi8(_end)
	std Y+2,r25
	std Y+1,r24
	ldi r30,lo8(_end)
	ldi r31,hi8(_end)
.L312:
	ldi r24,hi8(__stack)
	cpi r30,lo8(__stack)
	cpc r31,r24
	brsh .L313
	ld r24,Z
	cpi r24,lo8(-91)
	brne .L313
	adiw r30,1
	rjmp .L312
.L313:
	subi r30,lo8(_end)
	sbci r31,hi8(_end)
	std Y+4,r31
	std Y+3,r30
	movw r30,r16
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(4)
	ldi r23,0
.L332:
	movw r24,r28
	adiw r24,1
.L331:
	call usb_ep_send_control_data
	rjmp .L330
.L291:
	call usb_ep_clearSETUP
	ldi r20,lo8(66)
	ldi r21,0
	ldi r22,lo8(-1)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call memset
	movw r30,r16
	ldd r20,Z+6
	ldd r21,Z+7
	cpi r20,65
	cpc r21,__zero_reg__
	brlo .L315
	ldi r20,lo8(64)
	ldi r21,0
.L315:
	movw r22,r28
	subi r22,-71
	sbci r23,-1
	movw r24,r28
	adiw r24,3
	call usb_ep_recv_control_data
	adiw r28,71-62
	ldd r20,Y+62
	ldd r21,Y+63
	sbiw r28,71-62
	ldi r24,lo8(2)
	add r24,r20
	std Y+1,r24
	ldi r24,lo8(3)
	std Y+2,r24
	subi r20,-2
	sbci r21,-1
	ldi r22,lo8(eeprom)
	ldi r23,hi8(eeprom)
	movw r24,r28
	adiw r24,1
	call __eeupd_block_m32u2
.L330:
	ldi r24,0
	rjmp .L278
.L317:
	ldi r24,lo8(1)
.L278:
/* epilogue start */
	subi r28,-72
	sbci r29,-1
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	ret
	.size	usb_event_ep0_request, .-usb_event_ep0_request
.global	usb_ep0_task
	.type	usb_ep0_task, @function
usb_ep0_task:
	push r12
	push r13
	push r15
	push r16
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 8 */
/* stack size = 15 */
.L__stack_usage = 15
	sts 233,__zero_reg__
	lds r24,232
	sbrs r24,3
	rjmp .L333
	ldi r22,lo8(8)
	movw r24,r28
	adiw r24,1
	call usb_ep_read_data
	ldd r24,Y+1
	mov r25,r24
	andi r25,lo8(96)
	breq .+2
	rjmp .L337
	ldd r25,Y+2
	cpi r25,lo8(6)
	breq .L338
	brsh .L339
	cpi r25,lo8(5)
	brne .+2
	rjmp .L340
	rjmp .L337
.L339:
	cpi r25,lo8(8)
	breq .L341
	cpi r25,lo8(9)
	brne .+2
	rjmp .L342
	rjmp .L337
.L341:
	cpi r24,lo8(-128)
	breq .+2
	rjmp .L337
	lds r24,g_usb_dev
	andi r24,lo8(24)
	brne .+2
	rjmp .L337
	call usb_ep_clearSETUP
	ldi r18,lo8(1)
	ldi r19,0
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	ldi r24,lo8(g_usb_dev+1)
	ldi r25,hi8(g_usb_dev+1)
	rjmp .L378
.L338:
	cpi r24,lo8(-128)
	breq .+2
	rjmp .L337
	ldd r16,Y+3
	ldd r17,Y+4
	mov r24,r17
	ldi r25,0
	cpi r24,2
	cpc r25,__zero_reg__
	breq .L344
	cpi r24,3
	cpc r25,__zero_reg__
	breq .L345
	sbiw r24,1
	breq .+2
	rjmp .L337
	call usb_ep_clearSETUP
	ldd r18,Y+7
	ldd r19,Y+8
	lds r20,g_usb_dev+12
	ldi r22,lo8(18)
	ldi r23,0
	lds r24,g_usb_dev+2
	lds r25,g_usb_dev+2+1
	rjmp .L378
.L344:
	cpse r16,__zero_reg__
	rjmp .L337
	call usb_ep_clearSETUP
	ldd r18,Y+7
	ldd r19,Y+8
	lds r20,g_usb_dev+12
	lds r22,g_usb_dev+6
	ldi r23,0
	lds r24,g_usb_dev+4
	lds r25,g_usb_dev+4+1
.L378:
	call usb_ep_send_control_data
	rjmp .L343
.L345:
	mov r15,r16
	lds r30,g_usb_dev+10
	lds r31,g_usb_dev+10+1
	sbiw r30,0
	brne .L347
.L349:
	lds r24,g_usb_dev+9
	cp r15,r24
	brlo .L348
	rjmp .L337
.L347:
	mov r22,r16
	movw r24,r28
	adiw r24,1
	icall
	cpse r24,__zero_reg__
	rjmp .L349
	rjmp .L343
.L348:
	lds r24,g_usb_dev+12
	lds r30,g_usb_dev+7
	lds r31,g_usb_dev+7+1
	clr r17
	cpi r24,lo8(1)
	breq .L351
	cpi r24,lo8(2)
	brne .L380
	movw r24,r16
	lsl r24
	rol r25
	add r24,r30
	adc r25,r31
	call __eerd_word_m32u2
	movw r16,r24
	rjmp .L353
.L351:
	lsl r16
	rol r17
	add r30,r16
	adc r31,r17
/* #APP */
 ;  368 "../avrutilslib/usb/usb.c" 1
	lpm r16, Z+
	lpm r17, Z
	
 ;  0 "" 2
/* #NOAPP */
	rjmp .L353
.L380:
	lsl r16
	rol r17
	add r30,r16
	adc r31,r17
	ld r16,Z
	ldd r17,Z+1
.L353:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .+2
	rjmp .L337
	call usb_ep_clearSETUP
	ldd r12,Y+7
	ldd r13,Y+8
	lds r15,g_usb_dev+12
	mov r22,r15
	movw r24,r16
	call usb_mem_read_byte
	mov r22,r24
	ldi r23,0
	movw r18,r12
	mov r20,r15
	movw r24,r16
	rjmp .L378
.L340:
	cpse r24,__zero_reg__
	rjmp .L337
	lds r24,g_usb_dev
	andi r24,lo8(12)
	brne .+2
	rjmp .L337
	ldd r17,Y+3
	andi r17,lo8(127)
	call usb_ep_clearSETUP
	sts 227,r17
	call usb_ep_send_zlp
	lds r24,227
	ori r24,lo8(-128)
	sts 227,r24
	tst r17
	breq .L354
	ldi r24,lo8(8)
	rjmp .L379
.L354:
	ldi r24,lo8(4)
.L379:
	sts g_usb_dev,r24
	rjmp .L343
.L342:
	cpse r24,__zero_reg__
	rjmp .L337
	lds r24,g_usb_dev
	andi r24,lo8(24)
	breq .L337
	ldd r17,Y+3
	cpse r17,__zero_reg__
	rjmp .L355
	ldi r24,lo8(8)
	sts g_usb_dev,r24
	sts g_usb_dev+1,__zero_reg__
	rjmp .L356
.L355:
	lds r22,g_usb_dev+12
	lds r24,g_usb_dev+4
	lds r25,g_usb_dev+4+1
	adiw r24,5
	call usb_mem_read_byte
	cpse r17,r24
	rjmp .L337
	ldi r24,lo8(16)
	sts g_usb_dev,r24
	sts g_usb_dev+1,r17
	sts 233,__zero_reg__
.L356:
	call usb_ep_clearSETUP
	call usb_ep_send_zlp
.L343:
	ldi r24,0
.L358:
	sts 233,__zero_reg__
	tst r24
	breq .L333
	call usb_ep_clearSETUP
	lds r24,235
	ori r24,lo8(32)
	sts 235,r24
	rjmp .L333
.L337:
	movw r24,r28
	adiw r24,1
	call usb_event_ep0_request
	rjmp .L358
.L333:
/* epilogue start */
	adiw r28,8
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r13
	pop r12
	ret
	.size	usb_ep0_task, .-usb_ep0_task
.global	__vector_18
	.type	__vector_18, @function
__vector_18:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r24
	push r25
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	lds r24,g_timer
	subi r24,lo8(-(1))
	sts g_timer,r24
	andi r24,lo8(15)
	brne .L382
	in r25,0x5
	ldi r24,lo8(32)
	eor r24,r25
	out 0x5,r24
.L382:
	lds r24,g_timer_led_msg_read
	tst r24
	breq .L384
	subi r24,lo8(-(-1))
	sts g_timer_led_msg_read,r24
	cpse r24,__zero_reg__
	rjmp .L384
	cbi 0x5,6
.L384:
	lds r24,g_timer_led_msg_write
	tst r24
	breq .L381
	subi r24,lo8(-(-1))
	sts g_timer_led_msg_write,r24
	cpse r24,__zero_reg__
	rjmp .L381
	cbi 0x5,7
.L381:
/* epilogue start */
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_18, .-__vector_18
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
/* #APP */
 ;  511 "sboxnet2usb.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	call init_system
	clr r12
	inc r12
	ldi r24,lo8(gs(get_string))
	mov r14,r24
	ldi r24,hi8(gs(get_string))
	mov r15,r24
	ldi r16,lo8(3)
	ldi r18,lo8(g_strings_P)
	ldi r19,hi8(g_strings_P)
	ldi r20,lo8(18)
	ldi r22,lo8(g_config_1_P)
	ldi r23,hi8(g_config_1_P)
	ldi r24,lo8(g_device_desc_P)
	ldi r25,hi8(g_device_desc_P)
	call usb_init
	in r24,0x33
	andi r24,lo8(-15)
	out 0x33,r24
	in r24,0x33
	ori r24,lo8(1)
	out 0x33,r24
/* #APP */
 ;  521 "sboxnet2usb.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L390:
	call usb_ep0_task
	rjmp .L390
	.size	main, .-main
	.section	.rodata
	.type	CSWTCH.146, @object
	.size	CSWTCH.146, 25
CSWTCH.146:
	.byte	2
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	18
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	50
	.byte	34
	.section	.progmem.data,"a",@progbits
	.type	epsizemap.2616, @object
	.size	epsizemap.2616, 4
epsizemap.2616:
	.byte	8
	.byte	16
	.byte	32
	.byte	64
.global	g_strings_P
	.type	g_strings_P, @object
	.size	g_strings_P, 6
g_strings_P:
	.word	g_string0_P
	.word	g_string1_P
	.word	g_string2_P
.global	g_string2_P
	.type	g_string2_P, @object
	.size	g_string2_P, 2
g_string2_P:
	.byte	24
	.byte	3
	.string	"s"
	.string	"b"
	.string	"o"
	.string	"x"
	.string	"n"
	.string	"e"
	.string	"t"
	.string	"2"
	.string	"u"
	.string	"s"
	.string	"b"
	.string	""
	.string	""
.global	g_string1_P
	.type	g_string1_P, @object
	.size	g_string1_P, 2
g_string1_P:
	.byte	68
	.byte	3
	.string	"T"
	.string	"h"
	.string	"o"
	.string	"m"
	.string	"a"
	.string	"s"
	.string	" "
	.string	"M"
	.string	"a"
	.string	"i"
	.string	"e"
	.string	"r"
	.string	" "
	.string	"<"
	.string	"b"
	.string	"a"
	.string	"l"
	.string	"a"
	.string	"g"
	.string	"i"
	.string	"@"
	.string	"j"
	.string	"u"
	.string	"s"
	.string	"t"
	.string	"m"
	.string	"a"
	.string	"i"
	.string	"l"
	.string	"."
	.string	"d"
	.string	"e"
	.string	">"
	.string	""
	.string	""
.global	g_string0_P
	.type	g_string0_P, @object
	.size	g_string0_P, 2
g_string0_P:
	.byte	4
	.byte	3
	.word	1033
.global	g_config_1_P
	.type	g_config_1_P, @object
	.size	g_config_1_P, 18
g_config_1_P:
	.byte	9
	.byte	2
	.word	18
	.byte	1
	.byte	1
	.byte	0
	.byte	-128
	.byte	25
	.byte	9
	.byte	4
	.byte	0
	.byte	0
	.byte	0
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	0
.global	g_device_desc_P
	.type	g_device_desc_P, @object
	.size	g_device_desc_P, 18
g_device_desc_P:
	.byte	18
	.byte	1
	.word	272
	.byte	-1
	.byte	-1
	.byte	-1
	.byte	64
	.word	-3805
	.word	-24574
	.word	257
	.byte	1
	.byte	2
	.byte	3
	.byte	1
.global	eeprom
	.section	.eeprom,"aw",@progbits
	.type	eeprom, @object
	.size	eeprom, 66
eeprom:
	.zero	66
	.comm	g_usb_dev,13,1
.global	g_timer_led_msg_write
	.section .bss
	.type	g_timer_led_msg_write, @object
	.size	g_timer_led_msg_write, 1
g_timer_led_msg_write:
	.zero	1
.global	g_timer_led_msg_read
	.type	g_timer_led_msg_read, @object
	.size	g_timer_led_msg_read, 1
g_timer_led_msg_read:
	.zero	1
.global	g_timer
	.type	g_timer, @object
	.size	g_timer, 1
g_timer:
	.zero	1
.global	g_sboxnet_status
	.type	g_sboxnet_status, @object
	.size	g_sboxnet_status, 1
g_sboxnet_status:
	.zero	1
.global	g_sboxnet_flags
	.type	g_sboxnet_flags, @object
	.size	g_sboxnet_flags, 1
g_sboxnet_flags:
	.zero	1
	.comm	g_sboxnet,563,1
	.comm	g_sboxnet_addr,1,1
	.ident	"GCC: (Gentoo 4.9.3 p1.3, pie-0.6.3) 4.9.3"
.global __do_copy_data
.global __do_clear_bss
