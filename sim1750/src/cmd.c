/***************************************************************************/
/*                                                                         */
/* Project   :        sim1750 -- Mil-Std-1750 Software Simulator           */
/*                                                                         */
/* Component :               cmd.c -- Command Interpreter                  */
/*                                                                         */
/* Copyright :         (C) Daimler-Benz Aerospace AG, 1994-97              */
/*                         (C) 2017 Oliver M. Kellogg                      */
/* Contact   :           okellogg@users.sourceforge.net                    */
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
#include <signal.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#if defined (__VMS)
#include <lib$routines.h>
#elif defined (__MSDOS__)
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef GNU_READLINE
#include <readline/readline.h>
#endif

#include "arch.h"
#include "break.h"
#include "cmd.h"
#include "cpu.h"
#include "exec.h"
#include "flt1750.h"
#include "loadfile.h"
#include "phys_mem.h"
#include "peekpoke.h"
#include "smemacc.h"
#include "version.h"
#include "status.h"
#include "tekops.h"
#include "utils.h"

/* imports not mentioned in includefiles */

extern int  dism1750 (char *, ushort *);  /* dism1750.c */
extern char *disassemble ();		  /* sdisasm.c */

/* private stuff */

#define MAXCOMARGS 32
#define isspc(c)  (c == '\n' || c == '\t' || c == ' ')

static const char *prompt = "command > ";

static bool  batchbreak = TRUE;	/* breaking batch allowed                 */
static int   int_count = 0;	/* number of interrupts from keyboard     */
static char  logfilename[128];
static int   leave = 0;			/* leave command interpreter */

#define P (int argc, char *argv[])
static int co_batch P, co_logopen P, co_logclose P, co_sh P, co_exit P;
static int co_echo P, co_help P, co_info P, co_speed P, co_timers P;
static int co_version P, co_shoc P, co_war P;
static int si_dispreg P, si_disasm P, si_dispmem P, si_dispflt P;
static int si_dispeflt P, si_dispchar P, si_changemem P, si_changereg P;
static int si_init P, si_reset P, si_tr P, si_page P, si_fill P;
#undef P

