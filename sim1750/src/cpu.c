/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :         cpu.c -- Central Processing Unit engine             */
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
/*#include <stdlib.h>*/
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "xiodef.h"
#include "targsys.h"
#include "status.h"
#include "stime.h"
#include "arith.h"
#include "flt1750.h"
#include "peekpoke.h"
#include "smemacc.h"
#ifndef BSVC
#include "break.h"
#endif
#include "cpu.h"

/* Exports */

int   execute (void);
ulong instcnt;		     /* Total number of instructions executed */
struct regs simreg;	     /* The 1750 register file */
int   bpindex = -1;	     /* Index of breakpoint when hitting one */
			     /* (unused in BSVC) */

/* Total execution time in uSec since go command */
double total_time_in_us = 0.0;

/* 
 * Back trace buffer
 */
struct regs bt_buff [BT_SIZE];
int bt_next;
int bt_cnt;


static void
add_to_backtrace ()
{
  bt_buff [bt_next] = simreg;
  bt_next++;
  if (bt_next >= BT_SIZE)
    bt_next = 0;

  if (bt_cnt < BT_SIZE)
    bt_cnt++;
}



/* Each MMU page points to 4k words of memory.
   Indexing of a page is done by: pagereg[D/I][AS][logaddr_hinibble].ppa
   where D/I is the processsor Data/Instruction signal, AS is the
   Address State (least significant nibble of the Status Word), and
   logaddr_hinibble is the most significant nibble of the 16 bit logical
   address. The pagereg array, thus indexed, yields an 8-bit number, which
   becomes the most significant byte in the 20-bit physical address.
   The remaining bits of the physical address, i.e. the lower 12 bits,
   are a copy of the least significant 12 bits of the logical address.

   Default (startup) setting of pagereg[][][].ppa is done in
   init_cpu(). It can be changed in any desired way.
   By default, the MMU behaves just as though it were not there at all.
 */

struct mmureg pagereg[2][16][16];

/* CPU & MMU initialization function */

void
init_cpu (void)
{
  int as, i = 0;

  /* initialize MMU regs */
  memset ((void *) pagereg, 0, 512 * sizeof (ushort));
  /* initialize pagereg.ppa to "quasi-non-MMU". */
  for (as = 0; as <= 15; as++)
    {
      int logaddr_hinibble = 0;
      for (; logaddr_hinibble <= 0xF; logaddr_hinibble++)
        {
	      pagereg[CODE][as][logaddr_hinibble].ppa = i;
	      pagereg[DATA][as][logaddr_hinibble].ppa = i++;
        }
    }
  /* Write zeros to 1750 regs */
  memset ((void *) &simreg, 0, sizeof (struct regs));
  /* Reset counter of total instructions executed */
  instcnt = 0L;
  /* Reset counter of total simulation time */
  total_time_in_us = 0.0;
}


/********** functions for handling simulation time and interrupts ***********/

static void
workout_timing (int cycles)
{
  static ushort one_tatick_in_ns = 0, one_tbtick_in_tatix = 0;
  static ushort one_gotick_in_10usec = 0;
#ifdef MAS281
#define TIMER_A_LIMIT_IN_NS 20000
  /* CCFN: Timer A limit modified to allow for 50KHz clock on ERA board */
#else
#define TIMER_A_LIMIT_IN_NS 10000
#endif

  one_tatick_in_ns += uP_CYCLE_IN_NS * cycles;

  while (one_tatick_in_ns >= TIMER_A_LIMIT_IN_NS)
    {
      one_tatick_in_ns -= TIMER_A_LIMIT_IN_NS;
      if (simreg.sys & SYS_TA)
	{
	  if (simreg.ta == 0xFFFF)
	    {
	      simreg.ta = 0;
	      simreg.pir |= INTR_TA;
	    }
	  else
	    simreg.ta++;
	}
      if (simreg.sys & SYS_TB)
	{
	  if (one_tbtick_in_tatix == 9)
	    {
	      one_tbtick_in_tatix = 0;
	      if (simreg.tb == 0xFFFF)
		{
		  simreg.tb = 0;
		  simreg.pir |= INTR_TB;
		}
	      else
	        simreg.tb++;
	    }
          else
            one_tbtick_in_tatix++;
	}

      if (++one_gotick_in_10usec >= GOTIMER_PERIOD_IN_10uSEC)
	{
	  one_gotick_in_10usec = 0;
	  if (!++simreg.go)         /* GO Watchdog */
	    {
	      simreg.pir |= INTR_MACHERR;   /* machine error         */
	      simreg.ft |= FT_SYSFAULT0;    /* sysfault 0 : watchdog */
	      simreg.go = 0;
	      info ("BARF! goes the watchdog\n");
	    }
	}
    }
}

/* A quickie for communication between workout_interrupts() and ex_bex() */
static ushort bex_index;

static void
workout_interrupts ()
{
  ushort intnum, pirmask;
  ushort old_mk = simreg.mk, old_sw = simreg.sw, old_ic = simreg.ic;
  ushort lp, svp;
  static char *intr_name[] =
    { "Power-Down", "Machine-Error", "User-0", "Floating-Overflow",
      "Integer-Overflow", "Executive-Call", "Floating-Underflow", "Timer-A",
      "User-1", "Timer-B", "User-2", "User-3",
      "IO-Level-1", "User-4", "IO-Level-2", "User-5" };

  if (simreg.pir == 0)
    return;

  for (intnum = 0; intnum < 16; intnum++)
    {
      pirmask = 1 << (15 - intnum);
      if (simreg.pir & pirmask)
	{
	  if ((simreg.sys & SYS_INT) == 0
	      && intnum != 0 && intnum != 1 && intnum != 5)
	    continue;           /* Master Interrupt Enable is not set */
	  info ("\tInterrupt %2d (%s)", (unsigned) intnum, intr_name[intnum]);
	  if ((simreg.mk & pirmask) == 0  && intnum != 0 && intnum != 5)
	    info ("  pending but masked");
	  else
	    {
	      ushort as;
	      simreg.pir &= ~pirmask;
	      simreg.sys &= ~SYS_INT;  /* clear the Master Interrupt Enable */
	      /************** Switch to the interrupt context ***************/
	      simreg.sw &= 0xFFF0;      /* LP and SVP in AS 0 */
	      get_raw (CODE, 0, 0x20 + intnum * 2, &lp);
	      get_raw (CODE, 0, 0x21 + intnum * 2, &svp);
	      /* get new MK/SW/IC */
	      get_raw (DATA, 0, svp, &simreg.mk);
	      get_raw (DATA, 0, svp + 1, &simreg.sw);
	      get_raw (DATA, 0, svp + 2 + (intnum == 5 ? bex_index : 0),
		 		     &simreg.ic); /* bex_index: see ex_bex() */
	      /* save old MK/SW/IC in new AS */
	      as = simreg.sw & 0x000F;
	      store_raw (DATA, as, lp, old_mk);
	      store_raw (DATA, as, lp + 1, old_sw);
	      store_raw (DATA, as, lp + 2, old_ic);
	      break;
	    }
	}
    }
}


/************************** Internal Definitions ****************************/

static char *bankname[] = { "Code", "Data" };

/* Return value of get_word and store_word is one of:
   OKAY, BREAKPT, or MEMERR. */

static int
get_word (int bank, ushort address, short *data)
{
  ushort al, ak = (simreg.sw >> 4) & 0xF, as = simreg.sw & 0xF;
  ulong phys_address;

  if (bank != CODE && bank != DATA)
    {
      error ("FATAL ERR:  bank-number %d invalid\n", bank);
      return MEMERR;
    }
  if (ak != 0)
    {
      al = pagereg[bank][(int) as][(int)(address >> 12)].al;
      if (al != 0xF && ak != al)
	{
	  simreg.pir |= INTR_MACHERR;
	  simreg.ft |= FT_MEMPROT;
	  error
	   ("MACHINE ERR: AL %hX / AK %hX during %s fetch from %hX:%04hX\n",
		    al, ak, bankname[bank], as, address);
	  return MEMERR;
	}
    }
  if (pagereg[bank][(int) as][(int)(address >> 12)].e_w)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_MEMPROT;
      error
	("MACHINE ERR: attempt to fetch %s from protected address %hX:%04hX\n",
		bankname[bank], as, address);
      return MEMERR;
    }
  phys_address = get_phys_address (bank, as, address);
#ifndef BSVC
  /* Check for breakpoint */
  if ((bpindex = find_breakpt (READ, phys_address)) >= 0)
    return BREAKPT;
#endif
  if (peek (phys_address, (ushort *) data) == 0)
    {
      error ("read error at ic = %04X\n", simreg.ic);
      return MEMERR;
    }
  return OKAY;
}


static int
store_word (int bank, ushort address, ushort data)
{
  ushort al, ak = (simreg.sw >> 4) & 0xF, as = simreg.sw & 0xF;
  ulong phys_address;

  if (bank != CODE && bank != DATA)
    {
      error ("intern err (store_word):  bank-number %d invalid\n", bank);
      return MEMERR;
    }
  if (ak != 0)
    {
      al = pagereg[bank][(int) as][(int)(address >> 12)].al;
      if (al != 0xF && ak != al)
	{
	  simreg.pir |= INTR_MACHERR;
	  simreg.ft |= FT_MEMPROT;
	  error
	   ("MACHINE ERR: AL=%hX / AK=%hX on storing %s to address %hX:%04hX\n",
		    al, ak, bankname[bank], as, address);
	  return MEMERR;
	}
    }
  if (pagereg[bank][(int) as][(int)(address >> 12)].e_w)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_MEMPROT;
      error
	("MACHINE ERR: attempt to store %s to protected address %hX:%04hX\n",
		bankname[bank], as, address);
      return MEMERR;
    }
  phys_address = get_phys_address (bank, as, address);
