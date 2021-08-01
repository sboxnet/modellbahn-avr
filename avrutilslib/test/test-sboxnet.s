	.file	"test-sboxnet.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__CCP__ = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
	.type	sboxnet_crc8_add, @function
sboxnet_crc8_add:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
/* #APP */
 ;  321 "/usr/avr/include/util/crc16.h" 1
		eor	r24, r22
		ldi	r25, 8
		ldi	r22, 0x8C
	1:	lsr	r24
		brcc	2f
		eor	r24, r22
	2:	dec	r25
		brne	1b
	
 ;  0 "" 2
/* #NOAPP */
	ret
	.size	sboxnet_crc8_add, .-sboxnet_crc8_add
	.type	sboxnet_recv_buf_write, @function
sboxnet_recv_buf_write:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r25,g_sboxnet_dev_def+12
	lds r18,g_sboxnet_dev_def+10
	add r25,r18
	cpi r25,lo8(-96)
	brsh .L5
	subi r18,lo8(-(1))
	sts g_sboxnet_dev_def+10,r18
	lds r25,g_sboxnet_dev_def+9
	mov r30,r25
	ldi r31,0
	subi r30,lo8(-(g_sboxnet_dev_def+8))
	sbci r31,hi8(-(g_sboxnet_dev_def+8))
	std Z+5,r24
	ldi r24,lo8(1)
	add r24,r25
	cpi r24,lo8(-96)
	brsh .L4
	sts g_sboxnet_dev_def+9,r24
	rjmp .L6
.L4:
	sts g_sboxnet_dev_def+9,__zero_reg__
.L6:
	ldi r24,lo8(1)
	ret
.L5:
	ldi r24,0
	ret
	.size	sboxnet_recv_buf_write, .-sboxnet_recv_buf_write
	.type	sboxnet_set_timer, @function
sboxnet_set_timer:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,0
	ldi r31,lo8(8)
	ldd r18,Z+32
	ldd r19,Z+33
	add r18,r24
	adc r19,r25
	clr r19
	std Z+40,r18
	std Z+41,r19
	ldi r24,lo8(16)
	std Z+12,r24
	ldd r24,Z+7
	andi r24,lo8(-4)
	ori r24,lo8(1)
	std Z+7,r24
	ret
	.size	sboxnet_set_timer, .-sboxnet_set_timer
	.type	lcd_ks0070_write4, @function
lcd_ks0070_write4:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r25,1636
	andi r25,lo8(-121)
	andi r22,lo8(15)
	ldi r18,lo8(8)
	mul r22,r18
	movw r22,r0
	clr __zero_reg__
	or r25,r22
	sts 1636,r25
	tst r24
	breq .L10
	ldi r24,lo8(-128)
	sts 1637,r24
	rjmp .L11
.L10:
	ldi r24,lo8(-128)
	sts 1638,r24
.L11:
	ldi r24,lo8(1)
	sts 1669,r24
	ldi r25,lo8(10)
	1: dec r25
	brne 1b
	rjmp .
	sts 1670,r24
	ret
	.size	lcd_ks0070_write4, .-lcd_ks0070_write4
	.type	lcd_ks0070_write, @function
lcd_ks0070_write:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	mov r28,r24
	mov r29,r22
	swap r22
	andi r22,lo8(15)
	call lcd_ks0070_write4
	mov r22,r29
	mov r24,r28
/* epilogue start */
	pop r29
	pop r28
	jmp lcd_ks0070_write4
	.size	lcd_ks0070_write, .-lcd_ks0070_write
	.type	lcd_ks0070_command, @function
lcd_ks0070_command:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	mov r22,r24
	ldi r24,0
	call lcd_ks0070_write
	ldi r24,lo8(359)
	ldi r25,hi8(359)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	lcd_ks0070_command, .-lcd_ks0070_command
	.type	lcd_set_cursor, @function
lcd_set_cursor:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	subi r22,lo8(-(-1))
	cpi r22,lo8(3)
	brsh .L16
	mov r30,r22
	ldi r31,0
	subi r30,lo8(-(CSWTCH.45))
	sbci r31,hi8(-(CSWTCH.45))
	ld r25,Z
	rjmp .L15
.L16:
	ldi r25,0
.L15:
	add r24,r25
	ori r24,lo8(-128)
	jmp lcd_ks0070_command
	.size	lcd_set_cursor, .-lcd_set_cursor
	.type	lcd_ks0070_data, @function
lcd_ks0070_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	mov r22,r24
	ldi r24,lo8(1)
	call lcd_ks0070_write
	ldi r24,lo8(399)
	ldi r25,hi8(399)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	lcd_ks0070_data, .-lcd_ks0070_data
	.type	lcd_str, @function
lcd_str:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r28,r24
.L19:
	ld r24,Y
	tst r24
	breq .L21
	call lcd_ks0070_data
	adiw r28,1
	rjmp .L19
