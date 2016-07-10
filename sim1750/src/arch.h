/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :       arch.h -- processor architectural definitions         */
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


#ifndef _ARCH_H
#define _ARCH_H

#include "type.h"

/* condition status bit masks (within Status Register) */
#define  CS_CARRY     0x8000
#define  CS_POSITIVE  0x4000
#define  CS_ZERO      0x2000
#define  CS_NEGATIVE  0x1000
#define  CS_ERROR     0x0000

/* mask for simreg.sys */
#define  SYS_INT   0x1
#define  SYS_DMA   0x2
#define  SYS_TA    0x4
#define  SYS_TB    0x8

/* mask for simreg.ft */
#define  FT_MEMPROT	0x8000   /* CPU memory protection error */
#define  FT_ILL_IO	0x0400   /* illegal XIO address */
#define  FT_SYSFAULT0	0x0100   /* sysfault 0 watchdog (F9450/MODUS) */
#define  FT_ILL_ADDR	0x0080   /* illegal instruction/operand address */
#define  FT_ILL_INSTR	0x0040   /* illegal instruction opcode */
#define  FT_PRIV_INSTR	0x0020   /* privileged instruction (AK!=0) */

/* interrupts in PIR */
#define  INTR_NULL      0x0000
#define  INTR_PWRDWN    0x8000
#define  INTR_MACHERR   0x4000
#define  INTR_USER0     0x2000
#define  INTR_FLTOFL    0x1000
#define  INTR_FIXOFL    0x0800
#define  INTR_BEX       0x0400
#define  INTR_FLTUFL    0x0200
#define  INTR_TA        0x0100
#define  INTR_USER1     0x0080
#define  INTR_TB        0x0040
#define  INTR_USER2     0x0020
#define  INTR_USER3     0x0010
#define  INTR_IOLVL1    0x0008
#define  INTR_USER4     0x0004
#define  INTR_IOLVL2    0x0002
#define  INTR_USER5     0x0001

/* memory banks: instruction page = CODE, operand page = DATA */
#define  CODE     0
#define  DATA     1

/* define arithmetic operations for sim_arith */
typedef enum { ARI_ADD, ARI_SUB, ARI_MULS, ARI_MUL, ARI_DIVV, ARI_DIV }
	operation_kind;

/* define types of variables for sim_arith */
typedef enum { VAR_INT, VAR_LONG, VAR_FLOAT, VAR_DOUBLE } datatype;

/* Simulator register file */
struct regs
  {
    short  r[16];  /* 0..15 */
    ushort pir;    /* 16 */
    ushort mk;     /* 17 */
    ushort ft;     /* 18 */
    ushort ic;     /* 19 */
    ushort sw;     /* 20 */
    ushort ta;     /* 21 */
    ushort tb;     /* 22 */
    ushort go;     /* not a real register but handled like TA/TB */
    ushort sys;    /* system configuration register */
  };

extern struct regs simreg;  /* defined in cpu.c */

/* MMU related */
struct mmureg
  {
    ushort ppa      : 8;
    ushort reserved : 3;
    ushort e_w      : 1;
    ushort al       : 4;
  };

extern struct mmureg pagereg[2][16][16];  /* defined in cpu.c */

#endif

