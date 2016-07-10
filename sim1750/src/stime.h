/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :     stime.h -- chips' instruction timing definitions        */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                                                                         */
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

/************************************************************************/
/* Change record:							*/
/*									*/
/* 95-02-17	Added timing for IMA31750	(Jiri Gaisler, ESTEC)	*/
/*									*/
/************************************************************************/

#include "targsys.h"

/* Manufacturers' processor-timing info */

#if defined (PACE)
       /* Notes:
	  1. Figures given are for one wait state in both instruction fetch
	     and data read/write.
          2. Instructions appear in the same order as in Table 2.2
             of Performance Semiconductor's PACE1750A product description. */

/* Integer arithmetic and logic: */
#define nc_AR      5
#define nc_AB     13
#define nc_ABX    13
#define nc_AISP    8
#define nc_A      15
#define nc_AIM    10
#define nc_DAR     9
#define nc_DA     24
#define nc_SR      5
#define nc_SBB    13
#define nc_SBBX   13
#define nc_SISP    8
#define nc_S      15
#define nc_SIM    10
#define nc_DSR     9
#define nc_DS     24
#define nc_MSR    23
#define nc_MISP   26
#define nc_MISN   26
#define nc_MS     33
#define nc_MSIM   28
#define nc_MR     26
#define nc_MB     34
#define nc_MBX    34
#define nc_M      36
#define nc_MIM    31
#define nc_DMR    69
#define nc_DM     84
#define nc_DVR    58
#define nc_DISP   61
#define nc_DISN   61
#define nc_DV     68
#define nc_DVIM   63
#define nc_DR     73
#define nc_DB     81
#define nc_DBX    81
#define nc_D      83
#define nc_DIM    78
#define nc_DDR   133
#define nc_DD    148
#define nc_INCM   18
#define nc_DECM   20
#define nc_ABS   (negative ?  9 : 6)
#define nc_DABS  (negative ? 12 : 9)
#define nc_NEG     5
#define nc_DNEG    9
#define nc_CR      5
#define nc_CB     13
#define nc_CBX    13
#define nc_CISP    8
#define nc_CISN    8
#define nc_C      15
#define nc_CIM    10
#define nc_CBL    16
#define nc_DCR     6
#define nc_DC     21
#define nc_ORR     5
#define nc_ORB    13
#define nc_ORBX   13
#define nc_OR     15
#define nc_ORIM   10
#define nc_XORR    5
#define nc_XOR    15
#define nc_XORM   10
#define nc_ANDR    5
#define nc_ANDB   13
#define nc_ANDX   13
#define nc_AND    15
#define nc_ANDM   10
#define nc_NR      5
#define nc_N      15
#define nc_NIM    10
/* Floating point arithmetic */
#define nc_FAR    28
#define nc_FAB    41
#define nc_FABX   41
#define nc_FA     43
#define nc_EFAR   50
#define nc_EFA    70
#define nc_FSR    28
#define nc_FSB    41
#define nc_FSBX   41
#define nc_FS     43
#define nc_EFSR   50
#define nc_EFS    70
#define nc_FMR    43
#define nc_FMB    56
#define nc_FMBX   56
#define nc_FM     58
#define nc_EFMR   99
#define nc_EFM   116
#define nc_FDR    89
#define nc_FDB    96
#define nc_FDBX   96
#define nc_FD     98
#define nc_EFDR  183
#define nc_EFD   194
#define nc_FCR     6
#define nc_FCB    19
#define nc_FCBX   19
#define nc_FC     21
#define nc_EFCR   17
#define nc_EFC    37
#define nc_FABS   (negative ? 21 : 9)
#define nc_FNEG   18
#define nc_FIX    23
#define nc_FLT    17
#define nc_EFIX   44
#define nc_EFLT   26
/* Bit operations */
#define nc_SBR     5
#define nc_SB     15
#define nc_SBI    20
#define nc_RBR     5
#define nc_RB     15
#define nc_RBI    20
#define nc_TBR     6
#define nc_TB     16
#define nc_TBI    21
#define nc_TSB    21
#define nc_SVBR    5
#define nc_RVBR    5
#define nc_TVBR    6
/* Shift operations */
#define nc_SLL    (8 + n_shifts)
#define nc_SRL    (8 + n_shifts)
#define nc_SRA    (8 + n_shifts)
#define nc_SLC    (8 + n_shifts)
#define nc_DSLL   (15 + n_shifts)
#define nc_DSRL   (15 + n_shifts)
#define nc_DSRA   (15 + n_shifts)
#define nc_DSLC   (15 + n_shifts)
#define nc_SLR    (n_shifts<0 ? 14-n_shifts : n_shifts>0 ? 17+n_shifts : 12)
#define nc_SAR    (n_shifts<0 ? 14-n_shifts : n_shifts>0 ? 17+n_shifts : 12)
#define nc_SCR    (n_shifts<0 ? 14-n_shifts : n_shifts>0 ? 17+n_shifts : 12)
#define nc_DSLR   (n_shifts<0 ? 20-n_shifts : n_shifts>0 ? 23+n_shifts : 12)
#define nc_DSAR   (n_shifts<0 ? 20-n_shifts : n_shifts>0 ? 23+n_shifts : 12)
#define nc_DSCR   (n_shifts<0 ? 20-n_shifts : n_shifts>0 ? 23+n_shifts : 12)
/* Load/store/exchange */
#define nc_LR      5
#define nc_LB     13
#define nc_LBX    13
#define nc_LISP    5
#define nc_LISN    5
#define nc_L      15
#define nc_LIM    10
#define nc_LI     20
#define nc_DLR     9
#define nc_DLB    17
#define nc_DLBX   17
#define nc_DL     19
#define nc_DLI    29
#define nc_EFL    32
#define nc_LUB    16
#define nc_LUBI   21
#define nc_LLB    15
#define nc_LLBI   20
#define nc_STB    13
#define nc_STBX   13
#define nc_ST     15
#define nc_STI    20
#define nc_STC    15
#define nc_STCI   20
#define nc_DSTB   18
#define nc_DSTX   18
#define nc_DST    20
#define nc_DSTI   25
#define nc_SRM    24
#define nc_EFST   25
#define nc_STUB   20
#define nc_SUBI   25
#define nc_STLB   20
#define nc_SLBI   25
#define nc_XBR     6
#define nc_XWR     6
/* Multiple load/store */
#define nc_PSHM   (10 + 11 * n_pushes)
#define nc_POPM   (11 + 14 * n_pops)
#define nc_LM     (15 +  5 * n_loads)
#define nc_STM    (13 +  5 * n_stores)
#define nc_MOV    (n_moves == 0 ? 11 : 29 + 10 * n_moves)
/* Program control */
#define nc_JC     (jump_taken ? 18 : 10)
#define nc_JCI    (jump_taken ? 25 : 15)
#define nc_JS     15
#define nc_SOJ    (jump_taken ? 19 : 17)
#define nc_BR     15
#define nc_BRcc   (branch_taken ? 15 : 5)    /* for all conditional branches */
#define nc_BEX   109
#define nc_LST    41
#define nc_LSTI   47
#define nc_SJS    19
#define nc_URS    15
#define nc_NOP     5
#define nc_BPT    20
#define nc_BIF    37
#define nc_XIO    40	/* not very exact... */
#define nc_VIO   300	/* ...not even trying this one! */

