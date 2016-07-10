/* do_xio.c -- user definable external IO call interface for sim1750 */

#include <stdio.h>
#if defined (__MSDOS__)
#include <io.h>
#elif defined (__VMS)
#include <unixio.h>
#else
#include <unistd.h>
#endif

#include "arch.h"
#include "status.h"
#include "utils.h"
#include "targsys.h"


/* Export */

void  do_xio (ushort address, ushort *value)
{
#ifdef MAS281
  /* 
   * Code to support ERA MA281 board's serial interface
   */
  if (address == 0x8501)
    {
      /* read for serial interface 1 status register */
      *value = 0x06;
    }
  else if (address == 0x0500)
    {
      /* write to serial interface 1 data register */
      printf ("%c", *value);
    }
  else if (address == 0x8500)
    {
      /* read from serial interface 1 data register */
      *value = getchar ();
    }
  else
#endif

  if (address & 0x8000)  /* XIO read */
    {
      unsigned input_value;

      info ("IO from %04hX < == ", address);
      if (scanf ("%i", &input_value) <= 0)
	{                     /* check for input redirection */
	  if (!isatty (fileno (stdin)))
	    problem ("Reading past end of input file !");
	}
      *value = (ushort) input_value;
      if (!isatty (fileno (stdin)))
        info ("0x%04hX\n", value);
    }
  else			/* XIO write */
    info ("IO  to  %04hX == > 0x%04hX\n", address, *value);
}

