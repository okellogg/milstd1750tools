/***************************************************************************/
/*                                                                         */
/* Project   :        as1750 -- Mil-Std-1750 Assembler and Linker          */
/*                                                                         */
/* Component :     tekhex.c -- Tektronix Extended Hex file generation      */
/*                                                                         */
/* Copyright :          (C) Daimler-Benz Aerospace AG, 1994-1997           */
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
#include <ctype.h>
#include <string.h>

#include "utils.h"
#ifdef AS1750
#include "common.h"
extern status error (char *layout,...);
#else /* sim1750 */
#include "type.h"
#include "status.h"
#endif

#ifndef elsecase
#define elsecase   break; case
#endif

int
check_tekline (char *line)
{
  int i, tekval, len = strlen (line) - 1, chksum = 0;
  char *p = line + 1;

  for (i = 1; i < len; i++, p++)
    {
      if (i != 4 && i != 5)	/* else skip checksum */
	{
	  if (isdigit (*p))
	    tekval = *p - '0';
	  else if (isupper (*p))
	    tekval = *p - 'A' + 10;
	  else if (islower (*p))
	    tekval = *p - 'a' + 40;
	  else
	    {
	      switch (*p)
		{
		case '$':
		  tekval = 36;
		elsecase '%':
		  tekval = 37;
		elsecase '.':
		  tekval = 38;
		elsecase '_':
		  tekval = 39;
		  break;
		default:
		  return error ("illegal character '%c' in tekhex line", *p);
		}
	    }
	  chksum += tekval;
	}
    }
  return (chksum & 0xff);
}


static void
put_xnum (char **p, ushort num)
{
  put_nibbles (*p, (ulong) num, 4);
  *p += 4;
}

static FILE *fp;
static char tekline[128];
static char *linep;
static bool is_new_line;
#define DATASTART 12

void
finish_tekline ()
{
  int datalen = linep - (tekline + DATASTART);

  if (datalen > 0)		/* close line if still open */
    {
      /* linelength without '%' and '\n' */
      put_nibbles (tekline + 1, (ulong) (linep - tekline - 1), 2);
      strcpy (linep, "\n");
      put_nibbles (tekline + 4, (ulong) check_tekline (tekline), 2);
      fputs (tekline, fp);
      linep = tekline + DATASTART;
      is_new_line = TRUE;
    }
}


void
emit_tekword (ulong startaddr, ushort word)
{
  static ulong last_addr = 0xEFFFFFFF;	/* any impossible start value */
  int n_words = (linep - (tekline + DATASTART)) / 4;

  if (startaddr != last_addr + 1 && n_words > 0)
    {
      finish_tekline ();
      n_words = 0;
    }
  put_xnum (&linep, word);	/* add to line */
  if (n_words == 0)		/* new line; put start address */
    put_nibbles (tekline + 7, startaddr << 1, 5);	/* BYTE address */
  else if (n_words >= 15)	/* finish line */
    finish_tekline ();
  last_addr = startaddr;
}

#ifdef AS1750

/* TekHex symbol type codes */
#define GLOBAL_ADDRESS		1
#define GLOBAL_SCALAR		2
#define GLOBAL_CODE_ADDRESS	3
#define GLOBAL_DATA_ADDRESS	4
#define LOCAL_ADDRESS		5
#define LOCAL_SCALAR		6
#define LOCAL_CODE_ADDRESS	7
#define LOCAL_DATA_ADDRESS	8

static bool seen_section[4];
static section_t currsect;
static const char *sectname[] =
{"Init", "Normal", "Konst", "Static"};

static void
write_symbol (symbol_t sym)
{
  struct objblock *obj = &objblk[sym->frag_index];
  int len, type;

  if (obj->section != currsect || !sym->is_global)
    return;
  if (!seen_section[obj->section] || is_new_line)
    {
      is_new_line = FALSE;
      len = strlen (sectname[obj->section]);
      tekline[6] = itox (len);	/* length of following Section Name */
      linep = tekline + 7;
      strcpy (linep, sectname[obj->section]);
      linep += len;
      if (!seen_section[obj->section])
	{
	  *linep++ = '0';	/* Section Definition type */
	  *linep++ = '5';	/* length of following Base Address */
	  put_nibbles (linep, objblk[sym->frag_index].start_address, 5);
	  linep += 5;
	  *linep++ = '4';	/* length of following Section Length */
	  put_xnum (&linep, (ushort) objblk[sym->frag_index].n_used);
	  seen_section[obj->section] = TRUE;
	}
    }
  if (obj->section == Init || obj->section == Normal)
    type = GLOBAL_CODE_ADDRESS;
  else
    type = GLOBAL_DATA_ADDRESS;
  *linep++ = itox (type);
  len = strlen (sym->name);
  if (len > 16)
    len = 16;
  *linep++ = itox ((len == 16) ? 0 : len);	/* coding of length of Symbol */
  strncpy (linep, sym->name, len);
  linep += len;
  *linep++ = '4';		/* length of following Value of symbol */
  put_xnum (&linep, (ushort) sym->value);
  if (linep - tekline > 64)
    finish_tekline ();
}

#endif

int
create_tekfile (char *outfname)
{
  if ((fp = fopen (outfname, "w")) == (FILE *) 0)
    return ERROR;

#ifdef AS1750
  tekline[0] = '%';
  tekline[3] = '3';		/* Type 3 = Symbol Block */
  /* Format of Symbol Block: "%bb3cclSECTNAMEtlBASEADDRlSECTLENlSYMBOLlVALUE.."
     for symbol definition:  "%bb3cclSECTNAMEtlSYMBOLlVALUElSYMBOLlVALUE..."
     where bb = block length, cc = checksum, l = length of following string,
     t = Type;  all numeric values in ASCII hex characters ('0'..'9','A'..'F')
   */

  for (currsect = Init; currsect <= Static; currsect++)
    {
      linep = tekline + 7;
      apply (symroot, write_symbol);
      finish_tekline ();
    }
#endif

  strcpy (tekline, "%!!6!!5!!!!!####");
  linep = tekline + DATASTART;
  return OKAY;
}


void
close_tekfile (ulong transfer_address)
{
  finish_tekline ();
  /* Termination Block */
  sprintf (tekline, "%%0B8xx5%05lX\n", transfer_address * 2);
  put_nibbles (tekline + 4, (ulong) check_tekline (tekline), 2);
  fputs (tekline, fp);
  fclose (fp);
}
