/* utils.h  --  exports of utils.c */

#include <stdlib.h>
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

extern void  problem (char *msg);
extern char  *nopath (char *full_filename);
extern char  upcase (char c);
extern char  itox (int i);
extern int   xtoi (char c);
#ifndef STRDUP
extern char  *strdup (char *str);
#endif
extern char  *strndup (char *str, int len);
#ifndef STRNCASECMP
extern int   strncasecmp (char *s1, char *s2, int len);
#endif
extern char  *strupper (char *s);
extern char  *strlower (char *s);
#define eq(s1,s2)       !strcmp(s1,s2)
#define strmatch(s1,s2) !strncmp(s1,s2,strlen(s2))
extern char  *findc (char *s, char c);
extern char  *skip_white (char *s);
extern char  *skip_nonwhite (char *s);
extern char  *skip_symbol (char *s);
extern long  get_16bit_hexnum (char *s);
extern long  get_nibbles (char *src, int n_nibbles);
extern void  put_nibbles (char *dst, unsigned long src, int n_nibbles);

