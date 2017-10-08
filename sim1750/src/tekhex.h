/* tekhex.h  --  exports of tekhex.c */

#include "type.h"

extern int      check_tekline (char *line);
extern void     emit_tekword (uint startaddr, unsigned short word);
extern void     finish_tekline ();
extern uint     create_tekfile (char *outfname);
extern void     close_tekfile (uint transfer_address);

