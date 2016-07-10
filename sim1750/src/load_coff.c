/*
 * load_coff.c
 *
 * 1750 Simulator loader for GCC-generated executable COFF files
 * 
 * Copyright (c) 1996, Chris Nettleton Software
 *
 * The authors hereby grant permission to use, copy, modify, distribute, 
 * and license this software and its documentation for any purpose, 
 * provided that existing copyright notices are retained in all copies 
 * and that this notice is included verbatim in any distributions. No 
 * written agreement, license, or royalty fee is required for any of the 
 * authorized uses. Modifications to this software may be copyrighted by 
 * their authors and need not follow the licensing terms described here, 
 * provided that the new terms are clearly indicated on the first page 
 * of each file where they apply. 
 *
 * $Log$
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "peekpoke.h"
#include "loadfile.h"
#include "arch.h"
#include "status.h"
#include "utils.h"


static int optf = 0;   /* print file header */
static int opts = 1;   /* print section headers */
static int optz = 0;   /* print strings */


/*
 * This the coff file header. 
 * See 'Understanding and Using COFF', page 22. 
 */
/* typedef unsigned char byte; */

struct filehdr {
  byte f_magic [2];    /* magic number             */
  byte f_nscns [2];    /* number of sections       */
  byte f_timdat [4];   /* time & date stamp        */
  byte f_symptr [4];   /* file pointer to symtab   */
  byte f_nsyms [4];    /* number of symtab entries */
  byte f_opthdr [2];   /* sizeof (optional hdr)    */
  byte f_flags [2];    /* flags                    */
} file_header;

#define FILHDR struct filehdr
#define FILHSZ sizeof (FILHDR)

/*
 * MIL-STD-1750 Flags (top 4 bits)
 * F_M1750B1       1750B option 1 -  
 * F_M1750B2       1750B option 2 -  
 * F_M1750B3       1750B option 3 - 
 * F_M1750MMU      Memory management unit required 
 */
#define F_M1750B1        (0x1000)
#define F_M1750B2        (0x2000)
#define F_M1750B3        (0x4000)
#define F_M1750MMU       (0x8000)

/* 
 * Bits for f_flags:
 *  F_RELFLG        relocation info stripped from file
 *  F_EXEC          file is executable (no unresolved external references)
 *  F_LNNO          line numbers stripped from file
 *  F_LSYMS         local symbols stripped from file
 *  F_AR16WR        file is 16-bit little-endian
 *  F_AR32WR        file is 32-bit little-endian
 *  F_AR32W         file is 32-bit big-endian
 *  F_DYNLOAD       rs/6000 aix: dynamically loadable w/imports & exports
 *  F_SHROBJ        rs/6000 aix: file is a shared object
 *  F_DLL           PE format DLL
 */

#define F_RELFLG    (0x0001)
#define F_EXEC      (0x0002)
#define F_LNNO      (0x0004)
#define F_LSYMS     (0x0008)
#define F_AR16WR    (0x0080)
#define F_AR32WR    (0x0100)
#define F_AR32W     (0x0200)
#define F_DYNLOAD   (0x1000)
#define F_SHROBJ    (0x2000)
#define F_DLL       (0x2000)

/*
 * This is the optional header
 * See 'Understanding and Using COFF', page 27. 
 */
typedef struct 
{
  byte magic [2];        /* type of file                          */
  byte vstamp [2];       /* version stamp                         */
  byte tsize [4];        /* text size in bytes, padded to FW bdry */
  byte dsize [4];        /* initialized data      ..     ..   ..  */
  byte bsize [4];        /* uninitialized data    ..     ..   ..  */
  byte entry [4];        /* entry pt.                             */
  byte text_start [4];   /* base of text used for this file       */
  byte data_start [4];   /* base of data used for this file       */
} AOUTHDR;
AOUTHDR aout_header;

#define AOUTSZ 28
#define AOUTHDRSZ 28

/* 
 * This is a section header
 * See 'Understanding and Using COFF', page 36. 
 */
struct scnhdr {
  byte s_name [8];    /* section name                     */
  byte s_paddr [4];   /* physical address, aliased s_nlib */
  byte s_vaddr [4];   /* virtual address                  */
  byte s_size [4];    /* section size                     */
  byte s_scnptr [4];  /* file ptr to raw data for section */
  byte s_relptr [4];  /* file ptr to relocation           */
  byte s_lnnoptr [4]; /* file ptr to line numbers         */
  byte s_nreloc [2];  /* number of relocation entries     */
  byte s_nlnno [2];   /* number of line number entries    */
  byte s_flags [4];   /* flags                            */
} sec_header;

