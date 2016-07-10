/***************************************************************************/
/*                                                                         */
/* Project   :       as1750 -- Mil-Std-1750 Assembler and Linker           */
/*                                                                         */
/* Component :      main.c -- top-level routines and main program          */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-1997            */
/*                     (C) 1998-2012 Oliver M. Kellogg                     */
/* Contact   :             okellogg@users.sourceforge.net                  */
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

#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdlib.h>

#include "version.h"
#include "common.h"
#include "as1750.h"
#include "macsym.h"
#include "utils.h"
#include "tekhex.h"
#include "tldldm.h"
#include "flt1750.h"

/* Global data */

struct objblock objblk[MAX_FRAGS];

struct relblock relblk;

struct fileinfo file[MAX_FILES];
int curr_file;

int curr_frag;
int n_frags = 4;		/* Init,Normal,Konst,Static are always there */

struct macro_symbol macsym[MAX_MACSYMS];
int n_macsyms = 0;

static ulong transfer_address;	/* set by option "-t"; default: code_startaddr */

bool case_sensitive = FALSE;	/* symbol-name case sensitivity; option: -S */

char *as1750_inc_path;		/* system symbol */

/* Local data */

static int file_highmark = 0;	/* Counter of total files */

typedef enum { Assembling, Skipping_hot, Skipping_cold } state_t;
#define MAX_IFDEPTH 32
static state_t ifstate[MAX_IFDEPTH];
static int ifdepth;

static struct
  {
    char *name;
    struct linelist *startline, *endline;
  }
macro[MAX_MACROS];
static int n_macros = 0;

#define MAX_WHILEDEPTH 16
static struct linelist *whilestart[MAX_WHILEDEPTH];
static int whiledepth;

static char *call_env_mainname;	/* Option `-C' sets the main program's name */

static char include_option_path[132];	/* for option `-I' */

static bool verbose = FALSE;	/* option `-v' */

static char outfname[128];
static char exportfname[100];

static FILE *export_file;

#ifndef min
#define min(x,y)  ((x) < (y) ? (x) : (y))
#endif

static status assemble_file (char *, bool);


static void
dsprintf (char *string, char *layout,...)
{
  va_list vargu;
  char output_line[132];

  va_start (vargu, layout);
  vsprintf (output_line, layout, vargu);
  va_end (vargu);

  strcat (string, output_line);
}


/* Determine if we are currently assembling, or just skipping (due to an IF
   whose condition evaluated to 0.)  "Hot" skipping means that we were
   assembling prior to the last IF processed. "Cold" skipping is when we
   were not assembling prior to the last IF. */
static state_t
check_ifstate ()
{
  if (ifdepth > 0)
    {
      int i = ifdepth;
      while (i > 0)
	{
	  if (ifstate[i] != Assembling)
	    return ifstate[i];
	  i--;
	}
    }
  return Assembling;
}


/* Find a macro symbol.
   Return index into macsym[] if found, or -1 if not found. */
int
find_macsym (char *name)
{
  int i = n_macsyms;

  while (--i >= 0)
    if (eq (name, macsym[i].name))
      return i;
  return -1;
}


/* Do an in-place expansion of a macro symbol. */
static status
expand_macsym (char *start, int curr_macro, char **macparm, int n_macparms)
{
  int i, name_len, val_len, ttl_len;
  char *sym, *after_sym;
  bool done = FALSE;
  static char valbuf[80];

  sym = start + 1;
  if ((after_sym = findc (sym, '`')) == NULL)
    return error ("macro symbol ending backquote not found");
  name_len = after_sym - sym;
  if (name_len == 0)
    return error ("syntax error at macro symbol usage");
  ttl_len = strlen (start);
  if (name_len == 1 && curr_macro >= 0)	 /* Could be a special macro param. */
    {
      if (isdigit (*sym))
	{
	  int parnum = *sym - '0';
	  if (parnum > n_macparms)
	    return error ("macro parameter used which has not been supplied");
	  strcpy (valbuf, macparm[parnum]);
	  done = TRUE;
	}
      else if (*sym == '#')
	{
	  sprintf (valbuf, "%d", n_macparms);
	  done = TRUE;
	}
      else if (!issymstart (*sym))
	return error ("unknown special macro parameter at '%s'", start);
    }
  *after_sym = '\0';		/* ending backquote is no longer needed */
  if (!done)
    {
      if ((i = find_macsym (sym)) < 0)
	return error ("undefined macro symbol '%s'", sym);
      strcpy (valbuf, macsym[i].value);
    }
  val_len = strlen (valbuf);
  if (val_len > name_len + 2)
    {
      int remaining_len = strlen (after_sym + 1);

      while (remaining_len >= 0)
	*(start + val_len + remaining_len--) = *(start + ttl_len--);
    }
  else if (val_len < name_len + 2)
    strcpy (start + val_len, start + name_len + 2);
  strncpy (start, valbuf, val_len);	/* strncpy doesn't copy the '\0' ! */
  return OKAY;
}

/* Expand all macro symbols in text */
static status
expand (char *text, int curr_macro, char **macparm, int n_macparms)
{
  while ((text = findc (text, '`')) != NULL)
    if (expand_macsym (text, curr_macro, macparm, n_macparms) == ERROR)
      return ERROR;
  return OKAY;
}

/**********************************************************************/

static char path_and_name[256];
static int filename_start;
static bool any_error = FALSE, unix_compatible = FALSE;

static void
assemble_file_for_unresolved_reference (symbol_t sym)
{
  struct stat stat_buf;

  if (sym->defining_file < 0 && sym->is_referenced)
    {
      if (!sym->is_global && !unix_compatible)
	{
	  fprintf (stderr, "Error (link): undefined local symbol '%s'\n",
		   sym->name);
	  any_error = TRUE;
	}
      else
	{
	  sprintf (path_and_name + filename_start, "%s.s", sym->name);
	  if (stat (path_and_name, &stat_buf) < 0)
	    {
	      fprintf (stderr,
		       "Error (link): can't resolve global reference '%s'\n",
		       sym->name);
	      any_error = TRUE;
	    }
	  else
	    {
	      curr_frag = Normal;
	      ifdepth = whiledepth = 0;
	      if (assemble_file (path_and_name, FALSE) == ERROR)
		any_error = TRUE;
	    }
	}
    }
}


static void
assemble_library_modules_for_unbound_globals ()
{
  char *libpath = getenv ("AS1750_LIB_PATH");

  filename_start = 0;
  path_and_name[0] = '\0';
  if (libpath != NULL)
    {
      filename_start = strlen (libpath);
      strcpy (path_and_name, libpath);
#ifdef __VMS
      if (libpath[filename_start - 1] != ':'
	  && libpath[filename_start - 1] != ']')
	path_and_name[filename_start++] = ':';
#else
      if (libpath[filename_start - 1] != '/')
	path_and_name[filename_start++] = '/';
#endif
    }
  apply (symroot, assemble_file_for_unresolved_reference);
  if (any_error)
    exit (EXIT_FAILURE);
}


enum output_file_format { TEK_HEX, TLD_LDM };
static enum output_file_format output_format;
/* Quickie: TLD output routines can write LLM format instead of LDM format */
bool llmflag = FALSE;

/* auxiliary functions for output file writing */

