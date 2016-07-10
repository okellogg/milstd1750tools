/***************************************************************************/
/*                                                                         */
/* Project   :        as1750 -- Mil-Std-1750 Assembler and Linker          */
/*                                                                         */
/* Component :       tldldm.c -- write out a TLD Load Module file          */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-1997            */
/*                                                                         */
/* Author    :       Oliver M. Kellogg, Dornier Satellite Systems,         */
/*                       Dept. RST13, D-81663 Munich, Germany.             */
/* Contact   :             oliver.kellogg@space.otn.dasa.de                */
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
#include "type.h"
#include "utils.h"
#include "tldldm.h"

#define COMMAND    1
#define ADDRESS    2
#define COUNT      7
#define CHECKSUM   8
#define DATASTART 12

#define get_word(from)      get_nibbles (from, 4)
#define put_word(to,word)   put_nibbles (to, (ulong)(word), 4)

int
rotl16 (int num, int n_shifts)	/* Rotate-left a 16 bit word */
{
  ulong buf;

  if (n_shifts < 0 || n_shifts > 15)
    problem ("tldldm: rotl16 called with illegal n_shifts");
  buf = ((ulong) num & 0xFFFFL) << n_shifts;
  buf = (buf & 0xFFFFL) | (buf >> 16);
  return (int) buf;
}


static int global_chksum;
static int chksum_count;

static int
check_tldline (char *line)
{
  int i, code, datacnt;
  bool address_field_used = TRUE, cmd_m_c_t = FALSE;

  if (line[0] != '/')
    problem ("ldm: illegal start character for LDM line");

  switch (line[COMMAND])
  {
   case 'M':
    cmd_m_c_t = TRUE;		/* slides through to following case */
   case 'I':
   case 'O':
    code = 0x9;
    break;
   case 'N':
    code = 0xA;
    break;
   case 'L':
    code = 0xA + (*(line + 5) - '0'); /* Instruction => 0xA, Oprnd => 0xB */
    break;
   case 'Q':
    code = 0xB;
    break;
   case 'A':
    address_field_used = FALSE;
    code = 0x6;
    break;
   case 'T':
    code = 0x6;
    cmd_m_c_t = TRUE;
    break;
   case 'Z':
    address_field_used = FALSE;	/* slides through to following case */
   case 'G':
   case 'H':
    code = 0x8;
    break;
   case 'C':
    cmd_m_c_t = TRUE;		/* slides through to following case */
   case 'B':
   case 'P':
    code = 0xE;
    break;
   default:
    problem ("illegal Command type in TLD LDM line");
  }
  code <<= 1;
  if (address_field_used)
  {
    int addr_hinibble = xtoi (line[ADDRESS]);
    long addr16 = get_word (line + ADDRESS + 1);
    if (addr16 == -1L)
      problem ("ldm: illegal char in address field");
    code ^=  (int) addr16;
    if (addr_hinibble == -1)
      problem ("ldm: illegal char in MS-nibble of address");
    if (cmd_m_c_t)
      code = rotl16 (code, 1) ^ addr_hinibble;
  }
  if ((datacnt = xtoi (line[COUNT])) == -1)
    problem ("ldm: illegal character in data count");
  for (i = 0; i < datacnt; i++)
  {
    long dataword = get_word (line + DATASTART + (4 * i));
    if (dataword == -1L)
      problem ("ldm: illegal char in data word");
    code = rotl16 (code, 1) ^ (int) dataword;
  }

  /* Update the global checksum */
  if (chksum_count == 0)
    global_chksum = rotl16 (code, 1);
  else
    global_chksum = rotl16 (global_chksum, 1) ^ code;
  chksum_count++;

  return code;
}


extern section_t curr_sect;	/* from main.c, set in do_output() */
extern int n_assigns[2];	/* Instruction/Operand ASSIGN counter */
extern bool llmflag;		/* create LLM format instead of LDM */
static FILE *fp;
static char line[128];
static int data_count;

