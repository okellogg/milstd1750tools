/* type.h  --  type shorthands */

#ifndef _TYPE_H
#define _TYPE_H

#include <sys/types.h>

/* If you get "undefined type" errors about uint / ushort
   then remove -DHAVE_UTYPES from CFLAGS in the Makefile.      */
#ifndef HAVE_UTYPES
# ifndef uint
#  define uint    unsigned int
# endif
# ifndef ushort
#  define ushort  unsigned short
# endif
#endif

#ifndef uchar
# define uchar   unsigned char
#endif
#ifndef byte
#define byte    unsigned char
#endif
#ifndef bool
#define bool	char
#endif
#ifndef FALSE
#define FALSE	0
#define TRUE	(!FALSE)
#endif

#define elsecase  break;case

#endif