static status (*createfile[]) (char *) =
  { create_tekfile, create_tldfile };
static void (*closefile[]) (ulong) =
  { close_tekfile, close_tldfile };
static void (*emitword[]) (ulong, ushort) =
  { emit_tekword, emit_tldword };
static void (*finishline[]) () =
  { finish_tekline, finish_tldline };

static void
create_file (char *name)
{
  if ((*createfile[(int) output_format]) (name) != OKAY)
    {
      fprintf (stderr, "Can't create output file %s\n", name);
      exit (EXIT_FAILURE);
    }
}

static void
close_file ()
{
  (*closefile[(int) output_format]) (transfer_address);
}

static void
finish_line ()
{
  (*finishline[(int) output_format]) ();
}


static int
j_to_jc (char *filename, int linenum)
{
  FILE *fpin, *fpout;
  char line[200];
  int i, c, c1, need_comma;

  if ((fpin = fopen (filename, "r")) == (FILE *) 0)
    return ERROR;
  if ((fpout = fopen ("as1750.jjc", "w")) == (FILE *) 0)
    return ERROR;
  for (i = 1; i < linenum; i++)
    {
      if (fgets (line, sizeof (line), fpin) == NULL)
	{
	  fclose (fpin);
	  fclose (fpout);
	  return ERROR;
	}
      fputs (line, fpout);
    }
  while ((c = fgetc (fpin)) == ' ' || c == '\t')
    fputc (c, fpout);
  if (c == EOF || tolower (c) != 'j')
    {
      fclose (fpin);
      fclose (fpout);
      return ERROR;
    }
  fputc ('j', fpout);
  fputc ('c', fpout);
  fputc (' ', fpout);
  c = fgetc (fpin);
  c1 = fgetc (fpin);
  if (! isalpha (c))  /* "J" => "JC 15" */
    {
      fputc ('1', fpout);
      fputc ('5', fpout);
      fputc (',', fpout);
      need_comma = FALSE;
    }
  else
    need_comma = TRUE;
  fputc (c, fpout);
  fputc (c1, fpout);
  if (need_comma)
    fputc (',', fpout);
  while ((c = fgetc (fpin)) != EOF)
    fputc (c, fpout);
  fclose (fpin);
  fclose (fpout);
  if (rename ("as1750.jjc", filename))
    return ERROR;
  return OKAY;
}


bool list_before_reloc = FALSE, list_after_reloc = FALSE, list_symbols = FALSE;
section_t curr_sect;

static void
do_output (int obj_index, ulong address)
{
  int i, dspl;
  struct reloc *rel;
  struct linelist *lp;
  ushort word;
  static int n_words_seen = 0;
  static section_t last_section = Void;
  static char listline[132];

  /* Check if need for relocation */
  for (i = 0; i < relblk.n_used; i++)
    {				/* not all relocations done yet */
      rel = &relblk.data[i];
      if (!rel->done && rel->frag_index == curr_frag
	  && rel->obj_index == obj_index)
	{
	  if (rel->sym == SNULL)
	    problem ("do_output: reloc.sym == SNULL");
	  switch (rel->reltype)
	    {
	    case Byte_Reloc:
	      if (rel->sym->frag_index != curr_frag)
		{
		  fprintf (stderr,
		  "Byte-reloc. of %s only possible within symbol def. fragment",
			   rel->sym->name);
		  exit (EXIT_FAILURE);
		}
	      dspl = (int) (rel->sym->value - (long) address);
	      if (dspl < -128 || dspl > 127)
		{
		  if (j_to_jc (file[rel->sym->defining_file].name,
			       objblk[curr_frag].line[obj_index]->linenum))
		    {
		      fprintf (stderr,
			  "ByteRel displacemt out of range; addr = #%04X, dspl = %d\n",
			   address, dspl);
		      exit (EXIT_FAILURE);
		    }
		  else
		    {
		      fprintf (stderr,
			      "J to JC correction applied to file %s line %d;",
			       file[rel->sym->defining_file].name,
			       objblk[curr_frag].line[obj_index]->linenum);
		      fprintf (stderr, " please reinvoke as1750.\n");
		    }
		}
	      else
	        objblk[curr_frag].data[obj_index] |= (ushort) (dspl & 0xFF);
	      break;

	    case Word_Reloc:
	      objblk[curr_frag].data[obj_index]
				 += (ushort) (rel->sym->value & 0xffff);
	      break;

	    default:
	      problem ("link: illegal reloc. type in do_output()");
	    }
	  rel->done = TRUE;
	}
    }

  word = objblk[curr_frag].data[obj_index];
  curr_sect = objblk[curr_frag].section;

  /* Do listing file output if wanted */
  if (list_after_reloc)
    {
      if (curr_sect != last_section)
	{
	  last_section = curr_sect;
	  printf ("\n\t\tSECTION: %s\n", objblk[curr_sect].name);
	}
      if ((lp = objblk[curr_frag].line[obj_index]) == LNULL)
	printf ("%07X:  %04hX  Strange, (lp == LNULL)\n",
		address, word);
      else if (n_words_seen == 0)
	{
	  listline[0] = '\0';
	  if (lp->n_words == 0)
	    dsprintf (listline, "\t\t\t %5hd.  (lp->n_words == 0)",
		      lp->linenum);
	  else
	    {
	      dsprintf (listline, "%07lX:  %04hX           %5hd.  ",
			address, word, lp->linenum);
	      if (lp->label != NULL)
		dsprintf (listline, "%-15.15s ", lp->label);
	      else if (lp->operation != NULL)
		dsprintf (listline, "                ");
	      if (lp->operation != NULL)
		{
		  dsprintf (listline, "%-8.8s", lp->operation);
		  for (i = 0; i < lp->n_operands; i++)
		    {
		      if (i > 0)
			dsprintf (listline, ",");
		      dsprintf (listline, "%s", lp->operand[i]);
		    }
		}
	    }
	  if (lp->n_words < 2)
	    printf ("%s\n", listline);
	  else
	    n_words_seen = 1;
	}
      else
	{
	  ulong long_word = (ulong) word;
	  put_nibbles (listline + 10 + (n_words_seen * 5), long_word, 4);
	  if (++n_words_seen >= min (lp->n_words, 3))
	    {
	      printf ("%s\n", listline);
	      n_words_seen = 0;
	    }
	}
    }
  (*emitword[(int) output_format]) (address, word);
}


unsigned int common[4];

static void
relocate_symbol (symbol_t sym)
{
  if (sym->is_orged)
    sym->is_constant = TRUE;
  if (!sym->is_constant)
    {
      if (sym->defining_file < 0 && !sym->is_global)
	{
	  fprintf (stderr, "Error (relocate): undefined local symbol '%s'",
		   sym->name);
	  any_error = TRUE;
	}
      else
	{
	  struct objblock *obj = &objblk[sym->frag_index];
	  sym->value += obj->start_address + common[obj->section];
	  common[obj->section] += sym->reserve;
	}
    }
  if (sym->is_export)
    {
      if (export_file == (FILE *) 0)
	{
	  if (!exportfname[0])
	    {		/* export file name has not been explicitly set */
	      char *dot = strrchr (outfname, '.');
	      if (dot == NULL)
		problem ("EXPORT internal error: outfilename not set");
	      strcat (strncpy (exportfname, outfname, dot - outfname), ".xim");
	    }
	  if ((export_file = fopen (exportfname, "w")) == (FILE *) 0)
	    problem ("cannot open export file");
	}
      fprintf (export_file, "%-40s %07lX\n", sym->name, sym->value);
    }
}

