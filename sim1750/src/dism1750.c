/***************************************************************************/
/*                                                                         */
/* Project   :              Reuseable Software Components                  */
/*                                                                         */
/* Component :         dism1750.c -- Mil-Std-1750 disassembler             */
/*                                                                         */
/* Copyright :          (C) Daimler-Benz Aerospace AG, 1994-97             */
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

/* In order to use this standalone, you need four other files:
   type.h, targsys.h, xiodef.h, and xiodef.c */
/* If the macro symbol GVSC is defined, then the extended architecture
   instructions implemented by the IBM GVSC chip are supported. */
/* If the macro symbol HAVE_SYMBOLS is defined, then instruction and
   operand addresses are displayed with their symbolic names if possible.
   In order to do this, the external function
       char *find_label (int bank, unsigned short address);
   must be supplied. The 'bank' argument is 0 for instruction-page addresses
   and 1 for operand-page addresses. The function returns the label string
   if found at the logical address given, or NULL if there is no label at
   that address. */
#define HAVE_SYMBOLS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>

#include "type.h"
#include "targsys.h"
#include "xiodef.h"

/* Export */

int dism1750 (char *text, ushort *word);

/* Symbolic display of labels */

#ifdef HAVE_SYMBOLS
extern char *find_label (int bank, ushort address);
#else
#define find_label(bank,addr)	NULL
#endif

/* Internal data */

static ushort opcode;
static ushort upper;  /* Upper nibble of lower byte of opcode (bits 8..11) */
static ushort lower;  /* Lower nibble of lower byte of opcode (bits 12..15) */
static ushort dataword; /* Buffer for data word that might follow opcode */

static char *msg;	/* aux. text pointer */


/******************** auxiliary print functions ************************/

void 
dsprintf (char *string, char *layout, ...)
{
  va_list vargu;
  char output_line[132];

  va_start (vargu, layout);
/*  layout = va_arg (vargu, char *); */
  vsprintf (output_line, layout, vargu);
  va_end (vargu);

  strcat (string, output_line);
}

static void 
pr_reg (ushort regno)
{
  dsprintf (msg, "R%hd", regno);
}

static void 
pr_num (ushort number)
{
  dsprintf (msg, "%hd", number);
}

static void
pr_comma ()
{
  dsprintf (msg, ",");
}

static void 
pr_str (char *string)
{
  dsprintf (msg, "%s", string);
}

/* Instruction/Operand access codes */
#define CODE 0
#define DATA 1

static void 
pr_addr (int bank)
{
  char *sym = find_label (bank, dataword);
  if (sym != NULL)
    dsprintf (msg, "%s", sym);
  else
    dsprintf (msg, "%04hX", dataword);
}


/***************** Address Mode disassembly functions ******************/

static int
dis_ill ()
{
  dsprintf (msg, "illegal opcode %04hX\n", opcode);
  return 0;
}

