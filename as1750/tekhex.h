/* tekhex.h  --  exports of tekhex.c */

#include "type.h"

extern int      check_tekline (char *line);
extern void     emit_tekword (ulong startaddr, unsigned short word);
extern void     finish_tekline ();
extern unsigned create_tekfile (char *outfname);
extern void     close_tekfile (ulong transfer_address);