/*
 * names of "special" sections
 */
#define _TEXT    ".text"
#define _DATA    ".data"
#define _RODATA  ".rodata"
#define _BSS     ".bss"
#define _COMMENT ".comment"

#define SCNHDR  struct scnhdr
#define SCNHSZ  sizeof (SCNHDR)

/*
 * The relocation record
 * See 'Understanding and Using COFF', page 40. 
 */
struct reloc {
  byte r_vaddr [4];     /* address of reference */
  byte r_symndx [4];    /* index into symbol table */
  byte r_type [2];      /* relocation type */
#ifdef M1750_COFF_OFFSET
  byte r_offset[4];
#endif
} re;

#define RELOC struct reloc
#define RELSZ (sizeof (RELOC))

/*
 * Type of a symbol, in low N bits of the word
 */
#define T_NULL          0
#define T_VOID          1       /* function argument (only used by compiler) */
#define T_CHAR          2       /* character             */
#define T_SHORT         3       /* short integer         */
#define T_INT           4       /* integer               */
#define T_LONG          5       /* long integer          */
#define T_FLOAT         6       /* floating point        */
#define T_DOUBLE        7       /* double word           */
#define T_STRUCT        8       /* structure             */
#define T_UNION         9       /* union                 */
#define T_ENUM          10      /* enumeration           */
#define T_MOE           11      /* member of enumeration */
#define T_UCHAR         12      /* unsigned character    */
#define T_USHORT        13      /* unsigned short        */
#define T_UINT          14      /* unsigned integer      */
#define T_ULONG         15      /* unsigned long         */
#define T_LNGDBL        16      /* long double           */

/*
 * derived types, in n_type
 */
#define DT_NON          (0)     /* no derived type */
#define DT_PTR          (1)     /* pointer */
#define DT_FCN          (2)     /* function */
#define DT_ARY          (3)     /* array */

#define BTYPE(x)        ((x) & N_BTMASK)

#define ISPTR(x)        (((x) & N_TMASK) == (DT_PTR << N_BTSHFT))
#define ISFCN(x)        (((x) & N_TMASK) == (DT_FCN << N_BTSHFT))
#define ISARY(x)        (((x) & N_TMASK) == (DT_ARY << N_BTSHFT))
#define ISTAG(x)        ((x)==C_STRTAG||(x)==C_UNTAG||(x)==C_ENTAG)
#define DECREF(x)       ((((x)>>N_TSHIFT)&~N_BTMASK)|((x)&N_BTMASK))


/*
 * Storage classes
 */
#define C_NULL          0
#define C_AUTO          1       /* automatic variable           */
#define C_EXT           2       /* external symbol              */
#define C_STAT          3       /* static                       */
#define C_REG           4       /* register variable            */
#define C_EXTDEF        5       /* external definition          */
#define C_LABEL         6       /* label                        */
#define C_ULABEL        7       /* undefined label              */
#define C_MOS           8       /* member of structure          */
#define C_ARG           9       /* function argument            */
#define C_STRTAG        10      /* structure tag                */
#define C_MOU           11      /* member of union              */
#define C_UNTAG         12      /* union tag                    */
#define C_TPDEF         13      /* type definition              */
#define C_USTATIC       14      /* undefined static             */
#define C_ENTAG         15      /* enumeration tag              */
#define C_MOE           16      /* member of enumeration        */
#define C_REGPARM       17      /* register parameter           */
#define C_FIELD         18      /* bit field                    */
#define C_AUTOARG       19      /* auto argument                */
#define C_LASTENT       20      /* dummy entry (end of block)   */

/*
 * Symbol table entries 
 */
#define E_SYMNMLEN  8   /* # characters in a symbol name         */
#define E_FILNMLEN 14   /* # characters in a file name           */
#define E_DIMNUM    4   /* # array dimensions in auxiliary entry */

struct syment 
{
  union {
    byte e_name [E_SYMNMLEN];
    struct {
      byte e_zeroes [4];
      byte e_offset [4];
    } e;
  } e;
  byte e_value [4];
  byte e_scnum [2];
  byte e_type [2];
  byte e_sclass [1];
  byte e_numaux [1];
} se;

