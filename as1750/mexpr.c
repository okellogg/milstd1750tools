/* expr -- evaluate expressions.
   Copyright (C) 1986, 1991 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Author: Mike Parker.
   Changes for as1750 macro expressions: Oliver Kellogg.

   This program evaluates expressions.  Each token (operator, operand,
   parenthesis) of the expression must be a seperate argument.  The
   parser used is a reasonably general one, though any incarnation of
   it is language-specific.  It is especially nice for expressions.

   No parse tree is needed; a new node is evaluated immediately.
   One function can handle multiple operators all of equal precedence,
   provided they all associate ((x op x) op x).
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "types.h"
#include "utils.h"
#include "macsym.h"

extern char *get_num ();
extern status error ();

#define NEW(type) ((type *) myalloc (sizeof (type)))
#define OLD(x) free ((char *) x)

/* The kinds of value we can have.  */
enum valtype
  {
    integer,
    string
  };
typedef enum valtype TYPE;

/* A value is.... */
struct valinfo
  {
    TYPE type;			/* Which kind. */
    union
      {				/* The value itself. */
	int i;
	char *s;
      }
    u;
  };
typedef struct valinfo VALUE;

/* Input expression working pointer */
static char *argp;

static VALUE *eval ();
static VALUE *int_value ();
static VALUE *str_value ();
static int isstring ();
static int nextarg ();
static int nomoreargs ();
static int null ();
static int toarith ();
static void freev ();
static void tostring ();


char *
evaluate (char *expr)
{
  VALUE *v;
  static char outbuf[80];

  argp = expr;

  v = eval ();
  if (!nomoreargs ())
    {
      error ("evaluate: args leftover error");
      return NULL;
    }
  switch (v->type)
    {
    case integer:
      sprintf (outbuf, "%d", v->u.i);
      return dupstr (outbuf);
    case string:
      return v->u.s;
    default:
      error ("unknown VALUE type in evaluate()");
      return NULL;
    }
}


/* Return a VALUE for I.  */

static VALUE *
int_value (i)
     int i;
{
  VALUE *v;

  v = NEW (VALUE);
  v->type = integer;
  v->u.i = i;
  return v;
}

/* Return a VALUE for S.  */

static VALUE *
str_value (len)
     int len;
{
  VALUE *v;

  v = NEW (VALUE);
  v->type = string;
  v->u.s = dupstrn (argp, len);
  argp += len;
  return v;
}

/* Free VALUE V, including structure components.  */

static void
freev (v)
     VALUE *v;
{
/*
   if (v->type == string)
   free (v->u.s);
   OLD (v);
 */
}

/* Return nonzero if V is a null-string or zero-number.  */

static int
null (v)
     VALUE *v;
{
  switch (v->type)
    {
    case integer:
      return v->u.i == 0;
    case string:
      return v->u.s[0] == '\0' || strcmp (v->u.s, "0") == 0;
    default:
      problem ("unknown VALUE type given to evaluate()");
    }
  return 0;
}

/* Return nonzero if V is a string value.  */

static int
isstring (v)
     VALUE *v;
{
  return v->type == string;
}

/* Coerce V to a string value (can't fail).  */

static void
tostring (v)
     VALUE *v;
{
  char *temp;

  switch (v->type)
    {
    case integer:
      temp = (char *) myalloc (12);
      sprintf (temp, "%d", v->u.i);
      v->u.s = temp;
      v->type = string;
      break;
    case string:
      break;
    default:
      problem ("unknown VALUE type given to evaluate()");
    }
}

/* Coerce V to an integer value.  Return 1 on success, 0 on failure.  */

static int
toarith (v)
     VALUE *v;
{
  int i;

  switch (v->type)
    {
    case integer:
      return 1;
    case string:
      if (get_num (v->u.s, &i) == NULL)
	return 0;
/*    free (v->u.s); */
      v->u.i = i;
      v->type = integer;
      return 1;
    default:
      problem ("unknown VALUE type given to evaluate()");
    }
  return 0;
}

/* Return nonzero if the next token matches STR exactly.
   STR must not be NULL.  */

static int
nextarg (str)
     char *str;
{
  int len = strlen (str), match;

  if (argp == NULL)
    return 0;
  argp = skip_white (argp);
  match = strncmp (argp, str, len) == 0;
  if (match)
    argp += len;
  return match;
}

/* Return nonzero if there are no more tokens.  */

static int
nomoreargs ()
{
  if (argp == NULL)
    return TRUE;
  return *argp == '\0' || *argp == ',';
}

/* The comparison operator handling functions.  */