static const struct {
		      char *name;
		      int  (*function) (int, char **);
		      char *brief_description;
		      char *helptext;
		    } comtab[] =
 {
   /* This table is sorted such that the more commonly used commands
      come first. The reason is that abbreviations are permitted,
      and the first command that matches the abbreviation is 
      selected. E.g. on inputting "co", the command "coffload" is
      selected as it comes before "conditions".
    */
   { "help [command]",         co_help,     "print this helptext",
       "Issued without the optional command argument, the HELP command\n"
       "displays a text screen listing all available commands with a brief\n"
       "description of each. If a command name is supplied (as currently),\n"
       "then more detailed information is displayed.\n"
       "For help on address expression syntax, see help on the TR command." },
   { "go [address]",            si_go,       "start or continue execution",
       "If the optional address argument is not supplied, then execution\n"
       "starts at the current Instruction Counter location and in the\n"
       "current Address State." },
   { "ss [n_instructions]",     si_snglstp,  "single step",
       "If the optional n_instructions argument is not supplied, then step\n"
       "for just one instruction." },
   { "ss *",                    si_snglstp,  "step over subroutine call",
       "" },

   { "break <address>",        si_brkset,   "set breakpoint",
       "Set breakpoint at the given address. For the syntax of the address\n"
       "expression, see the help info on the TR command. Additionally,\n"
       "a symbolic label name may be given for the <address> if the load\n"
       "format last used supports them, and the file last loaded contains\n"
       "such symbolic labels." },
   { "blist",                  si_brklist,  "list breakpoints",
       "" },
   { "bclear <address>",       si_brkclear, "clear breakpoint",
       "" },
   { "bclear *",               si_brkclear, "clear all breakpoints",
       "" },
   { "bsave <file>",           si_brksave,  "save breakpoints to batch file",
       "" },

   { "trace [n]",              si_trace,    "trace the next n instructions",
       "Continually display the register file as each instruction is executed." },
   { "btrace n1 [n2]",         si_bt,       "go back n1 and retrace the next n2 instructions",
       "" },

   { "info [on|off]",          co_info,     "print interpreter status info",
       "If given without an argument, the 'info' command displays statistics\n"
       "of the execution underway. If given with the argument 'on' or 'off',\n"
       "it toggles between the verbose and quiet mode of simulator operation." },
   { "speed [on|off]",         co_speed,    "trade features for speed",
       "SP ON disables the BT (backtrace) feature, giving a slight speedup\n"
       "in simulation execution. Under DOS only, SP ON additionally\n"
       "disables <Ctrl-C> keypress checking, resulting in a substantial\n"
       "simulation speedup. SP OFF re-enables the above feature(s).\n" },
   { "version",                co_version,  "print version information",
       "" },

   { "init",                   si_init,     "initialize the entire simulator",
       "" },
   { "reset",                  si_reset,    "reset registers and MMU only",
       "All CPU and MMU registers are set to zero." },
   { "translate <address>",    si_tr,       "translate logical to phys. addr.",
       "The simulator supports MMU operations. When using the MMU, a\n"
       "mapping is done to get from a 16-bit logical address to a 20-bit\n"
       "physical address. Of the 16-bit logical address, the lower 12 bits\n"
       "are the also used for the lower 12 bits of the physical address.\n"
       "The most significant nibble of the logical address is used together\n"
       "with the Address State bits and the Instruction/Operand bit to\n"
       "obtain the upper 8 bits of the physical address. The TR command\n"
       "takes a logical address as input, and translates this to the\n"
       "corresponding physical address.\n"
       "The syntax of the address argument is the same for all other sim1750\n"
       "commands that also take an address argument, and consists of the\n"
       "(optional) Address State number (one hexadecimal digit), followed by\n"
       "the letter 'i' for Instruction page or the letter 'o' for Operand\n"
       "page, followed by the logical address. If the Address State number\n"
       "is left away, then the currently effective Address State (as per\n"
       "the Status Word) is used. If neither the AS number not the letter\n"
       "'i' or 'o' is specified, then a physical address is assumed, which\n"
       "may then be of 20 bit maximum size. (In the case of the TR command,\n"
       "giving a physical address does not make sense as it just translates\n"
       "to itself.) While the address expressions are specified in hexa-\n"
       "decimal, the <length> argument (of other commands) is in decimal\n"
       "by default (to input a length in hex, prefix it by '0x')." },
   { "fill <start> <fill_expr>", si_fill,   "fill memory with values",
       "Fill memory starting at address <start> (given in hexadecimal.)\n"
       "The fill_expr can be just a plain number, or a number followed by\n"
       "an increment factor. The increment factor is specified by a '+' or\n"
       "'-' sign, optionally followed by the increment amount. If no amount\n"
       "is given, then it defaults to 1. Further, the number or number/\n"
       "increment pair may be preceded by a repeat factor, in which case it\n"
       "must be enclosed in parentheses. Such fill_expressions may also be\n"
       "recursively nested. The increment amount, if given, may be followed\n"
       "by the letter 'n' (for 'norestart'), in which case in nested loops\n"
       "the respective value is not reset to its initial value (i.e. it is\n"
       "continually incremented throughout all iterations.)\n"
       "The numeric base for all numbers in the fill_expr is decimal.\n"
       "Hexadecimal numbers must be prefixed by '0x'.\n"
       "Here are some examples:\n\n"
       "Write 20 zeros to the locations at and following address 100 (hex.):\n"
       "  command > fi 100 20(0)\n\n"
       "Write a complex pattern starting at address 100 (hex.):\n"
       "  command > fi 100 8(10(0x20-2,0xd00+),0x50-)\n"
       "  command > dd 100 60\n"
       "  0:0100    0020   0D00   001E   0D01   001C   0D02   001A   0D03\n"
       "  0:0108    0018   0D04   0016   0D05   0014   0D06   0012   0D07\n"
       "  0:0110    0010   0D08   000E   0D09   0050   0020   0D00   001E\n"
       "  0:0118    0D01   001C   0D02   001A   0D03   0018   0D04   0016\n"
       "  0:0120    0D05   0014   0D06   0012   0D07   0010   0D08   000E\n"
       "  0:0128    0D09   004F   0020   0D00   001E   0D01   001C   0D02\n"
       "  0:0130    001A   0D03   0018   0D04   0016   0D05   0014   0D06\n"
       "  0:0138    0012   0D07   0010   0D08   000E   0D09   004E   0020\n"
       "  0:0140    0D00   001E   0D01   001C   0D02   001A   0D03   0018\n"
       "  0:0148    0D04   0016   0D05   0014   0D06   0012   0D07   0010\n"
       "  0:0150    0D08   000E   0D09   004D   0020   0D00   001E   0D01\n"
       "  0:0158    001C   0D02   001A   0D03   0018   0D04   0016   0D05\n" },

   { "psload <file>",             si_pslo,   "load IBM UC Program Store file",
       "Load a binary file in the IBM User Console PSD (Program Store\n"
       "Download) format." },
   { "promload <file>",           si_prolo,  "load absolute binary PROM image",
       "Load a 'flat' binary file which is a simple memory image starting\n"
       "at address 00000 (and continuing until the file gives EOF.)\n"
       "The order of bytes within each 16-bit word in the file is assumed\n"
       "to be: most significant byte first." },
   { "load <file>",            si_lo,     "load Tek-Hex file",
       "Load an ASCII file in Tektronix Extended Hexadecimal format." },
   { "lcf <file>",             si_lcf,   "load COFF file",
       "Load a file in the binary Common Object File Format." },
   { "tld <file>",             si_tld,    "load TLD LDM or LLM file",
       "Load an ASCII file in TLD Physical Load Module (LDM) or Logical\n"
       "Load Module (LLM) format." },
   { "xtc <file>",             si_xtc,    "load TLD LDM or LLM file",
       "Load an ASCII file in XTC MACRO-50 Load Module (LDM) format." },
   { "save <file>",               si_save,     "save image of memory to file",
       "Save an image of the currently active memory, i.e. those parts of\n"
       "the entire address space that have been written to at least once.\n"
       "The file format used for saving is the Extended Tek Hex format." },
   { "symbols",                  si_dispsym,  "display symbols found in memory",
       "Some load module formats support the embedding of symbol strings.\n"
       "Display all such symbol names currently loaded." },
   { "dreg",                     si_dispreg,   "display registers",
       "Display the register file" },
   { "dmem [address [length]]",  si_dispmem,   "display memory",
       "Display memory locations. For address expression syntax, see TR." },
   { "dfloat [reg | [addr [len]]", si_dispflt,   "display data as floats",
       "Display memory as 1750A 32-bit floats. For address expression\n"
       "syntax, see the TR command." },
   { "defloat [reg | [addr [len]]", si_dispeflt,  "display extended floats",
       "Display memory as 1750A 48-bit floats. For address expression\n"
       "syntax, see the TR command." },
   { "dchar [address [length]",si_dispchar,  "display data as ASCII characters",
       "Display memory as characters. For the syntax of the address\n"
       "argument, see the TR command." },
   { "disassemble [start [length]]",    si_disasm,   "disassemble",
       "Disassemble the memory locations starting at <start> and continuing\n"
       "for <length>. If no start argument is given, then disassemble from\n"
       "the current Instruction Counter onwards. If no length argument is\n"
       "given, then disassemble for one instruction, or for the last\n"
       "specified length in a previous DI command if any such was given." },
   { "creg r<num> <val>",      si_changereg,"change register to new value",
       "Change the register contents of the given register. The <val>\n"
       "argument is in hexadecimal." },
   { "cmem <address>",         si_changemem,"change memory",
       "The Change Memory command is for interactively changing memory\n"
       "locations. The simulator prompts for values to be put\n"
       "in successive locations at/after the start address given.\n"
       "Enter such values in hexadecimal. In order to back up to the\n"
       "previous location, enter a backslash (\\). In order to skip the\n"
       "current location (i.e. leave its value unchanged), enter a slash\n"
       "(/). In order to terminate input of values, enter the letter 'q'." },
   { "pagereg [as]",           si_page, "page register assignment",
       "When given without the <as> argument, displays the page register\n"
       "assignments of the current Address State. If the <as> parameter\n"
       "is supplied, then displays the page registers of the given Address\n"
       "State." },

   { "lopen <file>",           co_logopen,  "open log file",
       "A logfile of the given name is opened. All subsequent command\n"
       "inputs and program outputs are logged to the logfile until either\n"
       "the logfile is closed via the LOGCLOSE command, or the simulator\n"
       "is exited." },
   { "lclose",                 co_logclose, "close log file",
       "The logfile opened by LOGOPEN is closed." },
   { "batch <file>",           co_batch,    "execute batch file",
       "(untested) Load and execute a sequence of sim1750 commands from file." },
   { "shell",                  co_sh,       "spawn an OS shell",
       "The simulator is temporarily left, and control is turned over\n"
       "to the operating system, i.e. the user can enter commands at the\n"
       "OS shell level. This shell is left via the applicable operating\n"
       "system command (e.g. Unix/DOS: exit ; VMS : logout) After exiting\n"
       "the shell, the user is returned into the simulator in its same\n"
       "state as prior to the 'sh' command." },
   { "timers [off|on]",        co_timers,   "switch timers A/B off or on",
       "On startup, the timers are on.\n"
       "If no argument is given to the command then the on/off state is toggled." },
   { "conditions",             co_shoc,     "show software copying policy",
       "The GNU General Public License is displayed." },
   { "warranty",               co_war,      "show warranty information",
       "Section 10 of the GNU General Public License is displayed." },
   { "quit",                   co_exit,     "leave the simulator",
       "" },
   { "exit",                   co_exit,     "synonym for 'quit'",
       "" },
/*
   { "echo <text>",            co_echo,     "echo text to stdout",
       "" },
 */
   { NULL,                     NULL,        NULL, NULL }
 };


