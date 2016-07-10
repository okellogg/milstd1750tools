#include <stdio.h>

main (int argc, char *argv[])
{
  short n1, n2;
  unsigned long op1, op2, result;

  sscanf (argv[1], "%hd", &n1);
  sscanf (argv[2], "%hd", &n2);
  op1 = *(unsigned short *) &n1;
  op2 = *(unsigned short *) &n2;
  result = op1 + op2;
  printf ("%08lX + %08lX = %08lX\n", op1, op2, result);
}

