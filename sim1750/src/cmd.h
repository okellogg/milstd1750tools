/* exports */

#include "type.h"

extern bool  parse_address (char *str, uint *phys_address);
extern int   init_system (int mode), init_simulator (int mode);
extern int   interpreter (char *startup_batchfile);
extern void  dis_reg ();
extern void  int_handler_install ();
extern int   sys_int (int);
extern int   si_go (int argc, char *argv[]);