struct internal_syment 
{
  char e_name [E_SYMNMLEN];
  struct {
    long e_zeroes;
    long e_offset;
  } e;
  long e_value;
  short e_scnum;
  unsigned short e_type;
  char e_sclass;
  char e_numaux;
} *syms = NULL;


#define N_BTMASK  (017)
#define N_TMASK   (060)
#define N_BTSHFT  (4)
#define N_TSHIFT  (2)
  
union auxent {
  struct {
    byte x_tagndx[4];    /* str, un, or enum tag indx */
    union {
      struct {
        byte  x_lnno[2]; /* declaration line number */
        byte  x_size[2]; /* str/union/array size */
      } x_lnsz;
      byte x_fsize[4];   /* size of function */
    } x_misc;
    union {
      struct {           /* if ISFCN, tag, or .bb */
        byte x_lnnoptr[4];  /* ptr to fcn line # */
        byte x_endndx[4];   /* entry ndx past block end */
      } x_fcn;
      struct {           /* if ISARY, up to 4 dimen. */
        byte x_dimen[E_DIMNUM][2];
      } x_ary;
    } x_fcnary;
    byte x_tvndx[2];     /* tv index */
  } x_sym;

  union {
    byte x_fname[E_FILNMLEN];
    struct {
      byte x_zeroes[4];
      byte x_offset[4];
    } x_n;
  } x_file;

  struct {
    byte x_scnlen[4];    /* section length */
    byte x_nreloc[2];    /* # relocation entries */
    byte x_nlinno[2];    /* # line numbers */
  } x_scn;

  struct {
    byte x_tvfill[4];    /* tv fill value */
    byte x_tvlen[2];     /* length of .tv */
    byte x_tvran[2][2];  /* tv range */
  } x_tv;         /* info about .tv section (in auxent of symbol .tv)) */
} ae;

#define SYMENT struct syment
#define SYMESZ 18      
#define AUXENT union auxent
#define AUXESZ 18


/* Contents of file header
 */
static ushort f_magic; 
static ushort f_nscns;
static long f_timdat;          
static long f_symptr;        
static long f_nsyms;       
static ushort f_opthdr;  
static ushort f_flags; 

/* Contents of the optional header
 */
static short magic;
static short vstamp;
static long tsize;
static long dsize;
static long bsize;
static long entry;
static long text_start;
static long data_start;

/* The string table
*/
static char *str_tab = NULL;
static int str_length = 0;

/* Contents of most recent section header
 */
static char   s_name [9];
static ulong  s_paddr;
static ulong  s_vaddr;
static ulong  s_size; 
static ulong  s_scnptr;
static ulong  s_relptr;
static ulong  s_lnnoptr;
static ushort s_nreloc;
static ushort s_nlnno; 
static ulong  s_flags;

/* Contents of most recent reloc record
 */
static ulong r_vaddr;
static ulong r_symndx;
static ushort r_type;
#ifdef M1750_COFF_OFFSET
static ulong r_offset;
#endif

/* Contents of the most recent line number record
*/
static ulong l_symndx; 
static ulong l_paddr; 
static ushort l_lnno; 


static void 
get_file_header ()
{
  f_magic  = ((ushort) file_header.f_magic  [0] << 8) +
              (ushort) file_header.f_magic  [1];
  f_nscns  = ((ushort) file_header.f_nscns  [0] << 8) +
              (ushort) file_header.f_nscns  [1];

  f_timdat = ((ulong) file_header.f_timdat [0] << 24) + 
             ((ulong) file_header.f_timdat [1] << 16) +
             ((ulong) file_header.f_timdat [2] << 8) + 
             ((ulong) file_header.f_timdat [3]);

  f_symptr = ((ulong) file_header.f_symptr [0] << 24) + 
             ((ulong) file_header.f_symptr [1] << 16) +
             ((ulong) file_header.f_symptr [2] << 8) + 
             ((ulong) file_header.f_symptr [3]);

  f_nsyms  = ((ulong) file_header.f_nsyms  [0] << 24) + 
             ((ulong) file_header.f_nsyms  [1] << 16) +
             ((ulong) file_header.f_nsyms  [2] << 8) + 
             ((ulong) file_header.f_nsyms  [3]);

  f_opthdr = ((ushort) file_header.f_opthdr [0] << 8) +
              (ushort) file_header.f_opthdr [1];
  f_flags  = ((ushort) file_header.f_flags  [0] << 8) +
              (ushort) file_header.f_flags  [1];
}