.L21:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	lcd_str, .-lcd_str
	.type	lcd_ks0070_display_clear, @function
lcd_ks0070_display_clear:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(1)
	call lcd_ks0070_command
	ldi r24,lo8(15999)
	ldi r25,hi8(15999)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ret
	.size	lcd_ks0070_display_clear, .-lcd_ks0070_display_clear
	.type	lcd_itoa, @function
lcd_itoa:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,7
	out __SP_L__,r28
	out __SP_H__,r29
/* prologue: function */
/* frame size = 7 */
/* stack size = 9 */
.L__stack_usage = 9
	ldi r20,lo8(10)
	ldi r21,0
	movw r22,r28
	subi r22,-1
	sbci r23,-1
	call itoa
	movw r24,r28
	adiw r24,1
	call lcd_str
/* epilogue start */
	adiw r28,7
	out __SP_L__,r28
	out __SP_H__,r29
	pop r29
	pop r28
	ret
	.size	lcd_itoa, .-lcd_itoa
	.type	lcd_ks0070_init, @function
lcd_ks0070_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(96)
	ldi r31,lo8(6)
	ldi r24,lo8(120)
	std Z+6,r24
	std Z+1,r24
	ldi r24,lo8(-128)
	std Z+6,r24
	std Z+1,r24
	ldi r30,lo8(-128)
	ldi r31,lo8(6)
	ldi r24,lo8(1)
	std Z+6,r24
	std Z+1,r24
	ldi r22,lo8(3)
	ldi r24,0
	call lcd_ks0070_write4
	ldi r24,lo8(-25537)
	ldi r25,hi8(-25537)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r22,lo8(3)
	ldi r24,0
	call lcd_ks0070_write4
	ldi r24,lo8(959)
	ldi r25,hi8(959)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r22,lo8(3)
	ldi r24,0
	call lcd_ks0070_write4
	ldi r24,lo8(959)
	ldi r25,hi8(959)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r22,lo8(2)
	ldi r24,0
	call lcd_ks0070_write4
	ldi r24,lo8(959)
	ldi r25,hi8(959)
	1: sbiw r24,1
	brne 1b
	rjmp .
	nop
	ldi r24,lo8(40)
	call lcd_ks0070_command
	ldi r24,lo8(12)
	call lcd_ks0070_command
	call lcd_ks0070_display_clear
	ldi r24,lo8(6)
	jmp lcd_ks0070_command
	.size	lcd_ks0070_init, .-lcd_ks0070_init
	.type	lcd_str_P, @function
lcd_str_P:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r28,r24
.L28:
	movw r30,r28
/* #APP */
 ;  208 "../../avrutilslib/lcd.c" 1
	lpm r24, Z
	
 ;  0 "" 2
/* #NOAPP */
	tst r24
	breq .L29
	call lcd_ks0070_data
	adiw r28,1
	rjmp .L28
.L29:
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	lcd_str_P, .-lcd_str_P
.global	sboxnet_prandom
	.type	sboxnet_prandom, @function
sboxnet_prandom:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet_dev_def+7)
	ldi r31,hi8(g_sboxnet_dev_def+7)
	ld r25,Z
	mov r24,r25
	lsl r24
	eor r24,r25
	mov r25,r24
	lsr r25
	eor r25,r24
	mov r24,r25
	lsl r24
	lsl r24
	eor r24,r25
	st Z,r24
	ret
	.size	sboxnet_prandom, .-sboxnet_prandom
	.type	sboxnet_check_try_transmit, @function
sboxnet_check_try_transmit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet_dev_def)
	ldi r31,hi8(g_sboxnet_dev_def)
	ldd r24,Z+4
	tst r24
	breq .L31
	ldd r25,Z+3
	cpse r25,r24
	rjmp .L31
	ld r24,Z
	cpse r24,__zero_reg__
	rjmp .L31
	call sboxnet_prandom
	andi r24,lo8(15)
	subi r24,lo8(-(20))
	ldi r25,lo8(1)
	sts 1612,r25
	sts g_sboxnet_dev_def,r25
	ldi r25,0
	adiw r24,60
	jmp sboxnet_set_timer
.L31:
	ret
	.size	sboxnet_check_try_transmit, .-sboxnet_check_try_transmit
.global	sboxnet_init
	.type	sboxnet_init, @function
