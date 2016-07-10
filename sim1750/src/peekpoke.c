/***************************************************************************/
/*                                                                         */
/* Project   :       sim1750 -- Mil-Std-1750 Software Simulator            */
/*                                                                         */
/* Component :        peekpoke.c -- low-level access to memory             */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                                                                         */
/* Author    :      Oliver M. Kellogg, Dornier Satellite Systems,          */
/*                     Dept. RST13, D-81663 Munich, Germany.               */
/* Contact   :            oliver.kellogg@space.otn.dasa.de                 */
/*                                                                         */
/* Disclaimer:                                                             */
/*                                                                         */
/*  This program is free software; you can redistribute it and/or modify   */
/*  it under the terms of the GNU General Public License as published by   */
/*  the Free Software Foundation; either version 2 of the License, or      */
/*  (at your option) any later version.                                    */
/*                                                                         */
/*  This program is distributed in the hope that it will be useful,        */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          */
/*  GNU General Public License for more details.                           */
/*                                                                         */
/*  You should have received a copy of the GNU General Public License      */
/*  along with this program; if not, write to the Free Software            */
/*  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.   */
/*                                                                         */
/***************************************************************************/


#include <stdio.h>

#include "phys_mem.h"
#include "status.h"
#include "utils.h"  /* for problem() */


/* peek() returns FALSE on reading an uninitialized location. */

bool
peek (ulong phys_address, ushort *value)
{
  unsigned page     = (unsigned) (phys_address >> 12);
  unsigned log_addr = (unsigned) (phys_address & 0x0FFF);
  mem_t *memptr;
  ulong was_written;

  if (page > 0xFF)
    problem ("peek: absolute memory address too large");
  if ((memptr = mem[page]) == MNULL)
    {
      if (verbose)
        lprintf ("peek: dynamically allocating page %02X\n", page);
      if ((memptr = mem[page] = (mem_t *) xalloc (1, sizeof (mem_t))) == MNULL)
	problem ("peek: memory allocation request refused by OS\n");
    }
  *value = memptr->word[log_addr];
  was_written = memptr->was_written[log_addr / 32] & (1L << (log_addr % 32));
  return was_written != 0;
}


void
poke (ulong phys_address, ushort value)
{
  unsigned page = (unsigned) (phys_address >> 12);
  unsigned log_addr = (unsigned) (phys_address & 0x0FFF);
  mem_t *memptr;

  if (page > 0xFF)
    problem ("poke: absolute memory address too large");
  if ((memptr = mem[page]) == MNULL)
    {
      if (verbose)
        lprintf ("poke: dynamically allocating page %02X\n", page);
      if ((memptr = mem[page] = (mem_t *) xalloc (1, sizeof (mem_t))) == MNULL)
	problem ("poke: dynamic memory exhausted");
    }
  memptr->word[log_addr] = value;
  memptr->was_written[log_addr / 32] |= 1L << (log_addr % 32);
}


