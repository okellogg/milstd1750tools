/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :            main.c -- simulator main program                 */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                                                                         */
/* Author    :      Oliver M. Kellogg, Dornier Satellite Systems,          */
/*                     Dept. RST13, D-81663 Munich, Germany.               */
/* Contact   :           oliver.kellogg@space.otn.dasa.de                  */
/*                                                                         */
/* Disclaimer:                                                             */
/*                                                                         */
/*  This program is free software; you can redistribute it and/or modify   */
/*  it under the terms of the GNU General Public License as published by   */
/*  the Free Software Foundation; either version 2 of the License, or      */
/*  (at your option) any later version.                                    */
/*                                                                         */
/*  This program is distributed in the hope that it will be useful,        */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          */
/*  GNU General Public License for more details.                           */
/*                                                                         */
/*  You should have received a copy of the GNU General Public License      */
/*  along with this program; if not, write to the Free Software            */
/*  Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.   */
/*                                                                         */
/***************************************************************************/


#include <stdio.h>
#if (! defined (__MSDOS__) && ! defined (__VMS))
#include <unistd.h>
#endif

#include "version.h"
#include "type.h"
#include "status.h"
#include "utils.h"
#include "targsys.h"
#include "cmd.h"
#include "loadfile.h"


/*
 * Program name is argv [0], see function main
 */
char *program_name;
char *program_version = "$Id$";

/*
 * Options for this run
 */
bool verbose = TRUE;      /* use -q to turn off excessive output */
bool need_speed = FALSE;  /* use -n to increase speed (see the SP command) */


static void
print_optionhelp ()
{
  puts ("available options:");
  puts ("  -q                      (quiet)");
  puts ("  -b <batch_startfile>    (load startup file)");
  puts ("  -c <coff_loadfile>      (directly run COFF file)");
  puts ("  -t <tekhex_loadfile>    (directly run TEKHEX file)");
  puts ("  -l <tldldm_loadfile>    (directly run TLDLDM file)");
  puts ("  -x <xtcldm_loadfile>    (directly run XTCLDM file)");
  puts ("  -n                      (gain speed/disable backtracing)");
}


int
main (int argc, char *argv[])
{
  char *batchfile = NULL, *loadfile = NULL, *chip;
  loadfile_t filetype = NONE;

#if defined   (F9450)
  chip = "F9450";
#elif defined (PACE)
  chip = "PACE";
#elif defined (GVSC)
  chip = "GVSC";
#elif defined (MA31750)
  chip = "MA31750";
#elif defined (MAS281)
  chip = "MAS281";
#endif

  if (init_system (0))                /* global system initialization */
    problem ("simulator initialization failed");

  if (argc >= 2)
    {
      int i;

      for (i = 1; i < argc; i++)
        {
          if (*argv[i] == '-')
            {
              switch (argv[i][1])
                {
                case     'q':        /* quiet */
                  verbose = FALSE;
                elsecase 'b':        /* batch startfile */
                  batchfile = argv[++i];
                elsecase 'c':        /* coff startfile */
                  loadfile = argv[++i];
                  filetype = COFF;
                elsecase 't':        /* tek startfile */
                  loadfile = argv[++i];
                  filetype = TEK_HEX;
                elsecase 'l':        /* tld startfile */
                  loadfile = argv[++i];
                  filetype = TLD_LDM;
                elsecase 'x':        /* xtc startfile */
                  loadfile = argv[++i];
                  filetype = XTC_LDM;
                elsecase 'n':        /* no backtrace */
                  need_speed = TRUE;
		elsecase 'h':        /* help on command line options */
                  print_optionhelp ();
                  break;
                default:
                  print_optionhelp ();
                  problem ("(unknown option given)");
                }
            }
        }
    }

  if (loadfile != NULL)
    {
      int  ans = 0, f_argc = 2;
      char *f_argv[3];

      f_argv[0] = NULL;
      f_argv[1] = loadfile;

      switch (filetype)
        {
        case     COFF:
          ans = si_lcf (f_argc, f_argv);
        elsecase TEK_HEX:
          ans = si_lo  (f_argc, f_argv);
        elsecase TLD_LDM:
          ans = si_tld (f_argc, f_argv);
        elsecase XTC_LDM:
          ans = si_xtc (f_argc, f_argv);
	  break;
	default:
	  ans = ERROR;
        }
      if (ans != OKAY)
	problem ("Could not run (see above)");
      f_argc = 0;
      f_argv [0] = NULL;
      ans = si_go (f_argc, f_argv);
      return ans;
    }

  if (verbose)
    {
      printf ("\nMIL-STD-1750 software simulator v. %s ", SIM1750_VERSION);
      if (chip != NULL)
        printf ("configured for %s\n", chip);
      else
        printf ("(unconfigured)\n");

      printf
  ("sim1750 is free software and you are welcome to distribute copies of it\n");
      printf
  (" under certain conditions; type \"cond\" to see the conditions.\n");
      printf
  ("There is absolutely no warranty for sim1750; type \"warr\" for details.\n");
      printf ("Copyright 1994-97 Daimler-Benz Aerospace AG\n");
    }

  interpreter (batchfile);  /* let's spend the night together */

  init_system (1);          /* tidy up before leaving */

  return (EXIT_SUCCESS);
}

