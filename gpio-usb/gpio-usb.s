	.file	"gpio-usb.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
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
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
/* #APP */
 ;  1601 "gpio-usb.c" 1
	wdr
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(24)
/* #APP */
 ;  1602 "gpio-usb.c" 1
	in __tmp_reg__, __SREG__
	cli
	sts 96, r24
	sts 96, __zero_reg__
	out __SREG__,__tmp_reg__
	
 ;  0 "" 2
/* #NOAPP */
	ldi r24,lo8(-1)
	out 0x5,r24
	out 0x8,r24
	out 0xb,r24
	out 0x4,__zero_reg__
	out 0x7,__zero_reg__
	out 0xa,__zero_reg__
	ldi r30,lo8(97)
	ldi r31,0
	ldi r25,lo8(-128)
	st Z,r25
	st Z,__zero_reg__
	sbi 0x4,5
	cbi 0x5,5
	lds r18,132
	lds r19,132+1
	subi r18,49
	sbci r19,-8
	sts 136+1,r19
	sts 136,r18
	sts 130,__zero_reg__
	sts 128,__zero_reg__
	ldi r25,lo8(2)
	sts 129,r25
	out 0x16,r24
	ldi r24,lo8(3)
	sts 111,r24
	ret
	.size	init_system, .-init_system
.global	usb_ep_read_data
	.type	usb_ep_read_data, @function
usb_ep_read_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.L6:
	tst r22
	breq .L9
	lds r18,241
	movw r30,r24
	st Z,r18
	subi r22,lo8(-(-1))
	adiw r24,1
	rjmp .L6
.L9:
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
.L11:
	lds r24,232
	sbrc r24,0
	rjmp .L15
	lds r24,g_usb_dev
	andi r24,lo8(28)
	brne .L11
	ldi r24,lo8(3)
	ret
.L15:
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
	rjmp .L17
	call usb_ep_clearIN
	jmp usb_ep_waitIN
.L17:
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
	breq .L20
	cpi r22,lo8(2)
	brne .L24
	jmp __eerd_byte_usb162
.L20:
	movw r30,r24
/* #APP */
 ;  139 "../avrutilslib/usb/usb.c" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	ret
.L24:
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
	subi r30,lo8(-(epsizemap.2209))
	sbci r31,hi8(-(epsizemap.2209))
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
.L29:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L34
	lds r24,232
	sbrc r24,3
	rjmp .L35
	lds r24,232
	mov r28,r24
	andi r28,lo8(4)
	sbrs r24,2
	rjmp .L28
	call usb_ep_clearOUT
	ldi r28,lo8(2)
	rjmp .L27
.L28:
	lds r24,232
	sbrs r24,0
	rjmp .L29
.L30:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L31
	lds r24,232
	sbrs r24,5
	rjmp .L31
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
	rjmp .L30
.L31:
	call usb_ep_clearIN
	lds r25,232
	andi r25,lo8(127)
	sts 232,r25
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .L29
	rjmp .L27
.L34:
	ldi r28,lo8(3)
	rjmp .L27
.L35:
	ldi r28,lo8(1)
.L27:
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
.L44:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L52
	lds r24,232
	sbrc r24,3
	rjmp .L53
	lds r24,232
	mov r29,r24
	andi r29,lo8(4)
	sbrs r24,2
	rjmp .L43
	call usb_ep_clearOUT
	ldi r29,lo8(2)
	rjmp .L55
.L43:
	lds r24,232
	sbrs r24,0
	rjmp .L44
	lds r28,242
.L45:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L46
	cp r28,r15
	brsh .L57
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
	rjmp .L45
.L52:
	ldi r29,lo8(3)
	rjmp .L55
.L53:
	ldi r29,lo8(1)
	rjmp .L55
.L46:
	cpse r28,r15
	rjmp .L55
.L50:
	call usb_ep_clearIN
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .L44
	rjmp .L55
.L57:
	cpse r28,r15
	rjmp .L44
	rjmp .L50
.L55:
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
.L63:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	brne .L59
.L65:
	ldi r28,lo8(3)
	rjmp .L60
.L59:
	lds r24,232
	sbrs r24,3
	rjmp .L61
.L66:
	ldi r28,lo8(1)
	rjmp .L60
.L61:
	lds r24,232
	mov r28,r24
	andi r28,lo8(4)
	sbrs r24,2
	rjmp .L62
	call usb_ep_clearOUT
	ldi r28,lo8(2)
	rjmp .L60
.L62:
	lds r24,232
	sbrs r24,0
	rjmp .L63
	call usb_ep_clearIN
.L64:
	lds r24,232
	sbrc r24,2
	rjmp .L72
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L65
	lds r24,232
	sbrs r24,3
	rjmp .L64
	rjmp .L66
.L72:
	call usb_ep_clearOUT
.L60:
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
	brsh .L74
	movw r18,r22
.L74:
	movw r22,r18
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L75
	jmp usb_ep_send_control_data_finish
.L75:
	ret
	.size	usb_ep_send_control_data, .-usb_ep_send_control_data
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
	breq .L84
	ldi r28,0
	ldi r29,0
.L78:
	lds r24,g_usb_dev
	andi r24,lo8(28)
	breq .L85
	lds r24,232
	mov r17,r24
	andi r17,lo8(8)
	sbrc r24,3
	rjmp .L86
	lds r24,232
	sbrs r24,2
	rjmp .L78
.L79:
	lds r24,242
	tst r24
	breq .L80
	cp r28,r14
	cpc r29,r15
	brsh .L80
	lds r24,241
	movw r30,r12
	st Z,r24
	adiw r28,1
	movw r24,r12
	adiw r24,1
	movw r12,r24
	rjmp .L79
.L80:
	lds r24,242
	cpse r24,__zero_reg__
	rjmp .L82
	call usb_ep_clearOUT
.L82:
	cp r28,r14
	cpc r29,r15
	brlo .L78
	movw r30,r10
	std Z+1,r29
	st Z,r28
	rjmp .L77
.L84:
	ldi r17,0
	rjmp .L77
.L85:
	ldi r17,lo8(3)
	rjmp .L77
.L86:
	ldi r17,lo8(1)
.L77:
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
	rjmp .L93
	jmp usb_ep_recv_control_data_finish
.L93:
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
	brsh .L96
	mov r30,r20
	ldi r31,0
	subi r30,lo8(-(CSWTCH.133))
	sbci r31,hi8(-(CSWTCH.133))
	ld r19,Z
	rjmp .L95
.L96:
	ldi r19,lo8(50)
.L95:
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
	ldd r22,Z+10
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
	rjmp .L100
	ldd r24,Y+1
	sbrc r24,3
	call usb_intr_endofreset
.L101:
	lds r24,225
	andi r24,lo8(-9)
	sts 225,r24
.L100:
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
	push r14
	push r16
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	ldi r21,lo8(11)
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
	sts g_usb_dev+10,r14
	ldi r24,lo8(32)
	sts 216,r24
	sts 251,__zero_reg__
	out 0x29,__zero_reg__
	ldi r24,lo8(2)
	out 0x29,r24
.L109:
	in __tmp_reg__,0x29
	sbrs __tmp_reg__,0
	rjmp .L109
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
	pop r14
	ret
	.size	usb_init, .-usb_init
.global	__vector_15
	.type	__vector_15, @function
__vector_15:
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
	lds r24,136
	lds r25,136+1
	subi r24,49
	sbci r25,-8
	sts 136+1,r25
	sts 136,r24
	lds r24,g_avr_timer
	tst r24
	breq .L112
	lds r24,g_avr_timer
	subi r24,lo8(-(-1))
	sts g_avr_timer,r24
.L112:
	lds r24,g_pdi_timer
	tst r24
	breq .L113
	lds r24,g_pdi_timer
	subi r24,lo8(-(-1))
	sts g_pdi_timer,r24
.L113:
	lds r24,g_nvmc_timer
	tst r24
	breq .L111
	lds r24,g_nvmc_timer
	subi r24,lo8(-(-1))
	sts g_nvmc_timer,r24
.L111:
/* epilogue start */
	pop r25
	pop r24
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_15, .-__vector_15
.global	avr_clkwait
	.type	avr_clkwait, @function
avr_clkwait:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,g_avr_clk_wait
.L125:
	ldi r25,lo8(13)
	1: dec r25
	brne 1b
	nop
	ldi r25,lo8(-1)
	add r25,r24
	tst r24
	breq .L127
	mov r24,r25
	rjmp .L125
.L127:
/* epilogue start */
	ret
	.size	avr_clkwait, .-avr_clkwait
.global	avr_sendbyte
	.type	avr_sendbyte, @function
avr_sendbyte:
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 3 */
.L__stack_usage = 3
	mov r17,r24
	lds r24,g_avr_flags
	mov r28,r24
	andi r28,lo8(2)
	sbrs r24,1
	rjmp .L129
	cbi 0x5,1
	ldi r29,0
	ldi r28,0
.L133:
	sbrs r17,7
	rjmp .L130
	sbi 0x5,2
	rjmp .L131
.L130:
	cbi 0x5,2