/****************************** end of PACE *******************************/

#elif defined (F9450)
	/* Source: Fairchild Semiconductor Corp.,
	   "F9450 High-Performance 16-Bit Bipolar Microprocessor",
	   Preliminary Data Sheet - November 1985  */

/* Integer arithmetic and logic: */
#define nc_AR      5
#define nc_AB     12 
#define nc_ABX    12 
#define nc_AISP    8
#define nc_A      13 
#define nc_AIM    12 
#define nc_DAR    18 
#define nc_DA     24
#define nc_SR      5 
#define nc_SBB    12 
#define nc_SBBX   12
#define nc_SISP    8
#define nc_S      13
#define nc_SIM    12
#define nc_DSR    18
#define nc_DS     24
#define nc_MSR    39
#define nc_MISP   42
#define nc_MISN   42
#define nc_MS     47
#define nc_MSIM   46
#define nc_MR     37
#define nc_MB     44
#define nc_MBX    44
#define nc_M      45
#define nc_MIM    44
#define nc_DMR   126
#define nc_DM    132
#define nc_DVR    98
#define nc_DISP   98
#define nc_DISN   98
#define nc_DV    103
#define nc_DVIM  105
#define nc_DR     97
#define nc_DB    101
#define nc_DBX   101
#define nc_D     102
#define nc_DIM   104
#define nc_DDR   239
#define nc_DD    245
#define nc_INCM   17
#define nc_DECM   17
#define nc_ABS    (negative ? 10 : 5)
#define nc_DABS   (negative ? 21 : 13)
#define nc_NEG     5
#define nc_DNEG   18
#define nc_CR      8
#define nc_CB     15
#define nc_CBX    15
#define nc_CISP   11
#define nc_CISN   11
#define nc_C      16
#define nc_CIM    15
#define nc_CBL    40
#define nc_DCR    21
#define nc_DC     27 
#define nc_ORR     4
#define nc_ORB    11
#define nc_ORBX   11
#define nc_OR     12
#define nc_ORIM   11
#define nc_XORR    4
#define nc_XOR    12
#define nc_XORM   11
#define nc_ANDR    4
#define nc_ANDB   11
#define nc_ANDX   11
#define nc_AND    12
#define nc_ANDM   11
#define nc_NR      7
#define nc_N      15
#define nc_NIM    14
/* Floating point arithmetic */
#define nc_FAR    62
#define nc_FAB    67
#define nc_FABX   67
#define nc_FA     68
#define nc_EFAR   71
#define nc_EFA    78
#define nc_FSR    62
#define nc_FSB    67
#define nc_FSBX   67
#define nc_FS     68
#define nc_EFSR   71
#define nc_EFS    78
#define nc_FMR   120
#define nc_FMB   125
#define nc_FMBX  125
#define nc_FM    126
#define nc_EFMR  251
#define nc_EFM   258
#define nc_FDR   234
#define nc_FDB   239
#define nc_FDBX  239
#define nc_FD    240
#define nc_EFDR  480
#define nc_EFD   487
#define nc_FCR    52
#define nc_FCB    57
#define nc_FCBX   57
#define nc_FC     58
#define nc_EFCR   52
#define nc_EFC    65
#define nc_FABS   (negative ? 62 : 16)
#define nc_FNEG   56
#define nc_FIX    10
#define nc_FLT    16
#define nc_EFIX   21
#define nc_EFLT   25
/* Bit operations */
#define nc_SBR     7
#define nc_SB     16
#define nc_SBI    20
#define nc_RBR     7
#define nc_RB     16
#define nc_RBI    20
#define nc_TBR     7
#define nc_TB     15
#define nc_TBI    19
#define nc_TSB    23
#define nc_SVBR    7
#define nc_RVBR    7
#define nc_TVBR    7
/* Shift operations */
#define nc_SLL    (7 + 3 * n_shifts)
#define nc_SRL    (7 + 3 * n_shifts)
#define nc_SRA    (7 + 3 * n_shifts)
#define nc_SLC    (7 + 3 * n_shifts)
#define nc_DSLL   (16 + 6 * n_shifts)
#define nc_DSRL   (16 + 6 * n_shifts)
#define nc_DSRA   (16 + 6 * n_shifts)
#define nc_DSLC   (16 + 6 * n_shifts)
#define nc_SLR    (n_shifts<0 ? 21+n_shifts*3 : n_shifts>0 ? 38+n_shifts*5 : 11)
#define nc_SAR    (n_shifts<0 ? 21+n_shifts*3 : n_shifts>0 ? 29+n_shifts*5 : 11)
#define nc_SCR    (n_shifts<0 ? 21+n_shifts*3 : n_shifts>0 ? 24+n_shifts*3 : 11)
#define nc_DSLR   (n_shifts<0 ? 30+n_shifts*6 : n_shifts>0 ? 44+n_shifts*8 : 11)
#define nc_DSAR   (n_shifts<0 ? 30+n_shifts*6 : n_shifts>0 ? 35+n_shifts*8 : 11)
#define nc_DSCR   (n_shifts<0 ? 33+n_shifts*9 : n_shifts>0 ? 33+n_shifts*9 : 11)
/* Load/store/exchange */
#define nc_LR      4
#define nc_LB     11
#define nc_LBX    11
#define nc_LISP    7
#define nc_LISN    7
#define nc_L      12
#define nc_LIM    12
#define nc_LI     16
#define nc_DLR    16
#define nc_DLB    21
#define nc_DLBX   21
#define nc_DL     22
#define nc_DLI    26
#define nc_EFL    26
#define nc_LUB    15
#define nc_LUBI   19
#define nc_LLB    12
#define nc_LLBI   16
#define nc_STB    11
#define nc_STBX   11
#define nc_ST     12
#define nc_STI    16
#define nc_STC    12
#define nc_STCI   16
#define nc_DSTB   15
#define nc_DSTX   15
#define nc_DST    16
#define nc_DSTI   20
#define nc_SRM    25
#define nc_EFST   20
#define nc_STUB   16
#define nc_SUBI   20
#define nc_STLB   16
#define nc_SLBI   20
#define nc_XBR     7
#define nc_XWR    10
/* Multiple load/store */
#define nc_PSHM   (16 + 12 * n_pushes)
#define nc_POPM   (20 + 16 * n_pops)
#define nc_LM     (16 +  8 * n_loads)
#define nc_STM    (17 +  9 * n_stores)
#define nc_MOV    (n_moves == 0 ? 9 : 37 + 13 * n_moves)
/* Program control */
#define nc_JC     (jump_taken ? 17 :  9)
#define nc_JCI    (jump_taken ? 21 : 13)
#define nc_JS     12
#define nc_SOJ    (jump_taken ? 17 : 13)
#define nc_BR     14
#define nc_BRcc	  (branch_taken ? 15 : 4)  /* for all conditional branches */
#define nc_BEX    87
#define nc_LST    42
#define nc_LSTI   46
#define nc_SJS    22
#define nc_URS    15
#define nc_NOP     9
#define nc_BPT    27
#define nc_BIF    34
#define nc_XIO    60  /* very roughly */
#define nc_VIO    70  /* very roughly */

