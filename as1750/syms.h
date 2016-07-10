/* syms.h  --  symbol related declarations and exports of syms.c */

#include "type.h"

typedef struct symbol {
	 bool      is_global;
	 int       frag_index;
	 int       defining_file;  /* -1 when not defined */
	 bool      is_orged;
	 bool      is_constant;
	 bool      is_referenced;
	 bool      is_export;
	 byte      namelen;
	 char      *name;
	 long      value;  /* if is_constant, then value; else address */
	 short     reserve;  /* reserved size in COMMONs */
	 struct symbol *left, *right;  /* alphabetical order of symbol names */
   } *symbol_t;

#define SNULL ((symbol_t) 0)

/* #define SYM_STARTLETTERS 28      'a'..'z', '$', '_' */

extern symbol_t symroot;

extern symbol_t find_symbol (char *name);
extern symbol_t enter_symbol (char *name);
extern symbol_t find_or_enter (char *name);

extern void apply (symbol_t sym, void (*func)());
extern void print_symbol (symbol_t sym);
extern void dump_symbols ();