#define MAXINFILES 10
FILE *infiles[MAXINFILES];
int  actinfile;


static int
get_line (char *buffer)
{
#ifdef GNU_READLINE
  if (actinfile == 0)
    {
      strcpy (buffer, readline (prompt));
      if (strlen (buffer) > 0)
	add_history (buffer);
      return 0;
    }
#endif

  buffer[0] = '\0';
  while (fgets (buffer, 255, infiles[actinfile]) == NULL)
    {
      if (actinfile > 0)
	{
	  fclose (infiles[actinfile]);
	  actinfile--;
	  if (actinfile == 0)
	    return (0);		/* last batchfile, return for prompt */
	}
      else
	{
	  leave = QUIT;
	  buffer[0] = '\0';
	  return (-1);		/*   EOF  */
	}
    }
  buffer[strlen (buffer) - 1] = '\0';
  strlower (buffer);
  return (0);
}


/* return index into comtab, or -1 if `s' is not a valid keyword. */
static int
parse_keyword (char *s)
{
  int i;
  for (i = 0; comtab[i].name != NULL; i++)
    {
      if (strmatch (comtab[i].name, s))
	return i;
    }
  return -1;
}


int
interpreter (char *startup_batchfile)
{
  char buffer[256];
  bool have_args;
  int f_argc;
  char *f_argv[MAXCOMARGS + 1], *keyword, *p;
  int i, comindex;

  if (startup_batchfile != NULL)
    {
      FILE *helpfp;
      if ((helpfp = fopen (startup_batchfile, "r")) != NULL)
        infiles[++actinfile] = helpfp;
      else
        error ("could not open startup batchfile %s", startup_batchfile);
    }

  f_argv[0] = (char *) calloc (1, 128);
  for (i = 1; i <= MAXCOMARGS; i++)
    if ((f_argv[i] = (char *) calloc (1, 64)) == NULL)
      problem ("interpreter: could not allocate cmd string space");

  while (1)
    {
      if (! batchbreak)
	int_count = 0;
      else if (actinfile)
	{
	  if (int_count)
	    {
	      sys_int (0);
	      lprintf ("\nInterrupt : %s >>> %s\n", f_argv[0], global_message);
	    }
	}
      else
	int_count = 0;

#ifndef GNU_READLINE
      if (!actinfile)
	lprintf ("\n%s", prompt);
#endif

      get_line (buffer);

      if (leave)
	return (QUIT);		/* END of stdin */

      if (strlen (buffer) < 1)
	continue;
      if ((keyword = skip_white (buffer)) == NULL || *keyword == '\0')
	continue;
      if (*keyword == '#')
	continue;		/* comment */

      if (strlen (strlower (keyword)) < 1)
	continue;

      have_args = FALSE;
      if ((p = strchr (keyword, ' ')) != NULL)
	{
	  *p = '\0';
	  have_args = TRUE;
	}
      if ((comindex = parse_keyword (keyword)) == -1)
	{
	  lprintf ("unknown command %s\n", keyword);
	  continue;
	}
      if (have_args)
	*p = ' ';
      strcpy (f_argv[0], keyword);
      f_argc = 1;
      if (have_args)
	{
	  while (*p)
	    {
	      while (isspace (*p))
		p++;
	      if (*p == '\0')
		break;
	      i = 0;
	      while (*p && ! isspace (*p))
		  *(f_argv[f_argc] + i++) = *p++;
	      *(f_argv[f_argc++] + i) = '\0';
	    }
	}
      *f_argv[f_argc] = '\0';

      (*comtab[comindex].function) (f_argc, f_argv);

      if (leave)
	return (QUIT);		/* END of stdin */

    }
}


