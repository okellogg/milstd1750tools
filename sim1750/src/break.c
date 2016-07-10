/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :        break.c -- breakpoint handling functions             */
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
#include <ctype.h>
#include <string.h>

#include "arch.h"
#include "status.h"
#include "utils.h"
#include "cmd.h"	/* for function parse_address() */
#include "loadfile.h"	/* for the loadfile_type variable */
#include "tekops.h"	/* for function find_tek_address() */
#include "coffops.h"	/* for function find_coff_address() */

#include "break.h"


/* Maximum number of breakpoints: */
#define MAX_BREAK 64

static struct
  {
    breaktype type;
    ulong addr;
    char *label;
    bool is_active;
  } breakpt[MAX_BREAK];	/* breakpoint array */

int n_breakpts = 0;	/* breakpoint counter */


/* Return breakpoint index if breakpoint found for given
   type/bank/address_state/logical_address, or -1 if no breakpoint found. */
int
find_breakpt (breaktype type, ulong phys_address)
{
  int i = n_breakpts;

  while (i-- > 0)
    {
      if (breakpt[i].is_active && breakpt[i].addr == phys_address
	  && (breakpt[i].type == READ_WRITE || breakpt[i].type == type))
	break;
    }
  return (i);
}

void
set_inactive (int bp_index)
{
  if (bp_index < 0)
    return;
  breakpt[bp_index].is_active = FALSE;
}

void
set_active (int bp_index)
{
  if (bp_index < 0)
    return;
  breakpt[bp_index].is_active = TRUE;
}


int
si_brkset (int argc, char *argv[])
{
  ulong address;
  breaktype type = READ_WRITE;

  if (argc <= 1)
    return error ("address argument missing");
  if (n_breakpts >= MAX_BREAK)
    return error ("too many breakpoints");
  if (parse_address (argv[1], &address) != OKAY)
    {
      if (isalpha (*argv[1]) || *argv[1] == '_')
        {
          long addr = find_address (argv[1]);
          if (addr < 0L)
            return error ("label name not found");
          address = (ulong) addr;
        }
      else
        return error ("invalid address syntax");
    }
  if (argc > 2)
    {
      char c = *argv[2];
      if (c == 'r')
        {
          if (*(argv[2] + 1) != 'w')
            type = READ;
        }
      else if (c == 'w')
        type = WRITE;
      else
        return error ("unknown brkpt. type %s (allowed values: R or W)",
			 argv[2]);
    }
  if (find_breakpt (READ_WRITE, address) >= 0)
    return error ("breakpoint already set");
  breakpt[n_breakpts].type = type;
  breakpt[n_breakpts].addr = address;
  breakpt[n_breakpts].is_active = TRUE;
  n_breakpts++;

  return OKAY;
}

static const char *typestr[] = { "RW", "R", "W" };

int
si_brklist (int argc, char *argv[])
{
  int i;

  if (n_breakpts == 0)
    return error ("no breakpoints set");
  lprintf ("\n\t\tBreakpoint List\n");
  for (i = 0; i < n_breakpts; i++)
    lprintf ("\t%05lX  %s\n", breakpt[i].addr, typestr[breakpt[i].type]);

  return (OKAY);
}


int
si_brkclear (int argc, char *argv[])
{
  int i;
  ulong addr;

  if (argc <= 1)
    return error ("address argument missing");
  if (n_breakpts == 0)
    return error ("no breakpoints set");
  if (*argv[1] == '*')
    {
      n_breakpts = 0;
      return OKAY;
    }
  if (parse_address (argv[1], &addr))
    return info ("invalid address syntax");
  for (i = 0; i < n_breakpts; i++)
    if (breakpt[i].addr == addr)
      break;
  if (i == n_breakpts)
    return info ("\tno breakpoint at that address");
  n_breakpts--;
  while (i < n_breakpts)
    {
      breakpt[i].type = breakpt[i+1].type;
      breakpt[i].addr = breakpt[i+1].addr;
      i++;
    }

  return (OKAY);
}


int
si_brksave (int argc, char *argv[])
{
  int i;
  FILE *savefile;

  if (argc <= 1)
    return error ("filename missing");
  else
    {
      if ((savefile = fopen (argv[1], "w")) == NULL)
	return error (" can't open save-file  \" %s \"", argv[1]);
      else
	{
	  fprintf (savefile, "#\t\tsaved BREAKPOINT LIST\n");
	  for (i = 0; i < n_breakpts; i++)
	    fprintf (savefile, "br  %05lX  %s\n",
		     breakpt[i].addr, typestr[breakpt[i].type]);
	}
    }
  fclose (savefile);
  return (OKAY);
}