.L131:
	lsl r17
	call avr_clkwait
	lsl r28
	sbic 0x3,3
	ori r28,lo8(1)
.L132:
	sbi 0x5,1
	call avr_clkwait
	cbi 0x5,1
	subi r29,lo8(-(1))
	cpi r29,lo8(8)
	brne .L133
.L129:
	mov r24,r28
/* epilogue start */
	pop r29
	pop r28
	pop r17
	ret
	.size	avr_sendbyte, .-avr_sendbyte
.global	cmd_avr_init
	.type	cmd_avr_init, @function
cmd_avr_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x4
	andi r24,lo8(-32)
	out 0x4,r24
	in r24,0x5
	andi r24,lo8(-16)
	out 0x5,r24
	sbi 0x5,4
	in r24,0x4
	ori r24,lo8(23)
	out 0x4,r24
	lds r24,g_avr_flags
	andi r24,lo8(-15)
	ori r24,lo8(1)
	sts g_avr_flags,r24
	jmp usb_ep_send_zlp
	.size	cmd_avr_init, .-cmd_avr_init
.global	cmd_avr_exit
	.type	cmd_avr_exit, @function
cmd_avr_exit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x4
	andi r24,lo8(-32)
	out 0x4,r24
	in r24,0x5
	ori r24,lo8(31)
	out 0x5,r24
	lds r24,g_avr_flags
	andi r24,lo8(-16)
	sts g_avr_flags,r24
	jmp usb_ep_send_zlp
	.size	cmd_avr_exit, .-cmd_avr_exit
.global	cmd_avr_progmode
	.type	cmd_avr_progmode, @function
cmd_avr_progmode:
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 3 */
.L__stack_usage = 3
	std Y+1,r24
	movw r18,r20
	lds r24,g_avr_flags
	sbrs r24,0
	rjmp .L143
	ldd r24,Y+1
	sbrs r24,1
	rjmp .L144
	sts g_avr_clk_wait,r22
	lds r24,g_avr_flags
	mov r25,r24
	andi r25,lo8(-15)
	ldd r24,Y+1
	andi r24,lo8(14)
	or r24,r25
	sts g_avr_flags,r24
	in r24,0x5
	andi r24,lo8(-8)
	out 0x5,r24
	sbi 0x5,4
	ldi r24,lo8(26)
	1: dec r24
	brne 1b
	rjmp .
	cbi 0x5,4
	rjmp .L145
.L144:
	sbi 0x5,4
	in r24,0x5
	andi r24,lo8(-8)
	out 0x5,r24
.L143:
	lds r24,g_avr_flags
	andi r24,lo8(-15)
	sts g_avr_flags,r24
	sts g_avr_clk_wait,__zero_reg__
.L145:
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	ldi r24,lo8(g_avr_flags)
	ldi r25,hi8(g_avr_flags)
	call usb_ep_send_control_data
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	cmd_avr_progmode, .-cmd_avr_progmode
.global	cmd_avr_rawcmd
	.type	cmd_avr_rawcmd, @function
cmd_avr_rawcmd:
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 10 */
.L__stack_usage = 10
	mov r13,r24
	mov r15,r22
	mov r14,r23
	movw r16,r20
	mov r24,r25
	call avr_sendbyte
	mov r24,r13
	call avr_sendbyte
	std Y+1,r24
	mov r24,r14
	call avr_sendbyte
	std Y+2,r24
	mov r24,r15
	call avr_sendbyte
	std Y+3,r24
	movw r18,r16
	ldi r20,0
	ldi r22,lo8(3)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	ret
	.size	cmd_avr_rawcmd, .-cmd_avr_rawcmd
.global	avr_sendcmd
	.type	avr_sendcmd, @function
avr_sendcmd:
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 5 */
.L__stack_usage = 5
	mov r29,r24
	mov r28,r22
	mov r17,r20
	mov r15,r18
	call avr_sendbyte
	mov r24,r28
	call avr_sendbyte
	mov r16,r24
	mov r24,r17
	call avr_sendbyte
	mov r17,r24
	mov r24,r15
	call avr_sendbyte
	cpse r29,r16
	rjmp .L151
	cp r28,r17
	breq .L152
.L151:
	lds r25,g_avr_error
	ori r25,lo8(1)
	sts g_avr_error,r25
.L152:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	ret
	.size	avr_sendcmd, .-avr_sendcmd
.global	cmd_avr_readflash
	.type	cmd_avr_readflash, @function
cmd_avr_readflash:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 8 */
.L__stack_usage = 8
	movw r16,r24
	movw r14,r22
.L154:
	cp r14,__zero_reg__
	cpc r15,__zero_reg__
	breq .L158
	ldi r18,0
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(32)
	call avr_sendcmd
	std Y+1,r24
	ldi r18,0
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(40)
	call avr_sendcmd
	std Y+2,r24
	ldi r20,0
	ldi r22,lo8(2)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L153
	subi r16,-1
	sbci r17,-1
	ldi r24,2
	sub r14,r24
	sbc r15,__zero_reg__
	rjmp .L154
.L158:
	call usb_ep_send_control_data_finish
.L153:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	cmd_avr_readflash, .-cmd_avr_readflash
.global	cmd_avr_readeeprom
	.type	cmd_avr_readeeprom, @function
cmd_avr_readeeprom:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 7 */
.L__stack_usage = 7
	movw r16,r24
	movw r14,r22
.L160:
	cp r14,__zero_reg__
	cpc r15,__zero_reg__
	breq .L164
	ldi r18,0
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(-96)
	call avr_sendcmd
	std Y+1,r24
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L159
	subi r16,-1
	sbci r17,-1
	ldi r24,1
	sub r14,r24
	sbc r15,__zero_reg__
	rjmp .L160
.L164:
	call usb_ep_send_control_data_finish
.L159:
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	cmd_avr_readeeprom, .-cmd_avr_readeeprom
.global	avr_poll_and_return
	.type	avr_poll_and_return, @function
avr_poll_and_return:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,g_avr_flags
	andi r24,lo8(12)
	cpi r24,lo8(4)
	breq .L167
	cpi r24,lo8(8)
	breq .L168
	cpse r24,__zero_reg__
	rjmp .L177
	ldi r24,lo8(12)
	sts g_avr_timer,r24
.L172:
	ldi r18,0
	ldi r20,0
	ldi r22,0
	ldi r24,lo8(-16)
	call avr_sendcmd
	sbrs r24,0
	rjmp .L165
	lds r24,g_avr_timer
	cpse r24,__zero_reg__
	rjmp .L172
	lds r24,g_avr_error
	ori r24,lo8(2)
	sts g_avr_error,r24
	ret
.L167:
	ldi r24,lo8(5)
	rjmp .L178
.L168:
	ldi r24,lo8(9)
	rjmp .L178
.L177:
	ldi r24,lo8(33)
.L178:
	sts g_avr_timer,r24
.L173:
	lds r24,g_avr_timer
	tst r24
	breq .L165
/* #APP */
 ;  370 "gpio-usb.c" 1
	sleep
	
 ;  0 "" 2
/* #NOAPP */
	rjmp .L173
.L165:
	ret
	.size	avr_poll_and_return, .-avr_poll_and_return
.global	cmd_avr_writeflashpage
	.type	cmd_avr_writeflashpage, @function
cmd_avr_writeflashpage:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 4 */
/* stack size = 12 */
.L__stack_usage = 12
	movw r12,r24
	movw r14,r22
	ldi r16,0
	ldi r17,0
.L180:
	ldi r24,2
	cp r14,r24
	cpc r15,__zero_reg__
	brlo .L184
	ldi r20,lo8(2)
	ldi r21,0
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	movw r24,r28
	adiw r24,3
	call usb_ep_recv_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L179
	ldd r18,Y+3
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(64)
	call avr_sendcmd
	ldd r18,Y+4
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(72)
	call avr_sendcmd
	ldi r24,2
	sub r14,r24
	sbc r15,__zero_reg__
	subi r16,-1
	sbci r17,-1
	rjmp .L180
.L184:
	ldi r18,0
	mov r20,r12
	mov r22,r13
	ldi r24,lo8(76)
	call avr_sendcmd
	call avr_poll_and_return
	call usb_ep_recv_control_data_finish
.L179:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	cmd_avr_writeflashpage, .-cmd_avr_writeflashpage
.global	cmd_avr_writeeeprompage
	.type	cmd_avr_writeeeprompage, @function
cmd_avr_writeeeprompage:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 11 */
.L__stack_usage = 11
	movw r12,r24
	movw r14,r22
	ldi r16,0
	ldi r17,0
.L186:
	cp r14,__zero_reg__
	cpc r15,__zero_reg__
	breq .L190
	ldi r20,lo8(1)
	ldi r21,0
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	movw r24,r28
	adiw r24,3
	call usb_ep_recv_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L185
	ldd r18,Y+3
	mov r20,r16
	mov r22,r17
	ldi r24,lo8(-63)
	call avr_sendcmd
	ldi r24,1
	sub r14,r24
	sbc r15,__zero_reg__
	subi r16,-1
	sbci r17,-1
	rjmp .L186
