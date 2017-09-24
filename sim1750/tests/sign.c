#include <stdio.h>

main (int argc, char *argv[])
{
  int n;

  sscanf (argv[1], "%i", &n);
  if (!strncmp (argv[1], "0x", 2) && strlen (argv[1]) == 6 && (n & 0x8000L))
    n |= 0xFFFF0000L;
  printf ("0x%08X => %d\n", n, n);
}

