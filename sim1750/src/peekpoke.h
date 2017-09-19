/* peekpoke.h  -- exports of peekpoke.c */

#include "type.h"

extern bool peek (uint phys_address, ushort *value);
extern void poke (uint phys_address, ushort value);