static void 
dump_file_header ()
{
  printf ("----File-Header---------------------------------------------\n");
  printf ("Magic number (in octal)  = 0%o\n",    f_magic);
  printf ("Number of sections       = %d\n",     f_nscns);
  printf ("Time & date stamp        = %s", ctime ((unsigned long *)&f_timdat));
  printf ("File pointer to symtab   = 0x%08lX\n", f_symptr);
  printf ("Number of symtab entries = %ld\n",     f_nsyms);
  printf ("Sizeof (optional hdr)    = %d\n",     f_opthdr);
  printf ("Flags                    = 0x%04X\n", f_flags);

  if (f_flags) 
    {
      printf ("Known flags: ");
      if (f_flags & F_M1750B1)
        printf ("M1750B1 ");
      if (f_flags & F_M1750B2)
        printf ("M1750B2 ");
      if (f_flags & F_M1750B3)
        printf ("M1750B3 ");
      if (f_flags & F_M1750MMU)
        printf ("M1750MMU ");
      if (f_flags & F_RELFLG)
        printf ("RELFLG ");
      if (f_flags & F_EXEC)   
        printf ("EXEC ");
      if (f_flags & F_LNNO)  
        printf ("LNNO ");
      if (f_flags & F_LSYMS)
        printf ("LSYMS ");
      if (f_flags & F_AR16WR) 
        printf ("AR16W ");
      if (f_flags & F_AR32WR)
        printf ("AR16WR ");
      if (f_flags & F_AR32W)     
        printf ("AR32W ");
      if (f_flags & F_DYNLOAD)   
        printf ("DYNLOAD ");
      if (f_flags & F_SHROBJ)   
        printf ("SHROBJ ");
      if (f_flags & F_DLL)
        printf ("DLL");
      printf ("\n");
    }
}

static void
get_opt_header ()
{
  magic  = ((ushort) aout_header.magic [0] << 8) +
            (ushort) aout_header.vstamp [1];
  vstamp = ((ushort) aout_header.magic [0] << 8) +
            (ushort) aout_header.vstamp [1];

  tsize  = ((ulong) aout_header.tsize [0] << 24) + 
           ((ulong) aout_header.tsize [1] << 16) +
           ((ulong) aout_header.tsize [2] << 8) + 
           ((ulong) aout_header.tsize [3]);

  dsize  = ((ulong) aout_header.dsize [0] << 24) + 
           ((ulong) aout_header.dsize [1] << 16) +
           ((ulong) aout_header.dsize [2] << 8) + 
           ((ulong) aout_header.dsize [3]);

  bsize  = ((ulong) aout_header.bsize [0] << 24) + 
           ((ulong) aout_header.bsize [1] << 16) +
           ((ulong) aout_header.bsize [2] << 8) + 
           ((ulong) aout_header.bsize [3]);

  entry  = ((ulong) aout_header.entry [0] << 24) + 
           ((ulong) aout_header.entry [1] << 16) +
           ((ulong) aout_header.entry [2] << 8) + 
           ((ulong) aout_header.entry [3]);

  text_start = ((ulong) aout_header.text_start [0] << 24) + 
               ((ulong) aout_header.text_start [1] << 16) +
               ((ulong) aout_header.text_start [2] << 8) + 
               ((ulong) aout_header.text_start [3]);

  data_start = ((ulong) aout_header.data_start [0] << 24) + 
               ((ulong) aout_header.data_start [1] << 16) +
               ((ulong) aout_header.data_start [2] << 8) + 
               ((ulong) aout_header.data_start [3]);
}

static void 
dump_opt_header ()
{
  printf ("----Optional-header------------------------------------------\n");
  printf ("Magic number             = %o\n", magic);
  printf ("Version stamp            = %X\n", vstamp);
  printf ("Size of first .text      = %ld\n", tsize);
  printf ("Size of first .data      = %ld\n", dsize);
  printf ("Size of first .bss       = %ld\n", bsize);
  printf ("Entry point              = 0x%08lX\n", entry);
  printf ("Start of text            = 0x%08lX\n", text_start);
  printf ("Start of data            = 0x%08lX\n", data_start);
}