#define cmpf(name, rel)                         \
static                                          \
int name (l, r) VALUE *l; VALUE *r;             \
{                                               \
  if (!toarith (l) || !toarith (r))             \
    {                                           \
       tostring (l);                            \
       tostring (r);                            \
       return strcmp (l->u.s, r->u.s) rel 0;    \
    }                                           \
 else                                           \
   return l->u.i rel r->u.i;                    \
}
cmpf (less_than, <)
cmpf (less_equal, <=)
cmpf (equal, ==)
cmpf (not_equal, !=)
cmpf (greater_equal, >=)
cmpf (greater_than, >)

#undef cmpf

     /* The arithmetic operator handling functions.  */

#define arithf(name, op)                        \
static                                          \
int name (l, r) VALUE *l; VALUE *r;             \
{                                               \
  if (!toarith (l) || !toarith (r))             \
    error ("evaluate: non-numeric argument");   \
  return l->u.i op r->u.i;                      \
}

#define arithdivf(name, op)                     \
int name (l, r) VALUE *l; VALUE *r;             \
{                                               \
  if (!toarith (l) || !toarith (r))             \
    error ("evaluate: non-numeric argument");   \
  if (r->u.i == 0)                              \
    error ("division by zero");                 \
  return l->u.i op r->u.i;                      \
}

#define unaryf(name, op)                        \
static                                          \
int name (l) VALUE *l;                          \
{                                               \
  if (!toarith (l))                             \
    error ("evaluate: non-numeric argument");   \
  return op l->u.i;                             \
}

arithf (plus, +)
arithf (minus, -)
arithf (multiply, *)
arithdivf (divide, /)
arithdivf (mod, %)
arithf (shl, <<)
arithf (shr, >>)
arithf (bor, |)
arithf (band, &)
arithf (bxor, ^)
unaryf (uminus, -)
unaryf (bnot, ~)
unaryf (lnot, !)

#undef arithf
#undef arithdivf
#undef unaryf


/* Handle bare operands and ( expr ) syntax.  */

     static VALUE *
       eval9 ()
{
  VALUE *v = (VALUE *) 0;
  char buf[16];
  int i;

  if (nomoreargs ())
    {
      error ("eval9: nomoreargs error");
      return (VALUE *) 0;
    }

  if (nextarg ("("))
    {
      v = eval ();
      if (!nextarg (")"))
	error ("eval9: missing closing parenthesis");
      return v;
    }

  if (nextarg (")"))
    error ("eval9: too many closing parentheses");

  if (issymstart (*argp))
    {
      char *p = argp;
      while (issymchar (*p))
	p++;
      v = NEW (VALUE);
      v->type = string;
      v->u.s = dupstrn (argp, p - argp);
      argp = p;
    }
  else if ((argp = get_num (argp, &i)) != NULL)
    {
      v = NEW (VALUE);
      sprintf (buf, "%d", i);
      v->type = string;
      v->u.s = dupstr (buf);
    }
  else
    error ("eval9: unidentified input '%s'", argp);
  return v;
}

/* Handle unary -, !, ~ operators.  */

static VALUE *
eval8 ()
{
  VALUE *l = (VALUE *) 0;
  int (*fxn) () = 0;
  int val;

  if (nomoreargs ())
    return l;
  else if (nextarg ("DEF") || nextarg ("def"))
    if (nextarg ("("))
      {
	int i = n_macsyms, len, defined = 0;
	char *closing_parenth = strchr (argp, ')');

	if (closing_parenth == NULL)
	  {
	    error ("eval8: missing closing parenth at DEF");
	    return l;
	  }
	len = closing_parenth - argp;
	while (i--)
	  if (len == strlen (macsym[i].name))
	    if (strncmp (argp, macsym[i].name, len) == 0)
	      {
		defined = 1;
		break;
	      }
	l = int_value (defined);
	argp = closing_parenth + 1;
	return l;
      }
    else
      argp -= 3;		/* reset argp to situation before `nextarg("DEF")' */
  else if (nextarg ("ORD") || nextarg ("ord"))
    if (nextarg ("("))
      {
	if (!nextarg ("\""))
	  {
	    error ("eval8: expecting quotation mark at ORD");
	    return l;
	  }
	l = int_value (((int) (*argp & 0xFF) << 8) | ((int) *(argp + 1) & 0xFF));
	if (*(argp += 2) != '"')
	  {
	    error ("eval8: expecting ending quotation mark at ORD");
	    return l;
	  }
	argp++;
	if (!nextarg (')'))
	  {
	    error ("eval8: expecting closing parenth at ORD");
	    return l;
	  }
	return l;
      }
    else
      argp -= 3;		/* reset argp to situation before `nextarg("ORD")' */
  else if (nextarg ("-"))
    fxn = uminus;
  else if (nextarg ("!"))
    fxn = lnot;
  else if (nextarg ("~"))
    fxn = bnot;
  l = eval9 ();
  if (fxn)
    {
      val = (*fxn) (l);
      freev (l);
      l = int_value (val);
    }
  return l;
}

