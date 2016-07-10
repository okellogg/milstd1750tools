/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :         exec.c -- control over simulator execution          */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                                                                         */
/* Author    :      Oliver M. Kellogg, Dornier Satellite Systems,          */
/*                     Dept. RST13, D-81663 Munich, Germany.               */
/* Contact   :           oliver.kellogg@space.otn.dasa.de                  */
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

#include "arch.h"
#include "status.h"
#include "cpu.h"
#include "smemacc.h"
#include "break.h"

/* Imports */

extern void  dis_reg ();	/* cmd.c */
extern int   sys_int (long);	/* cmd.c */
extern char *disassemble ();	/* sdisasm.c */


static bool
at_bpt_instruction ()
{
  ushort opcode;
  if (! get_raw (CODE, simreg.sw & 0xF, simreg.ic, &opcode))
    return FALSE;
  return (opcode == 0xFFFF);
}

static int
execute_without_breakpt ()
{
  int status;
  int bpi = bpindex;

  bpindex = -1;
  set_inactive (bpi);
  status = execute ();
  set_active (bpi);
  return status;
}


int
si_go (int argc, char *argv[])
{
  unsigned next;

  if (argc > 1)
    {
      sscanf (argv[1], "%x", &next);
      simreg.ic = (ushort) next;
    }

  if (at_bpt_instruction ())
    simreg.ic++;
  else if (bpindex >= 0)
    execute_without_breakpt ();
  while (1)
    {
      if (sys_int (1L))
	return INTERRUPT;
      if (execute () == MEMERR)
	break;
      if (at_bpt_instruction ())
	{
	  lprintf ("\tBPT at %04hX", simreg.ic);
	  break;
	}
      else if (bpindex >= 0)
	{
	  info ("\tBreakpoint at %04hX : %s",
		   simreg.ic, disassemble ());
	  break;
	}
    }
  return OKAY;
}


int
si_snglstp (int argc, char *argv[])
{
  int    count = 1;
  bool   step_over = FALSE;
  ushort target_addr;

  if (argc > 1)
    {
      if (*argv[1] == '*')
	{
	  step_over = TRUE;
	  target_addr = simreg.ic + 2;
	}
      else
	sscanf (argv[1], "%d", &count);
    }

  if (at_bpt_instruction ())
    simreg.ic++;

  if (step_over)
    {
      if (bpindex >= 0)
	{
	  info ("\tStepping past breakpoint at IC : %04hX   %s",
		   simreg.ic, disassemble ());
	  execute_without_breakpt ();
	}
      while (simreg.ic != target_addr)
	{
	  if (sys_int (1L))
	    return (INTERRUPT);
	  if (execute () == MEMERR)
	    break;
	  if (at_bpt_instruction ())
	    {
	      lprintf ("\tBPT at %04hX", simreg.ic);
	      break;
	    }
	  else if (bpindex >= 0)
	    {
	      info ("\tBreakpoint at %04hX : %s",
		       simreg.ic, disassemble ());
	      break;
	    }
	}
      if (! at_bpt_instruction () && bpindex < 0)
	info ("\tStep at %04hX : %s", simreg.ic, disassemble ());
    }
  else
    {
      while (count-- > 0)
	{
	  if (sys_int (1L))
	    return (INTERRUPT);
	  if (at_bpt_instruction ())
	    {
	      info ("\tStepping past BPT at IC : %04hX", simreg.ic);
	      simreg.ic++;
	    }
	  else if (bpindex >= 0)
	    {
	      info ("\tStepping past breakpoint at IC : %04hX   %s",
		       simreg.ic, disassemble ());
	      execute_without_breakpt ();
	    }
	  else if (execute () == MEMERR)
	    break;
	  info ("\tIC : %04hX   %s", simreg.ic, disassemble ());
	}
    }

  return (OKAY);
}


int
si_trace (int argc, char *argv[])
{
  int count = 1;

  if (argc > 1)
    {
      if (argc > 2)
	error ("excess arguments ignored");
      else
	sscanf (argv[1], "%x", &count);
    }

  while (count-- > 0)
    {
      if (sys_int (1L))
	return (INTERRUPT);

      if (at_bpt_instruction ())
	simreg.ic++;
      else if (bpindex >= 0)
	execute_without_breakpt ();
      else if (execute () == MEMERR)
	break;
      lprintf ("\tIC : %04hX   %s", simreg.ic, disassemble ());
      dis_reg (0);
    }

  return (OKAY);
}


int
si_bt (int argc, char *argv[])
{
  /* Backtrace the given number of instructions */

  int count = 10;
  int back = 10;
  struct regs save;
  int t;

  if (argc > 2)
    {
      if (argc > 3)
        error ("excess arguments ignored");
      else
        sscanf (argv[1], "%x %x", &back, &count);
    }

  if (count > bt_cnt)
    count = bt_cnt;

  /* save current regs */
  save = simreg;

  /* step back through backtrace buffer */
  t = bt_next - back;
  if (t < 0)
    t += BT_SIZE;

  while (count-- > 0)
    {
      simreg = bt_buff [t];
      lprintf ("\tIC : %04hX   %s", simreg.ic, disassemble ());
      dis_reg (0);

      t++;
      if (t >= BT_SIZE)
        t = 0;
    }

  /* restore old regs */
  simreg = save;

  return (OKAY);
}


