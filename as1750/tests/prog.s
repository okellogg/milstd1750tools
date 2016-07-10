        MODULE    PROG
$ISECT$ CSECT     instruction,read_only,rom
        EXPORT    $PROG
        IMPORT    INIT_$AAC2DUN002
        IMPORT    INIT_$AAC2DUM002
        IMPORT    INIT_$AAC2DUM001
        IMPORT    INIT_$AAC2ADA001
        IMPORT    DUMMY_MAIN_$AAC2DUO002
$DATA$  CSECT     operand,read_write,ram
D       res	10
$CONS$  CSECT     operand,read_only,rom
C       data	1
$ISECT$ CSECT     instruction,read_only,rom
$PROG   equ *
        SJS       R15,INIT_$AAC2ADA001
        SJS       R15,INIT_$AAC2DUM001
        SJS       R15,INIT_$AAC2DUM002
        SJS       R15,INIT_$AAC2DUN002
        SJS       R15,DUMMY_MAIN_$AAC2DUO002
        URS       R15
        END