.L190:
	ldi r18,0
	mov r20,r12
	mov r22,r13
	ldi r24,lo8(-62)
	call avr_sendcmd
	call avr_poll_and_return
	call usb_ep_recv_control_data_finish
.L185:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	cmd_avr_writeeeprompage, .-cmd_avr_writeeeprompage
.global	pdi_off
	.type	pdi_off, @function
pdi_off:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sts 200,__zero_reg__
	sts 201,__zero_reg__
	sts 202,__zero_reg__
	sts 203,__zero_reg__
	cbi 0xa,2
	cbi 0xb,2
	sbi 0xa,3
	cbi 0xb,3
	sbi 0xa,5
	sbi 0xb,5
	ret
	.size	pdi_off, .-pdi_off
.global	pdi_enter
	.type	pdi_enter, @function
pdi_enter:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	jmp pdi_off
	.size	pdi_enter, .-pdi_enter
.global	pdi_leave
	.type	pdi_leave, @function
pdi_leave:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sts 200,__zero_reg__
	sts 201,__zero_reg__
	sts 202,__zero_reg__
	sts 203,__zero_reg__
	cbi 0xa,5
	sbi 0xb,5
	cbi 0xa,2
	sbi 0xb,2
	cbi 0xa,3
	sbi 0xb,3
	ret
	.size	pdi_leave, .-pdi_leave
.global	pdi_start
	.type	pdi_start, @function
pdi_start:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	call pdi_off
	sbi 0xb,3
	ldi r24,lo8(-123)
	1: dec r24
	brne 1b
	nop
	ldi r24,lo8(39)
	ldi r25,0
	sts 204+1,r25
	sts 204,r24
	sts 200,__zero_reg__
	ldi r30,lo8(-55)
	ldi r31,0
	st Z,__zero_reg__
	ldi r24,lo8(111)
	sts 202,r24
	sts 203,__zero_reg__
	ld r24,Z
	ori r24,lo8(16)
	st Z,r24
	ldi r24,lo8(399)
	ldi r25,hi8(399)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	pdi_start, .-pdi_start
.global	pdi_write
	.type	pdi_write, @function
pdi_write:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 9 */
.L__stack_usage = 9
	movw r16,r24
	movw r14,r22
.L196:
	lds r24,200
	sbrs r24,7
	rjmp .L223
	lds r24,206
	rjmp .L196
.L223:
	lds r24,201
	ori r24,lo8(8)
	sts 201,r24
.L198:
	lds r24,200
	sbrs r24,5
	rjmp .L198
	lds r24,200
	ori r24,lo8(64)
	sts 200,r24
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L199
	movw r30,r16
	ld r24,Z
	std Y+3,r24
	subi r16,-1
	sbci r17,-1
	rjmp .L200
.L199:
	ldi r20,lo8(1)
	ldi r21,0
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	movw r24,r28
	adiw r24,3
	call usb_ep_recv_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L201
	ldd r24,Y+1
	ldd r25,Y+2
	sbiw r24,1
	breq .L200
.L201:
	ldi r24,lo8(21)
	rjmp .L202
.L200:
	ldd r24,Y+3
	sts 206,r24
.L203:
	lds r24,200
	sbrs r24,6
	rjmp .L203
	lds r24,200
	ori r24,lo8(64)
	sts 200,r24
	ldi r24,lo8(5)
	sts g_pdi_timer,r24
.L204:
	lds r24,200
	sbrc r24,7
	rjmp .L224
	lds r24,g_pdi_timer
	cpse r24,__zero_reg__
	rjmp .L204
	ldi r24,lo8(10)
.L202:
	lds r25,201
	andi r25,lo8(-9)
	sts 201,r25
	cbi 0xa,3
	cbi 0xb,3
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
.L224:
	lds r24,200
	lds r18,206
	ldd r25,Y+3
	cpse r18,r25
	rjmp .L214
	andi r24,lo8(28)
	brne .L214
	cp r14,__zero_reg__
	cpc r15,__zero_reg__
	breq .L225
	ldi r31,1
	sub r14,r31
	sbc r15,__zero_reg__
	rjmp .L198
.L225:
	ldi r24,0
	rjmp .L202
.L214:
	ldi r24,lo8(4)
	rjmp .L202
	.size	pdi_write, .-pdi_write
.global	pdi_read
	.type	pdi_read, @function
pdi_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r30,r24
	ldi r24,lo8(5)
	sts g_pdi_timer,r24
.L227:
	lds r24,200
	sbrc r24,7
	rjmp .L232
	lds r24,g_pdi_timer
	cpse r24,__zero_reg__
	rjmp .L227
	ldi r24,lo8(11)
	ret
.L232:
	lds r25,200
	lds r18,206
	mov r24,r25
	andi r24,lo8(28)
	brne .L231
	st Z,r18
	ret
.L231:
	ldi r24,lo8(5)
	ret
	.size	pdi_read, .-pdi_read
.global	pdi_cmd
	.type	pdi_cmd, @function
pdi_cmd:
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 7 */
.L__stack_usage = 7
	mov r29,r22
	tst r22
	brne .+2
	rjmp .L250
	movw r30,r24
	ld r13,Z
	cp r18,__zero_reg__
	cpc r19,__zero_reg__
	breq .L235
	movw r30,r18
	st Z,__zero_reg__
.L235:
	mov r28,r13
	andi r28,lo8(-32)
	cpi r28,lo8(96)
	brne .+2
	rjmp .L236
	movw r16,r18
	movw r14,r20
	movw r30,r24
	cpi r28,lo8(97)
	brsh .L237
	cpi r28,lo8(32)
	brne .+2
	rjmp .L238
	cpi r28,lo8(64)
	brne .+2
	rjmp .L239
	cpse r28,__zero_reg__
	rjmp .L251
	rjmp .L277
.L237:
	cpi r28,lo8(-96)
	brne .+2
	rjmp .L241
	brsh .L242
	cpi r28,lo8(-128)
	brne .+2
	rjmp .L243
	rjmp .L251
.L242:
	cpi r28,lo8(-64)
	brne .+2
	rjmp .L244
	cpi r28,lo8(-32)
	brne .+2
	rjmp .L245
	rjmp .L251
.L277:
	mov r18,r13
	andi r18,lo8(12)
	lsr r18
	lsr r18
	subi r18,lo8(-(1))
	ldi r19,0
	movw r24,r18
	adiw r24,1
	mov r22,r29
	ldi r23,0
	cp r24,r22
	cpc r25,r23
	breq .+2
	rjmp .L265
	movw r22,r18
	movw r24,r30
	call pdi_write
	cpse r24,__zero_reg__
	rjmp .L262
	mov r22,r13
	andi r22,lo8(3)
	clr r13
	inc r13
	add r13,r22
	ldi r29,0
.L246:
	movw r24,r14
	add r24,r29
	adc r25,__zero_reg__
	call pdi_read
	cpse r24,__zero_reg__
	rjmp .L262
	subi r29,lo8(-(1))
	cp r29,r13
	brlo .L246
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .+2
	rjmp .L272
	movw r30,r16
	st Z,r13
	rjmp .L234
.L239:
	mov r24,r13
	andi r24,lo8(12)
	lsr r24
	lsr r24
	subi r24,lo8(-(1))
	mov r22,r13
	andi r22,lo8(3)
	subi r22,lo8(-(1))
	ldi r23,0
	add r22,r24
	adc r23,__zero_reg__
	rjmp .L276
.L238:
	cpi r29,lo8(1)
	breq .+2
	rjmp .L265
	ldi r22,0
	ldi r23,0
	call pdi_write
	cpse r24,__zero_reg__
	rjmp .L262
	mov r22,r13
	andi r22,lo8(3)
	clr r13
	inc r13
	add r13,r22
	ldi r29,0
.L248:
	movw r24,r14
	add r24,r29
	adc r25,__zero_reg__
	call pdi_read
	cpse r24,__zero_reg__
	rjmp .L262
	subi r29,lo8(-(1))
	cp r29,r13
	brlo .L248
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L272
	movw r30,r16
	st Z,r13
	rjmp .L272
.L236:
	mov r22,r13
	andi r22,lo8(3)
	clr r13
	inc r13
	add r13,r22
	mov r18,r13
	ldi r19,0
	movw r20,r18
	subi r20,-1
	sbci r21,-1
	mov r22,r29
	ldi r23,0
	cp r20,r22
	cpc r21,r23
	brne .L265
	movw r22,r18
	rjmp .L274
.L243:
	cpi r29,lo8(1)
	brne .L265
	ldi r22,0
	ldi r23,0
	call pdi_write
	cpse r24,__zero_reg__
	rjmp .L262
	movw r24,r14
	call pdi_read
	cpse r24,__zero_reg__
	rjmp .L262
	movw r30,r16
	st Z,r29
.L272:
	ldi r28,0
	rjmp .L234
.L244:
	cpi r29,lo8(2)
	brne .L265
	ldi r22,lo8(1)
	ldi r23,0
	rjmp .L273
.L241:
	mov r22,r13
	andi r22,lo8(3)
	ldi r25,lo8(1)
	add r25,r22
	mov r22,r25
	ldi r23,0
