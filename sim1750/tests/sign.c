#include <stdio.h>

main (int argc, char *argv[])
{
  long n;

  sscanf (argv[1], "%li", &n);
  if (!strncmp (argv[1], "0x", 2) && strlen (argv[1]) == 6 && (n & 0x8000L))
    n |= 0xFFFF0000L;
  printf ("0x%08lX => %ld\n", n, n);
}