static int
init_io (int mode)
{
  int i;

  if (!mode)			/* START */
    {
      logfile = (FILE *) 0;
      actinfile = 0;
      infiles[actinfile] = stdin;
      int_handler_install (0);
    }
  else
    /* STOP  */
    {
      for (i = actinfile; i > 0; i--)
	fclose (infiles[i]);
      int_handler_install (1);	/* restore old interrupt handler */
      if (logfile != (FILE *) 0)
	fclose (logfile);
    }
  return 0;
}


int
init_system (int mode)
{
  int retval = 0;

  retval += init_io (mode);
  retval += init_simulator (mode);
  return retval;
}


static void
int_handler (int sig)
{
  int_count++;
  if (sig != SIGINT)
    info ("\n In signal %d handler\n", sig);
  signal (sig, int_handler);
}

void
int_handler_install (mode)
     int mode;       /* 0==install new handler || 1==reinstall old handler */
{
  typedef void (*PFI) ();
#ifndef NSIG
#define NSIG 6
#endif
  static PFI int_tab[NSIG - 1];

  if (mode == 0)
    {
      int_tab[0] = signal (SIGTERM, int_handler);
      int_tab[1] = signal (SIGINT, int_handler);
#ifndef __MSDOS__
      int_tab[2] = signal (SIGHUP, int_handler);
      int_tab[3] = signal (SIGQUIT, int_handler);
#ifdef __VMS
      VAXC$ESTABLISH ((unsigned int (*)())int_handler);
#endif
#endif
    }
  else
    {
      signal (SIGTERM, int_tab[0]);
      signal (SIGINT, int_tab[1]);
#ifndef __MSDOS__
      signal (SIGHUP, int_tab[2]);
      signal (SIGQUIT, int_tab[3]);
#endif
    }
}


/* sys_int() should be called upon execution of each 1750 instruction.
   It makes sure the interpreter gets to notice if the user presses
   <Control-C>. That is useful when execution is in an endless loop.
 */
int
sys_int (int val)
{
  int retval = 0;
#ifdef __MSDOS__
  extern int kbhit();
  /* Just do the kbhit() call for the sake of any I/O happening.
     Without I/O, the signal(SIGINT) mechanism does not work in DOS. */
  if (! need_speed)
    kbhit();
#endif

  if (int_count >= val)
    {
      while (actinfile > 0)
	{
	  fclose (infiles[actinfile--]);
	  retval++;
	}
      int_count = 0;
      retval++;
      sprintf (global_message, "%d Level(s) interrupted (%d)", retval, val);
    }

  return (retval);
}


/**********************  command execution functions  *************************/

static int
co_batch (int argc, char *argv[])
{
  int retval = 0;
  FILE *helpfp;

  if (argc >= 2)
    {
      if (actinfile >= MAXINFILES - 1)
	{
	  sprintf (global_message, "maximal batch level exceeded");
	  retval = 1;
	}
      else if ((helpfp = fopen (argv[1], "r")) == NULL)
	{
	  sprintf (global_message, "can't open batchfile  %s", argv[1]);
	  retval = 1;
	}
      else
	{
	  infiles[++actinfile] = helpfp;
	}
    }
  else
    {
      sprintf (global_message, "argument missing (filename)");
      retval = 1;
    }
  return (retval);
}

static int
co_logopen (int argc, char *argv[])
{
  if (argc >= 2)
    {
      if (logfile != (FILE *) 0)
	{
	  sprintf (global_message, "logfile >> %s << already open", logfilename);
	  return (1);
	}
      strcpy (logfilename, argv[1]);
    }
  else
    strcpy (logfilename, "sim1750.log");
  if ((logfile = fopen (argv[1], "a")) == NULL)
    {
      sprintf (global_message, "can't open logfile  %s", logfilename);
      return (1);
    }
  return (0);
}


static int
co_logclose (int argc, char *argv[])
{
  int retval = 0;

  if (logfile == (FILE *) 0)
    {
      sprintf (global_message, "no logfile open");
      retval = 1;
    }
  else
    fclose (logfile);
  return (retval);
}


static int
co_sh (int argc, char *argv[])
{
  int retval;
  int i;
  char commandline[1024];

#ifdef __VMS
  retval = lib$spawn ();
#else
  if (argc <= 1)
#ifdef __MSDOS__
    strcpy (commandline, "command");
#else
    strcpy (commandline, "sh");
#endif
  else
    {
      strcpy (commandline, "");
      for (i = 1; i < argc; i++)
	{
	  if (i > 1)
	    strcat (commandline, " ");
	  strcat (commandline, argv[i]);
	}
    }
  retval = system (commandline);
#endif

  return (retval);
}


static int
co_exit (int argc, char *argv[])
{
  if (actinfile > 0)
    fclose (infiles[actinfile--]);
  else
    leave = QUIT;
  return (0);
}


static int
co_echo (int argc, char *argv[])
{
  int i;

  for (i = 1; i < argc; i++)
    lprintf (" %s", argv[i]);
  lprintf ("\n");
  return (0);
}


static int
co_help (int argc, char *argv[])
{
  int i = 0;

  if (argc > 1)
    {
      char *p;
      if ((i = parse_keyword (argv[1])) < 0)
	return error ("unknown command name");
      p = comtab[i].helptext;
      putchar ('\t');
      while (*p)
	{
	  putchar (*p);
	  if (*p++ == '\n')
	    putchar ('\t');
	}
      putchar ('\n');
      return 0;
    }
  printf ("    Available commands (abbreviations are permitted)\n");
  while (comtab[i].name != (char *) 0)
    {
      printf ("%-34s", comtab[i].name);
      printf ("! %s\n", comtab[i].brief_description);
      i++;
    }
  printf ("\n");
  return (0);
}