void
emit_tldword (ulong startaddr, ushort word)
{
  char *data_start = line + DATASTART;

  if (data_count)
    {
      /* add to line */
      put_word (data_start + data_count * 4, word);
      data_count++;
      if (data_count == 15)
	{
	  /* close line because it's full */
	  line[COUNT] = itox (data_count);
	  strcpy (data_start + data_count * 4, "\n");
	  put_word (line + CHECKSUM, check_tldline (line));
	  if (llmflag)
	    line[1] = (curr_sect / 2 == 0) ? 'I' : 'O';		/* for logical addressing */
	  fputs (line, fp);
	  data_count = 0;
	}
    }
  else
    {
      /* make new line and add */
      put_nibbles (line + ADDRESS, startaddr, 5);
      put_word (data_start, word);
      data_count++;
    }
}


void
finish_tldline ()
{
  if (data_count > 0)		/* close line if still open */
    {
      line[COUNT] = itox (data_count);
      strcpy (line + DATASTART + data_count * 4, "\n");
      put_word (line + CHECKSUM, check_tldline (line));
      if (llmflag)
	line[1] = (curr_sect / 2 == 0) ? 'I' : 'O';	/* for logical addressing */
      fputs (line, fp);
      data_count = 0;
    }
}


status
create_tldfile (char *outfname)
{
  int i, n, i_o;

  if ((fp = fopen (outfname, "w")) == (FILE *) 0)
    return ERROR;

  line[0] = '/';

  /* do possible ASSIGNs */
  for (i_o = 0; i_o <= 1; i_o++)
    {
      for (n = 0; n < n_assigns[i_o]; n++)
	{
	  struct pagereg_assignment *pap = &assign[i_o][n];
	  if (llmflag)
	    {
	      line[COMMAND] = 'L';
	      line[ADDRESS] = itox ((int) pap->as);
	      put_nibbles (line + ADDRESS + 1, (ulong) i_o, 3);
	      line[ADDRESS + 4] = itox ((int) pap->logaddr_hinibble);
	      line[COUNT] = itox (pap->length);
	      for (i = 0; i < pap->length; i++)
		{
		  line[DATASTART + 4 * i] = '2';	/* code for Physical Allocation */
		  put_nibbles (line + DATASTART + 4 * i + 1,
			       (ulong) pap->contents++, 3);
		}
	    }
	  else
	    {
	      ulong first_pagereg = (ulong) ((pap->as << 4) | pap->logaddr_hinibble);
	      line[COMMAND] = (i_o == 0) ? 'N' : 'Q';
	      put_nibbles (line + ADDRESS, first_pagereg, 5);
	      line[COUNT] = itox (pap->length);
	      for (i = 0; i < pap->length; i++)
		put_word (line + DATASTART + 4 * i, (ulong) pap->contents++);
	    }
	  strcpy (line + DATASTART + 4 * i, "\n");
	  put_word (line + CHECKSUM, check_tldline (line));
	  fputs (line, fp);
	}
    }

  data_count = 0;
  chksum_count = 0;
  line[COMMAND] = 'M';
  return OKAY;
}


struct pagereg_assignment assign[2][MAX_ASSIGNS];
int n_assigns[2] = {0, 0};

void
close_tldfile (ulong transfer_address)
{
  if (llmflag == FALSE && (transfer_address >> 16) > 0L)
    {
      transfer_address &= 0xFFFFL;
      fprintf (stderr,
       "Transfer Address truncated, only least significant 16 bits used\n");
    }
  /* put Address State of Transfer Address ?  -- To Be Done
  line[COMMAND] = 'A';
  put_nibbles (line + ADDRESS, 0L, 5);	// Address field unused
  line[COUNT] = '1';
  ////////////// Certainly NOT how it should be done, just an example:
  put_word (line + DATASTART, transfer_address >> 16);
  ////////////////////////////////////////////////////////////////////
  put_word (line + CHECKSUM, check_tldline (line));
  strcpy (line + DATASTART + 4, "\n");
  fputs (line, fp);
   */
  /* put Transfer Address */
  line[COMMAND] = 'T';
  put_nibbles (line + ADDRESS, transfer_address & 0xFFFFL, 5);
  line[COUNT] = '0';
  put_word (line + CHECKSUM, check_tldline (line));
  strcpy (line + DATASTART, "\n");
  fputs (line, fp);
  /* put Global Checksum */
  strcpy (line, "/Z     1    xxxx\n");
  put_word (line + DATASTART, global_chksum);
  fputs (line, fp);
  fclose (fp);
}