#ifndef BSVC
  /* Check for breakpoint */
  if ((bpindex = find_breakpt (WRITE, phys_address)) >= 0)
    return BREAKPT;
#endif
  poke (phys_address, data);
  return OKAY;
}


#define BASEREG(opcode) (ushort) simreg.r[12 + (((opcode) & 0x0300) >> 8)]
#define CHK_RX()        (lower > 0 ? (ushort) simreg.r[lower] : 0)
static int ans;
#define GET(bank,addr,receiver) \
	  if ((ans = get_word (bank, addr, receiver)) != OKAY) return ans
#define PUT(bank,addr,emittee)  \
	  if ((ans = store_word (bank, addr, emittee)) != OKAY) return ans

static ushort opcode, upper, lower;
/* `upper' and `lower' are bits 8..11 and 12..15 respectively of the opcode */

/*************************** CPU instructions *******************************/

static int
ex_ill ()		/* illegal opcode */
{
  info ("opcode %04hX not implemented (IC=%04hX)\n", opcode, simreg.ic);
  return (MEMERR);
}

static int
ex_lb ()		/* 0[0-3]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);

  GET (DATA, addr, &simreg.r[2]);
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_LB);
}

static int
ex_dlb ()		/* 0[4-7]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);

  GET (DATA, addr, &simreg.r[0]);
  GET (DATA, addr + 1, &simreg.r[1]);
  update_cs (&simreg.r[0], VAR_LONG);

  simreg.ic++;
  return (nc_DLB);
}

static int
ex_stb ()		/* 0[9-B]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);

  PUT (DATA, addr, simreg.r[2]);

  simreg.ic++;
  return (nc_STB);
}

static int
ex_dstb ()		/* 0[A-F]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);

  PUT (DATA, addr, simreg.r[0]);
  PUT (DATA, addr + 1, simreg.r[1]);

  simreg.ic++;
  return (nc_DSTB);
}

static int
ex_ab ()		/* 1[0-3]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  arith (ARI_ADD, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_AB);
}

static int
ex_sbb ()		/* 1[4-7]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  arith (ARI_SUB, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_SBB);
}

static int
ex_mb ()		/* 1[8-B]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  arith (ARI_MUL, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_MB);
}

static int
ex_db ()		/* 1[C-F]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  arith (ARI_DIV, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_DB);
}

static int
ex_fab ()		/* 2[0-3]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_ADD, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FAB);
}

static int
ex_fsb ()		/* 2[4-7]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_SUB, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FSB);
}

static int
ex_fmb ()		/* 2[8-B]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_MUL, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FMB);
}

static int
ex_fdb ()		/* 2[C-F]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_DIV, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FDB);
}

static int
ex_orb ()		/* 3[0-3]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  simreg.r[2] |= help;
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_ORB);
}

static int
ex_andb ()		/* 3[4-7]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  simreg.r[2] &= help;
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_ANDB);
}

static int
ex_cb ()		/* 3[8-B]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help;

  GET (DATA, addr, &help);
  compare (VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_CB);
}

static int
ex_fcb ()		/* 3[C-F]xy */
{
  ushort addr = BASEREG (opcode) + (opcode & 0xFF);
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  compare (VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FCB);
}


static int
ex_lbx ()		/* 4[0-3]0y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();

  GET (DATA, addr, &simreg.r[2]);
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_LBX);
}

static int
ex_dlbx ()		/* 4[0-3]1y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();

  GET (DATA, addr, &simreg.r[0]);
  GET (DATA, addr + 1, &simreg.r[1]);
  update_cs (&simreg.r[0], VAR_LONG);

  simreg.ic++;
  return (nc_DLBX);
}

static int
ex_stbx ()		/* 4[0-3]2y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();

  PUT (DATA, addr, simreg.r[2]);

  simreg.ic++;
  return (nc_STBX);
}

static int
ex_dstx ()		/* 4[0-3]3y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();

  PUT (DATA, addr, simreg.r[0]);
  PUT (DATA, addr + 1, simreg.r[1]);

  simreg.ic++;
  return (nc_DSTX);
}

static int
ex_abx ()		/* 4[0-3]4y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  arith (ARI_ADD, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_ABX);
}

static int
ex_sbbx ()		/* 4[0-3]5y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  arith (ARI_SUB, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_SBBX);
}

static int
ex_mbx ()		/* 4[0-3]6y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  arith (ARI_MUL, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_MBX);
}

static int
ex_dbx ()		/* 4[0-3]7y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  arith (ARI_DIV, VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_DBX);
}

static int
ex_fabx ()		/* 4[0-3]8y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_ADD, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FABX);
}

static int
ex_fsbx ()		/* 4[0-3]9y */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_SUB, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FSBX);
}

static int
ex_fmbx ()		/* 4[0-3]Ay */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_MUL, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FMBX);
}

static int
ex_fdbx ()		/* 4[0-3]By */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  arith (ARI_DIV, VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FDBX);
}

static int
ex_cbx ()		/* 4[0-3]Cy */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;
  
  GET (DATA, addr, &help);
  compare (VAR_INT, &simreg.r[2], &help);

  simreg.ic++;
  return (nc_CBX);
}

static int
ex_fcbx ()		/* 4[0-3]Dy */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help[2];

  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);
  compare (VAR_FLOAT, &simreg.r[0], help);

  simreg.ic++;
  return (nc_FCBX);
}

static int
ex_andx ()		/* 4[0-3]Ey */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  simreg.r[2] &= help;
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_ANDX);
}

static int
ex_orbx ()		/* 4[0-3]Fy */
{
  ushort addr = BASEREG (opcode) + CHK_RX ();
  short help;

  GET (DATA, addr, &help);
  simreg.r[2] |= help;
  update_cs (&simreg.r[2], VAR_INT);

  simreg.ic++;
  return (nc_ORBX);
}

static int
ex_brx ()       /* opcode 4x distributor */
{
  static int (*start_4x[16]) () =
    {
       ex_lbx,  ex_dlbx, ex_stbx, ex_dstx,
       ex_abx,  ex_sbbx, ex_mbx,  ex_dbx,
       ex_fabx, ex_fsbx, ex_fmbx, ex_fdbx,
       ex_cbx,  ex_fcbx, ex_andx, ex_orbx
    };
  return (*start_4x[(unsigned) upper]) ();
}


/* auxiliary to ex_xio() : */

extern void do_xio (ushort, ushort *);	/* User XIO definition, do_xio.c */

static void
realize_xio (ushort xio_address, ushort *transfer)
{
  int i, bank;
  ushort addr_hibyte = xio_address & 0xFF00;
  ushort xio_upper = (xio_address & 0x00F0) >> 4;
  ushort xio_lower =  xio_address & 0x000F;

  switch (addr_hibyte)
    {
    case X_WIPR:
    case X_WOPR:
      bank = (addr_hibyte == X_WIPR) ? CODE : DATA;
      pagereg[bank][xio_upper][xio_lower].ppa = *transfer & 0xff;
      return;
    case X_RIPR:
    case X_ROPR:
      bank = (addr_hibyte == X_RIPR) ? CODE : DATA;
      *transfer = pagereg[bank][xio_upper][xio_lower].ppa;
      return;
    }
  for (i = 0; xio[i].value; i++)
    if (xio[i].value == xio_address)
      break;

  if (xio[i].value)
    {
      switch (xio_address)
	{
	case     X_ENBL:
	  simreg.sys |= SYS_INT;
	elsecase X_DSBL:
	  simreg.sys &= ~SYS_INT;
	elsecase X_DMAE:
	  simreg.sys |= SYS_DMA;
	elsecase X_DMAD:
	  simreg.sys &= ~SYS_DMA;
	elsecase X_TAH:
	  simreg.sys &= ~SYS_TA;
	elsecase X_TBH:
	  simreg.sys &= ~SYS_TB;
	elsecase X_TAS:
	  simreg.sys |= SYS_TA;
	elsecase X_OTA:
	  simreg.sys |= SYS_TA;
	  simreg.ta = *transfer;
	elsecase X_ITA:
	  *transfer = simreg.ta;
	elsecase X_TBS:
	  simreg.sys |= SYS_TB;
	elsecase X_OTB:
	  simreg.sys |= SYS_TB;
	  simreg.tb = *transfer;
	elsecase X_ITB:
	  *transfer = simreg.tb;
	elsecase X_GO:
	  simreg.go = 0;
	elsecase X_RSW:
	  *transfer = simreg.sw;
	elsecase X_WSW:
	  simreg.sw = *transfer;
	elsecase X_RPI:
	  simreg.pir &= ~(0x8000 >> *transfer);
	  if (*transfer == 1)
	    simreg.ft = 0;
	elsecase X_SPI:
	  simreg.pir |= *transfer;
	elsecase X_RMK:
	  *transfer = simreg.mk;
	elsecase X_SMK:
	  simreg.mk = *transfer;
	elsecase X_RPIR:
	  *transfer = simreg.pir;
	elsecase X_RCFR:
	  *transfer = simreg.ft;
	  simreg.ft = 0;
	  simreg.pir &= ~INTR_MACHERR;
	elsecase X_CLIR:
	  simreg.ft = simreg.pir = 0;
	elsecase X_CO:
#ifdef BSVC
	  info (".");
#endif
	  i = (*transfer >> 8) & 0xFF;
	  if (i)
	    {
	      info ("XIO CO highbyte 0x%02X\n", i);
	      *transfer &= 0xFF;
	    }
	  if (isprint (*transfer) || isspace(*transfer))
	    lprintf ("%c", *transfer);
	  else
	    info ("XIO CO: 0x%02hX\n", *transfer);
	  break;

	default:
	  /* info ("XIO %s: 0x%4hX\n", xio[i].name, *transfer); */
	  break;
	}
    }
  else
    do_xio (xio_address, transfer);	/* user defined XIO */
}

