/* peekpoke.h  -- exports of peekpoke.c */

#include "type.h"

extern bool peek (ulong phys_address, ushort *value);
extern void poke (ulong phys_address, ushort value);

