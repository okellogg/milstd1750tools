/* tldldm.h  --  exports of tldldm.c */

#include "type.h"

extern void     emit_tldword (uint startaddr, ushort word);
extern void     finish_tldline ();
extern unsigned create_tldfile (char *outfname);
extern void     close_tldfile (uint transfer_address);


struct pagereg_assignment   /* stuff for the ASSIGN statement */
  {
    uchar  as, logaddr_hinibble;
    uchar  length;
    ushort contents;
  };
#define MAX_ASSIGNS 32
extern struct pagereg_assignment assign[2][MAX_ASSIGNS];
extern int n_assigns[2];