static int
ex_xio ()		/* 48xy */
{
  unsigned ak = (unsigned) (simreg.sw >> 4) & 0xF; /* privileged instruction */
  ushort xio_address;

  GET (CODE, simreg.ic + 1, (short *) &xio_address);
  xio_address += CHK_RX ();

  if (ak != 0)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_PRIV_INSTR;
    }
  else
    realize_xio (xio_address, (ushort *) &simreg.r[upper]);

  simreg.ic += 2;
  return (nc_XIO);
}

static int
ex_vio ()		/* 49xy */
{
  unsigned ak = (unsigned) (simreg.sw >> 4) & 0xF; /* privileged instruction */
  ushort vio_address, vec_sel, i = 0, n, transfer, iocmd, iodata;

  GET (CODE, simreg.ic + 1, (short *) &vio_address);
  vio_address += CHK_RX ();
  GET (CODE, vio_address + 1, (short *) &vec_sel);

  if (ak != 0)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_PRIV_INSTR;
    }
  else
    {
      for (n = 0; n <= 15; n++)
	{
	  if (vec_sel & (1 << (15 - n)))
	    {
	      GET (DATA, vio_address, (short *) &iocmd);
	      iocmd += n * (ushort) simreg.r[upper];
	      transfer = vio_address + 2 + i;
	      GET (DATA, transfer, (short *) &iodata);
	      realize_xio (iocmd, &iodata);
	      if (iocmd & 0x8000)  /* XIO read */
		PUT (DATA, transfer, iodata);
	      i++;
	    }
	}
    }

  simreg.ic += 2;
  return (nc_VIO);
}


static int
ex_aim ()		/* 4Ax1 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_ADD, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_AIM);
}

static int
ex_sim ()		/* 4Ax2 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_SUB, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_SIM);
}

static int
ex_mim ()		/* 4Ax3 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_MUL, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_MIM);
}

static int
ex_msim ()		/* 4Ax4 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_MULS, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_MSIM);
}

static int
ex_dim ()		/* 4Ax5 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_DIV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_DIM);
}

static int
ex_dvim ()		/* 4Ax6 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  arith (ARI_DIVV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_DVIM);
}

static int
ex_andm ()		/* 4Ax7 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  simreg.r[upper] &= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_ANDM);
}

static int
ex_orim ()		/* 4Ax8 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  simreg.r[upper] |= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_ORIM);
}

static int
ex_xorm ()		/* 4Ax9 */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  simreg.r[upper] ^= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_XORM);
}

static int
ex_cim ()		/* 4AxA */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  compare (VAR_INT, &simreg.r[upper], &help); /* side effect on CS */

  simreg.ic += 2;
  return (nc_CIM);
}

static int
ex_nim ()		/* 4AxB */
{
  short help;

  GET (CODE, simreg.ic + 1, &help);
  simreg.r[upper] = ~(simreg.r[upper] & help);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_NIM);
}

static int
ex_imml ()      /* opcode 4A distributor */
{
  static int (*start_4a[16])() =
    {
       ex_ill,  ex_aim,  ex_sim,  ex_mim,
       ex_msim, ex_dim,  ex_dvim, ex_andm,
       ex_orim, ex_xorm, ex_cim,  ex_nim,
       ex_ill,  ex_ill,  ex_ill,  ex_ill
    };
  return (*start_4a[(unsigned) lower]) ();
}


static int
ex_esqr ()		/* 4Dxy */
{
  double input = from_1750eflt (&simreg.r[upper]);

  if (input < 0.0)
    simreg.pir |= INTR_FIXOFL;
  else
    {
      to_1750eflt (sqrt (input), &simreg.r[upper]);
      update_cs (&simreg.r[upper], VAR_DOUBLE);
    }

  simreg.ic++;
  return (180);  /* GVSC, a wild guess */
}


static int
ex_sqrt ()		/* 4Exy */
{
  double input = from_1750flt (&simreg.r[upper]);

  if (input < 0.0)
    simreg.pir |= INTR_FIXOFL;
  else
    {
      to_1750flt (sqrt (input), &simreg.r[upper]);
      update_cs (&simreg.r[upper], VAR_FLOAT);
    }

  simreg.ic++;
  return (130);  /* GVSC, a wild guess */
}


static int
ex_bif ()		/* 4Fxy */
{
  return (nc_BIF);
}


static int
ex_sb ()		/* 50xy */
{
  ushort addr;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &data);

  data |= 1 << (15 - upper);
  PUT (DATA, addr, data);

  simreg.ic += 2;
  return (nc_SB);
}

static int
ex_sbr ()		/* 51xy */
{
  simreg.r[lower] |= 1 << (15 - upper);

  simreg.ic++;
  return (nc_SBR);
}

static int
ex_sbi ()		/* 52xy */
{
  ushort addr;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &data);

  data |= 1 << (15 - upper);
  PUT (DATA, addr, data);

  simreg.ic += 2;
  return (nc_SBI);
}

static int
ex_rb ()		/* 53xy */
{
  ushort addr;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &data);

  data &= ~(1 << (15 - upper));
  PUT (DATA, addr, data);

  simreg.ic += 2;
  return (nc_RB);
}

static int
ex_rbr ()		/* 54xy */
{
  simreg.r[lower] &= ~(1 << (15 - upper));

  simreg.ic++;
  return (nc_RBR);
}

static int
ex_rbi ()		/* 55xy */
{
  ushort addr;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &data);

  data &= ~(1 << (15 - upper));
  PUT (DATA, addr, data);

  simreg.ic += 2;
  return (nc_RBI);
}

static int
ex_tb ()		/* 56xy */
{
  ushort addr, sw_save = simreg.sw & 0x0FFF;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &data);

  if (data & (1 << (15 - upper)))
    simreg.sw = sw_save | (upper ? CS_POSITIVE : CS_NEGATIVE);
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_TB);
}

static int
ex_tbr ()		/* 57xy */
{
  ushort sw_save = simreg.sw & 0x0FFF;

  if (simreg.r[lower] & (1 << (15 - upper)))
    simreg.sw = sw_save | (upper ? CS_POSITIVE : CS_NEGATIVE);
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic++;
  return (nc_TBR);
}

static int
ex_tbi ()		/* 58xy */
{
  ushort addr, sw_save = simreg.sw & 0x0FFF;
  short data;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &data);

  if (data & (1 << (15 - upper)))
    simreg.sw = sw_save | (upper ? CS_POSITIVE : CS_NEGATIVE);
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_TBI);
}

static int
ex_tsb ()		/* 59xy */
{
  ushort addr, sw_save = simreg.sw & 0x0FFF;
  short data, bit_set = 1 << (15 - upper);

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &data);
  PUT (DATA, addr, data | bit_set);

  if (data & bit_set)
    simreg.sw = sw_save | (upper ? CS_POSITIVE : CS_NEGATIVE);
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_TSB);
}

static int
ex_svbr ()		/* 5Axy */
{
  simreg.r[lower] |= 1 << (15 - (simreg.r[upper] & 0xF));

  simreg.ic++;
  return (nc_SVBR);
}

static int
ex_rvbr ()		/* 5Cxy */
{
  simreg.r[lower] &= ~(1 << (15 - (simreg.r[upper] & 0xF)));

  simreg.ic++;
  return (nc_RVBR);
}

static int
ex_tvbr ()		/* 5Exy */
{
  ushort data = simreg.r[lower] & (1 << (15 - (simreg.r[upper] & 0xF)));
  ushort sw_save = simreg.sw & 0x0FFF;

  if (data)
    simreg.sw = sw_save | (simreg.r[upper] & 0xF ? CS_POSITIVE : CS_NEGATIVE);
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic++;
  return (nc_TVBR);
}


static int
ex_sll ()		/* 60xy */
{
  int n_shifts = (int) upper + 1;

  simreg.r[lower] <<= n_shifts;
  update_cs (&simreg.r[lower], VAR_INT);

  simreg.ic++;
  return (nc_SLL);
}

static int
ex_srl ()		/* 61xy */
{
  int n_shifts = (int) upper + 1;
/* ATTENTION:  To solicit a `logical shift right' operation (instead of 
   `arithmetic shift right'), it is NOT enough to cast a signed to an
   unsigned in the shift expression.
   But a separate buffer variable of unsigned type does the job! */
  ushort buf;

  buf = (ushort) simreg.r[lower];  /* Needed to get a `logical shift right' */
  simreg.r[lower] = buf >> n_shifts;
  update_cs (&simreg.r[lower], VAR_INT);

  simreg.ic++;
  return (nc_SRL);
}

static int
ex_sra ()		/* 62xy */
{
  int n_shifts = (int) upper + 1;

  simreg.r[lower] >>= n_shifts;
  update_cs (&simreg.r[lower], VAR_INT);

  simreg.ic++;
  return (nc_SRA);
}

static int
ex_slc ()		/* 63xy */
{
  int n_shifts = (int) upper + 1;
  ushort buf;

  buf = (ushort) simreg.r[lower];  /* Needed to get a `logical shift right' */
  simreg.r[lower] = (buf << n_shifts) | (buf >> (16 - n_shifts));
  update_cs (&simreg.r[lower], VAR_INT);

  simreg.ic++;
  return (nc_SLC);
}