static int
co_info (int argc, char *argv[])
{
  uint addr = 0, end_addr;
  int i;
  bool seen_start = FALSE;

  if (argc > 1)
    {
      if (eq (argv[1], "on"))
	verbose = TRUE;
      else if (eq (argv[1], "off"))
	verbose = FALSE;
      else
	return error ("invalid parameter -- must be 'on' or 'off'");
      return OKAY;
    }
  lprintf ("\n  Simulation Status\n\n");
  lprintf ("\tBatch level:\t%d\n", actinfile);
  if (logfile != (FILE *) 0)
    lprintf ("\tLogfile:\t\t%s\n", logfilename);
  lprintf ("\tAllocation:\t%d words of simulation memory\n", allocated/2);
  lprintf ("\tOptimization in favor of (speed|features):\t%s\n",
	   need_speed ? "Speed" : "Features");
  lprintf ("\tInstruction Count:\t%d\n", instcnt);
  lprintf ("\tExecution time (uSec):\t%0.3f\n", total_time_in_us);
  lprintf ("\tMemory regions used:\n");
  for (i = 0; i < N_PAGES; i++)
    {
      if (mem[i] == MNULL)
	{
	  if (seen_start)
	    {
	      seen_start = FALSE;
	      lprintf ("%05lX\n", addr - 1);
	    }
	  continue;
	}
      addr = (uint) i << 12;
      end_addr = (((uint) i + 1) << 12) - 1;
      do
	{
	  if (was_written (addr))
	    {
	      if (! seen_start)
		{
		  seen_start = TRUE;
		  lprintf ("\t\t\t%05lX - ", addr);
		}
	      while (addr < end_addr && was_written (++addr))
	        ;
	      if (addr < end_addr)
		{
		  seen_start = FALSE;
		  lprintf ("%05lX\n", addr - 1);
		}
	    }
	} while (addr++ < end_addr);
    }

  instcnt = 0;
  total_time_in_us = 0.0;
  return (0);
}

static int
co_speed (int argc, char *argv[])
{
  if (argc > 1)
    {
      if (eq (argv[1], "on"))
	need_speed = TRUE;
      else if (eq (argv[1], "off"))
	need_speed = FALSE;
      else
	return error ("invalid parameter -- must be 'on' or 'off'");
    }
  else
    need_speed = ! need_speed;
  info ("Backtracing (BT)%s is now %sabled",
#ifdef __MSDOS__
      " and <Ctrl-C> keypress check during simulation",
#else
      "",
#endif
      need_speed ? "dis" : "en");
  return (OKAY);
}

static int
co_timers (int argc, char *argv[])
{
  if (argc > 1)
    {
      if (eq (argv[1], "on"))
	disable_timers = FALSE;
      else if (eq (argv[1], "off"))
	disable_timers = TRUE;
      else
	return error ("invalid parameter -- must be 'on' or 'off'");
    }
  else
    disable_timers = ! disable_timers;
  info ("Timers A/B are now %sabled", disable_timers ? "dis" : "en");
  return (OKAY);
}

static int
co_version (int argc, char *argv[])
{
  lprintf ("\n sim1750 version %s (%s)\n", SIM1750_VERSION, SIM1750_DATE);
  return (OKAY);
}

static int
co_shoc (int argc, char *argv[])
{
  int i = 0;
  extern char *license[];

  while (*license[i] != '\\')
    {
      lprintf ("%s\n", license[i]);
      if (i % 22 == 0 && i > 0)
	{
	  lprintf ("\ntype <Return> to continue ");
	  getchar ();
	}
      i++;
    }
  return (0);
}

static int
co_war (int argc, char *argv[])
{
  int i = 0;
  extern char *warranty[];

  while (*warranty[i] != '\\')
    {
      lprintf ("%s\n", warranty[i]);
      i++;
    }
  return (0);
}



#define DUMPLEN		8	/* line length of memory dump function */
#define CHARDUMPLEN     16	/* number of characters in one line */



/* Parse a string of the form:
     <hex_addr>
   or
     [<hex_as>]i<hex_addr>
   or
     [<hex_as>]o<hex_addr>
   In the first form, <hex_addr> is interpreted as a physical address
   of 24 bits maximum size.
   In the second and third form, the <hex_addr> is interpreted as a
   logical address within the instruction (i) or operand (o) page.
   In these forms, the letter 'i' or 'o' must precede the <hex_addr>.
   Further, the address state <hex_as> to which the address applies may
   optionally precede the Instruction/Operand page indication. If the
   <hex_as> is left away, then the current address state is used.

   Return OKAY for success or ERROR on invalid syntax of the input string.
 */
bool
parse_address (char *str, uint *phys_address)
{
  char *i_o = strpbrk (str, "io");
  int  hexdigit;
  ushort as = simreg.sw & 0xF;

  if (i_o != NULL)
    {
      if (i_o > str)
	{
	  if ((hexdigit = xtoi (*str)) == -1)
	    return ERROR;
	  as = (ushort) hexdigit;
	}
      str = i_o + 1;
    }
  *phys_address = 0;
  while (*str)
    {
      if ((hexdigit = xtoi (*str)) == -1)
	return ERROR;
      *phys_address = (*phys_address << 4) | (uint) hexdigit;
      str++;
    }
  if (i_o != NULL)
    *phys_address = get_phys_address (*i_o == 'i' ? CODE : DATA,
					as, (ushort) *phys_address);
  return OKAY;
}


