/***************************************************************************
 *   Copyright (C) 2012-2014
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


#if defined(__SELECT_USART) && __SELECT_USART == 0

# ifdef UDR0
#  define __UDR    UDR0
#  define __UBRRH  UBRR0H
#  define __UBRRL  UBRR0L
#  define __UCSRA  UCSR0A
#  define __UCSRB  UCSR0B
#  define __UCSRC  UCSR0C
#  ifdef UCSR0D
#   define __UCSRD  UCSR0D
#  else
#   undef __UCSRD
#  endif
#  define __RXC    RXC0
#  define __TXC    TXC0
#  define __UDRE   UDRE0
#  define __FE     FE0
#  define __DOR    DOR0
#  ifdef UPE0
#   define __UPE   UPE0
#  else
#   define __UPE   PE0
#  endif
#  define __U2X    U2X0
#  define __MPCM   MPCM0
#  define __RXCIE  RXCIE0
#  define __TXCIE  TXCIE0
#  define __UDRIE  UDRIE0
#  define __RXEN   RXEN0
#  define __TXEN   TXEN0
#  define __UCSZ2  UCSZ02
#  define __RXB8   RXB80
#  define __TXB8   TXB80
#  ifdef URSEL0
#   define __URSEL URSEL0
#  else
#   undef __URSEL
#  endif
#  ifdef UMSEL01
#   define __UMSEL1 UMSEL01
#   define __UMSEL0 UMSEL00
#  else
#   define __UMSEL  UMSEL0
#   define __UMSEL0 UMSEL0
#  endif
#  define __UPM1   UPM01
#  define __UPM0   UPM00
#  define __USBS   USBS0
#  define __UCSZ1  UCSZ01
#  define __UCSZ0  UCSZ00
#  define __UCPOL  UCPOL0
#  define __CTSEN  CTSEN
#  define __RTSEN  RTSEN
#  ifdef USART0_RXC_vect
#   define __USART_RX_vect USART0_RXC_vect
#   define __USART_TX_vect USART0_TXC_vect
#   define __USART_UDRE_vect USART0_UDRE_vect
#  elif defined(USART0_RX_vect)
#   define __USART_RX_vect USART0_RX_vect
#   define __USART_TX_vect USART0_TX_vect
#   define __USART_UDRE_vect USART0_UDRE_vect
#  else
#   define __USART_RX_vect USART_RX_vect
#   define __USART_TX_vect USART_TX_vect
#   define __USART_UDRE_vect USART_UDRE_vect
#  endif
# else
#  define __UDR    UDR
#  define __UBRRH  UBRRH
#  define __UBRRL  UBRRL
#  define __UCSRA  UCSRA
#  define __UCSRB  UCSRB
#  define __UCSRC  UCSRC
#  ifdef UCSRD
#   define __UCSRD  UCSRD
#  else
#   undef __UCSRD
#  endif
#  define __RXC    RXC
#  define __TXC    TXC
#  define __UDRE   UDRE
#  define __FE     FE
#  define __DOR    DOR
#  ifdef UPE
#   define __UPE   UPE
#  else
#   define __UPE   PE
#  endif
#  define __U2X    U2X
#  define __MPCM   MPCM
#  define __RXCIE  RXCIE
#  define __TXCIE  TXCIE
#  define __UDRIE  UDRIE
#  define __RXEN   RXEN
#  define __TXEN   TXEN
#  define __UCSZ2  UCSZ2
#  define __RXB8   RXB8
#  define __TXB8   TXB8
#  ifdef URSEL
#   define __URSEL URSEL
#  else
#   undef __URSEL
#  endif
#  ifdef UMSEL1
#   define __UMSEL1 UMSEL1
#   define __UMSEL0 UMSEL0
#  else
#   define __UMSEL  UMSEL
#   define __UMSEL0 UMSEL
#  endif
#  define __UPM1   UPM1
#  define __UPM0   UPM0
#  define __USBS   USBS
#  define __UCSZ1  UCSZ1
#  define __UCSZ0  UCSZ0
#  define __UCPOL  UCPOL
#  define __CTSEN  CTSEN
#  define __RTSEN  RTSEN
#  ifdef USART_RXC_vect
#   define __USART_RX_vect USART_RXC_vect
#   define __USART_TX_vect USART_TXC_vect
#  else
#   define __USART_RX_vect USART_RX_vect
#   define __USART_TX_vect USART_TX_vect
#  endif
#  define __USART_UDRE_vect USART_UDRE_vect
# endif

#elif __SELECT_USART == 1

# define __UDR    UDR1
# define __UBRRH  UBRR1H
# define __UBRRL  UBRR1L
# define __UCSRA  UCSR1A
# define __UCSRB  UCSR1B
# define __UCSRC  UCSR1C
# ifdef UCSR1D
#  define __UCSRD  UCSR1D
# else
#  undef __UCSRD
# endif
# define __RXC    RXC1
# define __TXC    TXC1
# define __UDRE   UDRE1
# define __FE     FE1
# define __DOR    DOR1
# ifdef UPE1
#  define __UPE   UPE1
# else
#  define __UPE   PE1
# endif
# define __U2X    U2X1
# define __MPCM   MPCM1
# define __RXCIE  RXCIE1
# define __TXCIE  TXCIE1
# define __UDRIE  UDRIE1
# define __RXEN   RXEN1
# define __TXEN   TXEN1
# define __UCSZ2  UCSZ12
# define __RXB8   RXB81
# define __TXB8   TXB81
# ifdef URSEL1
#   define __URSEL URSEL1
# else
#   undef __URSEL
# endif
# ifdef UMSEL11
#   define __UMSEL1 UMSEL11
#   define __UMSEL0 UMSEL10
# else
#   define __UMSEL  UMSEL1
#   define __UMSEL0 UMSEL1
# endif
# define __UPM1   UPM11
# define __UPM0   UPM10
# define __USBS   USBS1
# define __UCSZ1  UCSZ11
# define __UCSZ0  UCSZ10
# define __UCPOL  UCPOL1
# define __CTSEN  CTSEN
# define __RTSEN  RTSEN
# ifdef USART1_RXC_vect
#  define __USART_RX_vect USART1_RXC_vect
#  define __USART_TX_vect USART1_TXC_vect
# else
#  define __USART_RX_vect USART1_RX_vect
#  define __USART_TX_vect USART1_TX_vect
# endif
# define __USART_UDRE_vect USART1_UDRE_vect

#elif __SELECT_USART == 2

# define __UDR    UDR2
# define __UBRRH  UBRR2H
# define __UBRRL  UBRR2L
# define __UCSRA  UCSR2A
# define __UCSRB  UCSR2B
# define __UCSRC  UCSR2C
# ifdef UCSR2D
#  define __UCSRD  UCSR2D
# else
#  undef __UCSRD
# endif
# define __RXC    RXC2
# define __TXC    TXC2
# define __UDRE   UDRE2
# define __FE     FE2
# define __DOR    DOR2
# ifdef UPE2
#  define __UPE   UPE2
# else
#  define __UPE   PE2
# endif
# define __U2X    U2X2
# define __MPCM   MPCM2
# define __RXCIE  RXCIE2
# define __TXCIE  TXCIE2
# define __UDRIE  UDRIE2
# define __RXEN   RXEN2
# define __TXEN   TXEN2
# define __UCSZ2  UCSZ22
# define __RXB8   RXB82
# define __TXB8   TXB82
# ifdef URSEL2
#   define __URSEL URSEL2
# else
#   undef __URSEL
# endif
# ifdef UMSEL21
#   define __UMSEL1 UMSEL21
#   define __UMSEL0 UMSEL20
# else
#   define __UMSEL  UMSEL2
#   define __UMSEL0 UMSEL2
# endif
# define __UPM1   UPM21
# define __UPM0   UPM20
# define __USBS   USBS2
# define __UCSZ1  UCSZ21
# define __UCSZ0  UCSZ20
# define __UCPOL  UCPOL2
# define __CTSEN  CTSEN
# define __RTSEN  RTSEN
# ifdef USART2_RXC_vect
#  define __USART_RX_vect USART2_RXC_vect
#  define __USART_TX_vect USART2_TXC_vect
# else
#  define __USART_RX_vect USART2_RX_vect
#  define __USART_TX_vect USART2_TX_vect
# endif
# define __USART_UDRE_vect USART2_UDRE_vect

#elif !defined(__SELECT_USART) || __SELECT_USART == -1

# undef __SELECT_USART
# undef __UDR
# undef __UBRRH
# undef __UBRRL
# undef __UCSRA
# undef __UCSRB
# undef __UCSRC
# undef __UCSRD
# undef __RXC
# undef __TXC
# undef __UDRE
# undef __FE
# undef __DOR
# undef __UPE
# undef __U2X
# undef __MPCM
# undef __RXCIE
# undef __TXCIE
# undef __UDRIE
# undef __RXEN
# undef __TXEN
# undef __UCSZ2
# undef __RXB8
# undef __TXB8
# undef __URSEL
# undef __UMSEL
# undef __UMSEL1
# undef __UMSEL0
# undef __UPM1
# undef __UPM0
# undef __USBS
# undef __UCSZ1
# undef __UCSZ0
# undef __UCPOL
# undef __CTSEN
# undef __RTSEN
# undef __USART_RX_vect
# undef __USART_TX_vect
# undef __USART_UDRE_vect

#else
# error "__SELECT_USART with unknown value!"
#endif