sboxnet_init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r24,lo8(-29)
	ldi r30,lo8(g_sboxnet_dev_def)
	ldi r31,hi8(g_sboxnet_dev_def)
	movw r26,r30
	0:
	st X+,__zero_reg__
	dec r24
	brne 0b
	lds r24,84
	sts g_sboxnet_dev_def+7,r24
	ldi r30,lo8(64)
	ldi r31,lo8(6)
	ldi r25,lo8(4)
	std Z+2,r25
	ldd r24,Z+18
	andi r24,lo8(-57)
	ori r24,lo8(24)
	std Z+18,r24
	ldi r24,lo8(8)
	std Z+5,r24
	std Z+1,r24
	ldd r24,Z+9
	andi r24,lo8(-4)
	std Z+9,r24
	std Z+10,r25
	ldi r24,lo8(1)
	std Z+12,r24
	ldi r30,lo8(-96)
	ldi r31,lo8(8)
	std Z+7,__zero_reg__
	ldi r24,lo8(15)
	std Z+6,r24
	ldi r24,lo8(64)
	std Z+1,r24
	std Z+3,__zero_reg__
	ldi r24,lo8(2)
	std Z+4,r24
	ldi r24,lo8(39)
	std Z+5,r24
	ret
	.size	sboxnet_init, .-sboxnet_init
.global	sboxnet_receiver_enable
	.type	sboxnet_receiver_enable, @function
sboxnet_receiver_enable:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r30,lo8(g_sboxnet_dev_def+8)
	ldi r31,hi8(g_sboxnet_dev_def+8)
	std Z+2,__zero_reg__
	ldd r24,Z+3
	std Z+1,r24
	ldi r30,lo8(-96)
	ldi r31,lo8(8)
	ldd r24,Z+3
	andi r24,lo8(-49)
	ori r24,lo8(16)
	std Z+3,r24
	ldd r24,Z+4
	ori r24,lo8(18)
	std Z+4,r24
	ret
	.size	sboxnet_receiver_enable, .-sboxnet_receiver_enable
	.type	sboxnet_timer_bit, @function
sboxnet_timer_bit:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	lds r24,2055
	andi r24,lo8(-4)
	sts 2055,r24
	lds r24,g_sboxnet_dev_def
	cpi r24,lo8(1)
	breq .+2
	rjmp .L39
	lds r25,g_sboxnet_dev_def+4
	tst r25
	brne .+2
	rjmp .L40
	lds r24,g_sboxnet_dev_def+3
	cpse r24,r25
	rjmp .L40
	mov r30,r24
	ldi r31,0
	subi r30,lo8(-(g_sboxnet_dev_def+172))
	sbci r31,hi8(-(g_sboxnet_dev_def+172))
	ld r18,Z
	ldi r25,lo8(8)
	sts 1605,r25
	lds r25,2212
	ori r25,lo8(9)
	sts 2212,r25
	lds r25,1612
	sbrc r25,0
	rjmp .L41
	lds r25,1608
	sbrc r25,2
	rjmp .L42
.L41:
	call sboxnet_prandom
	andi r24,lo8(15)
	ldi r25,lo8(1)
	sts 1612,r25
	sts g_sboxnet_dev_def,r25
	ldi r25,0
	adiw r24,60
	rjmp .L55
.L42:
	lds r25,2212
	andi r25,lo8(-17)
	sts 2212,r25
	lds r25,2211
	andi r25,lo8(-61)
	sts 2211,r25
	lds r25,2212
	ori r25,lo8(16)
	sts 2212,r25
	lds r25,2211
	ori r25,lo8(4)
	sts 2211,r25
	sts 2208,r18
	lds r25,2212
	andi r25,lo8(-3)
	sts 2212,r25
	subi r24,lo8(-(-1))
	sts g_sboxnet_dev_def+3,r24
	ldi r24,lo8(3)
	sts g_sboxnet_dev_def,r24
	ret
.L40:
	sts g_sboxnet_dev_def,__zero_reg__
	ret
.L39:
	cpi r24,lo8(2)
	brne .L44
	ldi r24,lo8(1)
	sts 1612,r24
	ldi r24,lo8(8)
	sts 1605,r24
	ldi r24,lo8(4)
	sts g_sboxnet_dev_def,r24
	rjmp .L56
.L44:
	cpi r24,lo8(4)
	brne .L38
	lds r24,1612
	sbrs r24,0
	rjmp .L45
	lds r24,1608
	sbrs r24,2
	rjmp .L45
	call sboxnet_receiver_enable
	sts g_sboxnet_dev_def,__zero_reg__
	jmp sboxnet_check_try_transmit
.L45:
	ldi r24,lo8(1)
	sts 1612,r24
.L56:
	ldi r24,lo8(16)
	ldi r25,0
.L55:
	jmp sboxnet_set_timer
.L38:
	ret
	.size	sboxnet_timer_bit, .-sboxnet_timer_bit
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
	push r30
	push r31
	push r28
	push r29
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: Signal */
/* frame size = 1 */
/* stack size = 20 */
.L__stack_usage = 20
	ldi r16,lo8(g_sboxnet_dev_def)
	ldi r17,hi8(g_sboxnet_dev_def)
	lds r18,2209
	std Y+1,r18
	ldd r19,Y+1
	lds r24,2208
	mov r25,r18
	andi r25,lo8(28)
	std Y+1,r25
	movw r30,r16
	tst r25
	breq .L58
	subi r30,45
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	sbrs r18,4
	rjmp .L59
	movw r30,r16
	subi r30,43
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
.L59:
	sbrs r18,3
	rjmp .L60
	movw r30,r16
	subi r30,39
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
.L60:
	sbrs r18,2
	rjmp .L62
	movw r30,r16
	subi r30,41
	sbci r31,-1
	rjmp .L85