static int
ex_dsll ()		/* 65xy */
{
  int n_shifts = (int) upper + 1;
  ulong buf = ((ulong) simreg.r[lower] << 16)
  | ((ulong) simreg.r[lower + 1] & 0xFFFF);

  buf <<= n_shifts;
  simreg.r[lower] = (short) (buf >> 16);
  simreg.r[lower + 1] = (short) (buf & 0xFFFF);
  update_cs (&simreg.r[lower], VAR_LONG);

  simreg.ic++;
  return (nc_DSLL);
}

static int
ex_dsrl ()		/* 66xy */
{
  int n_shifts = (int) upper + 1;
  ulong buf = ((ulong) simreg.r[lower] << 16)
  | ((ulong) simreg.r[lower + 1] & 0xFFFF);

  buf >>= n_shifts;                       /* Attention: buf is unsigned, */
  simreg.r[lower] = (short) (buf >> 16);   /* i.e. *logical* shift right */
  simreg.r[lower + 1] = (short) (buf & 0xFFFF);
  update_cs (&simreg.r[lower], VAR_LONG);

  simreg.ic++;
  return (nc_DSRL);
}

static int
ex_dsra ()		/* 67xy */
{
  int n_shifts = (int) upper + 1;
  long buf = ((long) simreg.r[lower] << 16)
  | ((long) simreg.r[lower + 1] & 0xFFFF);

  buf >>= n_shifts;
  simreg.r[lower] = (short) (buf >> 16);
  simreg.r[lower + 1] = (short) (buf & 0xFFFF);
  update_cs (&simreg.r[lower], VAR_LONG);

  simreg.ic++;
  return (nc_DSRA);
}

static int
ex_dslc ()		/* 68xy */
{
  int n_shifts = (int) upper + 1;
  ulong buf = ((ulong) simreg.r[lower] << 16)  /* This variable is needed to */
  | ((ulong) simreg.r[lower + 1] & 0xFFFF);   /* get a `logical shift right' */

  buf = (buf << n_shifts) | (buf >> (32 - n_shifts));
  simreg.r[lower] = (short) (buf >> 16);
  simreg.r[lower + 1] = (short) (buf & 0xFFFF);
  update_cs (&simreg.r[lower], VAR_LONG);

  simreg.ic++;
  return (nc_DSLC);
}

static int
ex_slr ()		/* 6Axy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;
  ushort buf;

  buf = (ushort) simreg.r[upper];  /* Needed to get a `logical shift right' */
  if (shc < 0)                  /* negative => shift right */
    {
      if ((shc = -shc) > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /* behavior to be verified */
	}
      simreg.r[upper] = buf >> shc;
    }
  else
    /* positive => shift left  */
    {
      if (shc > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /* behavior to be verified */
	}
      simreg.r[upper] = buf << shc;
    }
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_SLR);
}

static int
ex_sar ()		/* 6Bxy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;

  if (shc < 0)                  /* negative => shift right */
    {
      if ((shc = -shc) > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /*  behavior to be verified */
	}
      simreg.r[upper] >>= shc;
    }
  else
    /* positive => shift left  */
    {
      if (shc > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /* behavior to be verified */
	}
      simreg.r[upper] <<= shc;
    }
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_SAR);
}

static int
ex_scr ()		/* 6Cxy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;
  ushort buf;

  buf = (ushort) simreg.r[upper];   /* Needed to get a `logical shift right' */
  if (shc < 0)                  /* negative , rotate right */
    {
      if ((shc = -shc) > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /* behavior to be verified */
	}
      simreg.r[upper] = (buf >> shc) | (buf << (16 - shc));
    }
  else
    /* positive , rotate left  */
    {
      if (shc > 16)
	{
	  simreg.pir |= INTR_FIXOFL;
	  /* behavior to be verified */
	}
      simreg.r[upper] = (buf << shc) | (buf >> (16 - shc));
    }
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_SCR);
}

static int
ex_dslr ()		/* 6Dxy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;
  ulong buf = ((ulong) simreg.r[upper] << 16)   /* This variable is needed */
  | ((ulong) simreg.r[upper + 1] & 0xFFFF);     /* to get a LOGICAL shift */

  if (shc < 0)                  /* negative => shift right */
    {
      if ((shc = -shc) > 32)
	simreg.pir |= INTR_FIXOFL;
      else
	buf >>= shc;
    }
  else				/* positive => shift left  */
    {
      if (shc > 32)
	simreg.pir |= INTR_FIXOFL;
      else
	buf <<= shc;
    }
  simreg.r[upper] = (short) (buf >> 16);
  simreg.r[upper + 1] = (short) (buf & 0xFFFF);

  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_DSLR);
}

static int
ex_dsar ()		/* 6Exy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;
  long buf = ((long) simreg.r[upper] << 16)
	   | ((long) simreg.r[upper + 1] & 0xFFFFL);

  if (shc < 0)                  /* negative => shift right */
    {
      if ((shc = -shc) > 32)
	simreg.pir |= INTR_FIXOFL;
      else if (shc == 32)
	{
	 if (buf < 0L)
	   buf = 0xFFFFFFFFL;
	 else
	   buf = 0L;
	}
      else
        buf >>= shc;
    }
  else				/* positive => shift left  */
    {
      if (shc > 32)
	simreg.pir |= INTR_FIXOFL;
      else if (shc == 32)
	buf = 0L;
      else
	buf <<= shc;
    }
  simreg.r[upper] = (short) (buf >> 16);
  simreg.r[upper + 1] = (short) (buf & 0xFFFF);

  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_DSAR);
}

static int
ex_dscr ()		/* 6Fxy */
{
  short shc = simreg.r[lower];
  int n_shifts = (int) shc;
  ulong buf = ((ulong) simreg.r[upper] << 16)   /* This variable is needed */
  | ((ulong) simreg.r[upper + 1] & 0xFFFF);     /* to get a LOGICAL shift */

  if (shc < 0)                  /* negative => rotate right */
    {
      if ((shc = -shc) > 32)
	{
	  simreg.pir |= INTR_FIXOFL;    /* behavior to be verified */
	}
      buf = (buf >> shc) | (buf << (32 - shc));
    }
  else
    /* positive => rotate left  */
    {
      if (shc > 32)
	{
	  simreg.pir |= INTR_FIXOFL;    /* behavior to be verified */
	}
      buf = (buf << shc) | (buf >> (32 - shc));
    }
  simreg.r[upper] = (short) (buf >> 16);
  simreg.r[upper + 1] = (short) (buf & 0xFFFF);

  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_DSCR);
}

/* auxiliary to ex_jc and ex_jci */

static bool
flag_test (ushort condition)
{
  if ((condition & 0x0007) == 0x0007)
    return (TRUE);
  if (condition & ((simreg.sw >> 12) & 0x000F))
    return (TRUE);
  else
    return (FALSE);
}

static int
ex_jc ()		/* 70xy */
{
  bool jump_taken = flag_test (upper);

  if (jump_taken)
    {
      GET (CODE, simreg.ic + 1, (short *) &simreg.ic);
      simreg.ic += CHK_RX ();
    }
  else
    simreg.ic += 2;

  return (nc_JC);
}


static int
ex_jci ()		/* 71xy */
{
  ushort addr;
  bool jump_taken = flag_test (upper);

  if (jump_taken)
    {
      GET (CODE, simreg.ic + 1, (short *) &addr);
      addr += CHK_RX ();
      GET (DATA, addr, (short *) &simreg.ic);
    }
  else
    simreg.ic += 2;

  return (nc_JCI);
}

static int
ex_js ()		/* 72xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  simreg.r[upper] = simreg.ic + 2;
  simreg.ic = addr + CHK_RX ();

  return (nc_JS);
}

static int
ex_soj ()		/* 73xy */
{
  short help;
  int jump_taken = 0;

  help = 1;
  arith (ARI_SUB, VAR_INT, &simreg.r[upper], &help);

  if (CS_ZERO & simreg.sw)
    simreg.ic += 2;                     /* end of loop */
  else
    {
      GET (CODE, simreg.ic + 1, (short *) &simreg.ic);
      simreg.ic += CHK_RX ();
      jump_taken = 1;
    }

  return (nc_SOJ);
}

static int
ex_br ()		/* 74xy */
{
  ushort distance = opcode & 0x00FF;

  if (distance >= 0x80)		/* branch backwards */
    simreg.ic -= 0x100 - distance;
  else
    simreg.ic += distance;

  return (nc_BR);
}

