/* tekhex.h  --  exports of tekhex.c */

extern int    check_tekline (char *line);
extern void   emit_tekword (unsigned long startaddr, unsigned short word);
extern void   finish_tekline ();
extern int    create_tekfile (char *outfname);
extern void   close_tekfile (unsigned long transfer_address);