.L58:
	ldd r25,Z+10
	sbrs r19,0
	rjmp .L63
	tst r25
	breq .L64
	subi r30,37
	sbci r31,-1
	ld r18,Z
	ldd r19,Z+1
	subi r18,-1
	sbci r19,-1
	st Z,r18
	std Z+1,r19
	movw r30,r16
	subi r30,45
	sbci r31,-1
	ld r18,Z
	ldd r19,Z+1
	subi r18,-1
	sbci r19,-1
	st Z,r18
	std Z+1,r19
	ldi r30,lo8(g_sboxnet_dev_def+8)
	ldi r31,hi8(g_sboxnet_dev_def+8)
	std Z+2,__zero_reg__
	ldd r25,Z+3
	std Z+1,r25
.L64:
	lds r25,g_dev_addr
	cp r24,r25
	breq .L65
	cpi r24,lo8(-1)
	breq .+2
	rjmp .L62
.L65:
	lds r25,2212
	andi r25,lo8(-3)
	sts 2212,r25
	call sboxnet_recv_buf_write
	movw r30,r16
	tst r24
	breq .L84
	subi r30,63
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	ldi r24,lo8(5)
	movw r30,r16
	std Z+6,r24
	rjmp .L57
.L63:
	movw r30,r16
	ldd r18,Z+6
	cp r25,r18
	brlo .L70
	subi r30,35
	sbci r31,-1
	rjmp .L83
.L70:
	cpi r25,lo8(2)
	brne .L71
	mov r25,r24
	andi r25,lo8(15)
	subi r25,lo8(-(5))
	movw r30,r16
	std Z+6,r25
.L71:
	call sboxnet_recv_buf_write
	movw r30,r16
	cpse r24,__zero_reg__
	rjmp .L72
.L84:
	subi r30,39
	sbci r31,-1
.L83:
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
.L69:
	movw r30,r16
	subi r30,45
	sbci r31,-1
	rjmp .L85
.L72:
	subi r30,63
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	movw r30,r16
	ldd r25,Z+10
	ldd r24,Z+6
	cpse r25,r24
	rjmp .L57
	std Z+6,__zero_reg__
	lds r24,2212
	ori r24,lo8(2)
	sts 2212,r24
	lds r25,g_sboxnet_dev_def+12
	lds r24,g_sboxnet_dev_def+10
	add r24,r25
	sts g_sboxnet_dev_def+12,r24
	sts g_sboxnet_dev_def+10,__zero_reg__
	lds r24,g_sboxnet_dev_def+9
	sts g_sboxnet_dev_def+11,r24
	rjmp .L57
.L85:
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
.L62:
	movw r30,r16
	std Z+6,__zero_reg__
	lds r24,2212
	ori r24,lo8(2)
	sts 2212,r24
	ldi r30,lo8(g_sboxnet_dev_def+8)
	ldi r31,hi8(g_sboxnet_dev_def+8)
	std Z+2,__zero_reg__
	ldd r24,Z+3
	std Z+1,r24
.L57:
/* epilogue start */
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
	pop r16
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_25, .-__vector_25
.global	__vector_27
	.type	__vector_27, @function
__vector_27:
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
	push r29
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 17 */
.L__stack_usage = 17
	ldi r28,lo8(g_sboxnet_dev_def)
	ldi r29,hi8(g_sboxnet_dev_def)
	lds r24,2209
	mov r19,r24
	andi r19,lo8(20)
	lds r20,2208
	ldd r25,Y+3
	movw r30,r28
	add r30,r25
	adc r31,__zero_reg__
	subi r30,83
	sbci r31,-1
	ld r25,Z
	ldi r18,lo8(1)
	cpse r20,r25
	rjmp .L87
	ldi r18,0
.L87:
	sbrs r24,7
	rjmp .L88
	tst r19
	breq .L89
.L92:
	movw r30,r28
	subi r30,51
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	rjmp .L90
.L89:
	tst r18
	brne .+2
	rjmp .L91
	rjmp .L90
.L88:
	movw r30,r28
	subi r30,53
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	cpse r19,__zero_reg__
	rjmp .L92