static void 
get_sec_header ()
{
  memset (s_name, 0, 9);
  memcpy (s_name, sec_header.s_name,8);
   
  s_paddr   = ((ulong) sec_header.s_paddr [0] << 24) + 
              ((ulong) sec_header.s_paddr [1] << 16) +
              ((ulong) sec_header.s_paddr [2] << 8) + 
              ((ulong) sec_header.s_paddr [3]);
    
  s_vaddr   = ((ulong) sec_header.s_vaddr [0] << 24) + 
              ((ulong) sec_header.s_vaddr [1] << 16) +
              ((ulong) sec_header.s_vaddr [2] << 8) + 
              ((ulong) sec_header.s_vaddr [3]);
    
  s_size    = ((ulong) sec_header.s_size  [0] << 24) + 
              ((ulong) sec_header.s_size  [1] << 16) +
              ((ulong) sec_header.s_size  [2] << 8) + 
              ((ulong) sec_header.s_size  [3]);
    
  s_scnptr  = ((ulong) sec_header.s_scnptr [0] << 24) + 
              ((ulong) sec_header.s_scnptr [1] << 16) +
              ((ulong) sec_header.s_scnptr [2] << 8) + 
              ((ulong) sec_header.s_scnptr [3]);
    
  s_relptr  = ((ulong) sec_header.s_relptr [0] << 24) + 
              ((ulong) sec_header.s_relptr [1] << 16) +
              ((ulong) sec_header.s_relptr [2] << 8) + 
              ((ulong) sec_header.s_relptr [3]);
    
  s_nreloc  = ((ushort) sec_header.s_nreloc [0] << 8) +
               (ushort) sec_header.s_nreloc [1];
  s_nlnno   = ((ushort) sec_header.s_nlnno  [0] << 8) +
               (ushort) sec_header.s_nlnno  [1];

  s_lnnoptr = ((ulong) sec_header.s_lnnoptr [0] << 24) + 
              ((ulong) sec_header.s_lnnoptr [1] << 16) +
              ((ulong) sec_header.s_lnnoptr [2] << 8) + 
              ((ulong) sec_header.s_lnnoptr [3]);
    
  s_flags   = ((ulong) sec_header.s_flags [0] << 24) + 
              ((ulong) sec_header.s_flags [1] << 16) +
              ((ulong) sec_header.s_flags [2] << 8) + 
              ((ulong) sec_header.s_flags [3]);
}

    
static void 
dump_sec_header (int sec)
{
  printf ("----Section-%d-header----------------------------------------\n", sec);
  printf ("Section name             = %s\n", s_name);
  printf ("Physical address         = %08lX\n", s_paddr);    
  printf ("Virtual address          = %08lX\n", s_vaddr);    
  printf ("Section size             = %08lX (%ld)\n", s_size, s_size);     
}

static void
summarize_sec_header (int sec)
{
  printf ("%08lX %05lX (%ld) %s\n", s_paddr >> 1, s_size >> 1, s_size >> 1, s_name);
}

void
get_strings (FILE *input_file)
{
  int strings;
  unsigned char length [4];

  strings = f_symptr + SYMESZ * f_nsyms;

  /* Note, the COFF book says a COFF file with no strings
     has a lenght field set to zero. GCC seems to omit the
     length field */
  fseek (input_file, strings, SEEK_SET);
  if (fread (length, 4, 1, input_file) < 1)
    {
      str_length = 0;
    }
  else
    {
      str_length = ((unsigned) length [0] << 24) + 
                   ((unsigned) length [1] << 16) +
                   ((unsigned) length [2] << 8) + 
                   ((unsigned) length [3]);
    }

  if (str_length < 0 || str_length > 10000)
    problem ("load_coff (get_strings): string table bad size");

  if (str_length > 0)
    {
      str_length -= 4;
      str_tab = (char *)malloc (str_length);
      fseek (input_file, strings + 4, SEEK_SET);
      fread (str_tab, str_length, 1, input_file);
    }
  else
    {
      str_tab = NULL;
    }
}

static void 
dump_strings ()
{
  printf ("----The-string-table-----------------------------------------\n");

  if (str_tab != NULL)
    {
      int length = str_length;
      char *ptr = str_tab;
      do 
         {
           printf ("%08X : %s\n", (ptr - str_tab), ptr);
           while (*ptr++ != '\0')
             ;
         }  
      while (ptr < (str_tab + length));
    }

}