static int
ex_bez ()		/* 75xy */
{
  bool branch_taken = FALSE;

  if (CS_ZERO & simreg.sw)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_blt ()		/* 76xy */
{
  bool branch_taken = FALSE;

  if (CS_NEGATIVE & simreg.sw)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_bex ()		/* 77xy */
{
  bex_index = (int) lower;
  simreg.pir |= INTR_BEX;

  simreg.ic++;
  return (nc_BEX);
}

static int
ex_ble ()		/* 78xy */
{
  bool branch_taken = FALSE;

  if (CS_ZERO & simreg.sw || CS_NEGATIVE & simreg.sw)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_bgt ()		/* 79xy */
{
  bool branch_taken = FALSE;

  if (CS_POSITIVE & simreg.sw)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_bnz ()		/* 7Axy */
{
  bool branch_taken = FALSE;

  if ((CS_ZERO & simreg.sw) == 0)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_bge ()		/* 7Bxy */
{
  bool branch_taken = FALSE;

  if (CS_ZERO & simreg.sw || CS_POSITIVE & simreg.sw)
    {
      ushort distance = opcode & 0x00FF;
      if (distance >= 0x80)
	simreg.ic -= 0x100 - distance;
      else
	simreg.ic += distance;
      branch_taken = TRUE;
    }
  else
    simreg.ic++;

  return (nc_BRcc);
}

static int
ex_lsti ()		/* 7Cxy */
{
  /* privileged instruction  */
  ushort ak = (simreg.sw >> 4) & 0xF, source;

  GET (CODE, simreg.ic + 1, (short *) &source);
  source += CHK_RX ();

  if (ak != 0)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_PRIV_INSTR;
    }
  else
    {
      GET (DATA, source, (short *) &source);
      GET (DATA, source,     (short *) &simreg.mk);
      GET (DATA, source + 1, (short *) &simreg.sw);
      GET (DATA, source + 2, (short *) &simreg.ic);
    }

  return (nc_LSTI);
}

static int
ex_lst ()		/* 7Dxy */
{
  /* privileged instruction  */
  ushort ak = (simreg.sw >> 4) & 0xF, source;

  GET (CODE, simreg.ic + 1, (short *) &source);
  source += CHK_RX ();

  if (ak != 0)
    {
      simreg.pir |= INTR_MACHERR;
      simreg.ft |= FT_PRIV_INSTR;
    }
  else
    {
      GET (DATA, (ushort) source,     (short *) &simreg.mk);
      GET (DATA, (ushort) source + 1, (short *) &simreg.sw);
      GET (DATA, (ushort) source + 2, (short *) &simreg.ic);
    }

  return (nc_LST);
}

static int
ex_sjs ()		/* 7Exy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();  /* needs to be BEFORE decrementing r[upper] ... */
  simreg.r[upper]--;          /* ... for the case of (lower == upper) */
  PUT (DATA, (ushort) simreg.r[upper], (short) simreg.ic + 2);
  simreg.ic = addr;

  return (nc_SJS);
}

static int
ex_urs ()		/* 7Fxy */
{
  if (lower)
    return (ex_ill ()); /* error */

  GET (DATA, (ushort) simreg.r[upper], (short *) &simreg.ic);
  simreg.r[upper]++;

  return (nc_URS);
}


static int
ex_l ()			/* 80xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &simreg.r[upper]);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_L);
}

static int
ex_lr ()		/* 81xy */
{
  simreg.r[upper] = simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_LR);
}

static int
ex_lisp ()		/* 82xy */
{
  simreg.r[upper] = (short) lower + 1;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_LISP);
}

static int
ex_lisn ()		/* 83xy */
{
  simreg.r[upper] = -(short) (lower + 1);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_LISN);
}

static int
ex_li ()		/* 84xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &simreg.r[upper]);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LI);
}

static int
ex_lim ()		/* 85xy */
{
  ushort immed;

  GET (CODE, simreg.ic + 1, (short *) &immed);
  immed += CHK_RX ();		/* IMX */

  simreg.r[upper] = (short) immed;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LIM);
}

static int
ex_dl ()		/* 86xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  if (upper == 15)
    {
      short help[2];
      GET (DATA, addr, &simreg.r[15]);
      help[0] = simreg.r[15];
      GET (DATA, addr + 1, &simreg.r[0]);
      help[1] = simreg.r[0];
      update_cs (help, VAR_LONG);
    }
  else
    {
      GET (DATA, addr, &simreg.r[upper]);
      GET (DATA, addr + 1, &simreg.r[upper + 1]);
      update_cs (&simreg.r[upper], VAR_LONG);
    }

  simreg.ic += 2;
  return (nc_DL);
}

static int
ex_dlr ()		/* 87xy */
{
  short help[2];

  if (upper == 15)
    {
      help[0] = simreg.r[15] = simreg.r[lower];
      help[1] = simreg.r[0] = simreg.r[lower + 1];
      update_cs (help, VAR_LONG);
    }
  else
    {
      if (upper == lower + 1)
	{
	  simreg.r[upper + 1] = simreg.r[lower + 1];
	  simreg.r[upper] = simreg.r[lower];
	}
      else
	{
	  simreg.r[upper] = simreg.r[lower];
	  simreg.r[upper + 1] = simreg.r[lower + 1];
	}
      update_cs (&simreg.r[upper], VAR_LONG);
    }

  simreg.ic++;
  return (nc_DLR);
}

static int
ex_dli ()		/* 88xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);

  if (upper == 15)
    {
      short help[2];
      GET (DATA, addr, &simreg.r[15]);
      help[0] = simreg.r[15];
      GET (DATA, addr + 1, &simreg.r[0]);
      help[1] = simreg.r[0];
      update_cs (help, VAR_LONG);
    }
  else
    {
      GET (DATA, addr, &simreg.r[upper]);
      GET (DATA, addr + 1, &simreg.r[upper + 1]);
      update_cs (&simreg.r[upper], VAR_LONG);
    }

  simreg.ic += 2;
  return (nc_DLI);
}

static int
ex_lm ()		/* 89xy */
{
  ushort i, addr;
  int n_loads = (int) upper + 1;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  for (i = 0; i <= upper; i++)
    GET (DATA, addr + i, &simreg.r[i]);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LM);
}

static int
ex_efl ()		/* 8Axy */
{
  ushort addr, i;
  short help[3];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  for (i = 0; i < 3; i++)
    {
      GET (DATA, addr + i, (short *) &help[i]);
      simreg.r[(upper + i) % 16] = help[i];
    }
  update_cs (help, VAR_DOUBLE);

  simreg.ic += 2;
  return (nc_EFL);
}

static int
ex_lub ()		/* 8Bxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] = (simreg.r[upper] & 0xFF00) | ((help >> 8) & 0x00FF);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LUB);
}

static int
ex_llb ()		/* 8Cxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] = (simreg.r[upper] & 0xFF00) | (help & 0x00FF);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LLB);
}

static int
ex_lubi ()		/* 8Dxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &help);

  simreg.r[upper] = (simreg.r[upper] & 0xFF00) | ((help >> 8) & 0x00FF);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LUBI);
}

static int
ex_llbi ()		/* 8Exy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &help);

  simreg.r[upper] = (simreg.r[upper] & 0xFF00) | (help & 0x00FF);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_LLBI);
}

static int
ex_popm ()		/* 8Fxy */
{
  int count, n_pops;

  if (upper <= lower)
    {
      n_pops = lower - upper + 1;
      for (count = upper; count <= lower; count++)
	{
	  if (count != 15)
	    GET (DATA, (ushort) simreg.r[15], &simreg.r[count]);
	  simreg.r[15]++;
	}
    }
  else
    {
      n_pops = 16 - upper + lower + 1;
      for (count = upper; count <= 15; count++)
	{
	  if (count != 15)
	    GET (DATA, (ushort) simreg.r[15], &simreg.r[count]);
	  simreg.r[15]++;
	}
      for (count = 0; count <= lower; count++)
	{
	  GET (DATA, (ushort) simreg.r[15], &simreg.r[count]);
	  simreg.r[15]++;
	}
    }

  simreg.ic++;
  return (nc_POPM);
}

static int
ex_st ()		/* 90xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  PUT (DATA, addr, simreg.r[upper]);

  simreg.ic += 2;
  return (nc_ST);
}

static int
ex_stc ()		/* 91xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  PUT (DATA, addr, (short) upper);

  simreg.ic += 2;
  return (nc_STC);
}

static int
ex_stci ()		/* 92xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);

  PUT (DATA, addr, (short) upper);

  simreg.ic += 2;
  return (nc_STCI);
}

static int
ex_mov ()		/* 93xy */
{
  short data;
  ushort source  = (ushort) simreg.r[lower];
  ushort destin  = (ushort) simreg.r[upper];
  ushort n_moves = 1;
  int single_cycle = nc_MOV;  /* computed on the basis of n_moves = 1 */

  n_moves = (ushort) simreg.r[upper + 1];
  while (n_moves)
    {
      GET (DATA, source, &data);
      PUT (DATA, destin, data);
      simreg.r[upper] = ++destin;	/* unsigned op */
      simreg.r[lower] = ++source;	/* unsigned op */
      simreg.r[upper + 1] = --n_moves;	/* unsigned op */
      workout_timing (single_cycle);
      workout_interrupts ();
    }

  simreg.ic++;
  return (nc_MOV);
}

static int
ex_sti ()		/* 94xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);

  PUT (DATA, addr, simreg.r[upper]);

  simreg.ic += 2;
  return (nc_STI);
}

static int
ex_sfbs ()		/* 95xy */
{
  ushort i;

  for (i = 0; i < 16; i++)
    if (simreg.r[upper] & (1 << (15 - i)))
      break;
  simreg.r[lower] = i;
  if (i == 16)
    simreg.sw = (simreg.sw & 0x0FFF) | CS_ZERO;
  else
    update_cs (&simreg.r[lower], VAR_INT);

  simreg.ic++;
  return (6);  /* that's what GVSC takes, anyways... */
}

static int
ex_dst ()		/* 96xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  PUT (DATA, addr, simreg.r[upper]);
  PUT (DATA, addr + 1, simreg.r[upper + 1]);

  simreg.ic += 2;
  return (nc_DST);
}

static int
ex_srm ()		/* 97xy */
{
  ushort destin;
  short help1, help2, mask;

  GET (CODE, simreg.ic + 1, (short *) &destin);
  destin += CHK_RX ();

  mask = simreg.r[upper + 1];
  help1 = simreg.r[upper] & mask;
  GET (DATA, destin, &help2);
  help2 &= ~mask;

  PUT (DATA, destin, help1 | help2);

  simreg.ic += 2;
  return (nc_SRM);
}

static int
ex_dsti ()		/* 98xy */
{
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);

  PUT (DATA, addr, simreg.r[upper]);
  PUT (DATA, addr + 1, simreg.r[upper + 1]);

  simreg.ic += 2;
  return (nc_DSTI);
}

