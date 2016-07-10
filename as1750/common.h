/***************************************************************************/
/*                                                                         */
/* Project   :        as1750 -- Mil-Std-1750 Assembler and Linker          */
/*                                                                         */
/* Component :      common.h -- common declarations & specifications       */
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

#include "types.h"
#include "syms.h"

/* Sourcetext related defs: */

struct linelist {
	 ushort linenum;
	 ushort n_words;
	 int    n_operands;
	 char   *label, *operation, *operand[16];
	 struct linelist *next;
   };

#define LNULL  ((struct linelist *) 0)

/* Code related defs : */

struct objblock {
	 int       n_allocated;
	 int       n_used;
	 char      *name;
	 section_t section;
	 long      start_address;
/* when start_address is -1L, use default linking mechanism (see main.c):
   Init   at 0x00100
   Normal at Init + sizeof(Init)
   Konst  at Normal + sizeof(Normal)
   Static at Konst + sizeof(Konst)
 */
	 ushort    *data;
	 struct linelist **line;
/* data[] and *line[] are indexed in parallel, i.e. for each word of data,
   `line' refers to the corresponding sourceline info. */
  };

/* Relocation & ORG stuff: */

typedef enum { Word_Reloc, Byte_Reloc } reloc_type;

struct reloc {
	 bool       done;
	 reloc_type reltype;
	 int        frag_index;
	 int        obj_index;
	 symbol_t   sym;
  };

struct relblock {
	 int      n_allocated;
	 int      n_used;
	 struct reloc *data;
  };

/* main.c: */

/* macro stuff: */
#define MAX_MACROS   64
#define MAX_MACPARMS 16

/* input file information */
#define MAX_FILES 20
struct fileinfo {
	char *name;
	FILE *fp;
	bool is_includefile;
	int  line_number;
  };


extern struct fileinfo file[MAX_FILES];
extern int    curr_file;

#define MAX_FRAGS 100
extern struct objblock objblk[MAX_FRAGS];
/* Relocatable code is collected in the first four elements
   of objblk as follows:
   objblk[0] => relocatable code of section Init
   objblk[1] => relocatable code of section Normal
   objblk[2] => relocatable code of section Konst
   objblk[3] => relocatable code of section Static
   All ORGed code fragments are at objblk indexes greater than 3.
 */
extern int curr_frag;

extern struct linelist *line;

extern struct relblock relblk;

extern void add_word (ushort);		/* as.c */
extern void add_reloc (symbol_t sym);	/* as.c */

