/* as1750.h  --  exports of as1750.c */
 
#include "types.h"

extern void init_as1750 ();
extern unsigned short as1750 (char *operation,
			      int n_operands, char *operand[]);

extern void add_word (ushort word);
extern void add_reloc (symbol_t sym);
extern char *get_num (char *s, int *outnum);
extern char *get_sym_num (char *s, int *outnum);
extern status parse_addr (char *s);
extern status error (char *layout, ...);