static int
si_disasm (int argc, char *argv[])
{
  int i, n_words;
  char *sym, disasm_text[100];
  ushort words[2];
  static uint address = 0;
  static int length = 1;
  bool verbose_save = verbose;

  if (argc > 1)
    {
      if (parse_address (argv[1], &address) != OKAY)
	return error ("invalid address syntax");
      if (argc > 2)
	{
	  sscanf (argv[2], "%i", &length);
	  if (argc > 3)
	    info ("excess argument(s) ignored");
	}
    }

  verbose = FALSE;
  lprintf ("\n");
  for (i = 0; i < length; i++)
    {
      if (sys_int (1))
	return (INTERRUPT);
      if ((sym = find_labelname (address)) != NULL)
	lprintf ("                      %s\n", sym);
      lprintf ("%05lX     ", address);
      if (! peek (address, &words[0]))
	{
	  verbose = verbose_save;
	  return error ("<no code loaded here>");
	}
      peek (address + 1, &words[1]);
      n_words = dism1750 (disasm_text, words);
      lprintf ("%04hX ", words[0]);
      if (! n_words)
	{
	  error ("     invalid opcode");
	  n_words = 1;
	}
      else
	{
	  if (n_words == 2)	/* print the data word following the opcode */
	    lprintf ("%04hX", words[1]);
	  else
	    lprintf ("    ");
	  lprintf ("     %s\n", disasm_text);
	}
      address += n_words;
    }
  lprintf ("\n");
  verbose = verbose_save;

  return (OKAY);
}


void
dis_reg ()
{
  int i;
#define state(flag)  ((simreg.sys & flag) ? 'E' : 'D')

  for (i = 0; i < 16; i++)
    {
      lprintf ("R%02d:%04X", i, (unsigned) simreg.r[i] & 0xFFFF);
      if (i == 7 || i == 15)
	lprintf ("\n");
      else
	lprintf ("  ");
    }
  lprintf ("PIR:%04hX  ", simreg.pir);
  lprintf ("MK: %04hX  ", simreg.mk);
  lprintf ("FT: %04hX  ", simreg.ft);
  lprintf ("SW: %04hX  ", simreg.sw);
  lprintf ("TA: %04hX  ", simreg.ta);
  lprintf ("TB: %04hX  ", simreg.tb);
  lprintf ("GO: %04hX\n", simreg.go);

  lprintf (" IC:%04hX%c %-20s", simreg.ic,
	   was_written (get_phys_address (CODE, simreg.sw & 0xF, simreg.ic))
	   ? ' ' : '!', disassemble ());
  lprintf ("CS:%c   ",
	    simreg.sw & CS_CARRY    ? 'C' :
	    simreg.sw & CS_POSITIVE ? 'P' :
	    simreg.sw & CS_ZERO     ? 'Z' :
	    simreg.sw & CS_NEGATIVE ? 'N' : '0');
  lprintf ("INT:%c DMA:%c", state(SYS_INT), state(SYS_DMA));
  lprintf (" TA:%c TB:%c\n", state(SYS_TA), state(SYS_TB));
}

static int
si_dispreg (int argc, char *argv[])
{
  info ("\t\tREGISTER DUMP\n");
  dis_reg ();

  return (OKAY);
}


static int
si_dispmem (int argc, char *argv[])
{
  int i;
  static uint address = 0;
  static int length = 1;
  bool verbose_save = verbose;

  if (argc > 1)
    {
      if (parse_address (argv[1], &address) != OKAY)
	return error ("invalid address syntax");
      if (argc > 2)
	{
	  sscanf (argv[2], "%i", &length);
	  if (argc > 3)
	    lprintf ("excess argument(s) ignored\n");
	}
    }

  verbose = FALSE;
  lprintf ("\n");
  for (i = 0; i < length; i++, address++)
    {
      ushort value;
      if (sys_int (1))
	return (INTERRUPT);
      if (i % DUMPLEN == 0)
	lprintf ("%05lX    ", address);
      if (! peek (address, &value))
	lprintf ("%04hX!  ", value);
      else
	lprintf ("%04hX   ", value);
      if (i % DUMPLEN == DUMPLEN - 1)
	lprintf ("\n");
    }
  lprintf ("\n");
  verbose = verbose_save;

  return (OKAY);
}

static int
si_dispflt (int argc, char *argv[])
{
  int i;
  short fltwords[2];
  static uint address = 0;
  static int length = 1;
  bool verbose_save = verbose;

  if (argc > 1)
    {
      if (*argv[1] == 'r')
	{
	  int n;
	  sscanf (argv[1] + 1, "%d", &n);
	  if (n < 0 || n > 14)
	    return error ("invalid register number");
	  lprintf ("%.7g\n", from_1750flt (&simreg.r[n]));
	  return OKAY;
	}
      if (parse_address (argv[1], &address) != OKAY)
	return error ("invalid address syntax");
      if (argc > 2)
	{
	  sscanf (argv[2], "%i", &length);
	  if (argc > 3)
	    info ("excess argument(s) ignored");
	}
    }

  verbose = FALSE;
  lprintf ("\n");
  for (i = 0; i < length; i++, address += 2)
    {
      bool word0_was_written, word1_was_written;
      if (sys_int (1))
	return (INTERRUPT);
      lprintf ("%05lX    ", address);
      word0_was_written = peek (address, (ushort *) &fltwords[0]);
      word1_was_written = peek (address + 1, (ushort *) &fltwords[1]);
      if (word0_was_written && word1_was_written)
	lprintf ("%.7g\n", from_1750flt (fltwords));
      else
	lprintf ("%04hX!  %04hX!\n", fltwords[0], fltwords[1]);
    }
  lprintf ("\n");
  verbose = verbose_save;

  return (OKAY);
}

