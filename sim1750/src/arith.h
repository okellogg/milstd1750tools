/* arith.h  --  exports of arith.c */

#include "arch.h"

void update_cs (short *operand, datatype data_type);
void compare (datatype data_type, short *operand0, short *operand1);
void arith (operation_kind operation, datatype vartyp,
	    short *operand0, short *operand1);

