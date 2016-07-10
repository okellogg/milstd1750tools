/* xiodef.h -- Mil-Std-1750 XIO related definitions */

#define  X_SMK     0x2000
#define  X_CLIR    0x2001
#define  X_ENBL    0x2002
#define  X_DSBL    0x2003
#define  X_RPI     0x2004
#define  X_SPI     0x2005
#define  X_OD      0x2008
#define  X_RNS     0x200A
#define  X_WSW     0x200E

#define  X_CO      0x4000
#define  X_CLC     0x4001
#define  X_MPEN    0x4003
#define  X_ESUR    0x4004
#define  X_DSUR    0x4005
#define  X_DMAE    0x4006
#define  X_DMAD    0x4007
#define  X_TAS     0x4008
#define  X_TAH     0x4009
#define  X_OTA     0x400A
#define  X_GO      0x400B
#define  X_TBS     0x400C
#define  X_TBH     0x400D
#define  X_OTB     0x400E

#define  X_LMP     0x5000
#define  X_WIPR    0x5100
#define  X_WOPR    0x5200

#define  X_RMK     0xA000
#define  X_RIC1    0xA001
#define  X_RIC2    0xA002
#define  X_RPIR    0xA004
#define  X_RDOR    0xA008
#define  X_RDI     0xA009
#define  X_TPIO    0xA00B
#define  X_RMFS    0xA00D
#define  X_RSW     0xA00E
#define  X_RCFR    0xA00F

#define  X_CI      0xC000
#define  X_RCS     0xC001
#define  X_ITA     0xC00A
#define  X_ITB     0xC00E

#define  X_RMP     0xD000
#define  X_RIPR    0xD100
#define  X_ROPR    0xD200


struct xioinf {
		unsigned short value;
		char *name;
	      };

extern const struct xioinf xio[];  /* see file xiodef.c */

