/* Implement timing-related runtime actions for CHILL.
   Copyright (C) 1992,1993 Free Software Foundation, Inc.
   Author: Wilfried Moser

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.  */

#include "rts.h"

/*
 * function __convert_duration_rtstime
 *
 * parameters:
 *      dur     the duration value
 *      t       pointer to the duration value converted to RtsTime
 *
 * returns:
 *      void
 *
 * exceptions:
 *      none
 *
 * abstract:
 *      converts a duration value (unsigned long in millisecs) to RtsTime
 *      format.
 *
 */

void
__convert_duration_rtstime (dur, t)
     unsigned long  dur;
     RtsTime       *t;
{
  unsigned long tmp;
    
  t->secs = dur / 1000;
  tmp = dur - (t->secs * 1000);
  t->nanosecs = tmp * 1000000;
}