.L276:
	movw r18,r22
	subi r18,-1
	sbci r19,-1
	mov r24,r29
	ldi r25,0
	cp r18,r24
	cpc r19,r25
	brne .L265
	rjmp .L273
.L245:
	cpi r29,lo8(9)
	brne .L265
	ldi r22,lo8(8)
	ldi r23,0
.L273:
	movw r24,r30
.L274:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	jmp pdi_write
.L250:
	ldi r28,lo8(1)
	rjmp .L234
.L251:
	ldi r28,lo8(2)
	rjmp .L234
.L262:
	mov r28,r24
	rjmp .L234
.L265:
	ldi r28,lo8(3)
.L234:
	mov r24,r28
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	pop r13
	ret
	.size	pdi_cmd, .-pdi_cmd
.global	pdi_cmd_lds_byte
	.type	pdi_cmd_lds_byte, @function
pdi_cmd_lds_byte:
	push r28
	push r29
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
/* stack size = 8 */
.L__stack_usage = 8
	ldi r18,lo8(12)
	std Y+1,r18
	std Y+2,r22
	std Y+3,r23
	std Y+4,r24
	std Y+5,r25
	ldi r18,0
	ldi r19,0
	ldi r22,lo8(5)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	pdi_cmd_lds_byte, .-pdi_cmd_lds_byte
.global	pdi_cmd_sts_byte
	.type	pdi_cmd_sts_byte, @function
pdi_cmd_sts_byte:
	push r28
	push r29
	rcall .
	rcall .
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 6 */
/* stack size = 8 */
.L__stack_usage = 8
	ldi r18,lo8(76)
	std Y+1,r18
	std Y+2,r22
	std Y+3,r23
	std Y+4,r24
	std Y+5,r25
	std Y+6,r20
	ldi r18,0
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(6)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	adiw r28,6
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	pdi_cmd_sts_byte, .-pdi_cmd_sts_byte
.global	pdi_cmd_st_ptr
	.type	pdi_cmd_st_ptr, @function
pdi_cmd_st_ptr:
	push r28
	push r29
	rcall .
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 5 */
/* stack size = 7 */
.L__stack_usage = 7
	ldi r18,lo8(107)
	std Y+1,r18
	std Y+2,r22
	std Y+3,r23
	std Y+4,r24
	std Y+5,r25
	ldi r18,0
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(5)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	pdi_cmd_st_ptr, .-pdi_cmd_st_ptr
.global	pdi_cmd_stcs
	.type	pdi_cmd_stcs, @function
pdi_cmd_stcs:
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 4 */
.L__stack_usage = 4
	andi r24,lo8(15)
	ori r24,lo8(-64)
	std Y+1,r24
	std Y+2,r22
	ldi r18,0
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(2)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	pdi_cmd_stcs, .-pdi_cmd_stcs
.global	pdi_cmd_ldcs
	.type	pdi_cmd_ldcs, @function
pdi_cmd_ldcs:
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r20,r22
	andi r24,lo8(15)
	ori r24,lo8(-128)
	std Y+1,r24
	ldi r18,0
	ldi r19,0
	ldi r22,lo8(1)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	pdi_cmd_ldcs, .-pdi_cmd_ldcs
.global	pdi_cmd_key_nvm
	.type	pdi_cmd_key_nvm, @function
pdi_cmd_key_nvm:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 9 */
/* stack size = 11 */
.L__stack_usage = 11
	ldi r24,lo8(-32)
	std Y+1,r24
	ldi r24,lo8(-1)
	std Y+2,r24
	ldi r24,lo8(-120)
	std Y+3,r24
	ldi r24,lo8(-40)
	std Y+4,r24
	ldi r24,lo8(-51)
	std Y+5,r24
	ldi r24,lo8(69)
	std Y+6,r24
	ldi r24,lo8(-85)
	std Y+7,r24
	ldi r24,lo8(-119)
	std Y+8,r24
	ldi r24,lo8(18)
	std Y+9,r24
	ldi r18,0
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(9)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	adiw r28,9
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
	pop r29
	pop r28
	ret
	.size	pdi_cmd_key_nvm, .-pdi_cmd_key_nvm
.global	pdi_cmd_repeat
	.type	pdi_cmd_repeat, @function
pdi_cmd_repeat:
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 5 */
.L__stack_usage = 5
	ldi r18,lo8(-95)
	std Y+1,r18
	std Y+2,r24
	std Y+3,r25
	ldi r18,0
	ldi r19,0
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(3)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	pdi_cmd_repeat, .-pdi_cmd_repeat
.global	pdi_cmd_load
	.type	pdi_cmd_load, @function
pdi_cmd_load:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 8 */
.L__stack_usage = 8
	movw r14,r20
	movw r16,r18
	call pdi_cmd_st_ptr
	cpse r24,__zero_reg__
	rjmp .L297
	movw r24,r16
	call pdi_cmd_repeat
	cpse r24,__zero_reg__
	rjmp .L297
	ldi r24,lo8(36)
	std Y+1,r24
	ldi r22,0
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call pdi_write
	cpse r24,__zero_reg__
	rjmp .L297
.L291:
	cp r14,__zero_reg__
	cpc r15,__zero_reg__
	breq .L287
	movw r24,r14
	call pdi_read
	ldi r25,-1
	sub r14,r25
	sbc r15,r25
	tst r24
	breq .L289
	rjmp .L297
.L287:
	movw r24,r28
	adiw r24,1
	call pdi_read
	cpse r24,__zero_reg__
	rjmp .L297
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data_chunk
	cpse r24,__zero_reg__
	rjmp .L295
.L289:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L296
	subi r16,1
	sbc r17,__zero_reg__
	rjmp .L291
.L296:
	ldi r24,0
	rjmp .L297
.L295:
	ldi r24,lo8(20)
.L297:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	pdi_cmd_load, .-pdi_cmd_load
.global	pdi_cmd_store
	.type	pdi_cmd_store, @function
pdi_cmd_store:
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 8 */
.L__stack_usage = 8
	movw r14,r20
	movw r16,r18
	call pdi_cmd_st_ptr
	cpse r24,__zero_reg__
	rjmp .L299
	movw r24,r16
	call pdi_cmd_repeat
	cpse r24,__zero_reg__
	rjmp .L299
	ldi r24,lo8(100)
	std Y+1,r24
	ldi r22,0
	ldi r23,0
	movw r24,r28
	adiw r24,1
	call pdi_write
	cpse r24,__zero_reg__
	rjmp .L299
	movw r22,r16
	movw r24,r14
	call pdi_write
.L299:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r16
	pop r15
	pop r14
	ret
	.size	pdi_cmd_store, .-pdi_cmd_store
.global	nvmc_init
	.type	nvmc_init, @function
nvmc_init:
	push r17
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 4 */
.L__stack_usage = 4
	ldi r22,lo8(89)
	ldi r24,lo8(1)
	call pdi_cmd_stcs
	mov r17,r24
	cpse r24,__zero_reg__
	rjmp .L304
	ldi r24,lo8(26)
	1: dec r24
	brne 1b
	rjmp .
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	ldi r24,lo8(1)
	call pdi_cmd_ldcs
	mov r17,r24
	cpse r24,__zero_reg__
	rjmp .L304
	ldd r24,Y+1
	sbrs r24,0
	rjmp .L307
	call pdi_cmd_key_nvm
	mov r17,r24
	cpse r24,__zero_reg__
	rjmp .L304
	ldi r24,lo8(1999)
	ldi r25,hi8(1999)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(100)
	sts g_nvmc_timer,r24
.L306:
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	ldi r24,0
	call pdi_cmd_ldcs
	mov r17,r24
	cpse r24,__zero_reg__
	rjmp .L304
	ldd r24,Y+1
	sbrc r24,1
	rjmp .L308
	lds r24,g_nvmc_timer
	cpse r24,__zero_reg__
	rjmp .L306
	ldi r17,lo8(12)
	rjmp .L304
.L307:
	ldi r17,lo8(99)
.L304:
	call pdi_off
	mov r24,r17
	rjmp .L305
.L308:
	ldi r24,0
.L305:
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	ret
	.size	nvmc_init, .-nvmc_init
.global	nvmc_exit
	.type	nvmc_exit, @function
nvmc_exit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,0
	ldi r24,0
	call pdi_cmd_stcs
	ldi r22,0
	ldi r24,lo8(1)
	jmp pdi_cmd_stcs
	.size	nvmc_exit, .-nvmc_exit
.global	nvmc_set_cmd
	.type	nvmc_set_cmd, @function
nvmc_set_cmd:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	mov r20,r24
	ldi r22,lo8(-54)
	ldi r23,lo8(1)
	ldi r24,0
	ldi r25,lo8(1)
	jmp pdi_cmd_sts_byte
	.size	nvmc_set_cmd, .-nvmc_set_cmd
.global	nvmc_trigger_cmdex
	.type	nvmc_trigger_cmdex, @function
nvmc_trigger_cmdex:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r20,lo8(1)
	ldi r22,lo8(-53)
	ldi r23,lo8(1)
	ldi r24,0
	ldi r25,lo8(1)
	jmp pdi_cmd_sts_byte
	.size	nvmc_trigger_cmdex, .-nvmc_trigger_cmdex