.L90:
	tst r18
	breq .L94
	movw r30,r28
	subi r30,49
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
.L94:
	ldi r24,lo8(8)
	sts 1606,r24
	lds r24,2212
	andi r24,lo8(-25)
	sts 2212,r24
	lds r24,2211
	andi r24,lo8(-13)
	sts 2211,r24
	ldi r24,lo8(2)
	st Y,r24
	movw r30,r28
	subi r30,55
	sbci r31,-1
	ld r24,Z
	ldd r25,Z+1
	adiw r24,1
	st Z,r24
	std Z+1,r25
	ldd r24,Y+2
	tst r24
	breq .L95
	subi r24,lo8(-(-1))
	std Y+2,r24
	ldd r24,Y+4
	std Y+3,r24
	subi r28,47
	sbci r29,-1
	rjmp .L103
.L95:
	std Y+4,__zero_reg__
	std Y+3,__zero_reg__
	subi r28,31
	sbci r29,-1
.L103:
	ld r24,Y
	ldd r25,Y+1
	adiw r24,1
	st Y,r24
	std Y+1,r25
	ldi r24,lo8(64)
	ldi r25,0
	call sboxnet_set_timer
	rjmp .L86
.L91:
	lds r24,2212
	andi r24,lo8(-17)
	sts 2212,r24
	lds r24,2211
	andi r24,lo8(-49)
	sts 2211,r24
	lds r24,2212
	ori r24,lo8(16)
	sts 2212,r24
	ldd r24,Y+3
	tst r24
	breq .L98
	subi r24,lo8(-(-1))
	std Y+3,r24
	lds r24,2212
	andi r24,lo8(-2)
	sts 2212,r24
	ldd r24,Y+3
	movw r30,r28
	add r30,r24
	adc r31,__zero_reg__
	subi r30,83
	sbci r31,-1
	ld r24,Z
	sts 2208,r24
	rjmp .L99
.L98:
	lds r24,2211
	andi r24,lo8(-13)
	sts 2211,r24
	std Y+4,__zero_reg__
	call sboxnet_receiver_enable
	ld r24,Y
	cpi r24,lo8(3)
	brne .L99
	st Y,__zero_reg__
.L99:
	subi r28,59
	sbci r29,-1
	ld r24,Y
	ldd r25,Y+1
	adiw r24,1
	st Y,r24
	std Y+1,r25
.L86:
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
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_27, .-__vector_27
.global	sboxnet_receive_msg
	.type	sboxnet_receive_msg, @function
sboxnet_receive_msg:
	push r16
	push r17
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 4 */
.L__stack_usage = 4
	movw r28,r24
	in r20,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	lds r24,g_sboxnet_dev_def+12
	cpi r24,lo8(5)
	brsh .L116
	ldi r17,0
	rjmp .L106
.L116:
	movw r26,r28
	ldi r19,lo8(5)
	ldi r17,0
.L105:
	lds r24,g_sboxnet_dev_def+12
	tst r24
	breq .L106
	cp r17,r19
	brsh .L106
	subi r24,lo8(-(-1))
	sts g_sboxnet_dev_def+12,r24
	lds r24,g_sboxnet_dev_def+8
	ldi r25,0
	subi r24,lo8(-(g_sboxnet_dev_def+8))
	sbci r25,hi8(-(g_sboxnet_dev_def+8))
	movw r30,r24
	ldd r18,Z+5
	cpi r17,lo8(2)
	brne .L107
	mov r24,r18
	andi r24,lo8(15)
	add r19,r24
.L107:
	movw r24,r26
	adiw r24,1
	st X,r18
	lds r18,g_sboxnet_dev_def+8
	subi r18,lo8(-(1))
	cpi r18,lo8(-96)
	brsh .L108
	sts g_sboxnet_dev_def+8,r18
	rjmp .L109
.L108:
	sts g_sboxnet_dev_def+8,__zero_reg__
.L109:
	subi r17,lo8(-(1))
	movw r26,r24
	rjmp .L105
.L106:
	out __SREG__,r20
	tst r17
	breq .L117
	ldi r24,lo8(-1)
	ldi r16,0
.L112:
	cp r16,r17
	brsh .L124
	cpi r16,lo8(4)
	breq .L113
	movw r30,r28
	add r30,r16
	adc r31,__zero_reg__
	ld r22,Z
	call sboxnet_crc8_add
.L113:
	subi r16,lo8(-(1))
	rjmp .L112
.L124:
	ldd r22,Y+4
	call sboxnet_crc8_add
	tst r24
	breq .L115
	lds r24,g_sboxnet_dev_def+211
	lds r25,g_sboxnet_dev_def+211+1
	adiw r24,1
	sts g_sboxnet_dev_def+211,r24
	sts g_sboxnet_dev_def+211+1,r25
	lds r24,g_sboxnet_dev_def+223
	lds r25,g_sboxnet_dev_def+223+1
	adiw r24,1
	sts g_sboxnet_dev_def+223,r24
	sts g_sboxnet_dev_def+223+1,r25
	ldi r24,lo8(2)
	rjmp .L111
