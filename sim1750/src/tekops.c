/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :   tekops.c -- Tektronix Extended Hex related functions      */
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
#include <stdlib.h>

#include "arch.h"
#include "cmd.h"  /* for sys_int() */
#include "phys_mem.h"
#include "smemacc.h"
#include "status.h"
#include "utils.h"
#include "peekpoke.h"
#include "loadfile.h"
#include "tekhex.h"


extern struct regs simreg;  /* from cpu.c */

/* Internal data */

#define MAX_SECTIONS 100

struct section
  {
    char  *name;
    ulong base_addr;
    int   length;
  };
static struct section section[MAX_SECTIONS];

int n_sections = 0;

struct symbol
  {
    char   *name;
    int    type;
    ulong  value;
    struct section *sect;
  };

static struct {
		int n_allocated;
		int n_used;
		struct symbol *sym;
	      } symdata;

/* TekHex symbol type codes */
#define GLOBAL_ADDRESS		1
#define GLOBAL_SCALAR		2
#define GLOBAL_CODE_ADDRESS	3
#define GLOBAL_DATA_ADDRESS	4
#define LOCAL_ADDRESS		5
#define LOCAL_SCALAR		6
#define LOCAL_CODE_ADDRESS	7
#define LOCAL_DATA_ADDRESS	8
static const char *typename[] =
     {
       "",
       "GLOBAL_ADDRESS",
       "GLOBAL_SCALAR",
       "GLOBAL_CODE_ADDRESS",
       "GLOBAL_DATA_ADDRESS",
       "LOCAL_ADDRESS",
       "LOCAL_SCALAR",
       "LOCAL_CODE_ADDRESS",
       "LOCAL_DATA_ADDRESS"
     };


void
init_tekops ()	/* free up previous memory allocations; initialize */
{
  while (n_sections > 0)
    free ((void *) section[--n_sections].name);
  while (symdata.n_used > 0)
    free ((void *) symdata.sym[--symdata.n_used].name);
  if (symdata.n_allocated == 0)
    {
      symdata.n_allocated = 42;  /* rat science */
      symdata.sym = (struct symbol *) calloc (symdata.n_allocated,
					      sizeof (struct symbol));
    }
}


long
find_tek_address (char *labelname)
{
  int i;

  for (i = 0; i < symdata.n_used; i++)
    if (eq (labelname, symdata.sym[i].name))
      return (long) symdata.sym[i].value;
  return -1L;
}


char *
find_tek_label (ulong address)
{
  int i = symdata.n_used;

  while (i-- > 0)
    if (symdata.sym[i].value == address)
      return symdata.sym[i].name;
  return NULL;
}


#define x_nib(ch) (isdigit (ch) ? (ch) - '0' :		\
		   isupper (ch) ? (ch) - ('A' - 10) :	\
		   islower (ch) ? (ch) - ('a' - 10) : -1)

static long 
get_xnum (char **string, int number)
{
  int nibble;
  ulong retval = 0L;
  char *p = *string;

  while (number-- > 0)
    {
      nibble = *p++;
      if ((nibble = x_nib (nibble)) < 0)
	return -1L;
      retval = (retval << 4) | (ulong) (nibble & 0xF);
    }
  *string = p;
  return (retval);
}


static void
add_symbol (char *name, int type, ushort value, struct section *section)
{
  int n;

  if ((n = symdata.n_used) == symdata.n_allocated)
    {
      symdata.n_allocated *= 2;
      if ((symdata.sym = (struct symbol *) realloc (symdata.sym,
	 symdata.n_allocated * sizeof (struct symbol))) == (struct symbol *) 0)
	problem ("tekhex: request for symbol space refused by OS");
    }
  symdata.sym[n].name = strdup (name);
  symdata.sym[n].type = type;
  symdata.sym[n].sect = section;
  symdata.sym[n].value = value;
  symdata.n_used++;
}


static int linecount;

/* Analyze and load a line from a Tektronix Extended Hex file */

