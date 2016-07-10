/* utils.h  --  export prototypes of utils.c */

#define eq(s1,s2)  !strcmp(s1,s2)
#define eq2(s1,s2) !strncmp(s1,s2,strlen(s2))

extern void  problem (char *msg);
extern char  *nopath (char *full_filename);
extern char  upcase (char c);
extern char  itox (int i);
extern int   xtoi (char c);
extern char  *dupstr (char *str);
extern char  *dupstrn (char *str, int len);
#ifndef STRCASECMP
extern int   strcasecmp (char *s1, char *s2);
#endif
#ifndef STRNCASECMP
extern int   strncasecmp (char *s1, char *s2, int len);
#endif
extern char  *strupper (char *s);
extern char  *strlower (char *s);
extern char  *findc (char *s, char c);
extern char  *skip_white (char *s);
extern char  *skip_nonwhite (char *s);
extern char  *skip_symbol (char *s);
extern int   get_16bit_hexnum (char *s);
extern int   get_nibbles (char *src, int n_nibbles);
extern void  put_nibbles (char *dst, unsigned int src, int n_nibbles);