.global	nvmc_nvmbusy_wait
	.type	nvmc_nvmbusy_wait, @function
nvmc_nvmbusy_wait:
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 3 */
.L__stack_usage = 3
	ldi r24,lo8(10)
	sts g_nvmc_timer,r24
.L314:
	ldi r24,lo8(199)
	ldi r25,hi8(199)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	movw r20,r28
	subi r20,-1
	sbci r21,-1
	ldi r22,lo8(-49)
	ldi r23,lo8(1)
	ldi r24,0
	ldi r25,lo8(1)
	call pdi_cmd_lds_byte
	cpse r24,__zero_reg__
	rjmp .L313
	ldd r24,Y+1
	sbrs r24,7
	rjmp .L316
	lds r24,g_nvmc_timer
	cpse r24,__zero_reg__
	rjmp .L314
	ldi r24,lo8(14)
	rjmp .L313
.L316:
	ldi r24,0
.L313:
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	nvmc_nvmbusy_wait, .-nvmc_nvmbusy_wait
.global	nvmc_read_nvm
	.type	nvmc_read_nvm, @function
nvmc_read_nvm:
	push r12
	push r13
	push r14
	push r15
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	movw r12,r22
	movw r14,r24
	movw r28,r20
	ldi r24,lo8(67)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L318
	movw r18,r28
	ldi r20,0
	ldi r21,0
	movw r24,r14
	movw r22,r12
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	jmp pdi_cmd_load
.L318:
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	nvmc_read_nvm, .-nvmc_read_nvm
.global	nvmc_erase_chip
	.type	nvmc_erase_chip, @function
nvmc_erase_chip:
	push r17
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 1 */
/* stack size = 4 */
.L__stack_usage = 4
	cpi r24,lo8(2)
	breq .L321
	brlo .L329
	cpi r24,lo8(3)
	brne .L335
	ldi r25,lo8(48)
	rjmp .L322
.L321:
	ldi r25,lo8(104)
	rjmp .L322
.L329:
	ldi r25,lo8(64)
.L322:
	mov r17,r24
	mov r24,r25
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L320
	call nvmc_trigger_cmdex
	cpse r24,__zero_reg__
	rjmp .L320
	cpse r17,__zero_reg__
	rjmp .L324
	ldi r24,lo8(50)
	sts g_nvmc_timer,r24
.L327:
	ldi r24,lo8(999)
	ldi r25,hi8(999)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	ldi r24,0
	call pdi_cmd_ldcs
	cpse r24,__zero_reg__
	rjmp .L320
	ldd r24,Y+1
	sbrc r24,1
	rjmp .L326
	lds r24,g_nvmc_timer
	cpse r24,__zero_reg__
	rjmp .L327
	ldi r24,lo8(13)
	rjmp .L320
.L326:
	ldi r24,0
	rjmp .L320
.L324:
	call nvmc_nvmbusy_wait
	rjmp .L320
.L335:
	ldi r24,lo8(15)
.L320:
/* epilogue start */
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	ret
	.size	nvmc_erase_chip, .-nvmc_erase_chip
.global	nvmc_flash_write_page
	.type	nvmc_flash_write_page, @function
nvmc_flash_write_page:
	push r12
	push r13
	push r14
	push r15
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	movw r12,r22
	movw r14,r24
	movw r28,r20
	ldi r24,lo8(38)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L337
	call nvmc_trigger_cmdex
	cpse r24,__zero_reg__
	rjmp .L337
	call nvmc_nvmbusy_wait
	cpse r24,__zero_reg__
	rjmp .L337
	ldi r24,lo8(35)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L337
	ldi r24,64
	add r14,r24
	adc r15,__zero_reg__
	lsl r12
	rol r13
	rol r14
	rol r15
	movw r18,r28
	ldi r20,0
	ldi r21,0
	movw r24,r14
	movw r22,r12
	call pdi_cmd_store
	cpse r24,__zero_reg__
	rjmp .L337
	ldi r24,lo8(47)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L337
	ldi r20,lo8(85)
	movw r24,r14
	movw r22,r12
	call pdi_cmd_sts_byte
	cpse r24,__zero_reg__
	rjmp .L337
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	jmp nvmc_nvmbusy_wait
.L337:
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	nvmc_flash_write_page, .-nvmc_flash_write_page
.global	nvmc_flash_read
	.type	nvmc_flash_read, @function
nvmc_flash_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r26,r24
	movw r24,r22
	subi r26,-64
	sbci r27,-1
	movw r22,r24
	movw r24,r26
	lsl r22
	rol r23
	rol r24
	rol r25
	jmp nvmc_read_nvm
	.size	nvmc_flash_read, .-nvmc_flash_read
.global	nvmc_eeprom_write
	.type	nvmc_eeprom_write, @function
nvmc_eeprom_write:
	push r12
	push r13
	push r14
	push r15
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	movw r12,r22
	movw r14,r24
	movw r28,r20
	ldi r24,lo8(54)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L347
	call nvmc_trigger_cmdex
	cpse r24,__zero_reg__
	rjmp .L347
	call nvmc_nvmbusy_wait
	cpse r24,__zero_reg__
	rjmp .L347
	ldi r24,lo8(51)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L347
	ldi r24,-116
	add r14,r24
	adc r15,__zero_reg__
	movw r18,r28
	ldi r20,0
	ldi r21,0
	movw r24,r14
	movw r22,r12
	call pdi_cmd_store
	cpse r24,__zero_reg__
	rjmp .L347
	ldi r24,lo8(53)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L347
	ldi r20,lo8(85)
	movw r24,r14
	movw r22,r12
	call pdi_cmd_sts_byte
	cpse r24,__zero_reg__
	rjmp .L347
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	jmp nvmc_nvmbusy_wait
.L347:
/* epilogue start */
	pop r29
	pop r28
	pop r15
	pop r14
	pop r13
	pop r12
	ret
	.size	nvmc_eeprom_write, .-nvmc_eeprom_write
.global	nvmc_eeprom_read
	.type	nvmc_eeprom_read, @function
nvmc_eeprom_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	subi r24,116
	sbci r25,-1
	jmp nvmc_read_nvm
	.size	nvmc_eeprom_read, .-nvmc_eeprom_read
.global	nvmc_fuse_write
	.type	nvmc_fuse_write, @function
nvmc_fuse_write:
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 3 */
.L__stack_usage = 3
	movw r28,r24
	mov r17,r22
	ldi r24,lo8(76)
	call nvmc_set_cmd
	cpse r24,__zero_reg__
	rjmp .L357
	movw r24,r28
	ldi r26,0
	ldi r27,0
	movw r22,r24
	movw r24,r26
	subi r22,-32
	sbci r23,-1
	sbci r24,112
	sbci r25,-1
	mov r20,r17
	call pdi_cmd_sts_byte
	cpse r24,__zero_reg__
	rjmp .L357
/* epilogue start */
	pop r29
	pop r28
	pop r17
	jmp nvmc_nvmbusy_wait
.L357:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	ret
	.size	nvmc_fuse_write, .-nvmc_fuse_write
.global	nvmc_fuse_read
	.type	nvmc_fuse_read, @function
nvmc_fuse_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r26,0
	ldi r27,0
	movw r22,r24
	movw r24,r26
	subi r22,-32
	sbci r23,-1
	sbci r24,112
	sbci r25,-1
	ldi r20,0
	ldi r21,0
	jmp nvmc_read_nvm
	.size	nvmc_fuse_read, .-nvmc_fuse_read
.global	nvmc_signaturerow_read
	.type	nvmc_signaturerow_read, @function
nvmc_signaturerow_read:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r20,r22
	ldi r26,0
	ldi r27,0
	movw r22,r24
	movw r24,r26
	subi r23,-2
	sbci r24,113
	sbci r25,-1
	jmp nvmc_read_nvm
	.size	nvmc_signaturerow_read, .-nvmc_signaturerow_read
.global	nvmc_read_dev_sign
	.type	nvmc_read_dev_sign, @function
nvmc_read_dev_sign:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	movw r18,r24
	ldi r20,0
	ldi r21,0
	ldi r22,lo8(-112)
	ldi r23,0
	ldi r24,0
	ldi r25,lo8(1)
	jmp pdi_cmd_load
	.size	nvmc_read_dev_sign, .-nvmc_read_dev_sign
.global	pdi_exec
	.type	pdi_exec, @function
pdi_exec:
	push r12
	push r13
	push r14
	push r15
	push r16
	push r17
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,21
	in __tmp_reg__,__SREG__
	cli
	out __SP_H__,r29
	out __SREG__,__tmp_reg__
	out __SP_L__,r28
/* prologue: function */
/* frame size = 21 */
/* stack size = 29 */
.L__stack_usage = 29
	movw r30,r24
	ldd r14,Z+2
	ldd r15,Z+3
	ldd r26,Z+4
	ldd r27,Z+5
	ldd r16,Z+6
	ldd r17,Z+7
	ldi r18,lo8(1)
	sts g_pdi_last_rc,r18
	ldd r25,Z+1
	cpi r25,lo8(82)
	brne .+2
	rjmp .L365
	brsh .L366
	cpi r25,lo8(69)
	brne .+2
	rjmp .L367
	brsh .L368
	cpi r25,lo8(67)
	brne .+2
	rjmp .L369
	brlo .+2
	rjmp .L370
	cpi r25,lo8(66)
	breq .L371
	rjmp .L363