static int
dis_ra_addr_rx ()   /* addr in operand page */
{
  pr_reg (upper);
  pr_comma ();
  pr_addr (DATA);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_ra_caddr_rx ()  /* same as previous, but addr in instruction page */
{
  pr_reg (upper);
  pr_comma ();
  pr_addr (CODE);
  if (lower != 0)
  {
    pr_comma ();
    pr_reg (lower);
  }
  return 2;
}

static int
dis_n_addr_rx ()
{
  pr_num (upper);
  pr_comma ();
  pr_addr (DATA);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_n1_addr_rx ()   /* INCM, DECM */
{
  pr_num (upper + 1);
  pr_comma ();
  pr_addr (DATA);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_ra_rb ()
{
  pr_reg (upper);
  pr_comma ();
  pr_reg (lower);
  return 1;
}

static int
dis_n_rb ()
{
  pr_num (upper);
  pr_comma ();
  pr_reg (lower);
  return 1;
}

static int
dis_rb_n1 ()    /* shift instructions */
{
  pr_reg (lower);
  pr_comma ();
  pr_num (upper + 1);
  return 1;
}

static int
dis_ra_n1 ()   /* Immediate Short instructions */
{
  pr_reg (upper);
  pr_comma ();
  pr_num (lower + 1);
  return 1;
}

static int
dis_icr ()   /* Instruction Counter Relative branches */
{
  int distance = opcode & 0x00ff;

  if (distance >= 0x80)		/* backward branch */
    {
      distance = 0x100 - distance;
      dsprintf (msg, "-");
    }
  else
    dsprintf (msg, "+");
  dsprintf (msg, "%d", distance);
  if (distance > 9)
    dsprintf (msg, " dec.");
  return 1;
}

static int
dis_ra_data ()  /* Immediate with opcode extension */
{
  if ((opcode & 0xFF00) != 0xF500)  /* exclude UCIM from legality check */
    {
      switch (lower)
	{
	case 0x0:
	case 0xc:
	case 0xd:
	case 0xe:
	case 0xf:
	  return dis_ill ();
        }
    }
  pr_reg (upper);
  pr_comma ();
  pr_addr (DATA);
  return 2;
}

static int
dis_br_rx ()   /* Base Relative with index register */
{
  dsprintf (msg, "B");
  pr_num (((opcode & 0x0300) >> 8) + 12);
  pr_comma ();
  pr_reg (lower);
  return 1;
}

static int
dis_br_dspl ()   /* Base Relative with displacement */
{
  dsprintf (msg, "B");
  pr_num (((opcode & 0x0300) >> 8) + 12);
  pr_comma ();
  dsprintf (msg, "%02hX", opcode & 0x00FF);
  return 1;
}


static int
dis_ra_cmd_rx ()	/* XIO */
{
  int i;

  pr_reg (upper);
  pr_comma ();
  for (i = 0; xio[i].value; i++)
    if (xio[i].value == dataword)
      break;
  if (xio[i].value)
    dsprintf (msg, "%s", xio[i].name);
  else
    dsprintf (msg, "%04hX", dataword);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_bif ()		/* BIF */
{
  pr_num ((upper & 0x03) + '0');
  pr_comma ();
  pr_addr (DATA);
  if (upper & 0x04)
    {
      pr_comma ();
      pr_addr (DATA);
    }
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_c_caddr_rx ()	/* JC */
{
  static const char *cond[16] =
    {				/*    CPZN */
      "NOP",			/* 0  0000 */
      "LT",			/* 1  0001 */
      "EZ",			/* 2  0010 */
      "LE",			/* 3  0011 */
      "GT",			/* 4  0100 */
      "NZ",			/* 5  0101 */
      "GE",			/* 6  0110 */
      "UC",			/* 7  0111 */
      "CY",			/* 8  1000 */
      "CLT",			/* 9  1001 */
      "CEZ",			/* A  1010 */
      "CLE",			/* B  1011 */
      "CGT",			/* C  1100 */
      "CNZ",			/* D  1101 */
      "CGE",			/* E  1110 */
      "UC"			/* F  1111 */
    };

  dsprintf (msg, "%s,", cond[upper]);
  pr_addr (CODE);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_n ()		/* BEX */
{
  if (upper != 0)
    return dis_ill ();
  pr_num (lower);
  return 1;
}

static int
dis_addr_rx ()		/* LST(I) */
{
  if (upper != 0)
    return dis_ill ();
  pr_addr (DATA);
  if (lower != 0)
    {
      pr_comma ();
      pr_reg (lower);
    }
  return 2;
}

static int
dis_ra ()		/* XBR and URS */
{
  if (lower != 0)
    return dis_ill ();
  pr_reg (upper);
  return 1;
}

/****************** Auxiliary data for dism1750() ********************/

static const struct
  { 
    char   *mnemon;
    int    (*disasm)();
  } mnemonics[256] =
  {
    { "LB",   dis_br_dspl },		/* 00 */
    { "LB",   dis_br_dspl },		/* 01 */
    { "LB",   dis_br_dspl },		/* 02 */
    { "LB",   dis_br_dspl },		/* 03 */
    { "DLB",  dis_br_dspl },		/* 04 */
    { "DLB",  dis_br_dspl },		/* 05 */
    { "DLB",  dis_br_dspl },		/* 06 */
    { "DLB",  dis_br_dspl },		/* 07 */
    { "STB",  dis_br_dspl },		/* 08 */
    { "STB",  dis_br_dspl },		/* 09 */
    { "STB",  dis_br_dspl },		/* 0A */
    { "STB",  dis_br_dspl },		/* 0B */
    { "DSTB", dis_br_dspl },		/* 0C */
    { "DSTB", dis_br_dspl },		/* 0D */
    { "DSTB", dis_br_dspl },		/* 0E */
    { "DSTB", dis_br_dspl },		/* 0F */
    { "AB",   dis_br_dspl },		/* 10 */
    { "AB",   dis_br_dspl },		/* 11 */
    { "AB",   dis_br_dspl },		/* 12 */
    { "AB",   dis_br_dspl },		/* 13 */
    { "SBB",  dis_br_dspl },		/* 14 */
    { "SBB",  dis_br_dspl },		/* 15 */
    { "SBB",  dis_br_dspl },		/* 16 */
    { "SBB",  dis_br_dspl },		/* 17 */
    { "MB",   dis_br_dspl },		/* 18 */
    { "MB",   dis_br_dspl },		/* 19 */
    { "MB",   dis_br_dspl },		/* 1A */
    { "MB",   dis_br_dspl },		/* 1B */
    { "DB",   dis_br_dspl },		/* 1C */
    { "DB",   dis_br_dspl },		/* 1D */
    { "DB",   dis_br_dspl },		/* 1E */
    { "DB",   dis_br_dspl },		/* 1F */
    { "FAB",  dis_br_dspl },		/* 20 */
    { "FAB",  dis_br_dspl },		/* 21 */
    { "FAB",  dis_br_dspl },		/* 22 */
    { "FAB",  dis_br_dspl },		/* 23 */
    { "FSB",  dis_br_dspl },		/* 24 */
    { "FSB",  dis_br_dspl },		/* 25 */
    { "FSB",  dis_br_dspl },		/* 26 */
    { "FSB",  dis_br_dspl },		/* 27 */
    { "FMB",  dis_br_dspl },		/* 28 */
    { "FMB",  dis_br_dspl },		/* 29 */
    { "FMB",  dis_br_dspl },		/* 2A */
    { "FMB",  dis_br_dspl },		/* 2B */
    { "FDB",  dis_br_dspl },		/* 2C */
    { "FDB",  dis_br_dspl },		/* 2D */
    { "FDB",  dis_br_dspl },		/* 2E */
    { "FDB",  dis_br_dspl },		/* 2F */
    { "ORB",  dis_br_dspl },		/* 30 */
    { "ORB",  dis_br_dspl },		/* 31 */
    { "ORB",  dis_br_dspl },		/* 32 */
    { "ORB",  dis_br_dspl },		/* 33 */
    { "ANDB", dis_br_dspl },		/* 34 */
    { "ANDB", dis_br_dspl },		/* 35 */
    { "ANDB", dis_br_dspl },		/* 36 */
    { "ANDB", dis_br_dspl },		/* 37 */
    { "CB",   dis_br_dspl },		/* 38 */
    { "CB",   dis_br_dspl },		/* 39 */
    { "CB",   dis_br_dspl },		/* 3A */
    { "CB",   dis_br_dspl },		/* 3B */
    { "FCB",  dis_br_dspl },		/* 3C */
    { "FCB",  dis_br_dspl },		/* 3D */
    { "FCB",  dis_br_dspl },		/* 3E */
    { "FCB",  dis_br_dspl },		/* 3F */
    { "brx",  dis_br_rx },		/* 40    handled in dism1750() */
    { "brx",  dis_br_rx },		/* 41             "            */
    { "brx",  dis_br_rx },		/* 42             "            */
    { "brx",  dis_br_rx },		/* 43             "            */
    { "",     dis_ill },		/* 44 */
    { "",     dis_ill },		/* 45 */
    { "",     dis_ill },		/* 46 */
    { "",     dis_ill },		/* 47 */
    { "XIO",  dis_ra_cmd_rx },		/* 48 */
    { "VIO",  dis_ra_addr_rx },		/* 49 */
    { "imml", dis_ra_data },		/* 4A    handled in dism1750() */
    { "",     dis_ill },		/* 4B */
    { "",     dis_ill },		/* 4C */
#ifdef GVSC
    { "ESQR", dis_ra },			/* 4D */
    { "SQRT", dis_ra },			/* 4E */
#else
    { "",     dis_ill },		/* 4D */
    { "",     dis_ill },		/* 4E */
#endif
    { "BIF",  dis_bif },		/* 4F */
    { "SB",   dis_n_addr_rx },		/* 50 */
    { "SBR",  dis_n_rb },		/* 51 */
    { "SBI",  dis_n_addr_rx },		/* 52 */
    { "RB",   dis_n_addr_rx },		/* 53 */
    { "RBR",  dis_n_rb },		/* 54 */
    { "RBI",  dis_n_addr_rx },		/* 55 */
    { "TB",   dis_n_addr_rx },		/* 56 */
    { "TBR",  dis_n_rb },		/* 57 */
    { "TBI",  dis_n_addr_rx },		/* 58 */
    { "TSB",  dis_n_addr_rx },		/* 59 */
    { "SVBR", dis_ra_rb },		/* 5A */
    { "",     dis_ill },		/* 5B */
    { "RVBR", dis_ra_rb },		/* 5C */
    { "",     dis_ill },		/* 5D */
    { "TVBR", dis_ra_rb },		/* 5E */
    { "",     dis_ill },		/* 5F */
    { "SLL",  dis_rb_n1 },		/* 60 */
    { "SRL",  dis_rb_n1 },		/* 61 */
    { "SRA",  dis_rb_n1 },		/* 62 */
    { "SLC",  dis_rb_n1 },		/* 63 */
    { "",     dis_ill },		/* 64 */
    { "DSLL", dis_rb_n1 },		/* 65 */
    { "DSRL", dis_rb_n1 },		/* 66 */
    { "DSRA", dis_rb_n1 },		/* 67 */
    { "DSLC", dis_rb_n1 },		/* 68 */
    { "",     dis_ill },		/* 69 */
    { "SLR",  dis_ra_rb },		/* 6A */
    { "SAR",  dis_ra_rb },		/* 6B */
    { "SCR",  dis_ra_rb },		/* 6C */
    { "DSLR", dis_ra_rb },		/* 6D */
    { "DSAR", dis_ra_rb },		/* 6E */
    { "DSCR", dis_ra_rb },		/* 6F */
    { "JC",   dis_c_caddr_rx },		/* 70 */
    { "JCI",  dis_c_caddr_rx },		/* 71 */
    { "JS",   dis_ra_caddr_rx },	/* 72 */
    { "SOJ",  dis_ra_caddr_rx },	/* 73 */
    { "BR",   dis_icr },		/* 74 */
    { "BEZ",  dis_icr },		/* 75 */
    { "BLT",  dis_icr },		/* 76 */
    { "BEX",  dis_n },			/* 77 */
    { "BLE",  dis_icr },		/* 78 */
    { "BGT",  dis_icr },		/* 79 */
    { "BNZ",  dis_icr },		/* 7A */
    { "BGE",  dis_icr },		/* 7B */
    { "LSTI", dis_addr_rx },		/* 7C */
    { "LST",  dis_addr_rx },		/* 7D */
    { "SJS",  dis_ra_caddr_rx },	/* 7E */
    { "URS",  dis_ra },			/* 7F */
    { "L",    dis_ra_addr_rx },		/* 80 */
    { "LR",   dis_ra_rb },		/* 81 */
    { "LISP", dis_ra_n1 },		/* 82 */
    { "LISN", dis_ra_n1 },		/* 83 */
    { "LI",   dis_ra_addr_rx },		/* 84 */
    { "LIM",  dis_ra_addr_rx },		/* 85 */
    { "DL",   dis_ra_addr_rx },		/* 86 */
    { "DLR",  dis_ra_rb },		/* 87 */
    { "DLI",  dis_ra_addr_rx },		/* 88 */
    { "LM",   dis_n_addr_rx },		/* 89 */
    { "EFL",  dis_ra_addr_rx },		/* 8A */
    { "LUB",  dis_ra_addr_rx },		/* 8B */
    { "LLB",  dis_ra_addr_rx },		/* 8C */
    { "LUBI", dis_ra_rb },		/* 8D */
    { "LLBI", dis_ra_rb },		/* 8E */
    { "POPM", dis_ra_rb },		/* 8F */
    { "ST",   dis_ra_addr_rx },		/* 90 */
    { "STC",  dis_n_addr_rx },		/* 91 */
    { "STCI", dis_n_addr_rx },		/* 92 */
    { "MOV",  dis_ra_rb },		/* 93 */
    { "STI",  dis_ra_addr_rx },		/* 94 */
#if defined(GVSC) || defined (MA31750)
    { "SFBS", dis_ra_rb },		/* 95 */
#else
    { "",     dis_ill },		/* 95 */
#endif
    { "DST",  dis_ra_addr_rx },		/* 96 */
    { "SRM",  dis_ra_addr_rx },		/* 97 */
    { "DSTI", dis_ra_addr_rx },		/* 98 */
    { "STM",  dis_n_addr_rx },		/* 99 */
    { "EFST", dis_ra_addr_rx },		/* 9A */
    { "STUB", dis_ra_addr_rx },		/* 9B */
    { "STLB", dis_ra_addr_rx },		/* 9C */
    { "SUBI", dis_ra_addr_rx },		/* 9D */
    { "SLBI", dis_ra_addr_rx },		/* 9E */
    { "PSHM", dis_ra_rb },		/* 9F */
    { "A",    dis_ra_addr_rx },		/* A0 */
    { "AR",   dis_ra_rb },		/* A1 */
    { "AISP", dis_ra_n1 },		/* A2 */
    { "INCM", dis_n1_addr_rx },		/* A3 */
    { "ABS",  dis_ra_rb },		/* A4 */
    { "DABS", dis_ra_rb },		/* A5 */
    { "DA",   dis_ra_addr_rx },		/* A6 */
    { "DAR",  dis_ra_rb },		/* A7 */
    { "FA",   dis_ra_addr_rx },		/* A8 */
    { "FAR",  dis_ra_rb },		/* A9 */
    { "EFA",  dis_ra_addr_rx },		/* AA */
    { "EFAR", dis_ra_rb },		/* AB */
    { "FABS", dis_ra_rb },		/* AC */
#if defined(GVSC) || defined(MA31750)
    { "UAR",  dis_ra_rb },		/* AD */
    { "UA",   dis_ra_addr_rx },		/* AE */
#else
    { "",     dis_ill },		/* AD */
    { "",     dis_ill },		/* AE */
#endif
    { "",     dis_ill },		/* AF */
    { "S",    dis_ra_addr_rx },		/* B0 */
    { "SR",   dis_ra_rb },		/* B1 */
    { "SISP", dis_ra_n1 },		/* B2 */
    { "DECM", dis_n1_addr_rx },		/* B3 */
    { "NEG",  dis_ra_rb },		/* B4 */
    { "DNEG", dis_ra_rb },		/* B5 */
    { "DS",   dis_ra_addr_rx },		/* B6 */
    { "DSR",  dis_ra_rb },		/* B7 */
    { "FS",   dis_ra_addr_rx },		/* B8 */
    { "FSR",  dis_ra_rb },		/* B9 */
    { "EFS",  dis_ra_addr_rx },		/* BA */
    { "EFSR", dis_ra_rb },		/* BB */
    { "FNEG", dis_ra_rb },		/* BC */
#if defined(GVSC) || defined(MA31750)
    { "USR",  dis_ra_rb },		/* BD */
    { "US",   dis_ra_addr_rx },		/* BE */
#else
    { "",     dis_ill },		/* BD */
    { "",     dis_ill },		/* BE */
#endif
    { "",     dis_ill },		/* BF */
    { "MS",   dis_ra_addr_rx },		/* C0 */
    { "MSR",  dis_ra_rb },		/* C1 */
    { "MISP", dis_ra_n1 },		/* C2 */
    { "MISN", dis_ra_n1 },		/* C3 */
    { "M",    dis_ra_addr_rx },		/* C4 */
    { "MR",   dis_ra_rb },		/* C5 */
    { "DM",   dis_ra_addr_rx },		/* C6 */
    { "DMR",  dis_ra_rb },		/* C7 */
    { "FM",   dis_ra_addr_rx },		/* C8 */
    { "FMR",  dis_ra_rb },		/* C9 */
    { "EFM",  dis_ra_addr_rx },		/* CA */
    { "EFMR", dis_ra_rb },		/* CB */
#ifdef MA31750
    { "LSL",  dis_ra_addr_rx },		/* CC */
    { "LDL",  dis_ra_addr_rx },		/* CD */
    { "LEFL", dis_ra_addr_rx },		/* CE */
#else
    { "",     dis_ill },		/* CC */
    { "",     dis_ill },		/* CD */
    { "",     dis_ill },		/* CE */
#endif
    { "",     dis_ill },		/* CF */
    { "DV",   dis_ra_addr_rx },		/* D0 */
    { "DVR",  dis_ra_rb },		/* D1 */
    { "DISP", dis_ra_n1 },		/* D2 */
    { "DISN", dis_ra_n1 },		/* D3 */
    { "D",    dis_ra_addr_rx },		/* D4 */
    { "DR",   dis_ra_rb },		/* D5 */
    { "DD",   dis_ra_addr_rx },		/* D6 */
    { "DDR",  dis_ra_rb },		/* D7 */
    { "FD",   dis_ra_addr_rx },		/* D8 */
    { "FDR",  dis_ra_rb },		/* D9 */
    { "EFD",  dis_ra_addr_rx },		/* DA */
    { "EFDR", dis_ra_rb },		/* DB */
#ifdef GVSC
    { "STE",  dis_ra_addr_rx },		/* DC */
    { "DSTE", dis_ra_addr_rx },		/* DD */
    { "LE",   dis_ra_addr_rx },		/* DE */
    { "DLE",  dis_ra_addr_rx },		/* DF */
#else
#ifdef MA31750
    { "LSS",  dis_ra_addr_rx },		/* DC */
    { "LDS",  dis_ra_addr_rx },		/* DD */
    { "LEFS", dis_ra_addr_rx },		/* DE */
    { "",     dis_ill },		/* DF */
#else
    { "",     dis_ill },		/* DC */
    { "",     dis_ill },		/* DD */
    { "",     dis_ill },		/* DE */
    { "",     dis_ill },		/* DF */
#endif
#endif
    { "OR",   dis_ra_addr_rx },		/* E0 */
    { "ORR",  dis_ra_rb },		/* E1 */
    { "AND",  dis_ra_addr_rx },		/* E2 */
    { "ANDR", dis_ra_rb },		/* E3 */
    { "XOR",  dis_ra_addr_rx },		/* E4 */
    { "XORR", dis_ra_rb },		/* E5 */
    { "N",    dis_ra_addr_rx },		/* E6 */
    { "NR",   dis_ra_rb },		/* E7 */
    { "FIX",  dis_ra_rb },		/* E8 */
    { "FLT",  dis_ra_rb },		/* E9 */
    { "EFIX", dis_ra_rb },		/* EA */
    { "EFLT", dis_ra_rb },		/* EB */
    { "XBR",  dis_ra },			/* EC */
    { "XWR",  dis_ra_rb },		/* ED */
    { "",     dis_ill },		/* EE */
    { "",     dis_ill },		/* EF */
    { "C",    dis_ra_addr_rx },		/* F0 */
    { "CR",   dis_ra_rb },		/* F1 */
    { "CISP", dis_ra_n1 },		/* F2 */
    { "CISN", dis_ra_n1 },		/* F3 */
    { "CBL",  dis_ra_addr_rx },		/* F4 */
#ifdef GVSC
    { "UCIM", dis_ra_data },		/* F5 */
#else
    { "",     dis_ill },		/* F5 */
#endif
    { "DC",   dis_ra_addr_rx },		/* F6 */
    { "DCR",  dis_ra_rb },		/* F7 */
    { "FC",   dis_ra_addr_rx },		/* F8 */
    { "FCR",  dis_ra_rb },		/* F9 */
    { "EFC",  dis_ra_addr_rx },		/* FA */
    { "EFCR", dis_ra_rb },		/* FB */
#ifdef GVSC
    { "UCR",  dis_ra_rb },		/* FC */
    { "UC",   dis_ra_addr_rx },		/* FD */
#else
    { "",     dis_ill },		/* FC */
    { "",     dis_ill },		/* FD */
#endif
    { "",     dis_ill },		/* FE */
    { "NOP",  dis_ill }			/* FF    handled in dism1750() */
  };

static const char *start_4x[16] =
  {
    "LBX",  "DLBX", "STBX", "DSTX",
    "ABX",  "SBBX", "MBX",  "DBX",
    "FABX", "FSBX", "FMBX", "FDBX",
    "CBX",  "FCBX", "ANDX", "ORBX"
  };

static const char *start_4a[16] =
  {
    "??IM", "AIM",  "SIM",  "MIM",
    "MSIM", "DIM",  "DVIM", "ANDM",
    "ORIM", "XORM", "CIM",  "NIM",
    "??IM", "??IM", "??IM", "??IM"
  };

/*********************** Main Disassembly Function *********************/

/* Disassemble the word pointed at by `word'. If the instruction is
   of address mode Memory Direct, Memory Indirect, or Immediate Long,
   then disassemble the data word pointed at by `(word + 1)'. */
/* Put the disassembly text into `text'. */
/* Return the number of words disassembled (1 or 2.) */

int
dism1750 (char *text, ushort *word)
{
  ushort opc_hibyte;

  *(msg = text) = '\0';

  opcode = *word;
  upper = (opcode & 0x00F0) >> 4;
  lower = opcode & 0x000F;
  dataword = *(word + 1);

  opc_hibyte = (opcode & 0xFF00) >> 8;

  if (opc_hibyte >= 0x40 && opc_hibyte <= 0x43)  /* Base Relative, Indexed */
    {
      dsprintf (msg, "%-5s", start_4x[(unsigned)upper]);
      return dis_br_rx ();
    }

  if (opc_hibyte == 0x4A)			 /* Immediate Long */
    {
      dsprintf (msg, "%-5s", start_4a[(unsigned)lower]);
      return dis_ra_data ();
    }

  if (opc_hibyte == 0xFF)			 /* NOP or BPT */
    {
      switch (opcode & 0x00FF)
        {
        case 0x00:
          pr_str ("NOP");
	  break;
        case 0xFF:
	  pr_str ("BPT");
	  break;
        default:
	  return dis_ill ();
	}
      return 1;
    }

  dsprintf (msg, "%-5s", mnemonics[(unsigned)opc_hibyte].mnemon);
  return (*mnemonics[(unsigned)opc_hibyte].disasm) ();
}