static int
ex_stm ()		/* 99xy */
{
  ushort i, addr;
  int n_stores = (int) upper + 1;  /* for stime.h calculation of nc_STM */

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  for (i = 0; i <= upper; i++)
    PUT (DATA, addr + i, simreg.r[i]);

  simreg.ic += 2;
  return (nc_STM);
}

static int
ex_efst ()		/* 9Axy */
{
  ushort i;
  ushort addr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();

  for (i = 0; i < 3; i++)
    PUT (DATA, addr + i, simreg.r[(upper + i) % 16]);

  simreg.ic += 2;
  return (nc_EFST);
}

static int
ex_stub ()		/* 9Bxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);
  
  PUT (DATA, addr, (simreg.r[upper] << 8) | (help & 0x00FF));

  simreg.ic += 2;
  return (nc_STUB);
}

static int
ex_stlb ()		/* 9Cxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);
  
  PUT (DATA, addr, (simreg.r[upper] & 0x00FF) | (help & 0xFF00));

  simreg.ic += 2;
  return (nc_STLB);
}

static int
ex_subi ()		/* 9Dxy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &help);
  
  PUT (DATA, addr, (simreg.r[upper] << 8) | (help & 0x00FF));

  simreg.ic += 2;
  return (nc_SUBI);
}

static int
ex_slbi ()		/* 9Exy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, (short *) &addr);
  GET (DATA, addr, &help);
  
  PUT (DATA, addr, (simreg.r[upper] & 0x00FF) | (help & 0xFF00));

  simreg.ic += 2;
  return (nc_SLBI);
}

static int
ex_pshm ()		/* 9Fxy */
{
  int count = (int) lower, n_pushes;
  ushort stkptr = (ushort) simreg.r[15];

  if (upper <= lower)
    {
      n_pushes = lower - upper + 1;
      while (count >= (int) upper)
	{
	  PUT (DATA, --stkptr, simreg.r[count]);
	  count--;
	}
    }
  else
    {
      n_pushes = 16 - upper + lower + 1;
      while (count >= 0)
	{
	  PUT (DATA, --stkptr, simreg.r[count]);
	  count--;
	}
      count = 15;
      while (count >= (int) upper)
	{
	  PUT (DATA, --stkptr, simreg.r[count]);
	  count--;
	}
    }
  simreg.r[15] = (short) stkptr;

  simreg.ic++;
  return (nc_PSHM);
}

static int
ex_a ()			/* A0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_ADD, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_A);
}

static int
ex_ar ()		/* A1xy */
{
  arith (ARI_ADD, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_AR);
}

static int
ex_aisp ()		/* A2xy */
{
  short help = (short) lower + 1;

  arith (ARI_ADD, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_AISP);
}

static int
ex_incm ()		/* A3xy */
{
  ushort addr;
  short help1, help2 = upper + 1;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help1);

  arith (ARI_ADD, VAR_INT, &help1, &help2);
  PUT (DATA, addr, help1);

  simreg.ic += 2;
  return (nc_INCM);
}

static int
ex_abs ()		/* A4xy */
{
  int negative = 0;  /* for stime.h cycle computation */

  if (simreg.r[lower] == -32768)
    {
      simreg.r[upper] = -32768;
      simreg.pir |= INTR_FIXOFL;
    }
  else if (simreg.r[upper] < 0)
    {
      negative = 1;
      simreg.r[upper] = -simreg.r[lower];
    }
  else
    simreg.r[upper] = simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_ABS);
}

static int
ex_dabs ()		/* A5xy */
{
  long help = ((long) simreg.r[lower] << 16)
	    | ((long) simreg.r[lower+1] & 0x0000FFFF);
  int negative = 0;  /* for stime.h cycle computation */

  if (help == -0x80000000L)
    {
      simreg.r[upper] = -0x8000;
      simreg.r[upper + 1] = 0;
      simreg.pir |= INTR_FIXOFL;
    }
  else if (help < 0)
    {
      negative = 1;
      help = -help;
    }
  simreg.r[upper] = (short) (help >> 16);
  simreg.r[upper + 1] = (short) (help & 0xFFFF);
  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_DABS);
}

static int
ex_da ()		/* A6xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_ADD, VAR_LONG, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_DA);
}

static int
ex_dar ()		/* A7xy */
{
  arith (ARI_ADD, VAR_LONG, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DAR);
}

static int
ex_fa ()		/* A8xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_ADD, VAR_FLOAT, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_FA);
}

static int
ex_far ()		/* A9xy */
{
  arith (ARI_ADD, VAR_FLOAT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_FAR);
}

static int
ex_efa ()		/* AAxy */
{
  ushort addr;
  short help[3];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);
  GET (DATA, addr + 2, &help[2]);

  arith (ARI_ADD, VAR_DOUBLE, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_EFA);
}

static int
ex_efar ()		/* ABxy */
{
  arith (ARI_ADD, VAR_DOUBLE, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_EFAR);
}

static int
ex_fabs ()		/* ACxy */
{
  ushort mant_signword = (ushort) simreg.r[lower];
  short help[2];
  int negative = 0;

  if (mant_signword & 0x8000)   /* Look at the mantissa signbit */
    {
      negative = 1;
      help[0] = help[1] = 0;
      arith (ARI_SUB, VAR_FLOAT, help, &simreg.r[lower]);
    }
  else
      /* Going through a help variable guards against situations such as:  */
    { /* "FABS R2,R1" , where a needed register would otherwise be clobbered. */
      help[0] = simreg.r[lower];
      help[1] = simreg.r[lower + 1];
      update_cs (help, VAR_FLOAT);
    }
  simreg.r[upper] = help[0];
  simreg.r[upper + 1] = help[1];

  simreg.ic++;
  return (nc_FABS);
}

static int
ex_uar ()		/* ADxy */
{
  ulong lhelp = (ulong) simreg.r[upper] + (ulong) simreg.r[lower];

  simreg.r[upper] = (short) lhelp;
  if (lhelp > 0xFFFFL)
    simreg.sw = (simreg.sw & 0x0FFF) | CS_CARRY;
  else
    update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_AR);
}

static int
ex_ua ()		/* AExy */
{
  ushort addr;
  short help;
  ulong lhelp;  /* need it for some brain damaged C compilers */

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  lhelp = (ulong) simreg.r[upper] + (ulong) help;
  simreg.r[upper] = (short) lhelp;
  if (lhelp > 0xFFFFL)
    simreg.sw = (simreg.sw & 0x0FFF) | CS_CARRY;
  else
    update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_A);
}

static int
ex_s ()			/* B0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_SUB, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_S);
}

static int
ex_sr ()		/* B1xy */
{
  arith (ARI_SUB, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_SR);
}

static int
ex_sisp ()		/* B2xy */
{
  short help;

  help = (short) (lower + 1);
  arith (ARI_SUB, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_SISP);
}

static int
ex_decm ()		/* B3xy */
{
  ushort addr;
  short help1, help2 = upper + 1;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help1);

  arith (ARI_SUB, VAR_INT, &help1, &help2);
  PUT (DATA, addr, help1);

  simreg.ic += 2;
  return (nc_DECM);
}

static int
ex_neg ()		/* B4xy */
{
  simreg.r[upper] = -simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_NEG);
}

static int
ex_dneg ()		/* B5xy */
{
  long help = ((long) simreg.r[lower] << 16)
          | ((long) simreg.r[lower+1] & 0xFFFF);

  help = -help;
  simreg.r[upper] = (short) (help >> 16);
  simreg.r[upper + 1] = (short) (help & 0xFFFF);
  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_DNEG);
}

static int
ex_ds ()		/* B6xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_SUB, VAR_LONG, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_DS);
}

static int
ex_dsr ()		/* B7xy */
{
  arith (ARI_SUB, VAR_LONG, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DSR);
}

static int
ex_fs ()		/* B8xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_SUB, VAR_FLOAT, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_FS);
}

static int
ex_fsr ()		/* B9xy */
{
  arith (ARI_SUB, VAR_FLOAT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_FSR);
}

static int
ex_efs ()		/* BAxy */
{
  ushort addr;
  short help[3];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);
  GET (DATA, addr + 2, &help[2]);

  arith (ARI_SUB, VAR_DOUBLE, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_EFS);
}

static int
ex_efsr ()		/* BBxy */
{
  arith (ARI_SUB, VAR_DOUBLE, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_EFSR);
}

static int
ex_fneg ()		/* BCxy */
{
  short help[2];

  help[0] = help[1] = 0;    /* happens to be a floating zero */
  arith (ARI_SUB, VAR_FLOAT, help, &simreg.r[lower]);
  simreg.r[upper] = help[0];
  simreg.r[upper+1] = help[1];

  simreg.ic++;
  return (nc_FNEG);
}

static int
ex_usr ()		/* BDxy */
{
  ulong lhelp = (ulong) simreg.r[upper] - (ulong) simreg.r[lower];

  simreg.r[upper] = (short) lhelp;
  if (lhelp > 0xFFFFL)
    simreg.sw = (simreg.sw & 0x0FFF) | CS_CARRY;
  else
    update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_SR);
}

static int
ex_us ()		/* BExy */
{
  ushort addr;
  short help;
  ulong lhelp;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  lhelp = (ulong) simreg.r[upper] - (ulong) help;
  simreg.r[upper] = (short) lhelp;
  if (lhelp > 0xFFFFL)
    simreg.sw = (simreg.sw & 0x0FFF) | CS_CARRY;
  else
    update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_S);
}