int
load_tekline (char *line)
{
  char  sectname[32], *linep;
  int   i, checksum, type, blk_len, addr_len, line_len;
  ulong address;

  if (sys_int (1L))
    return (INTERRUPT);

  if (line[0] != '%')
    return error ("illegal format");
  line_len = strlen (line) - 2;
  linep = line + 1;
  blk_len = (int) get_xnum (&linep, 2);
  type = (int) get_xnum (&linep, 1);
  checksum = (int) get_xnum (&linep, 2);
  addr_len = (int) get_xnum (&linep, 1);
  if (addr_len == 0)
    addr_len = 16;

  if (line_len == blk_len)
    {
      if (checksum != (check_tekline (line) & 0xff))
	info ("checksum error in line %d", linecount);
    }
  else
    return error ("line length error in line %d", linecount);
  switch (type)
    {
    case 3:		/* Symbol */
      strncpy (sectname, linep, addr_len);
      sectname[addr_len] = '\0';
      linep += addr_len;
      while (linep - line < line_len)
	{
	  type = (int) get_xnum (&linep, 1);	/* symbol type */
	  if (type == 0)	/* section definition */
	    {
	      int   baseaddr_len = (int) get_xnum (&linep, 1);
	      ulong baseaddr     =       get_xnum (&linep, baseaddr_len);
	      int   sectlen_len  = (int) get_xnum (&linep, 1);
	      ulong sectlen      =       get_xnum (&linep, sectlen_len);

	      /* lprintf ("\nSEC %s %s\n", sectname, linep); */
	      section[n_sections].name = strdup (sectname);
	      section[n_sections].base_addr = baseaddr;
	      section[n_sections].length = sectlen;
	      n_sections++;
	    }
	  else			/* symbol definition */
	    {
	      int len, val_len;
	      char sym[40];
	      ulong val;

	      len = (int) get_xnum (&linep, 1); /* symbol name length */
	      if (len == 0)
		len = 16;
	      strncpy (sym, linep, len);	/* symbol name */
	      sym[len] = '\0';
	      linep += len;
	      val_len = (int) get_xnum (&linep, 1);	/* value length */
	      val = get_xnum (&linep, val_len);		/* value */
	      /* lprintf ("ELM %s %s %s\n", sym, sectname, linep); */
	      add_symbol (sym, type, val, &section[n_sections-1]);
	    }
	}
      break;

    case 6:		/* Data */
      address = get_xnum (&linep, addr_len);
      i = (ushort) address & 0x0001;
      address >>= 1;
      while (linep - line < line_len)
	{
	  ushort value;
	  if (i)
	    {
	      peek (address, &value);
	      poke (address, (value & 0xff00)
			   | (ushort) get_xnum (&linep, 2));
	      i = 0;
	      address++;
	    }
	  else
	    {
	      peek (address, &value);
	      poke (address, (value & 0x00ff)
			   | (ushort) (get_xnum (&linep, 2) << 8));
	      i++;
	    }
	}
      break;

    case 8:		/* Terminator */
      address = get_xnum (&linep, addr_len);
      simreg.ic = (ushort) ((address >> 1) & 0xffff);
      break;

    default:
      return error ("illegal type in line %d", linecount);
    }
  return (OKAY);
}

/* load file in Tektronix Extended Hex format */

int
si_lo (int argc, char *argv[])
{
  FILE *fpoint;
  char lline[132], *filename = argv[1];
  bool verbose_save = verbose;
  int retval;

  if (argc <= 1)
    return error ("filename missing");

  if (*filename == '"')
    {
      *filename++ = '\0';
      *(filename + strlen (filename) - 1) = '\0';
    }
  if ((fpoint = fopen (filename, "r")) == NULL)
  {
    strcat (strcpy (lline, filename), ".hex");
    if ((fpoint = fopen (lline, "r")) == NULL)
      return error ("cannot open load file '%s'", filename);
  }
  verbose = FALSE;  /* avoid info message from poke() */
  loadfile_type = TEK_HEX;
  linecount = 0;

  while (fgets (lline, sizeof (lline), fpoint) != NULL)
    {
      ++linecount;
      if (strlen (lline) < 2)
	continue;
      if ((retval = load_tekline (lline)) != OKAY)
	break;
    }
  fclose (fpoint);
  verbose = verbose_save;
  return retval;
}


/* Display symbols loaded from a TekHex loadfile */

int
display_tek_symbols ()
{
  int i;

  for (i = 0; i < n_sections; i++)
    {
      if (i == 0)
	lprintf ("Section             BaseAddr Length\n");
      lprintf ("%-20s  %04hXh     %d\n", section[i].name,
		section[i].base_addr, section[i].length);
    }
  for (i = 0; i < symdata.n_used; i++)
    {
      if (i == 0)
	{
	  lprintf ("-------------------------------------------------------\n");
	  lprintf ("Symbolname           Value Type                 Section\n");
	}
      lprintf ("%-20s %l05X %-20s %s\n", symdata.sym[i].name,
		symdata.sym[i].value, typename[symdata.sym[i].type],
		symdata.sym[i].sect->name);
    }
  return (OKAY);
}


int
si_save (int argc, char *argv[])
{
  int pgndx, locndx;
  mem_t *memptr;

  if (argc <= 1)
    return error ("filename missing");
  if (create_tekfile (argv[1]) != OKAY)
    return error ("cannot open save file '%s'", argv[1]);

  for (pgndx = 0; pgndx < N_PAGES; pgndx++)
    {
      ulong phys_address = pgndx << 12;
      if ((memptr = mem[pgndx]) == MNULL)
	continue;
      for (locndx = 0; locndx < 4096; locndx++)
	{
	  if (memptr->was_written[locndx / 32] & (1L << (locndx % 32)))
	    emit_tekword (phys_address + locndx, memptr->word[locndx]);
	}
    }

  close_tekfile ((ulong) simreg.ic);  /* other regs are lost, to be improved */

  return OKAY;
}