.L368:
	cpi r25,lo8(71)
	brne .+2
	rjmp .L372
	brsh .+2
	rjmp .L373
	cpi r25,lo8(72)
	brne .+2
	rjmp .L374
	cpi r25,lo8(73)
	brne .+2
	rjmp .L375
	rjmp .L363
.L366:
	cpi r25,lo8(87)
	brne .+2
	rjmp .L376
	brsh .L377
	cpi r25,lo8(84)
	brne .+2
	rjmp .L378
	brsh .+2
	rjmp .L379
	cpi r25,lo8(85)
	brne .+2
	rjmp .L380
	cpi r25,lo8(86)
	brne .+2
	rjmp .L381
	rjmp .L363
.L377:
	cpi r25,lo8(89)
	brne .+2
	rjmp .L382
	brsh .+2
	rjmp .L383
	cpi r25,lo8(96)
	brne .+2
	rjmp .L384
	cpi r25,lo8(97)
	breq .+2
	rjmp .L363
	mov r24,r14
	call nvmc_erase_chip
	rjmp .L427
.L371:
	movw r20,r16
	cpi r16,5
	cpc r17,__zero_reg__
	brlo .L386
	ldi r20,lo8(4)
	ldi r21,0
.L386:
	mov r16,r20
	cpse r20,__zero_reg__
	rjmp .L387
	ldi r16,lo8(1)
.L387:
	ldi r24,lo8(-1)
	add r24,r16
	ori r24,lo8(12)
	std Y+1,r24
	std Y+2,r26
	std Y+3,r27
	std Y+4,r14
	std Y+5,r15
	movw r18,r28
	subi r18,-21
	sbci r19,-1
	movw r20,r28
	subi r20,-13
	sbci r21,-1
	ldi r22,lo8(5)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
	sts g_pdi_last_rc,r24
	rjmp .L426
.L369:
	std Y+20,__zero_reg__
	std Y+19,__zero_reg__
	movw r20,r16
	cpi r16,5
	cpc r17,__zero_reg__
	brlo .L388
	ldi r20,lo8(4)
	ldi r21,0
.L388:
	cpse r20,__zero_reg__
	rjmp .L389
	ldi r20,lo8(1)
.L389:
	ldi r24,lo8(-1)
	add r24,r20
	ori r24,lo8(76)
	std Y+1,r24
	std Y+2,r26
	std Y+3,r27
	std Y+4,r14
	std Y+5,r15
	ldi r21,0
	movw r22,r28
	subi r22,-19
	sbci r23,-1
	movw r24,r28
	adiw r24,6
	call usb_ep_recv_control_data
	ldd r22,Y+19
	subi r22,lo8(-(5))
	rjmp .L428
.L370:
	movw r20,r16
	cpi r16,5
	cpc r17,__zero_reg__
	brlo .L390
	ldi r20,lo8(4)
	ldi r21,0
.L390:
	mov r16,r20
	cpse r20,__zero_reg__
	rjmp .L391
	ldi r16,lo8(1)
.L391:
	mov r30,r14
	andi r30,lo8(12)
	ldi r25,lo8(-1)
	add r25,r16
	ori r30,lo8(32)
	or r30,r25
	std Y+1,r30
	movw r18,r28
	subi r18,-21
	sbci r19,-1
	movw r20,r28
	subi r20,-13
	sbci r21,-1
	ldi r22,lo8(1)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
.L426:
	mov r18,r16
	ldi r19,0
	ldd r22,Y+21
	ldi r23,0
.L431:
	ldi r20,0
	movw r24,r28
	adiw r24,13
	call usb_ep_send_control_data
	rjmp .L363
.L367:
	std Y+20,__zero_reg__
	std Y+19,__zero_reg__
	movw r20,r16
	cpi r16,5
	cpc r17,__zero_reg__
	brlo .L392
	ldi r20,lo8(4)
	ldi r21,0
.L392:
	cpse r20,__zero_reg__
	rjmp .L393
	ldi r20,lo8(1)
.L393:
	mov r30,r14
	andi r30,lo8(12)
	ldi r25,lo8(-1)
	add r25,r20
	ori r30,lo8(96)
	or r30,r25
	std Y+1,r30
.L430:
	ldi r21,0
.L429:
	movw r22,r28
	subi r22,-19
	sbci r23,-1
	movw r24,r28
	adiw r24,2
	call usb_ep_recv_control_data
	ldd r22,Y+19
	subi r22,lo8(-(1))
.L428:
	movw r18,r28
	subi r18,-21
	sbci r19,-1
	movw r20,r28
	subi r20,-13
	sbci r21,-1
	movw r24,r28
	adiw r24,1
	call pdi_cmd
	sts g_pdi_last_rc,r24
	rjmp .L363
.L373:
	mov r30,r14
	andi r30,lo8(15)
	ori r30,lo8(-128)
	std Y+1,r30
	movw r18,r28
	subi r18,-21
	sbci r19,-1
	movw r20,r28
	subi r20,-13
	sbci r21,-1
	ldi r22,lo8(1)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
	sts g_pdi_last_rc,r24
	ldd r22,Y+21
	ldi r23,0
	movw r18,r16
	rjmp .L431
.L372:
	mov r30,r14
	andi r30,lo8(15)
	ori r30,lo8(-64)
	std Y+1,r30
	std Y+2,r26
	movw r18,r28
	subi r18,-21
	sbci r19,-1
	movw r20,r28
	subi r20,-13
	sbci r21,-1
	ldi r22,lo8(2)
	movw r24,r28
	adiw r24,1
	call pdi_cmd
	rjmp .L427
.L374:
	std Y+20,__zero_reg__
	std Y+19,__zero_reg__
	movw r20,r16
	cpi r16,5
	cpc r17,__zero_reg__
	brlo .L394
	ldi r20,lo8(4)
	ldi r21,0
.L394:
	cpse r20,__zero_reg__
	rjmp .L395
	ldi r20,lo8(1)
.L395:
	ldi r24,lo8(-1)
	add r24,r20
	ori r24,lo8(-96)
	std Y+1,r24
	rjmp .L430
.L375:
	std Y+20,__zero_reg__
	std Y+19,__zero_reg__
	ldi r24,lo8(-32)
	std Y+1,r24
	movw r20,r16
	cpi r16,9
	cpc r17,__zero_reg__
	brsh .+2
	rjmp .L429
	ldi r20,lo8(8)
	ldi r21,0
	rjmp .L429
.L382:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L397
	movw r20,r16
	subi r20,1
	sbc r21,__zero_reg__
	movw r16,r14
	ldi r18,0
	ldi r19,0
	movw r14,r16
	clr r13
	clr r12
	movw r16,r26
	ldi r18,0
	ldi r19,0
	movw r24,r14
	movw r22,r12
	or r22,r16
	or r23,r17
	or r24,r18
	or r25,r19
	call nvmc_read_nvm
.L424:
	sts g_pdi_last_rc,r24
.L397:
	call usb_ep_send_control_data_finish
	rjmp .L363
.L365:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L397
	movw r20,r16
	subi r20,1
	sbc r21,__zero_reg__
	movw r16,r14
	ldi r18,0
	ldi r19,0
	movw r14,r16
	clr r13
	clr r12
	movw r16,r26
	ldi r18,0
	ldi r19,0
	movw r24,r14
	movw r22,r12
	or r22,r16
	or r23,r17
	or r24,r18
	or r25,r19
	call nvmc_flash_read
	rjmp .L424
.L378:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L397
	movw r20,r16
	subi r20,1
	sbc r21,__zero_reg__
	movw r16,r14
	ldi r18,0
	ldi r19,0
	movw r14,r16
	clr r13
	clr r12
	movw r16,r26
	ldi r18,0
	ldi r19,0
	movw r24,r14
	movw r22,r12
	or r22,r16
	or r23,r17
	or r24,r18
	or r25,r19
	call nvmc_eeprom_read
	rjmp .L424
.L381:
	or r16,r17
	breq .L397
	movw r24,r14
	call nvmc_fuse_read
	rjmp .L424
.L383:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L397
	movw r22,r16
	subi r22,1
	sbc r23,__zero_reg__
	movw r24,r14
	call nvmc_signaturerow_read
	rjmp .L424
.L379:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L398
	movw r20,r16
	subi r20,1
	sbc r21,__zero_reg__
	movw r16,r14
	ldi r18,0
	ldi r19,0
	movw r14,r16
	clr r13
	clr r12
	movw r16,r26
	ldi r18,0
	ldi r19,0
	movw r24,r14
	movw r22,r12
	or r22,r16
	or r23,r17
	or r24,r18
	or r25,r19
	call nvmc_flash_write_page
.L425:
	sts g_pdi_last_rc,r24
.L398:
	call usb_ep_recv_control_data_finish
	rjmp .L363
