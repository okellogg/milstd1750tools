/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :    smemacc.c -- simulation-level memory access functions    */
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


#include "arch.h"
#include "peekpoke.h"

ulong
get_phys_address (int bank, ushort as, ushort log_addr)
{
  ushort page = pagereg[bank][(int) as][(int) (log_addr >> 12) & 0xF].ppa;
  return ((ulong) page << 12) | (ulong) (log_addr & 0x0FFF);
}

bool
get_raw (int bank, ushort as, ushort address, ushort *value)
{
  ulong phys_address = get_phys_address (bank, as, address);
  return peek (phys_address, value);
}

void
store_raw (int bank, ushort as, ushort address, ushort value)
{
  ulong phys_address = get_phys_address (bank, as, address);
  poke (phys_address, value);
}