/***************************** end of F9450 *******************************/

#elif defined (GVSC)
	/* 1. Source: Space Systems Loral,
	      "Software Programmer's Manual for the ASPS Computer",
	      Cage Code 52088, Document No. MO1.01, Rev. T-,
	      January 18, 1994
	   2. Notes:
	      a) For approx. half the instructions, SSL give a
	         fractional figure of estimated cycles. This is due to 
	         the pipelining used in the GVSC. In these cases, cycles
	         have been *rounded up*, therefore figures given here are
	         slightly pessimistic.
	      b) The estimates are based on a 3 cycle memory read and a 
	         3 cycle memory write. */

/* Integer arithmetic and logic: */
#define nc_AR      3
#define nc_AB      6
#define nc_ABX     6
#define nc_AISP    4
#define nc_A       7
#define nc_AIM     5
#define nc_DAR     4
#define nc_DA      8
#define nc_SR      4
#define nc_SBB     7
#define nc_SBBX    7
#define nc_SISP    5
#define nc_S       8
#define nc_SIM     8
#define nc_DSR     5
#define nc_DS      9
#define nc_MSR     8
#define nc_MISP    8
#define nc_MISN    8
#define nc_MS     12
#define nc_MSIM    9
#define nc_MR      8
#define nc_MB     11
#define nc_MBX    11
#define nc_M      12
#define nc_MIM     9
#define nc_DMR    10
#define nc_DM     14
#define nc_DVR    39
#define nc_DISP   39
#define nc_DISN   42
#define nc_DV     43
#define nc_DVIM   40
#define nc_DR     39
#define nc_DB     42
#define nc_DBX    43
#define nc_D      43
#define nc_DIM    40
#define nc_DDR    54 
#define nc_DD     58
#define nc_INCM   14
#define nc_DECM   14
#define nc_ABS     8
#define nc_DABS    6
#define nc_NEG     8
#define nc_DNEG    9
#define nc_CR      4
#define nc_CB     10
#define nc_CBX     7
#define nc_CISP    5
#define nc_CISN    4
#define nc_C       8
#define nc_CIM     6
#define nc_CBL    10
#define nc_DCR     5
#define nc_DC      9
#define nc_ORR     3
#define nc_ORB     6
#define nc_ORBX    7
#define nc_OR      7
#define nc_ORIM    5
#define nc_XORR    3
#define nc_XOR     7
#define nc_XORM    4
#define nc_ANDR    3
#define nc_ANDB    6
#define nc_ANDX    7
#define nc_AND     7
#define nc_ANDM    4
#define nc_NR      3
#define nc_N       7
#define nc_NIM     5
/* Floating point arithmetic */
#define nc_FAR     6
#define nc_FAB     9
#define nc_FABX    8
#define nc_FA     10
#define nc_EFAR    7
#define nc_EFA    14
#define nc_FSR     6
#define nc_FSB     9
#define nc_FSBX   10
#define nc_FS     10
#define nc_EFSR    7
#define nc_EFS    14
#define nc_FMR     9
#define nc_FMB    12
#define nc_FMBX   12
#define nc_FM     13
#define nc_EFMR   12
#define nc_EFM    19
#define nc_FDR    44
#define nc_FDB    47
#define nc_FDBX   48
#define nc_FD     48
#define nc_EFDR   60
#define nc_EFD    67
#define nc_FCR     6
#define nc_FCB     9
#define nc_FCBX   10
#define nc_FC     10
#define nc_EFCR   11
#define nc_EFC    18
#define nc_FABS    5
#define nc_FNEG    8
#define nc_FIX     5
#define nc_FLT     5
#define nc_EFIX    6
#define nc_EFLT    5
/* Bit operations */
#define nc_SBR     3
#define nc_SB      7
#define nc_SBI    12
#define nc_RBR     3
#define nc_RB     10
#define nc_RBI    15
#define nc_TBR     3
#define nc_TB      7
#define nc_TBI    12
#define nc_TSB    14
#define nc_SVBR    3
#define nc_RVBR    3
#define nc_TVBR    3
/* Shift operations */
#define nc_SLL     3
#define nc_SRL     3
#define nc_SRA     3
#define nc_SLC     3
#define nc_DSLL    3
#define nc_DSRL    3
#define nc_DSRA    3
#define nc_DSLC    3
#define nc_SLR     7
#define nc_SAR     6
#define nc_SCR     7
#define nc_DSLR    7
#define nc_DSAR    7
#define nc_DSCR    7
/* Load/store/exchange */
#define nc_LR      3
#define nc_LB      6 
#define nc_LBX     7
#define nc_LISP    3
#define nc_LISN    3
#define nc_L       7
#define nc_LIM     4
#define nc_LI     12
#define nc_DLR     4
#define nc_DLB     7
#define nc_DLBX    8
#define nc_DL      8
#define nc_DLI    14
#define nc_EFL    12
#define nc_LUB     7
#define nc_LUBI   12
#define nc_LLB     7
#define nc_LLBI   12
#define nc_STB     7
#define nc_STBX    8
#define nc_ST      8
#define nc_STI    10
#define nc_STC     8
#define nc_STCI   13
#define nc_DSTB    7
#define nc_DSTX    8
#define nc_DST     8
#define nc_DSTI   13
#define nc_SRM    10
#define nc_EFST   12
#define nc_STUB   14
#define nc_SUBI   17
#define nc_STLB    9
#define nc_SLBI   10
#define nc_XBR     3
#define nc_XWR     3
/* Multiple load/store */
#define nc_PSHM   57
#define nc_POPM   32
#define nc_LM     32
#define nc_STM    55
#define nc_MOV    42
/* Program control */
#define nc_JC      7
#define nc_JCI    12
#define nc_JS      9
#define nc_SOJ     8
#define nc_BR      6
#define nc_BRcc    6		/* for all conditional branches */
#define nc_BEX    13
#define nc_LST    11
#define nc_LSTI   16
#define nc_SJS     9
#define nc_URS     8
#define nc_NOP     3
#define nc_BPT     6
#define nc_BIF     2
#define nc_XIO    15		/* very roughly */
#define nc_VIO    68		/* very roughly */