/* auxiliary fuction for relocate_and_write_output() */
static int
cmp_startaddr (o1, o2)
     struct objblock *o1, *o2;
{
  if (o1->start_address < o2->start_address)
    return -1;
  else if (o1->start_address > o2->start_address)
    return 1;
  return 0;
}

static void
relocate_and_write_output (char *outfname)
{
  int i;

  common[Init] = common[Normal] = common[Konst] = common[Static] = 0;

  /* Configure relocation start addresses. */
  /* The Init/Normal mechanism is to be refined. For example,
     Normal should not have to be physically adjacent to Init;
     instead, Init in its entirety should be made a subroutine, and
     an "SJS R15,init_start" should be synthesized at the start of Normal.
   */
  objblk[Normal].start_address = objblk[Init].start_address
			       + objblk[Init].n_used;
  for (i = Konst; i <= Static; i++)
    if (objblk[i].start_address == -1L)
      objblk[i].start_address = objblk[i-1].start_address + objblk[i-1].n_used;

  /* Relocate symbols. */
  apply (symroot, relocate_symbol);

  if (export_file != (FILE *) 0)  /* Close the export file, if one was opened */
    fclose (export_file);

  create_file (outfname);

  /* Now that symbol addresses are absolutized, we can relocate
     code, and the address references within code. */

  for (curr_frag = 0; curr_frag < n_frags; curr_frag++)
    {
      struct objblock *obj = &objblk[curr_frag];
      if (obj->n_used > 0)
	{
	  ulong reloc_addr = obj->start_address;
	  for (i = 0; i < obj->n_used; i++)
	    do_output (i, reloc_addr++);
	  finish_line ();
	}
    }
  close_file ();
  if (any_error)
    exit (EXIT_FAILURE);
}


struct linelist *line;
static struct linelist *lineroot;

static status
get_line ()
{
#define MAX_LINELEN 132
  static char buf[MAX_LINELEN];
  char *p, *start, *end;
  int len;

  while (TRUE)
    {
      if (fgets (buf, MAX_LINELEN, file[curr_file].fp) == NULL)
	return END;
      file[curr_file].line_number++;
      if ((len = strlen (buf)) > 0)
	if (buf[len - 1] == '\n')
	  buf[--len] = '\0';
      if (len < 1)
	continue;
      if (buf[0] == ';' || buf[0] == '#')	/* comment */
	continue;
      if ((end = findc (buf, ';')) != NULL)
	*end-- = '\0';
      else
	end = buf + len - 1;
      while (end >= buf && isspace (*end))
	*end-- = '\0';
      if (end >= buf)
	break;
    }
  if (!case_sensitive)
    strlower (buf);
  if (line == LNULL)
    lineroot = line = (struct linelist *) calloc (1, sizeof (struct linelist));
  else
    {
      line->next = (struct linelist *) calloc (1, sizeof (struct linelist));
      line = line->next;
    }
  line->linenum = (ushort) file[curr_file].line_number;
  len = strlen (buf);
  start = buf;
  /* GAS directives may begin at start of line; if so, shift them right
     so they won't be interpreted as labels. */
  if (*start == '.')
    {
      int i = ++len;
      while (--i)
	buf[i] = buf[i-1];
      buf[0] = ' ';
    }
  /* Store the label field, if present. */
  if (!isspace (*start) || findc (start, ':') != NULL)
    {
      if ((p = findc (start, ':')) != NULL)
	{		/* Unix assembler compatibility (colon) */
	  start = skip_white (start);
	  if (p - buf >= len - 1)
	    len--;
	}
      else
        p = skip_nonwhite (start);
      *p = '\0';
      line->label = dupstr (start);
      if (p - buf >= len)
	return OKAY;
    }
  else
    p = start;
  /* Store the operation field. */
  p = skip_white (p + 1);
  if (p == NULL || *p == '\0')
    return error ("get_line internal error 1");
  end = skip_nonwhite (p);
  *end = '\0';
  line->operation = dupstr (p);
  if (end - buf >= len)
    return OKAY;
  /* Store the operand(s). */
  p = skip_white (end + 1);
  if (p == NULL || *p == '\0')
    return error ("get_line internal error 2");
  do
    {
      char *delim = findc (p, ','), *compact = p, *orig = p;
      bool in_string = FALSE;

      if (delim == NULL)
	delim = buf + len;
      *delim = '\0';
      end = delim;
      while (isspace (*(end - 1)))
	*--end = '\0';
      /* Throw out whitespace between the arguments. */
      while (*orig)
	{
	  if (!isspace (*orig) || in_string)
	    {
	      *compact++ = *orig;
	      if (*orig == '\"' || *orig == '\'')
		in_string = !in_string;
	    }
	  orig++;
	}
      *compact = '\0';
      line->operand[line->n_operands++] = dupstr (p);
      if (delim - buf >= len)
	break;
      p = skip_white (delim + 1);
    }
  while (p != NULL && *p != '\0');
  return OKAY;
}


/* Auxiliary to DATAL: same as get_num, only for datatype `long'.
   Returns pointer to first char after the number consumed,
   or NULL on error. */

static char *
get_long (char *s, long *outnum)
{
  bool is_neg = FALSE, intel = FALSE, c_lang = FALSE, tld = FALSE;
  char *start;

  *outnum = 0;
  if (*s == '-')
    {
      is_neg = TRUE;
      ++s;
    }
  else if (*s == '+')
    ++s;
  /* determine if Intel format */
  if (isdigit (*s))
    {
      char *p = s;
      while (isxdigit (*++p))
	;
      if (upcase (*p) == 'H')
	intel = TRUE;
    }
  if (intel
      || (c_lang = (*s == '0' && upcase (*(s + 1)) == 'X'))
      || (tld = strncmp (s, "16#", 3) == 0))
    {
      s += c_lang ? 2 : tld ? 3 : 0;
      start = s;
      while (isxdigit (*s))
	{
	  *outnum = (*outnum << 4) | xtoi (*s);
	  ++s;
	}
      if (s - start > 8)
	{
	  error ("get_num -- number at '%s' too large", start);
	  return NULL;
	}
      if (intel)
	s++;
      else if (tld)
	{
	  if (*s != '#')
	    {
	      error ("get_num -- expecting '#' at end of number");
	      return NULL;
	    }
	  s++;
	}
    }
  else if (*s == '0' || (tld = (*s == '8' && *(s + 1) == '#')))
    {
      s += tld ? 2 : 1;
      start = s;
      while (*s >= '0' && *s <= '7')
	{
	  *outnum = (*outnum << 3) | (*s - '0');
	  ++s;
	}
      if (s - start > 11)
	{
	  error ("get_long -- number at '%s' too large", start);
	  return NULL;
	}
      if (tld)
	{
	  if (*s != '#')
	    {
	      error ("get_long -- expecting '#' at end of number");
	      return NULL;
	    }
	  ++s;
	}
    }
  else if (*s == '@' || (tld = (*s == '2' && *(s + 1) == '#')))
    {
      s += (tld ? 2 : 1);
      start = s;
      while (*s == '0' || *s == '1')
	{
	  *outnum = (*outnum << 1) | (*s - '0');
	  ++s;
	}
      if (s - start > 32)
	{
	  error ("get_num -- number at '%s' too large", start);
	  return NULL;
	}
      if (tld)
	{
	  if (*s != '#')
	    {
	      error ("get_num -- expecting '#' at end of number");
	      return NULL;
	    }
	  ++s;
	}
    }
  else if (isdigit (*s))
    {
      start = s;
      while (isdigit (*s))
	{
	  *outnum = (*outnum * 10) + dtoi (*s);
	  ++s;
	}
      if (s - start > 10)
	{
	  error ("get_num -- number at '%s' too large", start);
	  return NULL;
	}
    }
  else
    return NULL;
  if (is_neg)
    *outnum = -*outnum;
  return s;
}