static int
ex_ms ()		/* C0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_MULS, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_MS);
}

static int
ex_msr ()		/* C1xy */
{
  arith (ARI_MULS, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_MSR);
}

static int
ex_misp ()		/* C2xy */
{
  short help = (short) (lower + 1);
  arith (ARI_MULS, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_MISP);
}

static int
ex_misn ()		/* C3xy */
{
  short help = -(short) (lower + 1);
  arith (ARI_MULS, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_MISN);
}

static int
ex_m ()			/* C4xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_MUL, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_M);
}

static int
ex_mr ()		/* C5xy */
{
  arith (ARI_MUL, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_MR);
}

static int
ex_dm ()		/* C6xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_MUL, VAR_LONG, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_DM);
}

static int
ex_dmr ()		/* C7xy */
{
  arith (ARI_MUL, VAR_LONG, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DMR);
}

static int
ex_fm ()		/* C8xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_MUL, VAR_FLOAT, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_FM);
}

static int
ex_fmr ()		/* C9xy */
{
  arith (ARI_MUL, VAR_FLOAT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_FMR);
}

static int
ex_efm ()		/* CAxy */
{
  ushort addr;
  short help[3];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);
  GET (DATA, addr + 2, &help[2]);

  arith (ARI_MUL, VAR_DOUBLE, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_EFM);
}

static int
ex_efmr ()		/* CBxy */
{
  arith (ARI_MUL, VAR_DOUBLE, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_EFMR);
}

static int
ex_dv ()		/* D0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_DIVV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_DV);
}

static int
ex_dvr ()		/* D1xy */
{
  arith (ARI_DIVV, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DVR);
}

static int
ex_disp ()		/* D2xy */
{
  short help = (short) (lower + 1);

  arith (ARI_DIVV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_DISP);
}

static int
ex_disn ()		/* D3xy */
{
  short help = -(short) (lower + 1);

  arith (ARI_DIVV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_DISN);
}

static int
ex_d ()			/* D4xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  arith (ARI_DIV, VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_D);
}

static int
ex_dr ()		/* D5xy */
{
  arith (ARI_DIV, VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DR);
}

static int
ex_dd ()		/* D6xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_DIV, VAR_LONG, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_DD);
}

static int
ex_ddr ()		/* D7xy */       /* Heute Ostdeutschland! */
{
  arith (ARI_DIV, VAR_LONG, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DDR);
}

static int
ex_fd ()		/* D8xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help[0]);
  GET (DATA, addr + 1, &help[1]);

  arith (ARI_DIV, VAR_FLOAT, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_FD);
}

static int
ex_fdr ()		/* D9xy */
{
  arith (ARI_DIV, VAR_FLOAT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_FDR);
}

static int
ex_efd ()		/* DAxy */
{
  ushort addr;
  short help[3];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);
  GET (DATA, addr + 2, &help[2]);

  arith (ARI_DIV, VAR_DOUBLE, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_EFD);
}

static int
ex_efdr ()		/* DBxy */
{
  arith (ARI_DIV, VAR_DOUBLE, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_EFDR);
}

static int
ex_ste ()		/* DCxy */
{
  ushort addr;
  ulong laddr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  laddr = (ulong) addr;

  if (lower > 0)
    {
      laddr += (ulong) simreg.r[lower];
      laddr += (ulong) (simreg.r[lower-1] & 0x7F) << 16;
    }
  else
    laddr += (ulong) simreg.r[15];

  poke (simreg.r[upper], laddr);

  simreg.ic += 2;
  return (nc_ST);  /* in lack of the right timing figure */
}

static int
ex_dste ()		/* DDxy */
{
  ushort addr;
  ulong laddr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  laddr = (ulong) addr;

  if (lower > 0)
    {
      laddr += (ulong) simreg.r[lower];
      laddr += (ulong) (simreg.r[lower-1] & 0x7F) << 16;
    }
  else
    laddr += (ulong) simreg.r[15];

  poke (simreg.r[upper], laddr);
  poke (simreg.r[upper+1], laddr + 1);

  simreg.ic += 2;
  return (nc_DST);  /* in lack of the right timing figure */
}

static int
ex_le ()		/* DExy */
{
  ushort addr;
  ulong laddr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  laddr = (ulong) addr;

  if (lower > 0)
    {
      laddr += (ulong) simreg.r[lower];
      laddr += (ulong) (simreg.r[lower-1] & 0x7F) << 16;
    }
  else
    laddr += (ulong) simreg.r[15];

  if (peek (laddr, (ushort *) &simreg.r[upper]) == 0)
    error ("read error at ic = %04hX\n", simreg.ic);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_L);  /* in lack of the right timing figure */
}

static int
ex_dle ()		/* DFxy */
{
  ushort addr;
  ulong laddr;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  laddr = (ulong) addr;

  if (lower > 0)
    {
      laddr += (ulong) simreg.r[lower];
      laddr += (ulong) (simreg.r[lower-1] & 0x7F) << 16;
    }
  else
    laddr += (ulong) simreg.r[15];

  if (peek (laddr, (ushort *) &simreg.r[upper]) == 0)
    error ("read error at ic = %04hX\n", simreg.ic);
  if (peek (laddr + 1, (ushort *) &simreg.r[upper+1]) == 0)
    error ("read error at ic = %04hX\n", simreg.ic);
  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic += 2;
  return (nc_DL);  /* in lack of the right timing figure */
}

static int
ex_or ()		/* E0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] |= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_OR);
}

static int
ex_orr ()		/* E1xy */
{
  simreg.r[upper] |= simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_ORR);
}

static int
ex_and ()		/* E2xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] &= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_AND);
}

static int
ex_andr ()		/* E3xy */
{
  simreg.r[upper] &= simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_ANDR);
}

static int
ex_xor ()		/* E4xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] ^= help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_XOR);
}

static int
ex_xorr ()		/* E5xy */
{
  simreg.r[upper] ^= simreg.r[lower];
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_XORR);
}

static int
ex_n ()			/* E6xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  simreg.r[upper] = ~(simreg.r[upper] & help);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic += 2;
  return (nc_N);
}

static int
ex_nr ()		/* E7xy */
{
  simreg.r[upper] = ~(simreg.r[upper] & simreg.r[lower]);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_NR);
}

static int
ex_fix ()		/* E8xy */
{
  long help = (long) from_1750flt (&simreg.r[lower]);

  /* To Be Clarified:
     The following behavior is what this operation SHOULD do, but
     *not* what the Fairchild F9450 manual prescribes (the manual says
     FIXOFL occurs, regardless of mantissa, if the exponent of the addr
     number exceeds 2^15). */
  if (help < -32768 || help > 32767)
    simreg.pir |= INTR_FIXOFL;
  else
    simreg.r[upper] = (short) help;
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_FIX);
}

static int
ex_flt ()		/* E9xy */
{
  /* To Be Tested  (depends on C hostcompiler cast). */
  to_1750flt ((double) simreg.r[lower], &simreg.r[upper]);
  update_cs (&simreg.r[upper], VAR_FLOAT);

  simreg.ic++;
  return (nc_FLT);
}

static int
ex_efix ()		/* EAxy */
{
  long help;
  /* This time, we do exactly as the F9450 manual prescribes! */
  if ((char) (simreg.r[lower + 1] & 0xFF) > 0x1F)
    simreg.pir |= INTR_FIXOFL;
  else
    {
      help = (long) from_1750eflt (&simreg.r[lower]);
      simreg.r[upper] = (short) (help >> 16);
      simreg.r[upper + 1] = (short) (help & 0xFFFF);
    }
  update_cs (&simreg.r[upper], VAR_LONG);

  simreg.ic++;
  return (nc_EFIX);
}

static int
ex_eflt ()		/* EBxy */
{
  long help = ((long) simreg.r[lower] << 16)
	    | ((long) simreg.r[lower+1] & 0xFFFFL);

  to_1750eflt ((double) help, &simreg.r[upper]);
  update_cs (&simreg.r[upper], VAR_DOUBLE);

  simreg.ic++;
  return (nc_EFLT);
}

static int
ex_xbr ()		/* ECxy */
{
  simreg.r[upper] = (simreg.r[upper] << 8) | ((simreg.r[upper] >> 8) & 0x00FF);
  update_cs (&simreg.r[upper], VAR_INT);

  simreg.ic++;
  return (nc_XBR);
}

static int
ex_xwr ()		/* EDxy */
{
  short help = simreg.r[lower];

  simreg.r[lower] = simreg.r[upper];
  simreg.r[upper] = help;
  update_cs (&help, VAR_INT);

  simreg.ic++;
  return (nc_XWR);
}

static int
ex_c ()			/* F0xy */
{
  ushort addr;
  short help;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &help);

  compare (VAR_INT, &simreg.r[upper], &help);

  simreg.ic += 2;
  return (nc_C);
}

static int
ex_cr ()		/* F1xy */
{
  compare (VAR_INT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_CR);
}

static int
ex_cisp ()		/* F2xy */
{
  short help = (short) (lower + 1);

  compare (VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_CISP);
}

static int
ex_cisn ()		/* F3xy */
{
  short help = -(short) (lower + 1);

  compare (VAR_INT, &simreg.r[upper], &help);

  simreg.ic++;
  return (nc_CISN);
}

static int
ex_cbl ()		/* F4xy */
{
  ushort addr, sw_save = simreg.sw & 0x0FFF;
  short lowlim, highlim;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr, &lowlim);
  GET (DATA, addr + 1, &highlim);

  if (lowlim > highlim)
    simreg.sw = sw_save | CS_CARRY;
  else if (simreg.r[upper] < lowlim)
    simreg.sw = sw_save | CS_NEGATIVE;
  else if (simreg.r[upper] > highlim)
    simreg.sw = sw_save | CS_POSITIVE;
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_CBL);
}

