/* types.h  --  often used system includefiles, basic declarations & types */

#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "type.h"

#define OKAY      0
#define END       1
#define ERROR     0xFFFD
#define NO_OPCODE 0xFFFE

#define status  unsigned

#define dtoi(c)         ((c) - '0')
#define issymstart(c)   (isalpha(c) || (c) == '$' || (c) == '_')
#define issymchar(c)    (issymstart(c) || isdigit(c) || (c) == '.')

/* The only declaration that is really project specific :
   I would've put it in a separate file, but then that file
   would have had just one line! */
typedef enum { Init, Normal, Konst, Static , Void } section_t;

extern void *myalloc (int siz);

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1
#endif

#endif