static void 
get_se (struct internal_syment *ise)
{
  int i;

  for (i = 0; i < E_SYMNMLEN; i++)
    ise->e_name [i] = se.e.e_name [i];

  ise->e.e_zeroes  = ((ulong) se.e.e.e_zeroes [0] << 24) + 
                     ((ulong) se.e.e.e_zeroes [1] << 16) +
                     ((ulong) se.e.e.e_zeroes [2] << 8) + 
                     ((ulong) se.e.e.e_zeroes [3]);

  ise->e.e_offset  = ((ulong) se.e.e.e_offset [0] << 24) + 
                     ((ulong) se.e.e.e_offset [1] << 16) +
                     ((ulong) se.e.e.e_offset [2] << 8) + 
                     ((ulong) se.e.e.e_offset [3]);

  ise->e_value     = ((ulong) se.e_value  [0] << 24) + 
                     ((ulong) se.e_value  [1] << 16) +
                     ((ulong) se.e_value  [2] << 8) + 
                     ((ulong) se.e_value  [3]);

  ise->e_scnum   = ((ushort) se.e_scnum  [0] << 8) +
                    (ushort) se.e_scnum  [1];
  ise->e_type    = ((ushort) se.e_type   [0] << 8) +
                    (ushort) se.e_type   [1];

  ise->e_sclass = se.e_sclass [0];
  ise->e_numaux = se.e_numaux [0];
}


static void 
get_dst (FILE *input_file)
{
  /* Read the debug symbol table into memory
   */
  int i;
  int j;

  /* Allocate space for f_nsyms */
  syms = (struct internal_syment *)malloc (sizeof (struct internal_syment) * f_nsyms);

  fseek (input_file, f_symptr, SEEK_SET);
  i = 0;
  while (i < f_nsyms)
    {
      fread (&se, SYMESZ, 1, input_file);
      get_se (&syms [i]);
      i++;
      for (j = 0; j < syms [i].e_numaux; j++)
        {
          fread (&ae, AUXESZ, 1, input_file);
/*          get_ae (); */
          i++;
        }
    }

}

static void 
print_se (struct internal_syment *se)
{
  /* pretty print a symbol table entry */

  int i;
  char *class;

  /* section numbers < 1 are special */
  if (se->e_scnum < 1)
    return;

  /* skip names begining with a dot */
  if (se->e.e_zeroes && se->e_name [0] == '.')
    return; 

  /* filter interesting classes */ 
  switch (se->e_sclass)
    {
    case C_NULL:    class = "        "; break;
    case C_EXT:     class = " extern "; break; 
    case C_STAT:    class = " static "; break; 
    default:
      return;
    }

  /* print the section number */
  printf ("%3d %s ", se->e_scnum, class);

  switch (se->e_type & 0xf)
    {
    case T_NULL:   printf ("        "); break;
    case T_CHAR:   printf ("char    "); break;
    case T_SHORT:  printf ("short   "); break;
    case T_INT:    printf ("int     "); break;
    case T_LONG:   printf ("long    "); break;
    case T_FLOAT:  printf ("float   "); break;
    case T_DOUBLE: printf ("double  "); break;
    case T_STRUCT: printf ("struct  "); break;
    case T_UNION:  printf ("union   "); break;
    case T_ENUM:   printf ("enum    "); break;
    case T_MOE:    printf ("member  "); break;
    case T_UCHAR:  printf ("uchar   "); break;
    case T_USHORT: printf ("ushort  "); break;
    case T_ULONG:  printf ("ulong   "); break;
    default:       printf ("%6d  ", se->e_type); break;
    }
 
  /* print the value */
  printf ("%08lX  ", se->e_value >> 1);

  if (ISPTR (se->e_type))
    printf ("*");

  /* print the name of the symbol */
  if (se->e.e_zeroes)
    {
      for (i = 0; i < 8; i++)
        {
          if ((se->e_name [i] > 0x1f) && (se->e_name [i] < 0x7f))
            printf ("%c", se->e_name [i]);
          else
            printf (" ");
        }
    }
  else
    printf ("%s", &str_tab [(se->e.e_offset - 4)]);

  if (ISFCN (se->e_type))
    printf (" ()");
  else if (ISARY (se->e_type))
    printf (" []");

  printf ("\n");
}

static void
pretty_print_dst ()
{
  int i;

  /*         4  static          00008141  floating_overflow_old    */
  printf ("-sec--class----type---address--name--------------------------\n");

  i = 0;
  while (i < f_nsyms)
    {
      print_se (&syms [i]);
      i++;
    }
}