static int
ex_ucim ()		/* F5xy */
{
  ushort sw_save = simreg.sw & 0x0FFF;
  ushort help1, help2;

  help1 = (ushort) simreg.r[upper];
  GET (CODE, simreg.ic + 1, (short *) &help2);

  if (help1 < help2)
    simreg.sw = sw_save | CS_NEGATIVE;
  else if (help1 > help2)
    simreg.sw = sw_save | CS_POSITIVE;
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_CIM);
}

static int
ex_dc ()		/* F6xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);

  compare (VAR_LONG, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_DC);
}

static int
ex_dcr ()		/* F7xy */
{
  compare (VAR_LONG, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_DCR);
}

static int
ex_fc ()		/* F8xy */
{
  ushort addr;
  short help[2];

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  GET (DATA, addr,     &help[0]);
  GET (DATA, addr + 1, &help[1]);

  compare (VAR_FLOAT, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_FC);
}

static int
ex_fcr ()		/* F9xy */
{
  compare (VAR_FLOAT, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_FCR);
}

static int
ex_efc ()		/* FAxy */
{
  ushort addr;
  short help[3];
  ushort i;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  for (i = 0; i < 3; i++)
    GET (DATA, addr + i, &help[i]);

  compare (VAR_DOUBLE, &simreg.r[upper], help);

  simreg.ic += 2;
  return (nc_EFC);
}

static int
ex_efcr ()		/* FBxy */
{
  compare (VAR_DOUBLE, &simreg.r[upper], &simreg.r[lower]);

  simreg.ic++;
  return (nc_EFCR);
}

static int
ex_ucr ()		/* FCxy */
{
  ushort sw_save = simreg.sw & 0x0FFF;
  ushort help1, help2;

  help1 = (ushort) simreg.r[upper];
  help2 = (ushort) simreg.r[lower];
  if (help1 < help2)
    simreg.sw = sw_save | CS_NEGATIVE;
  else if (help1 > help2)
    simreg.sw = sw_save | CS_POSITIVE;
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic++;
  return (nc_CR);
}

static int
ex_uc ()		/* FDxy */
{
  ushort sw_save = simreg.sw & 0x0FFF;
  ushort addr;
  ushort help1, help2;

  GET (CODE, simreg.ic + 1, (short *) &addr);
  addr += CHK_RX ();
  help1 = (ushort) simreg.r[upper];
  GET (DATA, addr, (short *) &help2);

  if (help1 < help2)
    simreg.sw = sw_save | CS_NEGATIVE;
  else if (help1 > help2)
    simreg.sw = sw_save | CS_POSITIVE;
  else
    simreg.sw = sw_save | CS_ZERO;

  simreg.ic += 2;
  return (nc_C);
}

static int
ex_nop_bpt ()		/* FFxy */
{
  if (upper == 0 && lower == 0)         /* NOP */
    {
      simreg.ic++;
      return (nc_NOP);
    }

  if (upper == 0xF && lower == 0xF)     /* BPT */
    {
      return (BREAKPT);
    }

  return (ex_ill ());
}


static int (*exfunc[256])() =
  {
    /* 00 - 0F */
    ex_lb,   ex_lb,   ex_lb,   ex_lb,
    ex_dlb,  ex_dlb,  ex_dlb,  ex_dlb,
    ex_stb,  ex_stb,  ex_stb,  ex_stb,
    ex_dstb, ex_dstb, ex_dstb, ex_dstb,
    /* 10 - 1F */
    ex_ab,   ex_ab,   ex_ab,   ex_ab,
    ex_sbb,  ex_sbb,  ex_sbb,  ex_sbb,
    ex_mb,   ex_mb,   ex_mb,   ex_mb,
    ex_db,   ex_db,   ex_db,   ex_db,
    /* 20 - 2F */
    ex_fab,  ex_fab,  ex_fab,  ex_fab,
    ex_fsb,  ex_fsb,  ex_fsb,  ex_fsb,
    ex_fmb,  ex_fmb,  ex_fmb,  ex_fmb,
    ex_fdb,  ex_fdb,  ex_fdb,  ex_fdb,
    /* 30 - 3F */
    ex_orb,  ex_orb,  ex_orb,  ex_orb,
    ex_andb, ex_andb, ex_andb, ex_andb,
    ex_cb,   ex_cb,   ex_cb,   ex_cb,
    ex_fcb,  ex_fcb,  ex_fcb,  ex_fcb,
    /* 40 - 4F */
    ex_brx,  ex_brx,  ex_brx,  ex_brx,
    ex_ill,  ex_ill,  ex_ill,  ex_ill,
    ex_xio,  ex_vio,  ex_imml, ex_ill,
    ex_ill,
#ifdef GVSC
	     ex_esqr, ex_sqrt,
#else
	     ex_ill,  ex_ill,
#endif
			       ex_bif,
    /* 50 - 5F */
    ex_sb,   ex_sbr,  ex_sbi,  ex_rb,
    ex_rbr,  ex_rbi,  ex_tb,   ex_tbr,
    ex_tbi,  ex_tsb,  ex_svbr, ex_ill,
    ex_rvbr, ex_ill,  ex_tvbr, ex_ill,
    /* 60 - 6F */
    ex_sll,  ex_srl,  ex_sra,  ex_slc,
    ex_ill,  ex_dsll, ex_dsrl, ex_dsra,
    ex_dslc, ex_ill,  ex_slr,  ex_sar,
    ex_scr,  ex_dslr, ex_dsar, ex_dscr,
    /* 70 - 7F */
    ex_jc,   ex_jci,  ex_js,   ex_soj,
    ex_br,   ex_bez,  ex_blt,  ex_bex,
    ex_ble,  ex_bgt,  ex_bnz,  ex_bge,
    ex_lsti, ex_lst,  ex_sjs,  ex_urs,
    /* 80 - 8F */
    ex_l,    ex_lr,   ex_lisp, ex_lisn,
    ex_li,   ex_lim,  ex_dl,   ex_dlr,
    ex_dli,  ex_lm,   ex_efl,  ex_lub,
    ex_llb,  ex_lubi, ex_llbi, ex_popm,
    /* 90 - 9F */
    ex_st,   ex_stc,  ex_stci, ex_mov,
    ex_sti,
#ifdef GVSC
	     ex_sfbs,
#else
	     ex_ill,
#endif
		      ex_dst,  ex_srm,
    ex_dsti, ex_stm,  ex_efst, ex_stub,
    ex_stlb, ex_subi, ex_slbi, ex_pshm,
    /* A0 - AF */
    ex_a,    ex_ar,   ex_aisp, ex_incm,
    ex_abs,  ex_dabs, ex_da,   ex_dar,
    ex_fa,   ex_far,  ex_efa,  ex_efar,
    ex_fabs,
#ifdef GVSC
	     ex_uar,  ex_ua,
#else
	     ex_ill,  ex_ill,
#endif
			       ex_ill,
    /* B0 - BF */
    ex_s,    ex_sr,   ex_sisp, ex_decm,
    ex_neg,  ex_dneg, ex_ds,   ex_dsr,
    ex_fs,   ex_fsr,  ex_efs,  ex_efsr,
    ex_fneg,
#ifdef GVSC
	     ex_usr,  ex_us,
#else
	     ex_ill,  ex_ill,
#endif
			       ex_ill,
    /* C0 - CF */
    ex_ms,   ex_msr,  ex_misp, ex_misn,
    ex_m,    ex_mr,   ex_dm,   ex_dmr,
    ex_fm,   ex_fmr,  ex_efm,  ex_efmr,
    ex_ill,  ex_ill,  ex_ill,  ex_ill,
    /* D0 - DF */
    ex_dv,   ex_dvr,  ex_disp, ex_disn,
    ex_d,    ex_dr,   ex_dd,   ex_ddr,
    ex_fd,   ex_fdr,  ex_efd,  ex_efdr,
#ifdef GVSC
    ex_ste,  ex_dste, ex_le,   ex_dle,
#else
    ex_ill,  ex_ill,  ex_ill,  ex_ill,
#endif
    /* E0 - EF */
    ex_or,   ex_orr,  ex_and,  ex_andr,
    ex_xor,  ex_xorr, ex_n,    ex_nr,
    ex_fix,  ex_flt,  ex_efix, ex_eflt,
    ex_xbr,  ex_xwr,  ex_ill,  ex_ill,
    /* F0 - FF */
    ex_c,    ex_cr,   ex_cisp, ex_cisn,
    ex_cbl,
#ifdef GVSC
	     ex_ucim,
#else
	     ex_ill,
#endif
		      ex_dc,   ex_dcr,
    ex_fc,   ex_fcr,  ex_efc,  ex_efcr,
#ifdef GVSC
    ex_ucr, ex_uc,
#else
    ex_ill, ex_ill,
#endif
		      ex_ill,  ex_nop_bpt
  };


int 
execute (void)
{
  unsigned opc_hibyte;
  int cycles;

  if (! need_speed)
    add_to_backtrace ();

  GET (CODE, simreg.ic, (short *) &opcode);
  opc_hibyte = opcode >> 8;
  upper = (opcode & 0x00F0) >> 4;
  lower =  opcode & 0x000F;

  cycles = (*exfunc[opc_hibyte]) ();
  if (cycles < 0)
    return cycles;  /* BREAKPT or MEMERR */

  instcnt++;
  total_time_in_us += (double)(uP_CYCLE_IN_NS * cycles) / 1000.0;
  workout_timing (cycles);
  workout_interrupts ();
  return OKAY;
}