/* Auxiliary to DATAE/DATAF. Returns OKAY on success and ERROR on error. */
static status
get_float (char *s, double *outnum)
{
  /* Doing it the easy way (sscanf)... to be improved! */
  if (!sscanf (s, "%lf", outnum))
    return ERROR;
  return (OKAY);
}



static status
dealloc (int argc, char *argv[])
{
  int i;
  for (i = 0; i < argc; i++)
    if (argv[i] != NULL)
      free (argv[i]);
  return OKAY;
}


static bool seen_symdef, assembled_endwhile;


static status	/* Returns values OKAY, ERROR, or END (on keyword "END".) */
assemble_line (int curr_macro, char **macparm, int n_macparms)
{		/* curr_macro == -1 : not in macro; >= 0 : index into macro[] */
  int i;
  unsigned n_words;
  char label[80], operation[80], *operand[16], *parstart;
  bool is_macsymdef = FALSE, is_csect = FALSE;
  state_t state;
  static symbol_t sym;
#define done   return dealloc (line->n_operands, operand)

  state = check_ifstate ();
  label[0] = '\0';
  if (line->label != NULL)	/* symbol definition */
    {
      if (state != Assembling)
	return OKAY;
      strcpy (label, line->label);	/* Do label-field text expansion. */
      if (expand (label, curr_macro, macparm, n_macparms) == ERROR)
	return ERROR;
      if (line->operation != NULL)
	{
	  if (!strcasecmp (line->operation, "SET"))
	    is_macsymdef = TRUE;	/* Macro symbol def. */
	  else if (!strcasecmp (line->operation, "CSECT"))
	    is_csect = TRUE;	/* TLD "CSECT" directive */
	}
      if (!is_macsymdef && !is_csect)
	{
	  if ((sym = find_symbol (label)) != SNULL)
	    {
	      /* The following check assumes that global symbols have been
	         declared GLOBAL *before* their definition. */
	      if (sym->defining_file >= 0
		  && (sym->is_global || sym->defining_file == curr_file))
		return error ("duplicate definition of symbol '%s'", label);
	    }
	  else if ((sym = enter_symbol (label)) == SNULL)
	    return ERROR;
	  sym->defining_file = curr_file;
	  sym->frag_index = curr_frag;
	  sym->value = (long) objblk[curr_frag].n_used;
	  if (objblk[curr_frag].start_address >= 0L)
	    {			/* ORG or explicit start address */
	      sym->is_orged = TRUE;
	      sym->value += objblk[curr_frag].start_address;
	    }
	  /* The sym->value assignment may be overridden by EQU. */
	  seen_symdef = TRUE;
	}
    }
  if (line->operation == NULL)
    return OKAY;
  /* Do operation-field text expansion. */
  strcpy (operation, line->operation);
  if (expand (operation, curr_macro, macparm, n_macparms) == ERROR)
    return ERROR;
  if (!issymstart (*operation))
    {
      if (*operation == '\0' || eq (operation, "..."))	/* TLD ("...") */
	return OKAY;
      else if (*operation != '.')  /* '.' : Unix assembler directives */
	return error ("ignoring line because of strange '%s'\n", operation);
    }
  strupper (operation);		/* Make the operation field upper-case. */
  /* Do operand-field text expansion. */
  for (i = 0; i < line->n_operands; i++)
    {
      strcpy ((operand[i] = (char *) malloc (80)), line->operand[i]);
      if (expand (operand[i], curr_macro, macparm, n_macparms) == ERROR)
	return ERROR;
    }
  parstart = operand[0];
  if (state != Assembling)
    goto non_opcode;
  if (is_macsymdef)
    {
      int macsym_index = find_macsym (label);
      if (macsym_index < 0)
	{
	  if (n_macsyms >= MAX_MACSYMS)
	    return error ("macro symbol table overflow");
	  macsym_index = n_macsyms++;
	  macsym[macsym_index].name = dupstr (label);
	}
      if (parstart == NULL)
	return error ("Incomplete macro symbol definition");
      macsym[macsym_index].value = evaluate (parstart);
      done;
    }
  if (strlen (operation) > 4)
    goto non_opcode;
  if ((n_words = as1750 (operation, line->n_operands, operand)) != NO_OPCODE)
    {
      if (n_words == ERROR)
	return ERROR;
      line->n_words = (ushort) n_words;
      if (line->label == NULL && seen_symdef)
	line->label = sym->name;
      if (objblk[curr_frag].section != Init &&
	  objblk[curr_frag].section != Normal)
	error ("info: encountered instruction while in data section");
      if (list_before_reloc)
	{
	  int addr = objblk[curr_frag].n_used - n_words;
	  ushort *opcode = objblk[curr_frag].data + addr;
	  struct reloc *rel;

	  if (objblk[curr_frag].start_address >= 0L)	/* ORGed */
	    printf ("%05lX:", objblk[curr_frag].start_address
		    + (long) objblk[curr_frag].n_used - (long) n_words);
	  else
	    printf ("r%04X:", addr);
	  printf ("\t%04hX", *opcode);
	  if (n_words == 2)
	    printf (" %04hX\t", *(opcode + 1));
	  else
	    printf ("\t\t");
	  if (relblk.n_used > 0 &&
	      (rel = &relblk.data[relblk.n_used - 1])->obj_index
	      == objblk[curr_frag].n_used - 1)
	    printf ("%-8.8s", rel->reltype == Byte_Reloc ? "ByteRel" : "Rel");
	  else if (n_words == 2)
	    printf ("Abs\t");
	  else
	    printf ("\t");
	  if (line->label != NULL)
	    printf ("%-15.15s ", line->label);
	  else
	    printf ("\t\t");
	  printf ("%-5.5s", operation);
	  for (i = 0; i < line->n_operands; i++)
	    {
	      if (i > 0)
		printf (",");
	      printf ("%s", operand[i]);
	    }
	  printf ("\n");
	  fflush (stdout);
	}
      seen_symdef = FALSE;
      done;
    }

non_opcode:
  if (eq (operation, "IF"))
    {
      ifdepth++;
      if (state == Assembling)
	{
	  char *val;
	  if ((val = evaluate (parstart)) == NULL)
	    return error ("\t (above error occurred in IF condition)");
	  if (*val != '0')
	    ifstate[ifdepth] = Assembling;
	  else
	    ifstate[ifdepth] = Skipping_hot;
	  free (val);
	}
      else
	ifstate[ifdepth] = Skipping_cold;
      /* seen_symdef = FALSE; */
      done;
    }

  if (eq (operation, "ELSEIF") || eq (operation, "ELSIF"))
    {
      if (state == Skipping_hot)
	{
	  char *val;
	  if ((val = evaluate (parstart)) == NULL)
	    return error ("\t (above error occurred in ELSEIF condition)");
	  if (*val != '0')
	    ifstate[ifdepth] = Assembling;
/*    free (val);    */
	}
      else if (state == Assembling)
	ifstate[ifdepth] = Skipping_cold;
      /* seen_symdef = FALSE; */
      done;
    }

  if (eq (operation, "ELSE"))
    {
      if (state == Assembling)
	ifstate[ifdepth] = Skipping_hot;
      else if (state == Skipping_hot)
	ifstate[ifdepth] = Assembling;
      /* seen_symdef = FALSE; */
      done;
    }

  if (eq (operation, "ENDIF"))
    {
      if (ifdepth == 0)
	return error ("ENDIF without previous IF");
      ifdepth--;
      /* seen_symdef = FALSE; */
      done;
    }

  if (eq (operation, "WHILE"))
    {
      ifdepth++;
      if (state == Assembling)
	{
	  char *val;
	  if (expand (parstart, curr_macro, macparm, n_macparms) == ERROR)
	    return ERROR;
	  if ((val = evaluate (parstart)) == NULL)
	    return error ("\t (above error occurred in WHILE condition)");
	  if (*val != '0')
	    {
	      ifstate[ifdepth] = Assembling;
	      whilestart[++whiledepth] = line;
	    }
	  else
	    ifstate[ifdepth] = Skipping_hot;
	}
      else
	ifstate[ifdepth] = Skipping_cold;
      /* seen_symdef = FALSE; */
      done;
    }

  if (eq (operation, "ENDWHILE"))
    {
      if (state == Assembling)
	{
	  struct linelist *endline = line;
	  char *val;

	  if (whiledepth == 0)
	    return error ("ENDWHILE without preceding WHILE");
	  while (TRUE)
	    {
	      if (line == endline)
		{
		  parstart = dupstr (whilestart[whiledepth]->operand[0]);
		  if (expand (parstart, curr_macro, macparm, n_macparms))
		    return ERROR;
		  if ((val = evaluate (parstart)) == NULL)
		    return error (" (above error occurred in WHILE condition)");
		  free (parstart);
		  if (*val == '0')
		    break;
		  line = whilestart[whiledepth]->next;
		}
	      if (assemble_line (curr_macro, macparm, n_macparms))
		return ERROR;
	      line = line->next;
	    }
	  line = endline;
	  whiledepth--;
	}
      if (ifdepth == 0)
	problem ("undetected ENDWHILE without previous WHILE");
      ifdepth--;
      /* seen_symdef = FALSE; */
      done;
    }

  if (state != Assembling)
    {
      done;
    }

  if (eq (operation, "GLOBAL") || eq (operation, ".GLOBL"))
    {
      if ((sym = find_or_enter (parstart)) == SNULL)
	return ERROR;
      sym->is_global = TRUE;
      done;
    }

  if (eq (operation, "IMPORT"))
    {
      if (find_symbol (parstart) == SNULL)
	return error ("symbol %s not found in import file", parstart);
      done;
    }

  if (eq (operation, "EXPORT"))
    {
      if ((sym = find_or_enter (parstart)) == SNULL)
	return ERROR;
      sym->is_global = TRUE;
      sym->is_export = TRUE;
      done;
    }

  if (eq (operation, "COMMON") || eq (operation, ".COMM"))
    {
      int val;
      if ((sym = find_or_enter (parstart)) == SNULL)
	return ERROR;
      sym->is_global = TRUE;
      if (sym->defining_file < 0)
	{
	  sym->defining_file = curr_file;
	  if (objblk[Static].start_address >= 0L)	/* ORGed */
	    {
	      sym->is_orged = TRUE;
	      sym->value = objblk[Static].start_address
			   + (long) objblk[Static].n_used;
	    }
	  else
	    sym->value = (long) objblk[Static].n_used;
	  sym->frag_index = Static;
	  sym->reserve = 1;
	}
      if (line->n_operands > 1)
	{
	  parstart = line->operand[1];
	  if (get_sym_num (parstart, &val) == NULL)
	    return error ("error at number of words in COMMON directive");
	  if (sym->reserve < val)
	    sym->reserve = (ushort) val;
	}
      line->n_words = sym->reserve;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "EQU"))
    {
      int factor, factor1;
      /* A maximum of two factors can be computed by EQU. */
      if (! seen_symdef)
	return error ("missing lhs of EQU");
      if (*parstart == '\0')
	return error ("missing rhs of EQU");
      if (*parstart != '*')	/* Cheating for now -- */
	{			/* handling of '*' TO BE DONE! */
	  if ((parstart = get_sym_num (parstart, &factor)) == NULL)
	    return error ("syntax error at rhs of EQU");
	  if (*parstart != '\0')
	    {
	      if ((parstart = get_sym_num (parstart, &factor1)) == NULL)
		return error ("error in rhs expression of EQU");
	      factor += factor1;
	      if (*parstart != '\0')
		return error ("rhs expression of EQU too complex");
	    }
	  sym->is_constant = TRUE;
	  sym->value = (long) factor;	/* previous value is overridden */
	}
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "DATA") || eq (operation, ".WORD"))
    {
      if (! seen_symdef)
	error ("DATA directive without previous symbol definition");

      line->n_words = 1;
      for (i = 0; i < line->n_operands; i++)
	{
	  parstart = dupstr (line->operand[i]);
	  if (expand (parstart, curr_macro, macparm, n_macparms))
	    return ERROR;
	  if (parse_addr (parstart) == ERROR)
	    return error ("numeric syntax error in DATA at: %s", parstart);
	}
      done;
    }

  if (eq (operation, "DATAL") || eq (operation, ".LONG"))
    {
      long lng;

      if (! seen_symdef)
	error ("DATAL directive without previous symbol definition");

      line->n_words = 2;
      for (i = 0; i < line->n_operands; i++)
	{
	  if ((parstart = get_long (operand[i], &lng)) == NULL)
	    return error ("numeric error in DATAL at: %s", operand[i]);
	  add_word ((ushort) ((lng >> 16) & 0xFFFF));
	  add_word ((ushort) (lng & 0xFFFF));
	  if (*parstart)
	    error ("warning: ignoring excess characters at '%s'", parstart);
	}
      done;
    }

  if (eq (operation, "DATAF") || eq (operation, ".FLOAT"))
    {
      int gas = eq (operation, ".FLOAT") ? 2 : 0;
      double flt;
      short flt1750[3];

      if (! seen_symdef)
	error ("DATAF directive without previous symbol definition");

      line->n_words = 2;
      for (i = 0; i < line->n_operands; i++)
	{
	  if (gas && (*operand[i] != '0' || ! isalpha (*(operand[i] + 1))))
	    return error ("syntax error in .float at: %s", operand[i]);
	  if (get_float (operand[i] + gas, &flt) == ERROR)
	    return error ("numeric error in DATAF at: %s", operand[i]);
	  if (to_1750flt (flt, flt1750) != OKAY)
	    return error ("float number in DATAF out of range at: %s", parstart);
	  add_word (flt1750[0]);
	  add_word (flt1750[1]);
	}
      done;
    }

  if (!strncmp (operation, "DATAE", 5) || eq (operation, ".DOUBLE"))
    {
      int gas = eq (operation, ".DOUBLE") ? 2 : 0;
      double flt;
      short flt1750[3];

      if (! seen_symdef)
	error ("DATAEF directive without previous symbol definition");

      line->n_words = 3;
      for (i = 0; i < line->n_operands; i++)
	{
	  if (gas && (*operand[i] != '0' || ! isalpha (*(operand[i] + 1))))
	    return error ("syntax error in .double at: %s", operand[i]);
	  if (get_float (operand[i], &flt) == ERROR)
	    return error ("numeric error in DATAEF at: %s", operand[i]);
	  if (to_1750eflt (flt, flt1750) != OKAY)
	    return error ("number in DATAE out of range at: %s", operand[i]);
	  add_word (flt1750[0]);
	  add_word (flt1750[1]);
	  add_word (flt1750[2]);
	}
      done;
    }

  if (eq (operation, "DATAC"))
    {
      int num;
      bool is_hibyte = TRUE;
      ushort word;

      if (! seen_symdef)
	error ("DATAC directive without previous symbol definition");

      n_words = 0;
      for (i = 0; i < line->n_operands; i++)
	{
	  parstart = operand[i];
	  if (*parstart == '"')
	    {
	      while (*++parstart != '"')
		{
		  if (*parstart == '\0')
		    return error ("Unterminated string in DATAC");
		  if (is_hibyte)
		    word = ((ushort) * parstart & 0xFF) << 8;
		  else
		    {
		      word |= (ushort) * parstart & 0xFF;
		      add_word (word);
		      n_words++;
		    }
		  is_hibyte = !is_hibyte;
		}
	      *parstart++ = '\0';	/* skip the ending '"' */
	    }
	  else
	    {
	      if ((parstart = get_sym_num (parstart, &num)) == NULL)
		return error ("numeric syntax error in DATAC");
	      if (num < 0 || num > 0xFF)
		return error ("character value out of range in DATAC");
	      if (*parstart)
		error ("warning: ignoring excess characters at '%s'", parstart);
	      if (is_hibyte)
		word = (ushort) num << 8;
	      else
		{
		  word |= (ushort) num;
		  add_word (word);
		  n_words++;
		}
	      is_hibyte = !is_hibyte;
	    }
	}
      if (!is_hibyte)
	{
	  add_word (word);
	  n_words++;
	}
/*    else if (objblk[curr_frag].data[objblk[curr_frag].n_used-1] & 0xFF)
        add_word (0);           Add string terminator if user didn't. */
      line->n_words = n_words;
      done;
    }

  if (eq (operation, "BLOCK") || eq (operation, "RES"))
    {
      int blklen;

      if (! seen_symdef)
	return error ("BLOCK without previous symbol definition");
      if (get_sym_num (parstart, &blklen) == NULL || blklen <= 0)
	return error ("error in BLOCK directive at: %s", parstart);

      line->n_words = (ushort) blklen;

      while (blklen-- > 0)
	add_word (0);
      done;
    }

  if (eq (operation, "INIT"))
    {
      curr_frag = Init;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "NORMAL") || eq (operation, ".TEXT"))
    {
      curr_frag = Normal;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "KONST") || eq (operation, ".RODATA"))
    {
      curr_frag = Konst;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "STATIC") || eq (operation, ".BSS"))
    {
      curr_frag = Static;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "CSECT"))
    {
      if (strstr (parstart, "instruction"))
	curr_frag = Normal;
      else if (strstr (parstart, "operand"))
	{
	  if (strstr (parstart, "read_only"))
	    curr_frag = Konst;
	  else if (strstr (parstart, "read_write"))
	    curr_frag = Static;
	  else
	    return error ("unknown attribute in CSECT directive");
	}
      else
	return error ("unknown attribute in CSECT directive");
      done;
    }

  if (eq (operation, "ORG"))
    {
      struct objblock *obj;
      section_t section = objblk[curr_frag].section;

      curr_frag = n_frags++;
      obj = &objblk[curr_frag];
      if ((parstart = get_long (parstart, &obj->start_address)) == NULL)
	return error ("syntax error in ORG address");
      if (*parstart)
	error ("warning: ignoring excess characters at '%s'", parstart);
      obj->section = section;
      if (seen_symdef)
	{
	  obj->name = sym->name;
	  seen_symdef = FALSE;
	}
      done;
    }

  if (eq (operation, "UNORG"))
    {
      curr_frag = objblk[curr_frag].section;
      seen_symdef = FALSE;
      done;
    }

  if (eq (operation, "RESUME"))
    {
      int frag;
      for (frag = 0; frag < n_frags; frag++)
	{
	  char *name = objblk[frag].name;
	  if (name != NULL && !strcasecmp (parstart, name))
	    {
	      curr_frag = frag;
	      seen_symdef = FALSE;
	      done;
	    }
	}
      return error ("name of ORG given in RESUME is unknown");
    }

  if (eq (operation, "INCLUDE"))
    {
      if (*parstart == '\"')
	{
	  parstart++;
	  *(parstart + strlen (parstart) - 1) = '\0';
	}
      if (assemble_file (parstart, TRUE) == ERROR)
	exit (EXIT_FAILURE);  /* Can't continue after error(s) in includefile */
      done;
    }

  if (eq (operation, "MACRO"))
    {
      bool first_line = TRUE;
      if (parstart == NULL || *parstart == '\0')
	{
	  error ("unrecoverable: MACRO name missing");
	  /* not using problem() here because error() gives line info */
	  exit (EXIT_FAILURE);
	}
      if (curr_macro >= 0)
	{
	  error ("unrecoverable: MACRO declarations cannot be nested");
	  /* not using problem() here because error() gives line info */
	  exit (EXIT_FAILURE);
	}
      macro[n_macros].name = strupper (dupstr (parstart));
      while (TRUE)
	{
	  if (get_line ())
	    {
	      error ("unrecoverable: missing ENDMACRO statement for MACRO %s",
		     macro[n_macros].name);
	      exit (EXIT_FAILURE);
	    }
	  if (line->operation != NULL
	      && strcasecmp (line->operation, "ENDMACRO") == 0)
	    {
	      macro[n_macros].endline = line;
	      break;
	    }
	  if (first_line)
	    {
	      macro[n_macros].startline = line;
	      first_line = FALSE;
	    }
	}
      n_macros++;
      done;
    }

  if (eq (operation, "ENDMACRO"))
    {
      done;
    }

  if (eq (operation, "ASSIGN"))	/* TLD special: page reg assignment */
    {
      int as, logaddr_hinibble, i_o, hi, lo, len;
      if (output_format != TLD_LDM)
	done;
      /* parse the Address State */
      if ((as = xtoi (*parstart++)) == -1)
	return error ("expected hex digit for Address State in ASSIGN");
      if (*parstart++ != '.')
	return error ("expected '.' after Address State in ASSIGN");
      /* parse the logical address most-significant nibble */
      if ((logaddr_hinibble = xtoi (*parstart++)) == -1)
	return error
	  ("expected hex digit for MS-nibble of logical address in ASSIGN");
      i_o = (*parstart == 'i') ? 0 : (*parstart = 'o') ? 1 : -1;
      if (i_o < 0)
	return error
	  ("expected Instruction (i) or Operand (o) indication in ASSIGN");
      assign[i_o][n_assigns[i_o]].as = as;
      assign[i_o][n_assigns[i_o]].logaddr_hinibble = logaddr_hinibble;
      /* Parse the pagereg contents field. */
      if ((hi = xtoi (operand[1][0])) == -1)
	return error ("expected hex number for pagereg contents in ASSIGN");
      assign[i_o][n_assigns[i_o]].contents = (ushort)
	((lo = xtoi (operand[1][1])) != -1) ? ((hi << 4) | lo) : hi;
      /* Parse the Length field. */
      if ((len = xtoi (operand[2][0])) == -1)
	return error ("expected hex digit for Length field in ASSIGN");
      assign[i_o][n_assigns[i_o]++].length = len;
      done;
    }

  if (eq (operation, "NAME") || eq (operation, "MODULE")
      || eq (operation, "TITLE") || eq (operation, "PAGE")
      || eq (operation, "LIST") || eq (operation, "NOLIST")
      || eq (operation, ".FILE") || eq (operation, ".OPTIM"))
    {
      done;
    }

  if (eq (operation, "END"))
    return END;

  /* Now, it can only be a macro call. */
  {
    int new_macro, new_n_macparms = 0;
    char *new_macparm[MAX_MACPARMS];
    struct linelist *line_save = line;

    seen_symdef = FALSE;
    for (new_macro = 0; new_macro < n_macros; new_macro++)
      if (eq (operation, macro[new_macro].name))
	break;
    if (new_macro == n_macros)
      return error ("unidentified operation at '%s'", operation);
    new_macparm[0] = dupstr (operation);	/* `0` : macro name */
    for (i = 0; i < line->n_operands; i++)
      new_macparm[++new_n_macparms] = dupstr (operand[i]);
    line = macro[new_macro].startline;
    while (line != macro[new_macro].endline)
      {
	if (line == LNULL)
	  break;
	if (assemble_line (new_macro, new_macparm, new_n_macparms))
	  break;
	line = line->next;
      }
/*  while (new_n_macparms >= 0)
   free (new_macparm[new_n_macparms--]); */
    line = line_save;
  }

  done;
}