.L380:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	breq .L398
	movw r20,r16
	subi r20,1
	sbc r21,__zero_reg__
	movw r16,r14
	ldi r18,0
	ldi r19,0
	movw r14,r16
	clr r13
	clr r12
	movw r16,r26
	ldi r18,0
	ldi r19,0
	movw r24,r14
	movw r22,r12
	or r22,r16
	or r23,r17
	or r24,r18
	or r25,r19
	call nvmc_eeprom_write
	rjmp .L425
.L376:
	mov r22,r26
	movw r24,r14
	call nvmc_fuse_write
	rjmp .L427
.L384:
	cp r16,__zero_reg__
	cpc r17,__zero_reg__
	brne .+2
	rjmp .L397
	movw r24,r16
	sbiw r24,1
	cpi r24,4
	cpc r25,__zero_reg__
	brlo .L399
	ldi r24,lo8(3)
	ldi r25,0
.L399:
	call nvmc_read_dev_sign
	rjmp .L424
.L427:
	sts g_pdi_last_rc,r24
	call usb_ep_send_zlp
.L363:
/* epilogue start */
	adiw r28,21
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
	pop r14
	pop r13
	pop r12
	ret
	.size	pdi_exec, .-pdi_exec
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
	push r14
	push r15
	push r17
	push r28
	push r29
	rcall .
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 2 */
/* stack size = 7 */
.L__stack_usage = 7
	movw r30,r24
	ld r18,Z
	mov r19,r18
	andi r19,lo8(127)
	cpi r19,lo8(65)
	breq .+2
	rjmp .L473
	movw r14,r24
	ldd r25,Z+1
	sbrc r18,7
	rjmp .L435
	cpi r25,lo8(74)
	brsh .L436
	cpi r25,lo8(71)
	brlo .+2
	rjmp .L437
	cpi r25,lo8(21)
	brne .+2
	rjmp .L438
	brsh .L439
	cpi r25,lo8(9)
	breq .L440
	cpi r25,lo8(20)
	brne .+2
	rjmp .L441
	cpi r25,lo8(8)
	breq .+2
	rjmp .L473
	call usb_ep_clearSETUP
	call cmd_avr_init
	rjmp .L530
.L439:
	cpi r25,lo8(65)
	brne .+2
	rjmp .L443
	brsh .L444
	cpi r25,lo8(64)
	brne .+2
	rjmp .L445
	rjmp .L473
.L444:
	cpi r25,lo8(67)
	brne .+2
	rjmp .L437
	cpi r25,lo8(69)
	brne .+2
	rjmp .L437
	rjmp .L473
.L436:
	cpi r25,lo8(81)
	brne .+2
	rjmp .L443
	brsh .L446
	cpi r25,lo8(76)
	breq .L447
	cpi r25,lo8(80)
	brne .+2
	rjmp .L448
	cpi r25,lo8(75)
	breq .+2
	rjmp .L473
	call usb_ep_clearSETUP
	call pdi_off
	lds r24,g_pdi_flags
	ori r24,lo8(1)
	rjmp .L528
.L446:
	cpi r25,lo8(85)
	brne .+2
	rjmp .L437
	brsh .L450
	cpi r25,lo8(83)
	brne .+2
	rjmp .L437
	rjmp .L473
.L450:
	cpi r25,lo8(87)
	brne .+2
	rjmp .L437
	cpi r25,lo8(97)
	brne .+2
	rjmp .L437
	rjmp .L473
.L440:
	call usb_ep_clearSETUP
	call cmd_avr_exit
.L530:
	ldi r17,0
	rjmp .L434
.L441:
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r22,Z+6
	ldd r23,Z+7
	ldd r24,Z+2
	ldd r25,Z+3
	call cmd_avr_writeflashpage
	rjmp .L530
.L438:
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r22,Z+6
	ldd r23,Z+7
	ldd r24,Z+2
	ldd r25,Z+3
	call cmd_avr_writeeeprompage
	rjmp .L530
.L447:
	call usb_ep_clearSETUP
	sts g_pdi_flags,__zero_reg__
	call pdi_leave
	rjmp .L453
.L445:
	call usb_ep_clearSETUP
	lds r24,g_pdi_flags
	sbrs r24,0
	rjmp .L451
	lds r24,g_pdi_flags
	sbrc r24,1
	rjmp .L452
	call pdi_start
	lds r24,g_pdi_flags
	ori r24,lo8(2)
.L528:
	sts g_pdi_flags,r24
.L452:
	sts g_pdi_last_rc,__zero_reg__
	rjmp .L453
.L448:
	call usb_ep_clearSETUP
	lds r24,g_pdi_flags
	sbrc r24,0
	rjmp .L533
.L451:
	ldi r24,lo8(100)
	sts g_pdi_last_rc,r24
.L453:
	call usb_ep_send_zlp
	rjmp .L530
.L533:
	sts g_pdi_last_rc,__zero_reg__
	lds r24,g_pdi_flags
	sbrc r24,2
	rjmp .L453
	lds r24,g_pdi_flags
	sbrc r24,1
	rjmp .L455
	call pdi_start
	lds r24,g_pdi_flags
	ori r24,lo8(2)
	sts g_pdi_flags,r24
.L455:
	call nvmc_init
	sts g_pdi_last_rc,r24
	cpse r24,__zero_reg__
	rjmp .L453
	lds r24,g_pdi_flags
	ori r24,lo8(4)
	sts g_pdi_flags,r24
	rjmp .L453
.L443:
	call usb_ep_clearSETUP
	lds r24,g_pdi_flags
	sbrs r24,0
	rjmp .L451
	call nvmc_exit
	call pdi_off
	lds r24,g_pdi_flags
	andi r24,lo8(-7)
	rjmp .L528
.L437:
	call usb_ep_clearSETUP
	lds r24,g_pdi_flags
	mov r17,r24
	andi r17,lo8(1)
	sbrs r24,0
	rjmp .L457
	lds r24,g_pdi_flags
	mov r17,r24
	andi r17,lo8(2)
	sbrs r24,1
	rjmp .L458
.L470:
	movw r24,r14
	call pdi_exec
	rjmp .L530
.L458:
	ldi r24,lo8(101)
	rjmp .L531
.L457:
	ldi r24,lo8(100)
.L531:
	sts g_pdi_last_rc,r24
	call usb_ep_send_zlp
	rjmp .L434
.L435:
	cpi r25,lo8(70)
	brne .+2
	rjmp .L459
	brsh .L460
	cpi r25,lo8(19)
	brne .+2
	rjmp .L461
	brsh .L462
	cpi r25,lo8(17)
	brne .+2
	rjmp .L463
	brlo .+2
	rjmp .L464
	cpi r25,lo8(16)
	breq .+2
	rjmp .L473
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r20,Z+6
	ldd r21,Z+7
	ldd r22,Z+2
	ldd r24,Z+3
	call cmd_avr_progmode
	rjmp .L530
.L462:
	cpi r25,lo8(66)
	brne .+2
	rjmp .L459
	cpi r25,lo8(68)
	brne .+2
	rjmp .L459
	cpi r25,lo8(32)
	breq .+2
	rjmp .L473
	call usb_ep_clearSETUP
	lds r24,g_avr_flags
	std Y+1,r24
	lds r24,g_avr_error
	std Y+2,r24
	sts g_avr_error,__zero_reg__
	movw r30,r14
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(2)
	ldi r23,0
	rjmp .L532
.L460:
	cpi r25,lo8(86)
	brne .+2
	rjmp .L459
	brsh .L467
	cpi r25,lo8(82)
	brne .+2
	rjmp .L459
	cpi r25,lo8(84)
	breq .L459
	cpi r25,lo8(74)
	breq .+2
	rjmp .L473
	call usb_ep_clearSETUP
	lds r24,g_pdi_last_rc
	std Y+1,r24
	sts g_pdi_last_rc,__zero_reg__
	movw r30,r14
	ldd r18,Z+6
	ldd r19,Z+7
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	rjmp .L532
.L467:
	cpi r25,lo8(88)
	brsh .+2
	rjmp .L473
	cpi r25,lo8(90)
	brlo .L459
	cpi r25,lo8(96)
	breq .L459
	rjmp .L473
.L463:
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r20,Z+6
	ldd r21,Z+7
	ldd r22,Z+4
	ldd r23,Z+5
	ldd r24,Z+2
	ldd r25,Z+3
	call cmd_avr_rawcmd
	rjmp .L530
.L464:
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r22,Z+6
	ldd r23,Z+7
	ldd r24,Z+2
	ldd r25,Z+3
	call cmd_avr_readflash
	rjmp .L530
.L461:
	call usb_ep_clearSETUP
	movw r30,r14
	ldd r22,Z+6
	ldd r23,Z+7
	ldd r24,Z+2
	ldd r25,Z+3
	call cmd_avr_readeeprom
	rjmp .L530
.L532:
	movw r24,r28
	adiw r24,1
	call usb_ep_send_control_data
	rjmp .L530
.L459:
	call usb_ep_clearSETUP
	lds r24,g_pdi_flags
	mov r17,r24
	andi r17,lo8(1)
	sbrs r24,0
	rjmp .L469
	lds r24,g_pdi_flags
	mov r17,r24
	andi r17,lo8(2)
	sbrc r24,1
	rjmp .L470
	ldi r24,lo8(101)
	rjmp .L529
