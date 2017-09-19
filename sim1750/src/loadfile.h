/* loadfile.h  --  exports of loadfile.c */

#ifndef _LOADFILE_H
#define _LOADFILE_H

extern char *find_labelname (unsigned int address);
extern int  find_address (char *labelname);
extern void init_load_formats ();
extern int  si_dispsym (int argc, char *argv[]);
extern int  si_prolo (int argc, char *argv[]);
extern int  si_pslo  (int argc, char *argv[]);
extern int  si_lo    (int argc, char *argv[]);	/* tekops.c */
extern int  si_save  (int argc, char *argv[]);	/* tekops.c */
extern int  si_lcf (int argc, char *argv[]);	/* load_coff.c */
extern int  si_tld (int argc, char *argv[]);	/* ldm.c */
extern int  si_xtc (int argc, char *argv[]);	/* ldm.c */

typedef enum { TLD_LDM, XTC_LDM, TEK_HEX, COFF, NONE} loadfile_t;

extern loadfile_t loadfile_type;

#endif

