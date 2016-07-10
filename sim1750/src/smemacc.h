/* smemacc.h  --  export prototypes of memacc.c */

#include "type.h"

extern ulong get_phys_address (int bank, ushort as, ushort log_addr);
extern bool  get_raw (int bank, ushort as, ushort address, ushort *value);
extern void  store_raw (int bank, ushort as, ushort address, ushort value);