.L469:
	ldi r24,lo8(100)
.L529:
	sts g_pdi_last_rc,r24
	call usb_ep_send_control_data_finish
	rjmp .L434
.L473:
	ldi r17,lo8(1)
.L434:
	mov r24,r17
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	pop r17
	pop r15
	pop r14
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
	rjmp .L534
	ldi r22,lo8(8)
	movw r24,r28
	adiw r24,1
	call usb_ep_read_data
	ldd r24,Y+1
	mov r25,r24
	andi r25,lo8(96)
	breq .+2
	rjmp .L537
	ldd r25,Y+2
	cpi r25,lo8(6)
	breq .L538
	brsh .L539
	cpi r25,lo8(5)
	brne .+2
	rjmp .L540
	rjmp .L537
.L539:
	cpi r25,lo8(8)
	breq .L541
	cpi r25,lo8(9)
	brne .+2
	rjmp .L542
	rjmp .L537
.L541:
	cpi r24,lo8(-128)
	breq .+2
	rjmp .L537
	lds r24,g_usb_dev
	andi r24,lo8(24)
	brne .+2
	rjmp .L537
	call usb_ep_clearSETUP
	ldi r18,lo8(1)
	ldi r19,0
	ldi r20,0
	ldi r22,lo8(1)
	ldi r23,0
	ldi r24,lo8(g_usb_dev+1)
	ldi r25,hi8(g_usb_dev+1)
	rjmp .L569
.L538:
	cpi r24,lo8(-128)
	breq .+2
	rjmp .L537
	ldd r16,Y+3
	ldd r17,Y+4
	mov r24,r17
	ldi r25,0
	cpi r24,2
	cpc r25,__zero_reg__
	breq .L544
	cpi r24,3
	cpc r25,__zero_reg__
	breq .L545
	sbiw r24,1
	breq .+2
	rjmp .L537
	call usb_ep_clearSETUP
	ldd r18,Y+7
	ldd r19,Y+8
	lds r20,g_usb_dev+10
	ldi r22,lo8(18)
	ldi r23,0
	lds r24,g_usb_dev+2
	lds r25,g_usb_dev+2+1
	rjmp .L569
.L544:
	cpse r16,__zero_reg__
	rjmp .L537
	call usb_ep_clearSETUP
	ldd r18,Y+7
	ldd r19,Y+8
	lds r20,g_usb_dev+10
	lds r22,g_usb_dev+6
	ldi r23,0
	lds r24,g_usb_dev+4
	lds r25,g_usb_dev+4+1
.L569:
	call usb_ep_send_control_data
	rjmp .L543
.L545:
	lds r24,g_usb_dev+9
	cp r16,r24
	brlo .+2
	rjmp .L537
	call usb_ep_clearSETUP
	lds r15,g_usb_dev+10
	lds r24,g_usb_dev+7
	lds r25,g_usb_dev+7+1
	clr r17
	ldi r18,lo8(1)
	cp r15,r18
	breq .L548
	ldi r18,lo8(2)
	cpse r15,r18
	rjmp .L571
	lsl r16
	rol r17
	add r24,r16
	adc r25,r17
	call __eerd_word_usb162
	movw r16,r24
	rjmp .L550
.L548:
	lsl r16
	rol r17
	movw r30,r24
	add r30,r16
	adc r31,r17
/* #APP */
 ;  366 "../avrutilslib/usb/usb.c" 1
	lpm r16, Z+
	lpm r17, Z
	
 ;  0 "" 2
/* #NOAPP */
	rjmp .L550
.L571:
	lsl r16
	rol r17
	movw r30,r24
	add r30,r16
	adc r31,r17
	ld r16,Z
	ldd r17,Z+1
.L550:
	ldd r12,Y+7
	ldd r13,Y+8
	mov r22,r15
	movw r24,r16
	call usb_mem_read_byte
	mov r22,r24
	ldi r23,0
	movw r18,r12
	mov r20,r15
	movw r24,r16
	rjmp .L569
.L540:
	cpse r24,__zero_reg__
	rjmp .L537
	lds r24,g_usb_dev
	andi r24,lo8(12)
	breq .L537
	ldd r17,Y+3
	andi r17,lo8(127)
	call usb_ep_clearSETUP
	sts 227,r17
	call usb_ep_send_zlp
	lds r24,227
	ori r24,lo8(-128)
	sts 227,r24
	tst r17
	breq .L551
	ldi r24,lo8(8)
	rjmp .L570
.L551:
	ldi r24,lo8(4)
.L570:
	sts g_usb_dev,r24
	rjmp .L543
.L542:
	cpse r24,__zero_reg__
	rjmp .L537
	lds r24,g_usb_dev
	andi r24,lo8(24)
	breq .L537
	ldd r17,Y+3
	cpse r17,__zero_reg__
	rjmp .L552
	ldi r24,lo8(8)
	sts g_usb_dev,r24
	sts g_usb_dev+1,__zero_reg__
	rjmp .L553
.L552:
	lds r22,g_usb_dev+10
	lds r24,g_usb_dev+4
	lds r25,g_usb_dev+4+1
	adiw r24,5
	call usb_mem_read_byte
	cpse r17,r24
	rjmp .L537
	ldi r24,lo8(16)
	sts g_usb_dev,r24
	sts g_usb_dev+1,r17
	sts 233,__zero_reg__
.L553:
	call usb_ep_clearSETUP
	call usb_ep_send_zlp
	rjmp .L543
.L537:
	movw r24,r28
	adiw r24,1
	call usb_event_ep0_request
.L555:
	sts 233,__zero_reg__
	tst r24
	breq .L534
	call usb_ep_clearSETUP
	lds r24,235
	ori r24,lo8(32)
	sts 235,r24
	rjmp .L534
.L543:
	ldi r24,0
	rjmp .L555
.L534:
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
	lds r24,g_led_counter
	subi r24,lo8(-(1))
	sts g_led_counter,r24
	cpi r24,lo8(10)
	brlo .L572
	in r25,0x5
	ldi r24,lo8(32)
	eor r24,r25
	out 0x5,r24
	sts g_led_counter,__zero_reg__
.L572:
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
 ;  1628 "gpio-usb.c" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	call init_system
	clr r14
	inc r14
	ldi r16,lo8(4)
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
 ;  1638 "gpio-usb.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L575:
	call usb_ep0_task
	rjmp .L575
	.size	main, .-main
	.section	.rodata
	.type	CSWTCH.133, @object
	.size	CSWTCH.133, 25
CSWTCH.133:
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
	.type	epsizemap.2209, @object
	.size	epsizemap.2209, 4
epsizemap.2209:
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.local	g_led_counter
	.comm	g_led_counter,1,1
.global	g_strings_P
	.type	g_strings_P, @object
	.size	g_strings_P, 8
g_strings_P:
	.word	g_string0_P
	.word	g_string1_P
	.word	g_string2_P
	.word	g_string3_P
.global	g_string3_P
	.type	g_string3_P, @object
	.size	g_string3_P, 2
g_string3_P:
	.byte	12
	.byte	3
	.string	"V"
	.string	" "
	.string	"1"
	.string	"."
	.string	"2"
	.string	""
	.string	""
.global	g_string2_P
	.type	g_string2_P, @object
	.size	g_string2_P, 2
g_string2_P:
	.byte	18
	.byte	3
	.string	"g"
	.string	"p"
	.string	"i"
	.string	"o"
	.string	"-"
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
	.word	-24575
	.word	256
	.byte	1
	.byte	2
	.byte	3
	.byte	1
.global	g_pdi_flags
	.section .bss
	.type	g_pdi_flags, @object
	.size	g_pdi_flags, 1
g_pdi_flags:
	.zero	1
.global	g_pdi_last_rc
	.type	g_pdi_last_rc, @object
	.size	g_pdi_last_rc, 1
g_pdi_last_rc:
	.zero	1
.global	g_nvmc_timer
	.type	g_nvmc_timer, @object
	.size	g_nvmc_timer, 1
g_nvmc_timer:
	.zero	1
.global	g_pdi_timer
	.type	g_pdi_timer, @object
	.size	g_pdi_timer, 1
g_pdi_timer:
	.zero	1
.global	g_avr_timer
	.type	g_avr_timer, @object
	.size	g_avr_timer, 1
g_avr_timer:
	.zero	1
.global	g_avr_clk_wait
	.type	g_avr_clk_wait, @object
	.size	g_avr_clk_wait, 1
g_avr_clk_wait:
	.zero	1
.global	g_avr_error
	.type	g_avr_error, @object
	.size	g_avr_error, 1
g_avr_error:
	.zero	1
.global	g_avr_flags
	.type	g_avr_flags, @object
	.size	g_avr_flags, 1
g_avr_flags:
	.zero	1
	.comm	g_usb_dev,11,1
	.ident	"GCC: (Gentoo 4.9.3 p1.3, pie-0.6.3) 4.9.3"
.global __do_copy_data
.global __do_clear_bss