static status
assemble_file (char *name, bool is_includefile)  /* Returns ERROR or OKAY. */
{
  int i, currfile_save;

  for (i = 0; i < file_highmark; i++)
    if (eq (name, file[i].name))	/* already done */
      return OKAY;
  currfile_save = curr_file;
  if ((curr_file = file_highmark++) >= MAX_FILES)
    problem ("too many files");
  file[curr_file].name = dupstr (name);
  file[curr_file].is_includefile = is_includefile;
  if ((file[curr_file].fp = fopen (name, "r")) == (FILE *) 0)
    {
      if (is_includefile)
	{
	  char fullname[100];

	  if (include_option_path[0])
	    {
	      strcat (strcpy (fullname, include_option_path), name);
	      file[curr_file].fp = fopen (fullname, "r");
	    }
	  if (file[curr_file].fp == (FILE *) 0)
	    {
	      if (as1750_inc_path == NULL)
		return error ("Couldn't open input file %s", name);
	      if (*(as1750_inc_path + strlen (as1750_inc_path) - 1) != '/')
		sprintf (fullname, "%s/%s", as1750_inc_path, name);
	      else
		strcat (strcpy (fullname, as1750_inc_path), name);
	      if ((file[curr_file].fp = fopen (fullname, "r")) == (FILE *) 0)
		return error ("Couldn't open input file %s", name);
	    }
	}
      else
	return error ("Couldn't open input file %s", name);
    }
  else if (verbose)
    printf ("assembling file %s\n", name);
  file[curr_file].line_number = 0;
  seen_symdef = assembled_endwhile = FALSE;
  while (!feof (file[curr_file].fp))
    {
      if (get_line ())
	break;
      if (assemble_line (-1, NULL, 0))
	break;			/* -1 : initially, not in macro */
    }
  fclose (file[curr_file].fp);
  curr_file = currfile_save;
  return OKAY;
}

