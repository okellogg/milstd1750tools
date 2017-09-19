/* type.h  --  type shorthands */

#ifndef _TYPE_H
#define _TYPE_H

#include <sys/types.h>

/* If you get "undefined type" errors about uint / ushort / uchar
   then change the #if 0 to #if 1 below:                       */
#if 0
#ifndef uint
#define uint    unsigned int
#endif
#ifndef ushort
#define ushort  unsigned short
#endif
#ifndef uchar
#define uchar   unsigned char
#endif
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