.L115:
	lds r24,g_sboxnet_dev_def+195
	lds r25,g_sboxnet_dev_def+195+1
	adiw r24,1
	sts g_sboxnet_dev_def+195,r24
	sts g_sboxnet_dev_def+195+1,r25
	ldi r24,0
	rjmp .L111
.L117:
	ldi r24,lo8(1)
.L111:
/* epilogue start */
	pop r29
	pop r28
	pop r17
	pop r16
	ret
	.size	sboxnet_receive_msg, .-sboxnet_receive_msg
.global	sboxnet_send_msg
	.type	sboxnet_send_msg, @function
sboxnet_send_msg:
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
/* stack size = 9 */
.L__stack_usage = 9
	movw r28,r24
.L126:
	lds r24,g_sboxnet_dev_def+4
	cpse r24,__zero_reg__
	rjmp .L126
	lds r24,g_dev_addr
	std Y+1,r24
	ldd r13,Y+2
	mov r24,r13
	andi r24,lo8(15)
	mov r13,r24
	ldi r25,lo8(5)
	add r13,r25
	mov r14,r13
	mov r15,__zero_reg__
	movw r16,r14
	subi r16,lo8(-(g_sboxnet_dev_def+173))
	sbci r17,hi8(-(g_sboxnet_dev_def+173))
	mov r12,__zero_reg__
	ldi r24,lo8(-1)
.L128:
	movw r30,r28
	add r30,r12
	adc r31,__zero_reg__
	ld r11,Z
	ldi r30,lo8(4)
	cp r12,r30
	breq .L127
	mov r22,r11
	call sboxnet_crc8_add
.L127:
	movw r30,r16
	st -Z,r11
	movw r16,r30
	inc r12
	cp r12,r13
	brlo .L128
	movw r30,r14
	subi r30,lo8(-(g_sboxnet_dev_def+168))
	sbci r31,hi8(-(g_sboxnet_dev_def+168))
	st Z,r24
	in r28,__SREG__
/* #APP */
 ;  50 "/usr/avr/include/util/atomic.h" 1
	cli
 ;  0 "" 2
/* #NOAPP */
	sts g_sboxnet_dev_def+4,r13
	sts g_sboxnet_dev_def+3,r13
	ldi r24,lo8(25)
	sts g_sboxnet_dev_def+2,r24
	call sboxnet_check_try_transmit
	out __SREG__,r28
	lds r24,g_sboxnet_dev_def+199
	lds r25,g_sboxnet_dev_def+199+1
	adiw r24,1
	sts g_sboxnet_dev_def+199,r24
	sts g_sboxnet_dev_def+199+1,r25
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
	ret
	.size	sboxnet_send_msg, .-sboxnet_send_msg
.global	__vector_14
	.type	__vector_14, @function
__vector_14:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r24
	push r25
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 6 */
.L__stack_usage = 6
	lds r24,g_lcd_timer
	lds r25,g_lcd_timer+1
	adiw r24,1
	sts g_lcd_timer,r24
	sts g_lcd_timer+1,r25
	lds r24,g_lcd_timer
	lds r25,g_lcd_timer+1
	cpi r24,-112
	sbci r25,1
	brlo .L133
	sts g_lcd_timer,__zero_reg__
	sts g_lcd_timer+1,__zero_reg__
	ldi r24,lo8(1)
	sts g_lcd_timedout,r24
.L133:
	lds r24,g_led_timer
	lds r25,g_led_timer+1
	adiw r24,1
	sts g_led_timer,r24
	sts g_led_timer+1,r25
	lds r24,g_led_timer
	lds r25,g_led_timer+1
	cpi r24,-24
	sbci r25,3
	brlo .L132
	sts g_led_timer,__zero_reg__
	sts g_led_timer+1,__zero_reg__
	ldi r24,lo8(32)
	sts 1543,r24
.L132:
/* epilogue start */
	pop r25
	pop r24
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_14, .-__vector_14
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
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	call sboxnet_timer_bit
/* epilogue start */
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
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_16, .-__vector_16
.global	init
	.type	init, @function
init:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
/* #APP */
 ;  135 "test-sboxnet.c" 1
	wdr
 ;  0 "" 2
/* #NOAPP */
	sts 1536,__zero_reg__
	ldi r25,lo8(-1)
	sts 176,r25
	ldi r24,lo8(24)
	sts 1552,r24
	sts 1568,__zero_reg__
	sts 176,r25
	sts 1584,r24
	sts 1600,__zero_reg__
	sts 176,r25
	sts 1616,r24
	sts 1632,__zero_reg__
	sts 176,r25
	sts 1648,r24
	sts 1664,__zero_reg__
	sts 176,r25
	sts 1680,r24
	lds r24,80
	ori r24,lo8(2)
	sts 80,r24