/****************************** end of GVSC *******************************/

#elif defined (MA31750)

/* MA31750 - 0 waitstates according to IMA31750 Data sheet (July 1994)  */
/* Average and weighted average timings are used (rounded upwards)      */
/* Added by J.Gaisler ESA/ESTEC/WS					*/

#define WSTATES 0		/* Number of waitstates */
#define MEM_CYCLE (2 + WSTATES)	/* Clocks per memory cycle */
#define INT_CYCLE 2		/* Clocks per internal cycle */

/* Integer arithmetic  and logic: */
#define nc_AR      (1*MEM_CYCLE)
#define nc_AB      (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ABX     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_AISP    (1*MEM_CYCLE)
#define nc_A       (3*MEM_CYCLE)
#define nc_AIM     (2*MEM_CYCLE)
#define nc_DAR     (1*MEM_CYCLE)
#define nc_DA      (4*MEM_CYCLE)
#define nc_SR      (1*MEM_CYCLE)
#define nc_SBB     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SBBX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SISP    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_S       (3*MEM_CYCLE)
#define nc_SIM     (2*MEM_CYCLE)
#define nc_DSR     (1*MEM_CYCLE)
#define nc_DS      (4*MEM_CYCLE)
#define nc_MSR     (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MISP    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MISN    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_MS      (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MSIM    (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MR      (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_MB      (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MBX     (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_M       (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_MIM     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DMR     (1*MEM_CYCLE + 18*INT_CYCLE)
#define nc_DM      (4*MEM_CYCLE + 18*INT_CYCLE)
#define nc_DVR     (1*MEM_CYCLE + 24*INT_CYCLE)
#define nc_DISP    (1*MEM_CYCLE + 24*INT_CYCLE)
#define nc_DISN    (1*MEM_CYCLE + 24*INT_CYCLE)
#define nc_DV      (3*MEM_CYCLE + 24*INT_CYCLE)
#define nc_DVIM    (2*MEM_CYCLE + 24*INT_CYCLE)
#define nc_DR      (1*MEM_CYCLE + 28*INT_CYCLE)
#define nc_DB      (2*MEM_CYCLE + 29*INT_CYCLE)
#define nc_DBX     (2*MEM_CYCLE + 29*INT_CYCLE)
#define nc_D       (3*MEM_CYCLE + 28*INT_CYCLE)
#define nc_DIM     (2*MEM_CYCLE + 28*INT_CYCLE)
#define nc_DDR     (1*MEM_CYCLE + 41*INT_CYCLE)
#define nc_DD      (4*MEM_CYCLE + 41*INT_CYCLE)
#define nc_INCM    (4*MEM_CYCLE)
#define nc_DECM    (4*MEM_CYCLE)
#define nc_ABS     (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DABS    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_NEG     (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DNEG    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CR      (1*MEM_CYCLE)
#define nc_CB      (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CBX     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CISP    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CISN    (1*MEM_CYCLE)
#define nc_C       (3*MEM_CYCLE)
#define nc_CIM     (2*MEM_CYCLE)
#define nc_CBL     (4*MEM_CYCLE + 3*INT_CYCLE)
#define nc_DCR     (1*MEM_CYCLE)
#define nc_DC      (4*MEM_CYCLE)
#define nc_ORR     (1*MEM_CYCLE)
#define nc_ORB     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ORBX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_OR      (3*MEM_CYCLE)
#define nc_ORIM    (2*MEM_CYCLE)
#define nc_XORR    (1*MEM_CYCLE)
#define nc_XOR     (3*MEM_CYCLE)
#define nc_XORM    (2*MEM_CYCLE)
#define nc_ANDR    (1*MEM_CYCLE)
#define nc_ANDB    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ANDX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_AND     (3*MEM_CYCLE)
#define nc_ANDM    (2*MEM_CYCLE)
#define nc_NR      (1*MEM_CYCLE)
#define nc_N       (3*MEM_CYCLE)
#define nc_NIM     (2*MEM_CYCLE)
/* Floating point arithmetic  */
#define nc_FAR    (1*MEM_CYCLE + 7*INT_CYCLE)
#define nc_FAB    (3*MEM_CYCLE + 9*INT_CYCLE)
#define nc_FABX   (3*MEM_CYCLE + 9*INT_CYCLE)
#define nc_FA     (4*MEM_CYCLE + 9*INT_CYCLE)
#define nc_EFAR   (1*MEM_CYCLE + 21*INT_CYCLE)
#define nc_EFA    (5*MEM_CYCLE + 20*INT_CYCLE)
#define nc_FSR    (1*MEM_CYCLE + 9*INT_CYCLE)
#define nc_FSB    (3*MEM_CYCLE + 10*INT_CYCLE)
#define nc_FSBX   (3*MEM_CYCLE + 10*INT_CYCLE)
#define nc_FS     (4*MEM_CYCLE + 9*INT_CYCLE)
#define nc_EFSR   (1*MEM_CYCLE + 23*INT_CYCLE)
#define nc_EFS    (5*MEM_CYCLE + 22*INT_CYCLE)
#define nc_FMR    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_FMB    (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_FMBX   (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_FM     (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_EFMR   (1*MEM_CYCLE + 34*INT_CYCLE)
#define nc_EFM    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_FDR    (1*MEM_CYCLE + 43*INT_CYCLE)
#define nc_FDB    (3*MEM_CYCLE + 44*INT_CYCLE)
#define nc_FDBX   (3*MEM_CYCLE + 44*INT_CYCLE)
#define nc_FD     (4*MEM_CYCLE + 43*INT_CYCLE)
#define nc_EFDR   (1*MEM_CYCLE + 113*INT_CYCLE)
#define nc_EFD    (5*MEM_CYCLE + 113*INT_CYCLE)
#define nc_FCR    (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_FCB    (3*MEM_CYCLE + 5*INT_CYCLE)
#define nc_FCBX   (3*MEM_CYCLE + 5*INT_CYCLE)
#define nc_FC     (4*MEM_CYCLE + 5*INT_CYCLE)
#define nc_EFCR   (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_EFC    (5*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FABS   (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_FNEG   (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FIX    (1*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FLT    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_EFIX   (1*MEM_CYCLE + 9*INT_CYCLE)
#define nc_EFLT   (1*MEM_CYCLE + 9*INT_CYCLE)
/* Bit operations */
#define nc_SBR    (1*MEM_CYCLE)
#define nc_SB     (4*MEM_CYCLE)
#define nc_SBI    (5*MEM_CYCLE)
#define nc_RBR    (1*MEM_CYCLE)
#define nc_RB     (4*MEM_CYCLE)
#define nc_RBI    (5*MEM_CYCLE)
#define nc_TBR    (1*MEM_CYCLE)
#define nc_TB     (3*MEM_CYCLE)
#define nc_TBI    (4*MEM_CYCLE)
#define nc_TSB    (2*MEM_CYCLE + 3*INT_CYCLE)
#define nc_SVBR   (1*MEM_CYCLE)
#define nc_RVBR   (1*MEM_CYCLE)
#define nc_TVBR   (1*MEM_CYCLE)
/* Shift operations */
#define nc_SLL    (1*MEM_CYCLE)
#define nc_SRL    (1*MEM_CYCLE)
#define nc_SRA    (1*MEM_CYCLE)
#define nc_SLC    (1*MEM_CYCLE)
#define nc_DSLL   (1*MEM_CYCLE)
#define nc_DSRL   (1*MEM_CYCLE)
#define nc_DSRA   (1*MEM_CYCLE)
#define nc_DSLC   (1*MEM_CYCLE)
#define nc_SLR    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_SAR    (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_SCR    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DSLR   (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DSAR   (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_DSCR   (1*MEM_CYCLE + 2*INT_CYCLE)
/* Load/store/exchange */
#define nc_LR     (1*MEM_CYCLE)
#define nc_LB     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LBX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LISP   (1*MEM_CYCLE)
#define nc_LISN   (1*MEM_CYCLE)
#define nc_L      (3*MEM_CYCLE)
#define nc_LIM    (2*MEM_CYCLE)
#define nc_LI     (4*MEM_CYCLE)
#define nc_DLR    (1*MEM_CYCLE)
#define nc_DLB    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DLBX   (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DL     (4*MEM_CYCLE)
#define nc_DLI    (5*MEM_CYCLE)
#define nc_EFL    (5*MEM_CYCLE)
#define nc_LUB    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LUBI   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LLB    (3*MEM_CYCLE)
#define nc_LLBI   (4*MEM_CYCLE)
#define nc_STB    (2*MEM_CYCLE)
#define nc_STBX   (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ST     (3*MEM_CYCLE)
#define nc_STI    (4*MEM_CYCLE)
#define nc_STC    (3*MEM_CYCLE)
#define nc_STCI   (4*MEM_CYCLE)
#define nc_DSTB   (3*MEM_CYCLE)
#define nc_DSTX   (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DST    (4*MEM_CYCLE)
#define nc_DSTI   (5*MEM_CYCLE)
#define nc_SRM    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_EFST   25
#define nc_STUB   (4*MEM_CYCLE)
#define nc_SUBI   (5*MEM_CYCLE)
#define nc_STLB   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SLBI   (5*MEM_CYCLE + 1*INT_CYCLE)
#define nc_XBR    (1*MEM_CYCLE)
#define nc_XWR    (1*MEM_CYCLE + 2*INT_CYCLE)
/* Multiple load/store */
#define nc_PSHM   (n_pushes*MEM_CYCLE + 8*INT_CYCLE)
#define nc_POPM   (n_pops*MEM_CYCLE + 4*INT_CYCLE)
#define nc_LM     ((3+n_loads)*MEM_CYCLE)
#define nc_STM    ((2+n_stores)*MEM_CYCLE + 1*INT_CYCLE)
#define nc_MOV    ((1+2*n_moves)*MEM_CYCLE + 7*INT_CYCLE)
/* Program control */
#define nc_JC     (3*MEM_CYCLE)
#define nc_JCI    (4*MEM_CYCLE)
#define nc_JS     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SOJ    (3*MEM_CYCLE)
#define nc_BR     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_BRcc   (2*MEM_CYCLE)   /* Not sure on this figure ... */
#define nc_BEX    (11*MEM_CYCLE + 14*INT_CYCLE)
#define nc_LST    (6*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LSTI   (7*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SJS    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_URS    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_NOP    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_BPT    (1*MEM_CYCLE + 6*INT_CYCLE)
#define nc_BIF    (3*MEM_CYCLE + 5*INT_CYCLE)   /* Same as XIO ...?  */
#define nc_XIO    (3*MEM_CYCLE + 5*INT_CYCLE)
#define nc_VIO   300	/* ...not even trying this one! */

