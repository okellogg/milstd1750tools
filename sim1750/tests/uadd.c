#include <stdio.h>

main (int argc, char *argv[])
{
  short n1, n2;
  unsigned op1, op2, result;

  sscanf (argv[1], "%hd", &n1);
  sscanf (argv[2], "%hd", &n2);
  op1 = *(unsigned short *) &n1;
  op2 = *(unsigned short *) &n2;
  result = op1 + op2;
  printf ("%08X + %08X = %08X\n", op1, op2, result);
}