static int 
process_file (FILE* input_file)
{
  /*
   * Read the file header and any sections
   */
  int sec;
  long offset;  /* offset in file */

  if (fread (&file_header, sizeof(struct filehdr), 1, input_file) != 1) {
    return error ("canot read file header");
  }

  get_file_header ();
  offset = FILHSZ;

  if (f_magic != 0333 && f_magic != 0334)
    {
      return error ("File format not recognized");
    }

  if (f_opthdr > 0)
    {
      fread (&aout_header, f_opthdr, 1, input_file);
      get_opt_header ();
      offset += f_opthdr;

      if (verbose)
        printf ("Entry point = 0x%08lX\n", entry >> 1);
      simreg.ic = entry >> 1;
    }

  get_strings (input_file);
  if (optz)
    dump_strings ();

  get_dst (input_file);

  if (optf)
    dump_file_header ();

  for (sec = 0; sec < f_nscns; sec++)
    {
      fseek (input_file, offset, 0);
      if (fread (&sec_header, SCNHSZ, 1, input_file) != 1)
        {
          return error ("cannot read section header");
        }

      get_sec_header ();
      offset += SCNHSZ;

      if (opts)
        summarize_sec_header (sec);

      /* load section */
      if (1)
	{
	  unsigned char *raw_data = (unsigned char *)malloc (s_size);
	  int j;
	  long address = s_paddr >> 1;

	  fseek (input_file, s_scnptr, 0);
	  fread (raw_data, s_size, 1, input_file);

	  j = 0;
	  while (j < s_size)
	    {
	      /* poke a word */
	      poke (address++, ((ushort) raw_data [j] << 8)
                              + (ushort) raw_data [j + 1]);
	      j += 2;
	    }
	  free (raw_data);
	}

      if (s_nreloc > 0)
	{
	  return error ("File contains relocations");
	}

    }

  return OKAY;
}


static int
load_coff (char *filename)
{
  FILE *loadfile;
  /* char lline [strlen (filename) + 5]; */
  char * lline;
  int retval;

  /* remove any previous COFF info */
  if (syms != NULL)
    free (syms);

  if (str_tab != NULL)
    free (str_tab);

  /* get space for extended file name */
  lline = (char *)malloc(strlen(filename) + 5);
  if (lline == (char *) NULL)
    {
      return error("Cannot malloc %d characters", strlen(filename)
                 + 5);
    }

  if ((loadfile = fopen (filename, "rb")) == (FILE *) 0)
    {
      strcat (strcpy (lline, filename), ".cof");
      if ((loadfile = fopen (lline, "rb")) == (FILE *) 0)
        {
           free(lline);
           return error ("cannot open COFF file '%s'", filename);
        }
    }

  retval = process_file (loadfile);
  free (lline);
  fclose (loadfile);
  return retval;
}


int
si_lcf (int argc, char *argv[])
{
  char *filename = argv [1];

  opts = verbose;

  if (argc <= 1)
    return error ("filename missing");
  loadfile_type = COFF;
  if (*filename == '"')
    {
      *filename++ = '\0';
      *(filename + strlen (filename) - 1) = '\0';
    }

  return load_coff (filename);
}


long
find_coff_address (char *id)
{
  /* Given a symbol id, search the symbol table for an
     an entry with this name, and return the value.
     Return -1 if not found.
   */
  int i;

  i = 0;
  while (i < f_nsyms)
    {
      char short_id [9];
      char *key;
      struct internal_syment *se = &syms [i];
      int j;

      if (se->e_scnum >= 1 &&
          (se->e_sclass == C_NULL ||
           se->e_sclass == C_EXT ||
           se->e_sclass == C_STAT ||
           se->e_sclass == C_AUTO))
        {
          if (se->e.e_zeroes)
            {
              for (j = 0; j < 8; j++)
                short_id [j] = se->e_name [j];
    
              short_id [8] = '\0';
              key = short_id;
            }
          else
            key = &str_tab [(se->e.e_offset - 4)];

          if (key [0] != '.' && strcmp (key, id) == 0)
            return se->e_value >> 1;
        }

      i++;
    }

  return -1;
}


int
display_coff_symbols ()
{
  /* dump_dst (); */
  pretty_print_dst ();

  return OKAY;
}