static int
si_dispeflt (int argc, char *argv[])
{
  int i;
  short fltwords[3];
  static uint address = 0;
  static int length = 1;
  bool verbose_save = verbose;

  if (argc > 1)
    {
      if (parse_address (argv[1], &address) != OKAY)
	return error ("invalid address syntax");
      if (argc > 2)
	{
	  sscanf (argv[2], "%i", &length);
	  if (argc > 3)
	    info ("excess argument(s) ignored");
	}
    }

  verbose = FALSE;
  lprintf ("\n");
  for (i = 0; i < length; i++, address += 3)
    {
      bool word0_was_written, word1_was_written, word2_was_written;
      if (sys_int (1))
	return (INTERRUPT);
      lprintf ("%05lX    ", address);
      word0_was_written = peek (address, (ushort *) &fltwords[0]);
      word1_was_written = peek (address + 1, (ushort *) &fltwords[1]);
      word2_was_written = peek (address + 2, (ushort *) &fltwords[2]);
      if (word0_was_written && word1_was_written && word2_was_written)
	lprintf ("%.7g\n", from_1750eflt (fltwords));
      else
	lprintf ("%04hX!  %04hX!  %04hX!\n",
		 fltwords[0], fltwords[1], fltwords[2]);
    }
  lprintf ("\n");
  verbose = verbose_save;

  return (OKAY);
}


#define DISPLAY(ch)  ((ch) > 127 ? '.' : iscntrl(ch) ? '.' : ch)

static int
si_dispchar (int argc, char *argv[])
{
  int i = 0, j = 0;
  ushort word, chr;
  static uint address = 0;
  static int length = 1;
  char line[CHARDUMPLEN + 1];
  bool verbose_save = verbose;

  if (argc > 1)
    {
      if (parse_address (argv[1], &address) != OKAY)
	return error ("invalid address syntax");
      if (argc > 2)
	{
	  sscanf (argv[2], "%i", &length);
	  if (argc > 3)
	    info ("excess argument(s) ignored");
	}
    }

  lprintf ("\n");

  verbose = FALSE;
  i = j = 0;

  while (i < length)
    {
      if (sys_int (1))
	return (INTERRUPT);

      if (j == 0)
	lprintf ("%05lX    ", address);

      if (! peek (address, &word))
	{
	  verbose = verbose_save;
	  return error (" <uninitialized>");
	}

      /* display high byte */
      chr = word >> 8;
      lprintf ("%02hX ", chr);
      line[j] = DISPLAY(chr);

      if (j < CHARDUMPLEN - 1)
	j++;
      else
	{
	  line[j + 1] = '\0';
	  lprintf ("   %s\n", line);
	  j = 0;
	}
      if (++i >= length)
	break;

      /* display low byte */
      chr = word & 0x00ff;
      lprintf ("%02hX ", chr);
      line[j] = DISPLAY(chr);

      if (j < CHARDUMPLEN - 1)
	j++;
      else
	{
	  line[j + 1] = '\0';
	  lprintf ("   %s\n", line);
	  j = 0;
	}
      i++;
      address++;
    }

  if (j)
    {
      for (i = CHARDUMPLEN - j; i > 0; i--)
	lprintf ("   ");
      line[j + 1] = '\0';
      lprintf ("   %s\n", line);
    }

  lprintf ("\n");
  verbose = verbose_save;

  return (OKAY);
}


static int
si_changemem (int argc, char *argv[])
{
  ushort word;
  uint address;
  unsigned number;
  char mline[256];
  bool verbose_save = verbose;

  if (argc <= 1)
    return error ("address missing");

  if (parse_address (argv[1], &address) != OKAY)
    return error ("invalid address syntax");

  verbose = FALSE;
  while (TRUE)
    {
      peek (address, &word);
      lprintf ("%05lX\t%04hX => ", address, word);
      if (fgets (mline, 255, infiles[actinfile]) == NULL)
	break;
      switch (mline[0])
	{
	case '\\':
	  address--;
	elsecase '/':
	  address++;
	elsecase 'q':
	  return (OKAY);
	default:
	  if (strlen (mline) > 1)
	    {
	      sscanf (mline, "%x", &number);
	      poke (address, (ushort) number);
	    }
	  address++;
	}
    }
  verbose = verbose_save;

  return (OKAY);
}


#define REGS  25
char *reg_names[] =
{
  "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
  "r8",  "r9",  "r10", "r11", "r12", "r13", "r14", "r15",
  "pir", "mk",  "ft",  "ic",  "sw",  "ta",  "tb",  "go",  "sys"
};


static int
si_changereg (int argc, char *argv[])
{
  register int i;
  unsigned readreg;

  if (argc <= 2)
    return error ("argument missing");
  for (i = 0; i < REGS; i++)
    if (eq (argv[1], reg_names[i]))
      break;
  if (i == REGS)
    return error ("illegal name");
  sscanf (argv[2], "%x", &readreg);
  *((ushort *) (&simreg) + i) = (ushort) (readreg & 0xffff);

  return (OKAY);
}


extern int  n_breakpts;  /* break.c */

int
init_simulator (int mode)
{
  int i, as;

  if (!mode)
    {
      init_mem ();
      /* initialize MMU regs */
#define WORD 2
      memset ((void *) pagereg, 0, 512 * WORD);
#undef WORD
      /* initialize pagereg.ppa to "quasi-non-MMU". */
      i = 0;
      for (as = 0; as <= 15; as++)
	{
	  int logaddr_hinibble = 0;
	  for (; logaddr_hinibble <= 0xF; logaddr_hinibble++)
	    {
	      pagereg[CODE][as][logaddr_hinibble].ppa = i;
	      pagereg[DATA][as][logaddr_hinibble].ppa = i++;
	    }
	}
      /* Write zeros to 1750 regs */
      memset ((void *) &simreg, 0, sizeof (struct regs));
      /* Reset breakpoint counter */
      n_breakpts = 0;
      /* Reset counter of total instructions executed */
      instcnt = 0;
      /* Reset simulation time tracking */
      total_time_in_us = 0.0;
      /* Do loadfile processing initializations */
      init_load_formats ();
    }
  else
    {
      /*   nix   */
    }
  return (OKAY);
}


static int
si_init (int argc, char *argv[])
{
  return init_simulator (0);
}


