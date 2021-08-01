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

#ifndef _AVR_H_
#define _AVR_H_


#define ORG_ADDR(_a)  ((_a) * 2)

#define HIGHBYTE(_a)	((_a) >> 8)
#define LOWBYTE(_a)		((_a) & 0x00ff)

#define IO_ADDR(_a)		((_a) + 0x20)



#define rXh		r27
#define rXl		r26
#define rYh		r29
#define rYl		r28
#define rZh		r31
#define rZl		r30




#endif /* _AVR_H_ */
