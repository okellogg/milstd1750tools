/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :           status.c -- status message handling               */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                         (C) 2017 Oliver M. Kellogg                      */
/* Contact   :           okellogg@users.sourceforge.net                    */
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
#include <stdarg.h>

#include "status.h"

FILE *logfile;  /* opening and closing of the logfile done elsewhere */

void 
lprintf (char *layout, ...)
{
  va_list vargu;
  char output_line[132];

  va_start (vargu, layout);

  vsprintf (output_line, layout, vargu);
  printf ("%s", output_line);

  if (logfile != (FILE *) 0)
    fprintf (logfile, "%s", output_line);

  va_end (vargu);

  return;
}


char global_message[1024];

/* The below functions fill the global_message buffer, and return
   a status value that is apppropriate for the message class
   (informational, warning, or error).
*/

int 
info (char *layout, ...)
{
  va_list vargu;

  if (! verbose)
    return INFO;

  va_start (vargu, layout);
  vsprintf (global_message, layout, vargu);
  va_end (vargu);

  lprintf ("%s\n", global_message);
  return INFO;
}


int 
warning (char *layout, ...)
{
  va_list vargu;

  va_start (vargu, layout);
  vsprintf (global_message, layout, vargu);
  va_end (vargu);

  lprintf ("%s\n", global_message);
  return WARNING;
}


int 
error (char *layout, ...)
{
  va_list vargu;

  va_start (vargu, layout);
  vsprintf (global_message, layout, vargu);
  va_end (vargu);

  lprintf ("%s\n", global_message);
  return ERROR;
}