static int
si_reset (int argc, char *argv[])
{
  init_cpu ();
  return (OKAY);
}


static int
si_tr (int argc, char *argv[])
{
  uint address;

  if (argc < 2)
    return error ("address missing");

  if (parse_address (argv[1], &address) != OKAY)
    return error ("invalid address syntax (see 'help tr')");

  lprintf ("logical address %s => physical %05lX\n", argv[1], address);
  return (OKAY);
}


/* Display instruction/operand page registers of MMU */
/* syntax:  pagereg <as>    ,where <as> is the Address State (0..15) */
static int
si_page (int argc, char *argv[])
{
  int i, as = simreg.sw & 0x000F;

  if (argc > 1)
    {
      sscanf (argv[1], "%i", &as);
      if (as > 15)
	return error ("Address State out of range (0..15)");
    }
  lprintf ("\n\t  AS%d Instruction:\n\t  ", as);
  for (i = 0; i < 16; i++)
    lprintf ("  %02hX", pagereg[CODE][as][i].ppa);
  lprintf ("\n");
  lprintf ("\t  AS%d Operand:\n\t  ", as);
  for (i = 0; i < 16; i++)
    lprintf ("  %02hX", pagereg[DATA][as][i].ppa);
  lprintf ("\n");
  return (0);
}


/* stuff for the FI command. */

struct term {
  int initval;
  int value;
  int increment;
  bool norestart;
};

#define RNULL (reprec)NULL
#define MAX_SUBNODES 8

typedef struct rep_rec {
  int rep_fact;
  /* rep_fact == 0 : terminal (use u.t)
     rep_fact  > 0 : non_terminal (use u.subnodes) */
  union {
    struct term t;
    struct rep_rec *subnodes[MAX_SUBNODES];
  } u;
} *reprec;

static uint phys_addr;

static void
apply (reprec r)
{
  int i, j;

  if (r->rep_fact == 0)  /* value leaf */
    {
      poke (phys_addr++, (ushort) r->u.t.value);
      r->u.t.value += r->u.t.increment;
      return;
    }
  /* rep_fact > 0 : subnodes */
  for (j = 0; j < MAX_SUBNODES; j++)  /* initialize if need be */
    {
      reprec p = r->u.subnodes[j];
      if (p == RNULL)
	break;
      if (p->rep_fact == 0 && !p->u.t.norestart)
	p->u.t.value = p->u.t.initval;
    }
  for (i = 0; i < r->rep_fact; i++)
    {
      for (j = 0; j < MAX_SUBNODES; j++)
	if (r->u.subnodes[j] == RNULL)
	  break;
	else
	  apply (r->u.subnodes[j]);
    }
}

static char *
get_repfact (char *s, int *num)
{
  *num = 0;
  if (! isdigit (*s))
    {
      if (*s != '(')
        return NULL;
      else
        {
          *num = 1;  /* default when no repeat factor given */
          return s;
        }
    }
  while (isdigit (*s))
    *num = 10 * *num + *s++ - '0';
  if (*s != '(')
    return NULL;
  return s;
}

static char *
find_closing (char *s)
{
  int paren_level = 0;
  if (*s != '(')
    return NULL;
  while (*++s)
    {
      if (*s == '(')
        {
          paren_level++;
        }
      else if (*s == ')')
        {
          if (paren_level == 0)
            return s;
          else
            paren_level--;
        }
    }
  return NULL;
}

char *
get_num (char *s, int *num)
{
  if (! isdigit (*s))
    return NULL;
  *num = 0;
  if (*s == '0' && *(s + 1) == 'x')
    {
      s += 2;
      while (isxdigit (*s))
	*num = (*num << 4) | xtoi (*s++);
    }
  else
    while (isdigit (*s))
      *num = 10 * *num + *s++ - '0';
  return s;
}

static reprec
parse (char **s)
{
  char *p, *open_paren, *close_paren;
  int i, rep_fact;
  reprec r = (reprec) calloc (1, sizeof(struct rep_rec));

  if ((open_paren = get_repfact (*s, &rep_fact)) == NULL)
    {
      *s = get_num (*s, &r->u.t.initval);
      if (*s == NULL)
	return RNULL;
      r->u.t.value = r->u.t.initval;
      p = *s;
      if (*p == '+' || *p == '-')
	{
	  if (! isdigit (*(p + 1)))
	    {
	      r->u.t.increment = (*p == '-') ? -1 : 1;
	      ++*s;
	    }
	  else
	    {
	      *s = get_num (p + 1, &r->u.t.increment);
	      if (*s == NULL)
	        return RNULL;
	      if (*p == '-')
	        r->u.t.increment = -r->u.t.increment;
	    }
	  if (**s == 'n' || **s == 'N')
	    {
	      r->u.t.norestart = TRUE;
	      ++*s;
	    }
	}
      return r;
    }

  r->rep_fact = rep_fact;
  i = 0;
  *s = open_paren;
  if ((close_paren = find_closing (*s)) == NULL)
    {
      error ("missing closing parenthesis in expression");
      return RNULL;
    }
  *close_paren = '\0';
  while (*++*s)
    {
      if (i >= MAX_SUBNODES)
	{
	  error ("value expression list too long");
	  return RNULL;
	}
      if ((r->u.subnodes[i++] = parse (s)) == RNULL)
	return RNULL;
      if (**s != ',')
	break;
    }
  *s = close_paren + 1;
  return r;
}


static int
si_fill (int argc, char *argv[])
{
  reprec r;
  char *expr;

  if (argc < 3)
    return error ("parameter missing");
  if (parse_address (argv[1], &phys_addr) != OKAY)
    return error ("invalid address syntax");
  expr = argv[2];
  if ((r = parse (&expr)) == RNULL)
    return error ("fill expression syntax error");
  apply (r);
  return (OKAY);
}


