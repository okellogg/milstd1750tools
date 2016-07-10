/* tekops.h  --  Special purpose exports of tekops.c
                 General purpose exports are mentioned in loadfile.h  */

extern void init_tekops ();
extern long find_tek_address (char *labelname);
extern char *find_tek_label (unsigned long address);
extern int  display_tek_symbols ();

