/***************************************************************************
 *   Copyright (C) 2008
 *   by Thomas Maier <balagi@justmail.de>
 *
 *   Copyright: See COPYING file that comes with this distribution         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#ifndef _ATTINY2313_H_
#define _ATTINY2313_H_


#include <avr.h>


.equiv SRAM_BEG,	0x60
.equiv SRAM_END,	0xdf
		

.equiv DIDR,	0x01
.equiv B_AIN0D,	0
.equiv B_AIN1D,	1
		
.equiv UBRRH,	0x02
		
.equiv UCSRC,	0x03
.equiv B_UCPOL,	0
.equiv B_UCSZ0,	1
.equiv B_UCSZ1,	2
.equiv B_USBS,	3
.equiv B_UPM0,	4
.equiv B_UPM1,	5
.equiv B_UMSEL,	6
		
.equiv ACSR,	0x08
.equiv B_ACIS0,	0
.equiv B_ACIS1,	1
.equiv B_ACIC,	2
.equiv B_ACIE,	3
.equiv B_ACI,	4
.equiv B_ACO,	5
.equiv B_ACBG,	6
.equiv B_ACD,	7
		
.equiv UBRRL,	0x09
		
.equiv UCSRB,	0x0a
.equiv B_TXB8,	0
.equiv B_RXB8,	1
.equiv B_UCSZ2,	2
.equiv B_TXEN,	3
.equiv B_RXEN,	4
.equiv B_UDRIE,	5
.equiv B_TXCIE,	6
.equiv B_RXCIE,	7		

.equiv UCSRA,	0x0b
.equiv B_MPCM,	0
.equiv B_U2X,	1
.equiv B_UPE,	2
.equiv B_DOR,	3
.equiv B_FE,	4
.equiv B_UDRE,	5
.equiv B_TXC,	6
.equiv B_RXC,	7
		
.equiv UDR,	0x0c
		
.equiv USICR,	0x0d
.equiv B_USITC,		0
.equiv B_USICLK,	1
.equiv B_USICS0,	2
.equiv B_USICS1,	3
.equiv B_USIWM0,	4
.equiv B_USIWM1,	5
.equiv B_USIOIE,	6
.equiv B_USISIE,	7
		
.equiv USISR,	0x0e
.equiv B_USICNT0,	0
.equiv B_USICNT1,	1
.equiv B_USICNT2,	2
.equiv B_USICNT3,	3
.equiv B_USIDC,		4
.equiv B_USIPF,		5
.equiv B_USIOIF,	6
.equiv B_USISIF,	7
		
.equiv USIDR,	0x0f
		
.equiv PIND,	0x10
.equiv B_PIND0,	0
.equiv B_PIND1,	1
.equiv B_PIND2,	2
.equiv B_PIND3,	3
.equiv B_PIND4,	4
.equiv B_PIND5,	5
.equiv B_PIND6,	6

.equiv DDRD,	0x11
.equiv B_DDD0,	0
.equiv B_DDD1,	1
.equiv B_DDD2,	2
.equiv B_DDD3,	3
.equiv B_DDD4,	4
.equiv B_DDD5,	5
.equiv B_DDD6,	6
		
.equiv PORTD,	0x12
.equiv B_PORTD0,	0
.equiv B_PORTD1,	1
.equiv B_PORTD2,	2
.equiv B_PORTD3,	3
.equiv B_PORTD4,	4
.equiv B_PORTD5,	5
.equiv B_PORTD6,	6
		
.equiv GPIOR0,	0x13
.equiv GPIOR1,	0x14
.equiv GPIOR2,	0x15
		
.equiv PINB,	0x16
.equiv B_PINB0,	0
.equiv B_PINB1,	1
.equiv B_PINB2,	2
.equiv B_PINB3,	3
.equiv B_PINB4,	4
.equiv B_PINB5,	5
.equiv B_PINB6,	6
.equiv B_PINB7,	7
		
.equiv DDRB,	0x17
.equiv B_DDB0,	0
.equiv B_DDB1,	1
.equiv B_DDB2,	2
.equiv B_DDB3,	3
.equiv B_DDB4,	4
.equiv B_DDB5,	5
.equiv B_DDB6,	6
.equiv B_DDB7,	7
		
.equiv PORTB,	0x18
.equiv B_PORTB0,	0
.equiv B_PORTB1,	1
.equiv B_PORTB2,	2
.equiv B_PORTB3,	3
.equiv B_PORTB4,	4
.equiv B_PORTB5,	5
.equiv B_PORTB6,	6
.equiv B_PORTB7,	7
		
.equiv PINA,	0x19
.equiv B_PINA0,	0
.equiv B_PINA1,	1
.equiv B_PINA2,	2

.equiv DDRA,	0x1a
.equiv B_DDA0,	0
.equiv B_DDA1,	1
.equiv B_DDA2,	2

.equiv PORTA,	0x1b
.equiv B_PORTA0,	0
.equiv B_PORTA1,	1
.equiv B_PORTA2,	2
		
.equiv EECR,	0x1c
.equiv B_EERE,	0
.equiv B_EEPE,	1
.equiv B_EEMPE,	2
.equiv B_EERIE,	3
.equiv B_EEPM0,	4
.equiv B_EEPM1,	5
		
.equiv EEDR,	0x1d
.equiv EEAR,	0x1e
		
.equiv PCMSK,	0x20
.equiv B_PCINT0,	0
.equiv B_PCINT1,	1
.equiv B_PCINT2,	2
.equiv B_PCINT3,	3
.equiv B_PCINT4,	4
.equiv B_PCINT5,	5
.equiv B_PCINT6,	6
.equiv B_PCINT7,	7
		
.equiv WDTCSR,	0x21
.equiv B_WDP0,	0
.equiv B_WDP1,	1
.equiv B_WDP2,	2
.equiv B_WDE,	3
.equiv B_WDCE,	4
.equiv B_WDP3,	5
.equiv B_WDIE,	6
.equiv B_WDIF,	7
		
.equiv TCCR1C,	0x22
.equiv B_FOC1B,	6
.equiv B_FOC1A,	7
		
.equiv GTCCR,	0x23
.equiv B_PSR10,	0
		
.equiv ICR1L,	0x24
.equiv ICR1H,	0x25
		
.equiv CLKPR,	0x26
.equiv B_CLKPS0,	0
.equiv B_CLKPS1,	1
.equiv B_CLKPS2,	2
.equiv B_CLKPS3,	3
.equiv B_CLKPCE,	7
		
.equiv OCR1BL,	0x28
.equiv OCR1BH,	0x29
.equiv OCR1AL,	0x2a
.equiv OCR1AH,	0x2b
.equiv TCNT1L,	0x2c
.equiv TCNT1H,	0x2d
		
.equiv TCCR1B,	0x2e
.equiv B_CS10,	0
.equiv B_CS11,	1
.equiv B_CS12,	2
.equiv B_WGM12,	3
.equiv B_WGM13,	4
.equiv B_ICES1,	6
.equiv B_ICNC1,	7
		
.equiv TCCR1A,	0x2f
.equiv B_WGM10,		0
.equiv B_WGM11,		1
.equiv B_COM1B0,	4
.equiv B_COM1B1,	5
.equiv B_COM1A0,	6
.equiv B_COM1A1,	7
		
.equiv TCCR0A,	0x30
.equiv B_WGM00,		0
.equiv B_WGM01,		1
.equiv B_COM0B0,	4
.equiv B_COM0B1,	5
.equiv B_COM0A0,	6
.equiv B_COM0A1,	7
		
.equiv OSCCAL,	0x31
		
.equiv TCNT0,	0x32
		
.equiv TCCR0B,	0x33
.equiv B_CS00,	0
.equiv B_CS01,	1
.equiv B_CS02,	2
.equiv B_WGM02,	3
.equiv B_FOC0B,	6
.equiv B_FOC0A,	7
		
.equiv MCUSR,	0x34
.equiv B_PORF,	0
.equiv B_EXTRF,	1
.equiv B_BORF,	2
.equiv B_WDRF,	3
		
.equiv MCUCR,	0x35
.equiv B_ISC00,	0
.equiv B_ISC01,	1
.equiv B_ISC10,	2
.equiv B_ISC11,	3
.equiv B_SM0,	4
.equiv B_SE,	5
.equiv B_SM1,	6
.equiv B_PUD,	7
		
.equiv OCR0A,	0x36

		
.equiv SPMCSR,	0x37
.equiv B_SELFPRGEN,	0
.equiv B_PGERS,	1
.equiv B_PGWRT,	2
.equiv B_RFLB,	3
.equiv B_CTPB,	4
		
.equiv TIFR,	0x38
.equiv B_OCF0A,	0
.equiv B_TOV0,	1
.equiv B_OCF0B,	2
.equiv B_ICF1,	3
.equiv B_OCF1B,	5
.equiv B_OCF1A,	6
.equiv B_TOV1,	7
		
.equiv TIMSK,	0x39
.equiv B_OCIE0A,	0
.equiv B_TOIE0,		1
.equiv B_OCIE0B,	2
.equiv B_ICIE1,		3
.equiv B_OCIE1B,	5
.equiv B_OCIE1A,	6
.equiv B_TOIE1,		7
		
.equiv EIFR,	0x3a
.equiv B_PCIF,	5
.equiv B_INTF0,	6
.equiv B_INTF1,	7
		
.equiv GIMSK,	0x3b
.equiv B_PCIE,	5
.equiv B_INT0,	6
.equiv B_INT1,	7
		
.equiv OCR0B,	0x3c
		
.equiv SPL,	0x3d
		
.equiv SREG,	0x3f
.equiv B_C,	0
.equiv B_Z,	1
.equiv B_N,	2
.equiv B_V,	3
.equiv B_S,	4
.equiv B_H,	5
.equiv B_T,	6
.equiv B_I,	7


#endif /* _ATTINY2313_H_ */