.L137:
	lds r24,81
	sbrs r24,1
	rjmp .L137
	ldi r24,lo8(-40)
	out __CCP__,r24
	ldi r18,lo8(1)
	sts 64,r18
	sts 2049,__zero_reg__
	sts 2051,__zero_reg__
	sts 2052,__zero_reg__
	sts 2054,r18
	sts 2055,__zero_reg__
	ldi r24,lo8(-1)
	sts 2060,r24
	ldi r24,lo8(-1)
	ldi r25,0
	sts 2086,r24
	sts 2086+1,r25
	ldi r24,lo8(5)
	sts 2048,r24
	sts 162,r18
	ldi r24,lo8(32)
	sts 1537,r24
	sts 1542,r24
	ret
	.size	init, .-init
	.section	.text.startup,"ax",@progbits
.global	main
	.type	main, @function
main:
	push r28
	push r29
	in r28,__SP_L__
	in r29,__SP_H__
	sbiw r28,20
	out __SP_L__,r28
	out __SP_H__,r29
/* prologue: function */
/* frame size = 20 */
/* stack size = 22 */
.L__stack_usage = 22
	call init
	ldi r24,0
	ldi r25,0
	call __eerd_byte_x16a4
	sts g_dev_addr,r24
	call lcd_ks0070_init
	call sboxnet_init
	call sboxnet_receiver_enable
/* #APP */
 ;  215 "test-sboxnet.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r18,lo8(639999)
	ldi r24,hi8(639999)
	ldi r25,hlo8(639999)
	1: subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ldi r16,0
	ldi r17,0
.L143:
	movw r24,r28
	adiw r24,1
	call sboxnet_receive_msg
	cpse r24,__zero_reg__
	rjmp .L144
	ldi r24,lo8(15)
	movw r30,r28
	adiw r30,6
	ldi r26,lo8(buf.4369)
	ldi r27,hi8(buf.4369)
	0:
	ld r0,Z+
	st X+,r0
	dec r24
	brne 0b
	rjmp .L143
.L144:
	cpi r24,lo8(1)
	brne .L143
	lds r24,g_sboxnet_dev_def+4
	cpse r24,__zero_reg__
	rjmp .L146
	lds r24,g_dev_addr
	cpi r24,lo8(11)
	brne .L153
	ldi r24,lo8(10)
	rjmp .L147
.L153:
	ldi r24,lo8(11)
.L147:
	std Y+1,r24
	ldi r24,lo8(5)
	std Y+3,r24
	ldi r24,lo8(90)
	std Y+4,r24
	ldi r24,lo8(72)
	std Y+6,r24
	ldi r24,lo8(101)
	std Y+7,r24
	ldi r24,lo8(108)
	std Y+8,r24
	std Y+9,r24
	ldi r24,lo8(111)
	std Y+10,r24
	movw r24,r28
	adiw r24,1
	call sboxnet_send_msg
.L146:
	lds r24,g_lcd_timedout
	tst r24
	breq .L143
	sts g_lcd_timedout,__zero_reg__
	subi r17,lo8(-(1))
	cpi r17,lo8(10)
	brlo .L149
	ldi r24,lo8(1)
	cpse r16,__zero_reg__
	ldi r24,0
.L150:
	mov r16,r24
	call lcd_ks0070_display_clear
	ldi r17,0
.L149:
	ldi r22,0
	ldi r24,0
	tst r16
	breq .L151
	call lcd_set_cursor
	ldi r24,lo8(__c.4374)
	ldi r25,hi8(__c.4374)
	call lcd_str_P
	ldi r22,0
	ldi r24,0
	call lcd_set_cursor
	lds r24,buf.4369
	call lcd_ks0070_data
	lds r24,buf.4369+1
	call lcd_ks0070_data
	lds r24,buf.4369+2
	call lcd_ks0070_data
	lds r24,buf.4369+3
	call lcd_ks0070_data
	lds r24,buf.4369+4
	call lcd_ks0070_data
	rjmp .L143
