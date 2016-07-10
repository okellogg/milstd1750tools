/***************************************************************************/
/*                                                                         */
/* Project   :        as1750 -- Mil-Std-1750 Assembler and Linker          */
/*                                                                         */
/* Component :       macsym.h -- macro symbol related declarations         */
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


#define MAX_MACSYMS  1024
struct macro_symbol {
	char     *name;
	char     *value;
   };

extern struct macro_symbol macsym[MAX_MACSYMS];  /* see main.c */
extern int    n_macsyms;			 /* see main.c */
extern char   *evaluate (char *);		 /* see mexpr.c */

