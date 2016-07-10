/* break.h  --  exports of break.c */

#ifndef _BREAK_H
#define _BREAK_H

#include "type.h"

typedef enum { READ_WRITE, READ, WRITE } breaktype;

extern int  find_breakpt (breaktype type, ulong phys_address);
extern void set_inactive (int bp_index);
extern void set_active   (int bp_index);

extern int  si_brkset   (int argc, char *argv[]);
extern int  si_brklist  (int argc, char *argv[]);
extern int  si_brkclear (int argc, char *argv[]);
extern int  si_brksave  (int argc, char *argv[]);

#endif
