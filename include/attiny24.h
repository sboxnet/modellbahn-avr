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

#ifndef _ATTINY24_H_
#define _ATTINY24_H_


#include <avr.h>


.equiv SRAM_BEG,	0x60
.equiv SRAM_END,	0xdf
		

.equiv PPR,		0x00
.equiv B_PRADC,	0
.equiv B_PRUSI,	1
.equiv B_PRTIM0,	2
.equiv B_PRTIM1,	3

.equiv DIDR0,	0x01
.equiv B_ADC0D,	0
.equiv B_ADC1D,	1
.equiv B_ADC2D,	2
.equiv B_ADC3D,	3
.equiv B_ADC4D,	4
.equiv B_ADC5D,	5
.equiv B_ADC6D,	6
.equiv B_ADC7D,	7
				
.equiv ADCSRB,	0x03
.equiv B_ADTS0,	0
.equiv B_ADTS1,	1
.equiv B_ADTS2,	2
.equiv B_ADLAR,	4
.equiv B_ACME,	6
.equiv B_BIN,	7

.equiv ADCL,	0x04

.equiv ADCH,	0x05

.equiv ADCSRA,	0x06
.equiv B_ADPS0,	0
.equiv B_ADPS1,	1
.equiv B_ADPS2,	2
.equiv B_ADIE,	3
.equiv B_ADIF,	4
.equiv B_ADATE,	5
.equiv B_ADSC,	6
.equiv B_ADEN,	7

.equiv ADMUX,	0x07
.equiv B_MUX0,	0
.equiv B_MUX1,	1
.equiv B_MUX2,	2
.equiv B_MUX3,	3
.equiv B_MUX4,	4
.equiv B_MUX5,	5
.equiv B_REFS0,	6
.equiv B_REFS1,	7

.equiv ACSR,	0x08
.equiv B_ACIS0,	0
.equiv B_ACIS1,	1
.equiv B_ACIC,	2
.equiv B_ACIE,	3
.equiv B_ACI,	4
.equiv B_ACO,	5
.equiv B_ACBG,	6
.equiv B_ACD,	7
		
.equiv TIFR1,	0x0b
.equiv B_TOV1,	0
.equiv B_OCF1A,	1
.equiv B_OCF1B,	2
.equiv B_ICF1,	5
		
.equiv TIMSK1,	0x0c
.equiv B_TOIE1,	0
.equiv B_OCIE1A,	1
.equiv B_OCIE1B,	2
.equiv B_ICIE1,	5

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
		
.equiv USIBR,	0x10

.equiv PCMSK0,	0x12
.equiv B_PCINT0,	0
.equiv B_PCINT1,	1
.equiv B_PCINT2,	2
.equiv B_PCINT3,	3
.equiv B_PCINT4,	4
.equiv B_PCINT5,	5
.equiv B_PCINT6,	6
.equiv B_PCINT7,	7

.equiv GPIOR0,	0x13
.equiv GPIOR1,	0x14
.equiv GPIOR2,	0x15
		
.equiv PINB,	0x16
.equiv B_PINB0,	0
.equiv B_PINB1,	1
.equiv B_PINB2,	2
.equiv B_PINB3,	3
		
.equiv DDRB,	0x17
.equiv B_DDB0,	0
.equiv B_DDB1,	1
.equiv B_DDB2,	2
.equiv B_DDB3,	3
		
.equiv PORTB,	0x18
.equiv B_PORTB0,	0
.equiv B_PORTB1,	1
.equiv B_PORTB2,	2
.equiv B_PORTB3,	3
		
.equiv PINA,	0x19
.equiv B_PINA0,	0
.equiv B_PINA1,	1
.equiv B_PINA2,	2
.equiv B_PINA3,	3
.equiv B_PINA4,	4
.equiv B_PINA5,	5
.equiv B_PINA6,	6
.equiv B_PINA7,	7

.equiv DDRA,	0x1a
.equiv B_DDA0,	0
.equiv B_DDA1,	1
.equiv B_DDA2,	2
.equiv B_DDA3,	3
.equiv B_DDA4,	4
.equiv B_DDA5,	5
.equiv B_DDA6,	6
.equiv B_DDA7,	7

.equiv PORTA,	0x1b
.equiv B_PORTA0,	0
.equiv B_PORTA1,	1
.equiv B_PORTA2,	2
.equiv B_PORTA3,	3
.equiv B_PORTA4,	4
.equiv B_PORTA5,	5
.equiv B_PORTA6,	6
.equiv B_PORTA7,	7
		
.equiv EECR,	0x1c
.equiv B_EERE,	0
.equiv B_EEPE,	1
.equiv B_EEMPE,	2
.equiv B_EERIE,	3
.equiv B_EEPM0,	4
.equiv B_EEPM1,	5
		
.equiv EEDR,	0x1d
.equiv EEARL,	0x1e
.equiv EEARH,	0x1f
		
.equiv PCMSK1,	0x20
.equiv B_PCINT8,	0
.equiv B_PCINT9,	1
.equiv B_PCINT10,	2
.equiv B_PCINT11,	3
		
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
.equiv B_TSM,	7

.equiv ICR1L,	0x24
.equiv ICR1H,	0x25
		
.equiv CLKPR,	0x26
.equiv B_CLKPS0,	0
.equiv B_CLKPS1,	1
.equiv B_CLKPS2,	2
.equiv B_CLKPS3,	3
.equiv B_CLKPCE,	7

.equiv DWDR,	0x27

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
.equiv B_BODSE,	2
.equiv B_SM0,	3
.equiv B_SM1,	4
.equiv B_SE,	5
.equiv B_PUD,	6
.equiv B_BODS,	7
		
.equiv OCR0A,	0x36

		
.equiv SPMCSR,	0x37
.equiv B_SPMEN,	0
.equiv B_PGERS,	1
.equiv B_PGWRT,	2
.equiv B_RFLB,	3
.equiv B_CTPB,	4
		
.equiv TIFR0,	0x38
.equiv B_TOV0,	0
.equiv B_OCF0A,	1
.equiv B_OCF0B,	2
		
.equiv TIMSK0,	0x39
.equiv B_TOIE0,	0
.equiv B_OCIE0A,	1
.equiv B_OCIE0B,	2
		
		
.equiv GIFR,	0x3a
.equiv B_PCIF0,	4
.equiv B_PCIF1,	5
.equiv B_INTF0,	6
		
.equiv GIMSK,	0x3b
.equiv B_PCIE0,	4
.equiv B_PCIE1,	5
.equiv B_INT0,	6
		
.equiv OCR0B,	0x3c
		
.equiv SPL,	0x3d
.equiv SPH, 0x3e
		
.equiv SREG,	0x3f
.equiv B_C,	0
.equiv B_Z,	1
.equiv B_N,	2
.equiv B_V,	3
.equiv B_S,	4
.equiv B_H,	5
.equiv B_T,	6
.equiv B_I,	7


#endif /* _ATTINY24_H_ */