/***************************** end of MA31750 *****************************/

#elif defined (MAS281)

/*
 * GEC-Plessey MAS281 timing from the GEC-Plessey data sheet, DS3563-3.3
 * dated July 1995. Added by Chris Nettleton, November 1996.
 */

#define MEM_CYCLE (8)	/* Clocks per memory cycle */
#define INT_CYCLE (11)	/* Clocks per internal cycle */

/* Integer arithmetic  and logic: */
#define nc_AR      (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_AB      (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_ABX     (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_AISP    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_A       (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_AIM     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DAR     (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_DA      (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SR      (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SBB     (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_SBBX    (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_SISP    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_S       (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SIM     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DSR     (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_DS      (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_MSR     (1*MEM_CYCLE + 7*INT_CYCLE)
#define nc_MISP    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_MISN    (1*MEM_CYCLE + 8*INT_CYCLE)
#define nc_MS      (3*MEM_CYCLE + 7*INT_CYCLE)
#define nc_MSIM    (2*MEM_CYCLE + 7*INT_CYCLE)
#define nc_MR      (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_MB      (2*MEM_CYCLE + 7*INT_CYCLE)
#define nc_MBX     (2*MEM_CYCLE + 7*INT_CYCLE)
#define nc_M       (3*MEM_CYCLE + 5*INT_CYCLE)
#define nc_MIM     (2*MEM_CYCLE + 5*INT_CYCLE)
#define nc_DMR     (1*MEM_CYCLE + 41*INT_CYCLE)
#define nc_DM      (4*MEM_CYCLE + 40*INT_CYCLE)
#define nc_DVR     (1*MEM_CYCLE + 21*INT_CYCLE)
#define nc_DISP    (1*MEM_CYCLE + 20*INT_CYCLE)
#define nc_DISN    (1*MEM_CYCLE + 21*INT_CYCLE)
#define nc_DV      (3*MEM_CYCLE + 21*INT_CYCLE)
#define nc_DVIM    (2*MEM_CYCLE + 21*INT_CYCLE)
#define nc_DR      (1*MEM_CYCLE + 22*INT_CYCLE)
#define nc_DB      (2*MEM_CYCLE + 23*INT_CYCLE)
#define nc_DBX     (2*MEM_CYCLE + 23*INT_CYCLE)
#define nc_D       (3*MEM_CYCLE + 22*INT_CYCLE)
#define nc_DIM     (2*MEM_CYCLE + 23*INT_CYCLE)
#define nc_DDR     (1*MEM_CYCLE + 80*INT_CYCLE)
#define nc_DD      (4*MEM_CYCLE + 78*INT_CYCLE)
#define nc_INCM    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DECM    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ABS     (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DABS    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_NEG     (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DNEG    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_CR      (1*MEM_CYCLE)
#define nc_CB      (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CBX     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_CISP    (1*MEM_CYCLE)
#define nc_CISN    (1*MEM_CYCLE)
#define nc_C       (3*MEM_CYCLE)
#define nc_CIM     (2*MEM_CYCLE)
#define nc_CBL     (4*MEM_CYCLE + 3*INT_CYCLE)
#define nc_DCR     (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DC      (4*MEM_CYCLE)
#define nc_ORR     (1*MEM_CYCLE)
#define nc_ORB     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ORBX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_OR      (3*MEM_CYCLE)
#define nc_ORIM    (2*MEM_CYCLE)
#define nc_XORR    (1*MEM_CYCLE)
#define nc_XOR     (3*MEM_CYCLE)
#define nc_XORM    (2*MEM_CYCLE)
#define nc_ANDR    (1*MEM_CYCLE)
#define nc_ANDB    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_ANDX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_AND     (3*MEM_CYCLE)
#define nc_ANDM    (2*MEM_CYCLE)
#define nc_NR      (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_N       (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_NIM     (2*MEM_CYCLE + 1*INT_CYCLE)
/* Floating point arithmetic  */
#define nc_FAR    (1*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FAB    (3*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FABX   (3*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FA     (4*MEM_CYCLE + 8*INT_CYCLE)
#define nc_EFAR   (1*MEM_CYCLE + 22*INT_CYCLE)
#define nc_EFA    (5*MEM_CYCLE + 20*INT_CYCLE)
#define nc_FSR    (1*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FSB    (3*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FSBX   (3*MEM_CYCLE + 8*INT_CYCLE)
#define nc_FS     (4*MEM_CYCLE + 8*INT_CYCLE)
#define nc_EFSR   (1*MEM_CYCLE + 23*INT_CYCLE)
#define nc_EFS    (5*MEM_CYCLE + 21*INT_CYCLE)
#define nc_FMR    (1*MEM_CYCLE + 13*INT_CYCLE)
#define nc_FMB    (3*MEM_CYCLE + 13*INT_CYCLE)
#define nc_FMBX   (3*MEM_CYCLE + 13*INT_CYCLE)
#define nc_FM     (4*MEM_CYCLE + 12*INT_CYCLE)
#define nc_EFMR   (1*MEM_CYCLE + 60*INT_CYCLE)
#define nc_EFM    (5*MEM_CYCLE + 58*INT_CYCLE)
#define nc_FDR    (1*MEM_CYCLE + 33*INT_CYCLE)
#define nc_FDB    (3*MEM_CYCLE + 33*INT_CYCLE)
#define nc_FDBX   (3*MEM_CYCLE + 33*INT_CYCLE)
#define nc_FD     (4*MEM_CYCLE + 33*INT_CYCLE)
#define nc_EFDR   (1*MEM_CYCLE + 103*INT_CYCLE)
#define nc_EFD    (5*MEM_CYCLE + 101*INT_CYCLE)
#define nc_FCR    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FCB    (2*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FCBX   (2*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FC     (3*MEM_CYCLE + 3*INT_CYCLE)
#define nc_EFCR   (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_EFC    (5*MEM_CYCLE + 3*INT_CYCLE)
#define nc_FABS   (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_FNEG   (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_FIX    (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_FLT    (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_EFIX   (1*MEM_CYCLE + 13*INT_CYCLE)
#define nc_EFLT   (1*MEM_CYCLE + 8*INT_CYCLE)
/* Bit operations */
#define nc_SBR    (1*MEM_CYCLE)
#define nc_SB     (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SBI    (5*MEM_CYCLE + 2*INT_CYCLE)
#define nc_RBR    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_RB     (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_RBI    (5*MEM_CYCLE + 2*INT_CYCLE)
#define nc_TBR    (1*MEM_CYCLE)
#define nc_TB     (3*MEM_CYCLE)
#define nc_TBI    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_TSB    (4*MEM_CYCLE)
#define nc_SVBR   (1*MEM_CYCLE)
#define nc_RVBR   (1*MEM_CYCLE)
#define nc_TVBR   (1*MEM_CYCLE)
/* Shift operations */
#define nc_SLL    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SRL    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SRA    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SLC    (1*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DSLL   (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_DSRL   (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DSRA   (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DSLC   (1*MEM_CYCLE + 3*INT_CYCLE)
#define nc_SLR    (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_SAR    (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_SCR    (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_DSLR   (1*MEM_CYCLE + 4*INT_CYCLE)
#define nc_DSAR   (1*MEM_CYCLE + 5*INT_CYCLE)
#define nc_DSCR   (1*MEM_CYCLE + 3*INT_CYCLE)
/* Load/store/exchange */
#define nc_LR     (1*MEM_CYCLE)
#define nc_LB     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LBX    (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LISP   (1*MEM_CYCLE)
#define nc_LISN   (1*MEM_CYCLE)
#define nc_L      (3*MEM_CYCLE)
#define nc_LIM    (2*MEM_CYCLE)
#define nc_LI     (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DLR    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DLB    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DLBX   (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DL     (4*MEM_CYCLE)
#define nc_DLI    (5*MEM_CYCLE + 1*INT_CYCLE)
#define nc_EFL    (5*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LUB    (3*MEM_CYCLE)
#define nc_LUBI   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LLB    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_LLBI   (4*MEM_CYCLE + 2*INT_CYCLE)
#define nc_STB    (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_STBX   (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_ST     (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_STI    (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_STC    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_STCI   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_DSTB   (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DSTX   (3*MEM_CYCLE + 2*INT_CYCLE)
#define nc_DST    (4*MEM_CYCLE)
#define nc_DSTI   (5*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SRM    (4*MEM_CYCLE + 3*INT_CYCLE)
#define nc_EFST   (5*MEM_CYCLE)
#define nc_STUB   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SUBI   (5*MEM_CYCLE + 3*INT_CYCLE)
#define nc_STLB   (4*MEM_CYCLE + 1*INT_CYCLE)
#define nc_SLBI   (5*MEM_CYCLE + 2*INT_CYCLE)
#define nc_XBR    (1*MEM_CYCLE)
#define nc_XWR    (1*MEM_CYCLE + 2*INT_CYCLE)
/* Multiple load/store */
#define nc_PSHM   (n_pushes*MEM_CYCLE + n_pushes*5*INT_CYCLE)
#define nc_POPM   (n_pops*MEM_CYCLE + n_pops*3*INT_CYCLE)
#define nc_LM     ((3+n_loads)*MEM_CYCLE + 1*INT_CYCLE)
#define nc_STM    ((3+n_stores)*MEM_CYCLE + 1*INT_CYCLE)
#define nc_MOV    ((1+4*n_moves)*MEM_CYCLE + (1+7*n_moves)*INT_CYCLE)
/* Program control */
#define nc_JC     (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_JCI    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_JS     (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_SOJ    (2*MEM_CYCLE + 3*INT_CYCLE)
#define nc_BR     (2*MEM_CYCLE + 2*INT_CYCLE)
#define nc_BRcc   (2*MEM_CYCLE + 1*INT_CYCLE)
#define nc_BEX    (16*MEM_CYCLE + 12*INT_CYCLE)
#define nc_LST    (8*MEM_CYCLE + 3*INT_CYCLE)
#define nc_LSTI   (9*MEM_CYCLE + 4*INT_CYCLE)
#define nc_SJS    (4*MEM_CYCLE + 3*INT_CYCLE)
#define nc_URS    (3*MEM_CYCLE + 1*INT_CYCLE)
#define nc_NOP    (1*MEM_CYCLE + 2*INT_CYCLE)
#define nc_BPT    (3*MEM_CYCLE + 4*INT_CYCLE)
#define nc_BIF    (3*MEM_CYCLE + 5*INT_CYCLE)   /* Same as XIO ...?  */
#define nc_XIO    (3*MEM_CYCLE + 5*INT_CYCLE)
#define nc_VIO   300	/* ...not even trying this one! */