.L151:
	call lcd_set_cursor
	ldi r24,lo8(__c.4376)
	ldi r25,hi8(__c.4376)
	call lcd_str_P
	ldi r22,0
	ldi r24,0
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+195
	lds r25,g_sboxnet_dev_def+195+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,0
	ldi r24,lo8(2)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+193
	lds r25,g_sboxnet_dev_def+193+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,lo8(1)
	ldi r24,0
	call lcd_set_cursor
	ldi r24,lo8(__c.4378)
	ldi r25,hi8(__c.4378)
	call lcd_str_P
	ldi r22,lo8(1)
	ldi r24,0
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+199
	lds r25,g_sboxnet_dev_def+199+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,lo8(1)
	ldi r24,lo8(2)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+197
	lds r25,g_sboxnet_dev_def+197+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,0
	ldi r24,lo8(5)
	call lcd_set_cursor
	ldi r24,lo8(__c.4380)
	ldi r25,hi8(__c.4380)
	call lcd_str_P
	ldi r22,0
	ldi r24,lo8(5)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+201
	lds r25,g_sboxnet_dev_def+201+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,lo8(1)
	ldi r24,lo8(5)
	call lcd_set_cursor
	ldi r24,lo8(__c.4382)
	ldi r25,hi8(__c.4382)
	call lcd_str_P
	ldi r22,lo8(1)
	ldi r24,lo8(5)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+209
	lds r25,g_sboxnet_dev_def+209+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,0
	ldi r24,lo8(8)
	call lcd_set_cursor
	ldi r24,lo8(__c.4384)
	ldi r25,hi8(__c.4384)
	call lcd_str_P
	ldi r22,0
	ldi r24,lo8(8)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+211
	lds r25,g_sboxnet_dev_def+211+1
	andi r24,63
	clr r25
	call lcd_itoa
	ldi r22,0
	ldi r24,lo8(10)
	call lcd_set_cursor
	ldi r24,lo8(__c.4386)
	ldi r25,hi8(__c.4386)
	call lcd_str_P
	ldi r22,0
	ldi r24,lo8(10)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+223
	lds r25,g_sboxnet_dev_def+223+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+213
	lds r25,g_sboxnet_dev_def+213+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+215
	lds r25,g_sboxnet_dev_def+215+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+217
	lds r25,g_sboxnet_dev_def+217+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+219
	lds r25,g_sboxnet_dev_def+219+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+221
	lds r25,g_sboxnet_dev_def+221+1
	andi r24,7
	clr r25
	call lcd_itoa
	ldi r22,lo8(1)
	ldi r24,lo8(8)
	call lcd_set_cursor
	ldi r24,lo8(__c.4388)
	ldi r25,hi8(__c.4388)
	call lcd_str_P
	ldi r22,lo8(1)
	ldi r24,lo8(8)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+225
	lds r25,g_sboxnet_dev_def+225+1
	clr r25
	call lcd_itoa
	ldi r22,lo8(1)
	ldi r24,lo8(13)
	call lcd_set_cursor
	ldi r24,lo8(__c.4390)
	ldi r25,hi8(__c.4390)
	call lcd_str_P
	ldi r22,lo8(1)
	ldi r24,lo8(13)
	call lcd_set_cursor
	lds r24,g_sboxnet_dev_def+203
	lds r25,g_sboxnet_dev_def+203+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+205
	lds r25,g_sboxnet_dev_def+205+1
	andi r24,7
	clr r25
	call lcd_itoa
	lds r24,g_sboxnet_dev_def+207
	lds r25,g_sboxnet_dev_def+207+1
	andi r24,7
	clr r25
	call lcd_itoa
	rjmp .L143
	.size	main, .-main
	.section	.rodata
	.type	CSWTCH.45, @object
	.size	CSWTCH.45, 3
CSWTCH.45:
	.byte	64
	.byte	20
	.byte	84
	.section	.progmem.data,"a",@progbits
	.type	__c.4390, @object
	.size	__c.4390, 5
__c.4390:
	.string	"    "
	.type	__c.4388, @object
	.size	__c.4388, 5
__c.4388:
	.string	"    "
	.type	__c.4386, @object
	.size	__c.4386, 5
__c.4386:
	.string	"    "
	.type	__c.4384, @object
	.size	__c.4384, 5
__c.4384:
	.string	"    "
	.type	__c.4382, @object
	.size	__c.4382, 4
__c.4382:
	.string	"   "
	.type	__c.4380, @object
	.size	__c.4380, 4
__c.4380:
	.string	"   "
	.type	__c.4378, @object
	.size	__c.4378, 5
__c.4378:
	.string	"    "
	.type	__c.4376, @object
	.size	__c.4376, 5
__c.4376:
	.string	"    "
	.type	__c.4374, @object
	.size	__c.4374, 8
__c.4374:
	.string	"       "
	.data
	.type	buf.4369, @object
	.size	buf.4369, 16
buf.4369:
	.string	"nixda"
	.zero	10
.global	g_led_timer
	.section .bss
	.type	g_led_timer, @object
	.size	g_led_timer, 2
g_led_timer:
	.zero	2
.global	g_lcd_timedout
	.type	g_lcd_timedout, @object
	.size	g_lcd_timedout, 1
g_lcd_timedout:
	.zero	1
.global	g_lcd_timer
	.type	g_lcd_timer, @object
	.size	g_lcd_timer, 2
g_lcd_timer:
	.zero	2
	.comm	g_sboxnet_dev_def,227,1
	.local	g_dev_addr
	.comm	g_dev_addr,1,1
	.ident	"GCC: (Gentoo 4.8.3 p1.1, pie-0.5.9) 4.8.3"
.global __do_copy_data
.global __do_clear_bss
