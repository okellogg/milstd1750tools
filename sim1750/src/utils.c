/***************************************************************************/
/*                                                                         */
/* Project   :               Reuseable Software Components                 */
/*                                                                         */
/* Component :           utils.c -- general purpose utilities              */
/*                                                                         */
/* Copyright :          (C) Daimler-Benz Aerospace AG, 1994-1997           */
/*                                                                         */
/* Author    :     Oliver M. Kellogg, Dept. RST13, Dasa/Space Systems,     */
/*                     P.O.Box 801169, D-81663 Munich, Germany.            */
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
#include "type.h"
#include "utils.h"

void
problem (char *msg)
{
  fprintf (stderr, "FATAL ERROR: %s\n", msg);
  exit (EXIT_FAILURE);
}


char *
nopath (char *full_filename)
{
  char *p;

  if ((p = strrchr (full_filename, '/')) != NULL)	/* UNIX */
    return p + 1;
  if ((p = strrchr (full_filename, ']')) != NULL)	/* VMS */
    return p + 1;
  if ((p = strrchr (full_filename, ':')) != NULL)	/* VMS */
    return p + 1;
  if ((p = strrchr (full_filename, '\\')) != NULL)	/* DOS or OS/2 */
    return p + 1;
  return full_filename;
}


char
upcase (char c)
{
  return (c >= 'a' && c <= 'z') ? c - 32 : c;
}


char
itox (int i)			/* This could be a macro, but we want to avoid side effects */
{				/* when the argument has ++ or --. */
  i &= 0xF;
  return (char) (i > 9 ? i + 55 : i + '0');
}


int
xtoi (char c)			/* This could be a macro, but we want to avoid side effects */
{				/* when the argument has ++ or --. */
  if (isspace (c))
    return 0;
  if (c >= '0' && c <= '9')
    return (int) c - 48;
  if (c >= 'A' && c <= 'F')
    return (int) c - 55;
  if (c >= 'a' && c <= 'f')
    return (int) c - 87;
  return -1;
}


#ifndef STRDUP
char *
strdup (char *str)
{
  char *p;
  if (str == NULL)
    return NULL;
  if ((p = (char *) malloc (strlen (str) + 1)) == NULL)
    problem ("dynamic memory exhausted");
  return strcpy (p, str);
}
#endif


char *
strndup (char *str, int len)
{
  char *p;
  if (str == NULL)
    return NULL;
  if ((p = (char *) malloc (len + 1)) == NULL)
    problem ("dynamic memory exhausted");
  *(p + len) = '\0';
  return strncpy (p, str, len);
}


#ifndef STRNCASECMP
int
strncasecmp (char *s1, char *s2, int len)
{
  char c1, c2;

  while (len-- > 0)
    {
      c1 = upcase (*s1++);
      c2 = upcase (*s2++);
      if (c1 < c2)
	return -1;
      if (c1 > c2)
	return 1;
    }
  return 0;
}
#endif


char *
strupper (char *s)
{
  char *start = s;
  int inside_string = 0;

  if (s == NULL)
    return NULL;
  while (*s)
    {
      if (*s >= 'a' && *s <= 'z' && !inside_string)
	*s -= 32;
      else if (*s == '"' || *s == '\'')
	{
	  if (s > start && *(s - 1) == '\\')
	    ;
	  else
	    inside_string = !inside_string;
	}
      s++;
    }
  return start;
}


char *
strlower (char *s)
{
  char *start = s;
  int inside_string = 0;

  if (s == NULL)
    return NULL;
  while (*s)
    {
      if (*s >= 'A' && *s <= 'Z' && !inside_string)
	*s += 32;
      else if (*s == '"' || *s == '\'')
	{
	  if (s > start && *(s - 1) == '\\')
	    ;
	  else
	    inside_string = !inside_string;
	}
      s++;
    }
  return start;
}


char *
findc (char *s, char c)		/* A strchr that excludes embedded character */
{				/*  constants and strings from matching. */
  char *start = s;
  int inside_string = 0;

  if (s == NULL)
    return NULL;
  while (*s)
    {
      if (*s == c && (c == '"' || c == '\'' ? TRUE : !inside_string))
	return s;
      else if (*s == '"' || *s == '\'')
	if (s > start && *(s - 1) == '\\')
	  ;
	else if (inside_string)
	  inside_string = 0;
	else
	  inside_string = 1;
      s++;
    }
  return NULL;
}


char *
skip_white (char *s)
{
  if (s == NULL || *s == '\0')
    return NULL;
  while (*s && (*s == ' ' || *s == '\t'))
    s++;
  return s;
}


char *
skip_nonwhite (char *s)
{
  if (s == NULL || *s == '\0')
    return NULL;
  while (*s && (*s != ' ' && *s != '\t'))
    s++;
  return s;
}


char *
skip_symbol (char *s)
{
  if (s == NULL || *s == '\0')
    return NULL;
  if (!isalpha (*s) && *s != '$' && *s != '_')
    return NULL;
  while (isalnum (*s) || *s == '$' || *s == '_' || *s == '.')
    s++;
  return (s);
}


long
get_16bit_hexnum (char *s)	/* Convert four-digit hex string to number */
{				/* Returns -1L on error, else a number in the range 0 .. 0xFFFF */
  int i;
  ulong hexdigit;
  long out = 0;

  for (i = 0; i < 4; i++)
    {
      if ((hexdigit = (ulong) xtoi (*(s + i))) == -1)
	return -1L;
      out |= (long) hexdigit << (12 - (4 * i));
    }
  return out;
}


long
get_nibbles (char *src, int n_nibbles)
  /* Convert n-digit (1 <= n <= 8) hex string to number. */
  /* Returns -1L on error, else a number in the range 0 .. 0x7FFFFFFF. */
  /* Of course, this means that for (n = 8), the most significant bit
     (sign bit) can not be used, so the maximum readable bitwidth is 31. */
{
  int i;
  long outnum = 0;

  while (n_nibbles-- > 0)
    {
      if ((i = xtoi (*src++)) == -1)
	return -1L;
      outnum = (outnum << 4) | (long) i;
    }
  return outnum;
}


void
put_nibbles (char *dst, ulong src, int n_nibbles)
{
  int nibble;

  while (n_nibbles--)
    {
      nibble = (src >> (n_nibbles << 2)) & 0xF;
      *dst++ = itox (nibble);
    }
}