/**************************** end of MAS281 *******************************/

#else
		/*************** DEFINE FURTHER BRANDS HERE ***************/

/* Integer arithmetic and logic: */
#define nc_AR  
#define nc_AB  
#define nc_ABX 
#define nc_AISP
#define nc_A   
#define nc_AIM 
#define nc_DAR 
#define nc_DA  
#define nc_SR  
#define nc_SBB 
#define nc_SBBX
#define nc_SISP
#define nc_S   
#define nc_SIM 
#define nc_DSR 
#define nc_DS  
#define nc_MSR 
#define nc_MISP
#define nc_MISN
#define nc_MS  
#define nc_MSIM
#define nc_MR  
#define nc_MB  
#define nc_MBX 
#define nc_M   
#define nc_MIM 
#define nc_DMR 
#define nc_DM  
#define nc_DVR 
#define nc_DISP
#define nc_DISN
#define nc_DV  
#define nc_DVIM
#define nc_DR  
#define nc_DB  
#define nc_DBX 
#define nc_D   
#define nc_DIM 
#define nc_DDR 
#define nc_DD  
#define nc_INCM
#define nc_DECM
#define nc_ABS 
#define nc_DABS
#define nc_NEG 
#define nc_DNEG
#define nc_CR  
#define nc_CB  
#define nc_CBX 
#define nc_CISP
#define nc_CISN
#define nc_C   
#define nc_CIM 
#define nc_CBL 
#define nc_DCR 
#define nc_DC  
#define nc_ORR 
#define nc_ORB 
#define nc_ORBX
#define nc_OR  
#define nc_ORIM
#define nc_XORR
#define nc_XOR 
#define nc_XORM
#define nc_ANDR
#define nc_ANDB
#define nc_ANDX
#define nc_AND 
#define nc_ANDM
#define nc_NR  
#define nc_N   
#define nc_NIM 
/* Floating point arithmetic */
#define nc_FAR 
#define nc_FAB 
#define nc_FABX
#define nc_FA  
#define nc_EFAR
#define nc_EFA 
#define nc_FSR 
#define nc_FSB 
#define nc_FSBX
#define nc_FS  
#define nc_EFSR
#define nc_EFS 
#define nc_FMR 
#define nc_FMB 
#define nc_FMBX
#define nc_FM  
#define nc_EFMR
#define nc_EFM 
#define nc_FDR 
#define nc_FDB 
#define nc_FDBX
#define nc_FD  
#define nc_EFDR
#define nc_EFD 
#define nc_FCR 
#define nc_FCB 
#define nc_FCBX
#define nc_FC  
#define nc_EFCR
#define nc_EFC 
#define nc_FABS
#define nc_FNEG
#define nc_FIX 
#define nc_FLT 
#define nc_EFIX
#define nc_EFLT
/* Bit operations */
#define nc_SBR 
#define nc_SB  
#define nc_SBI 
#define nc_RBR 
#define nc_RB  
#define nc_RBI 
#define nc_TBR 
#define nc_TB  
#define nc_TBI 
#define nc_TSB 
#define nc_SVBR
#define nc_RVBR
#define nc_TVBR
/* Shift operations */
#define nc_SLL 
#define nc_SRL 
#define nc_SRA 
#define nc_SLC 
#define nc_DSLL
#define nc_DSRL
#define nc_DSRA
#define nc_DSLC
#define nc_SLR 
#define nc_SAR 
#define nc_SCR 
#define nc_DSLR
#define nc_DSAR
#define nc_DSCR
/* Load/store/exchange */
#define nc_LR  
#define nc_LB    
#define nc_LBX 
#define nc_LISP
#define nc_LISN
#define nc_L   
#define nc_LIM 
#define nc_LI  
#define nc_DLR 
#define nc_DLB 
#define nc_DLBX
#define nc_DL  
#define nc_DLI 
#define nc_EFL 
#define nc_LUB 
#define nc_LUBI
#define nc_LLB 
#define nc_LLBI
#define nc_STB 
#define nc_STBX
#define nc_ST  
#define nc_STI 
#define nc_STC 
#define nc_STCI
#define nc_DSTB
#define nc_DSTX
#define nc_DST 
#define nc_DSTI
#define nc_SRM 
#define nc_EFST
#define nc_STUB
#define nc_SUBI
#define nc_STLB
#define nc_SLBI
#define nc_XBR 
#define nc_XWR 
/* Multiple load/store */
#define nc_PSHM
#define nc_POPM
#define nc_LM  
#define nc_STM 
#define nc_MOV 
/* Program control */
#define nc_JC  
#define nc_JCI 
#define nc_JS  
#define nc_SOJ 
#define nc_BR
#define nc_BRcc		/* for all conditional branches */
#define nc_BEX 
#define nc_LST 
#define nc_LSTI
#define nc_SJS 
#define nc_URS 
#define nc_NOP 
#define nc_BPT
#define nc_BIF 
#define nc_XIO 
#define nc_VIO 

#endif