bool debug_alloc = FALSE;

/* Apart from ORG directives, linking is in this fixed order
   (unless explicit start addresses are given) :
   Init    at `code_startaddr' (default 0x00100)
   Normal  after end of Init
   Konst   after end of Normal
   Static  after end of Konst
 */
static long code_startaddr = 0x0100L;
   /* Default when no explicit value supplied */
static long konst_startaddr = -1L;
   /* -1 indicates to put Konst after end of */
   /* Normal when no explicit value supplied */
static long static_startaddr = -1L;
   /* -1 indicates to put Static after end of */
   /* Konst when no explicit value supplied */

int
main (int argc, char *argv[])
{
  int i;
  char *p;

  transfer_address = code_startaddr;

  as1750_inc_path = getenv ("AS1750_INC_PATH");

  outfname[0] = '\0';

  for (i = 1; i < argc; i++)	/* Filter out any command-line flags. */
    {
      if (*argv[i] == '-')
	{
	  char option = *(argv[i] + 1);
	  char *nextarg = argv[i] + 2;
	  bool advanced_i = FALSE, argless_opt = FALSE;

	  *argv[i] = '\0';
	  if (i < argc - 1 && *nextarg == '\0')
	    {
	      nextarg = argv[++i];
	      /* Most options do take an argument. Therefore it is the
		 default to expect an argument, and only if the option
		 in the following is found not to take an argument then
		 we undo our expectation. */
	      advanced_i = TRUE;
	    }
	  switch (option)
	    {
	    case 'A':
	      debug_alloc = TRUE;
	      if (advanced_i)
		i--;		/* undo the anticipation of an argument */
	      argless_opt = TRUE;
	      break;
	    case 'C':
	      if (*nextarg == '-'
		  || eq (nextarg + strlen (nextarg) - 2, ".s"))
		call_env_mainname = "_main";  /* default */
	      else
		call_env_mainname = nextarg;
	      break;
	    case 'D':		/* define a macro symbol */
	      if ((p = findc (nextarg, '=')) != NULL)
		*p = '\0';
	      macsym[n_macsyms].name = dupstr (nextarg);
	      if (p != NULL)
		macsym[n_macsyms].value = dupstr (p + 1);
	      n_macsyms++;
	      break;
	    case 'I':
	      strcpy (include_option_path, nextarg);
	      if (*(include_option_path + strlen (include_option_path) - 1)
		  != '/')
		strcat (include_option_path, "/");
	      break;
	    case 'S':		/* case sensitive symbol names */
	      case_sensitive = TRUE;
	      if (advanced_i)
		i--;		/* undo the anticipation of an argument */
	      argless_opt = TRUE;
	      break;
	    case 'a':
	      if (advanced_i)
		{
		  list_before_reloc = TRUE;
		  list_after_reloc = TRUE;
		  list_symbols = TRUE;
		  i--;		/* undo the anticipation of an argument */
		  argless_opt = TRUE;
		}
	      else switch (*nextarg)
		{
		case 'l':
		  list_before_reloc = TRUE;
		  break;
		case 'r':
		  list_after_reloc = TRUE;
		  break;
		case 's':
		  list_symbols = TRUE;
		  break;
		default:
		  problem ("unknown listing option given");
		}
	      break;
	    case 'c':		/* code start address */
	      sscanf (nextarg, "%lx", &code_startaddr);
	      transfer_address = code_startaddr;
	      break;
	    case 'e':		/* explicit export file name */
	      strcpy (exportfname, nextarg);
	      break;
	    case 'i':		/* import file */
	      {
		FILE *import_file;
		static char buf[100], symname[80];
		long symvalue;
		symbol_t sym;

		strcpy (buf, nextarg);
		if ((import_file = fopen (buf, "r")) == (FILE *) 0)
		  problem ("import file not found");
		while (!feof (import_file))
		  {
		    if (fgets (buf, 100, import_file) == NULL)
		      break;
		    sscanf (buf, "%s %lx", symname, &symvalue);
		    if ((sym = find_or_enter (symname)) == SNULL)
		      problem ("fatal error while entering import symbol");
		    sym->value = symvalue;
		    sym->is_global = TRUE;
		    sym->is_constant = TRUE;
		    sym->defining_file = 9999;	/* (any value) */
		  }
		fclose (import_file);
	      }
	      break;
	    case 'k':		/* constant data start address */
	      sscanf (nextarg, "%lx", &konst_startaddr);
	      break;
	    case 'o':		/* explicit output file name */
	      strcpy (outfname, nextarg);
	      {
		char outname[50];
		int len;
		strcpy (outname, nopath (outfname));
		len = strlen (outname);
		if (eq (outname + len - 4, ".hex"))
		  output_format = TEK_HEX;
		else if (eq (outname + len - 4, ".ldm"))
		  output_format = TLD_LDM;
		else if (eq (outname + len - 4, ".llm"))
		  {
		    output_format = TLD_LDM;
		    llmflag = TRUE;
		  }
		else
		  {
		    fprintf (stderr, "Output file extension must be one of:\n");
		    fprintf (stderr, "  .hex   => Tektronix Extended Hex\n");
		    fprintf (stderr, "  .ldm   => TLD Load Module\n");
		    fprintf (stderr, "  .llm   => TLD Logical Load Module\n");
		    exit (EXIT_FAILURE);
		  }
	      }
	      break;
	    case 's':		/* read/write data start address */
	      sscanf (nextarg, "%lx", &static_startaddr);
	      break;
	    case 't':
	      sscanf (nextarg, "%x", &transfer_address);
	      break;
	    case 'u':
	      unix_compatible = TRUE;
	      /* When not `unix_compatible', references to external global
		 symbols must be explicitly declared by a GLOBAL or IMPORT
		 statement. The `-u' flag removes this restriction, i.e.
		 symbols that could not be resolved local to each file are
		 assumed to be references to external globals. */
	      if (advanced_i)
		i--;		/* undo the anticipation of an argument */
	      argless_opt = TRUE;
	      break;
	    case 'v':
	      verbose = TRUE;
	      printf ("as1750 version %s\n", AS1750_VERSION);
	      if (advanced_i)
		i--;		/* undo the anticipation of an argument */
	      argless_opt = TRUE;
	      break;
	    default:
	      fprintf (stderr,
		       "Unknown option given (%s) -- exiting\n", argv[i]);
	      return (EXIT_FAILURE);
	    }
	  if (advanced_i && !argless_opt)
	    *nextarg = '\0';
	}
    }

  /* Initialize 1750A instruction assembly */
  init_as1750 ();

  /* Initialize the object code area (see `struct objblock' in common.h) */
  objblk[0].name = "Init";
  objblk[0].section = Init;
  objblk[0].start_address = code_startaddr;
  objblk[1].name = "Normal";
  objblk[1].section = Normal;
  objblk[1].start_address = -1L;	/* ...see relocate_and_wr... */
  objblk[2].name = "Konst";
  objblk[2].section = Konst;
  objblk[2].start_address = konst_startaddr;
  objblk[3].name = "Static";
  objblk[3].section = Static;
  objblk[3].start_address = static_startaddr;

  curr_frag = Normal;
  if (call_env_mainname != NULL)
    {
      add_word (0x7EF0);	/* SJS R15,                    */
      add_reloc (find_or_enter (call_env_mainname));
      add_word (0);		/*         <call_env_mainname> */
      add_word (0xFFFF);	/* BPT                         */
      relblk.data[0].sym->is_referenced = TRUE;
    }

  for (i = 1; i < argc; i++)
    {
      if (!*(argv[i]))
	continue;
      if (!outfname[0])	/* Option "-o" was not given, so */
	{	/* make an output file name from the first input file name. */
	  char *dot;
	  strcpy (outfname, nopath (argv[i]));
	  if ((dot = strrchr (outfname, '.')) == NULL)
	    {
	      dot = outfname + strlen (outfname);
	      strcat (outfname, ".");
	    }
	  strcpy (dot + 1, "ldm");
	  output_format = TLD_LDM;
	}

      curr_frag = Normal;
      ifdepth = whiledepth = 0;
      ifstate[0] = Assembling;
      whilestart[0] = LNULL;
/* TBD:    n_macparms = n_macsyms = 0;     TBD: free() */
      if (assemble_file (argv[i], FALSE))
	return (EXIT_FAILURE);
    }

  if (!outfname[0])
    return (EXIT_SUCCESS);

  assemble_library_modules_for_unbound_globals ();
  relocate_and_write_output (outfname);
  if (list_symbols)
    dump_symbols ();

  /* Let's see if there were overlaps: */
  /* first, eliminate empty fragments */
  for (i = 0; i < n_frags; i++)
    {
      if (objblk[i].n_used == 0)
	{
	  if (i < n_frags - 1)
	    {
	      int j;
	      for (j = i; j < n_frags - 1; j++)
		objblk[j] = objblk[j + 1];
	      i--;	/* to counteract the i++ in the "for i" loop */
	    }
	  n_frags--;
	}
    }
  /* next, sort objblk by ascending start address */
  qsort (objblk, n_frags, sizeof (struct objblock), cmp_startaddr);
  /* now we can do the overlap checking */
  for (i = 0; i < n_frags - 1; i++)
    {
      if (objblk[i + 1].n_used == 0 || objblk[i].n_used == 0)
	continue;
      if (objblk[i + 1].start_address
	  < objblk[i].start_address + objblk[i].n_used)
	{
	  fprintf (stderr,
	      "link: %s at start address 0x%lX (length %d) overlaps with\n",
		   (objblk[i + 1].name != NULL) ? objblk[i + 1].name :
		   objblk[objblk[i + 1].section].name,
		   objblk[i + 1].start_address, objblk[i + 1].n_used);
	  fprintf (stderr,
		   "      %s at start address 0x%lX (length %d)\n",
		   (objblk[i].name != NULL) ? objblk[i].name :
		   objblk[objblk[i].section].name,
		   objblk[i].start_address, objblk[i].n_used);
	  any_error = TRUE;
	}
    }
  return (any_error ? EXIT_FAILURE : EXIT_SUCCESS);
}

