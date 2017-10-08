/***************************************************************************/
/*                                                                         */
/* Project   :        as1750 -- Mil-Std-1750 Assembler and Linker          */
/*                                                                         */
/* Component :             syms.c -- symbol related functions              */
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

#include "types.h"
#include "common.h"
#include "utils.h"

symbol_t symroot;

static char namebuf[80];


static symbol_t
findsym_1 (symbol_t sym)
{
  int cmpflag;

  if (sym == SNULL)
    return SNULL;
  cmpflag = strcmp (namebuf, sym->name);
  if (cmpflag < 0)
    return findsym_1 (sym->left);
  else if (cmpflag > 0)
    return findsym_1 (sym->right);
  else if (!sym->is_global && sym->defining_file >= 0
	   && sym->defining_file != curr_file
	   && !file[sym->defining_file].is_includefile)
	   /* This last check is kinda coarse. */
    return findsym_1 (sym->left);	/*  left: smaller _or_equal_  */
  else
    return (sym);
}


symbol_t
find_symbol (char *s)	/* returns pointer to symbol if found, NULL otherwise */
{
  strcpy (namebuf, s);
  return findsym_1 (symroot);
}


static symbol_t
find_parent (symbol_t sym)
{
  if (sym == SNULL)		/* serious problem! */
    return SNULL;
  if (strcmp (namebuf, sym->name) <= 0)
    {
      if (sym->left == SNULL)
	return sym;
      else
	return find_parent (sym->left);
    }
  else
    {
      if (sym->right == SNULL)
	return sym;
      else
	return find_parent (sym->right);
    }
}


symbol_t		/* Returns pointer to newly created symbol entry. */
enter_symbol (char *s)
{
  symbol_t sym;

  /* Manage storage for the symbol entry */
  if ((sym = (symbol_t) myalloc (sizeof (struct symbol))) == SNULL)
    problem ("request for symbol space rejected by OS");

  /* Manage the binary tree of symbols */
  if (symroot == SNULL)
    symroot = sym;
  else
    {
      symbol_t parent;
      strcpy (namebuf, s);
      if ((parent = find_parent (symroot)) == SNULL)
	problem ("internal error -- find_parent returns NULL");
      if (strcmp (s, parent->name) <= 0)
	parent->left = sym;
      else
	parent->right = sym;
    }

  /* Initialize the symbol entry */
  sym->left = sym->right = SNULL;
  sym->namelen = strlen (s);
  sym->name = dupstr (s);
  sym->is_global = FALSE;
  sym->frag_index = curr_frag;
  sym->defining_file = -1;	/* -1 : not defined */
  sym->is_orged = FALSE;
  sym->is_constant = FALSE;
  sym->is_referenced = FALSE;
  sym->is_export = FALSE;
  sym->value = 0;
  sym->reserve = 0;

  return sym;
}


/* Called on EXPORT/IMPORT declarations, and all label references. */
symbol_t		/* Returns pointer to symbol on success, else NULL. */
find_or_enter (char *s)
{
  symbol_t sym;

  if ((sym = find_symbol (s)) == SNULL)
    sym = enter_symbol (s);
  return sym;
}


void
apply (symbol_t sym, void (*func) ())
{
  if (sym == SNULL)
    return;
  (*func) (sym);
  apply (sym->left, func);
  apply (sym->right, func);
}


static bool any_error;

void
print_symbol (symbol_t sym)
  /* Structure:
     s        => line type = Symbol table entry
     %c|%x    => symbol type: L = local, G = global, U = undef.,
		 or hex number = common:reserved_size 
     %c       => section letter (i | n | k | s)
     %x       => symbol value
     %s       => symbol name
   */
{
  char *section_name = "inks ";
/*                   { "Init", "Normal", "Konst", "Static", "Void" }; */

  printf ("s ");
  if (sym->reserve > 0)
    printf ("%04hX ", sym->reserve);
  else if (sym->defining_file >= 0)
    {
      if (sym->is_global)
	putchar ('G');
      else
	putchar ('L');
    }
  else if (sym->is_referenced)
    {
      if (sym->is_global)
	putchar ('U');
      else
	{
	  fprintf (stderr, "Error: undefined local symbol '%s' referenced\n",
		   sym->name);
	  any_error = TRUE;
	  return;
	}
    }
  printf ("%c %06X %s\n", section_name[objblk[sym->frag_index].section],
	  sym->value, sym->name);
}


void
dump_symbols ()
{
  any_error = FALSE;
  apply (symroot, print_symbol);
  if (any_error)
    exit (EXIT_FAILURE);
}