/* Handle *, /, % operators.  */

static VALUE *
eval7 ()
{
  VALUE *l;
  VALUE *r;
  int (*fxn) ();
  int val;

  l = eval8 ();
  while (!nomoreargs ())
    {
      if (nextarg ("*"))
	fxn = multiply;
      else if (nextarg ("/"))
	fxn = divide;
      else if (nextarg ("%"))
	fxn = mod;
      else
	return l;
      r = eval8 ();
      val = (*fxn) (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}

/* Handle +, - operators.  */

static VALUE *
eval6 ()
{
  VALUE *l;
  VALUE *r;
  int (*fxn) ();
  int val;

  l = eval7 ();
  while (!nomoreargs ())
    {
      if (nextarg ("+"))
	fxn = plus;
      else if (nextarg ("-"))
	fxn = minus;
      else
	return l;
      r = eval7 ();
      val = (*fxn) (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}

/* Handle <<, >> operators. */

static VALUE *
eval5 ()
{
  VALUE *l;
  VALUE *r;
  int (*fxn) ();
  int val;

  l = eval6 ();
  while (!nomoreargs ())
    {
      if (nextarg ("<<"))
	fxn = shl;
      else if (nextarg (">>"))
	fxn = shr;
      else
	return l;
      r = eval6 ();
      val = (*fxn) (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}

/* Handle &. */

static VALUE *
eval4 ()
{
  VALUE *l;
  VALUE *r;
  int val;

  l = eval5 ();
  while (!nomoreargs ())
    {
      if (!nextarg ("&"))
	return l;
      if (nextarg ("&"))
	{
	  argp -= 2;		/* undo nextarg() action on argp */
	  return l;
	}
      r = eval5 ();
      val = band (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}


/* Handle | and ^. */

static VALUE *
eval3 ()
{
  VALUE *l;
  VALUE *r;
  int (*fxn) ();
  int val;

  l = eval4 ();
  while (!nomoreargs ())
    {
      if (nextarg ("|"))
	{
	  if (nextarg ("|"))
	    {
	      argp -= 2;	/* undo nextarg() action on argp */
	      return l;
	    }
	  fxn = bor;
	}
      else if (nextarg ("^"))
	fxn = bxor;
      else
	return l;
      r = eval4 ();
      val = (*fxn) (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}

/* Handle comparisons.  */

static VALUE *
eval2 ()
{
  VALUE *l;
  VALUE *r;
  int (*fxn) ();
  int val;

  l = eval3 ();
  while (!nomoreargs ())
    {
      if (nextarg ("<="))
	fxn = less_equal;
      else if (nextarg (">="))
	fxn = greater_equal;
      else if (nextarg ("==") || nextarg ("="))
	fxn = equal;
      else if (nextarg ("!="))
	fxn = not_equal;
      else if (nextarg ("<"))
	fxn = less_than;
      else if (nextarg (">"))
	fxn = greater_than;
      else
	return l;
      r = eval3 ();
      val = (*fxn) (l, r);
      freev (l);
      freev (r);
      l = int_value (val);
    }
  return l;
}

/* Handle &&.  */

static VALUE *
eval1 ()
{
  VALUE *l;
  VALUE *r;

  l = eval2 ();
  while (!nomoreargs ())
    {
      if (nextarg ("&&"))
	{
	  r = eval2 ();
	  if (null (l) || null (r))
	    {
	      freev (l);
	      freev (r);
	      l = int_value (0);
	    }
	  else
	    freev (r);
	}
      else
	return l;
    }
  return l;
}

/* Handle ||.  */

static VALUE *
eval ()
{
  VALUE *l;
  VALUE *r;

  l = eval1 ();
  while (!nomoreargs ())
    {
      if (nextarg ("||"))
	{
	  r = eval1 ();
	  if (null (l))
	    {
	      freev (l);
	      l = r;
	    }
	  else
	    freev (r);
	}
      else
	return l;
    }
  return l;
}


/* For testing.

   main (int argc, char *argv[])
   {
   int i;
   static char buf[80];

   for (i = 1; i < argc; i++)
   strcat(buf,argv[i]);

   evaluate (buf);
   }
 */
