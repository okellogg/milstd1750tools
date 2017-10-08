CC = gcc

CFLAGS =  -g -Og -Wall -DAS1750 -DHAVE_UTYPES -DSTRCASECMP -DSTRNCASECMP
#		Define -DHAVE_UTYPES if <sys/types.h> defines ushort/uint
# 		Define the STR... stuff only if <string.h> has these

LDFLAGS = -g -lm

SRC =	  utils.c mexpr.c syms.c tekhex.c tldldm.c as1750.c main.c flt1750.c
OBJ =	  $(SRC:%.c=%.o)

# Make the program
as1750: ${OBJ}
	${CC} -o as1750 ${OBJ} ${LDFLAGS}


# Dependencies
flt1750.o:  flt1750.c
utils.o:    utils.c    type.h utils.h
mexpr.o:    mexpr.c    type.h utils.h macsym.h
tekhex.o:   tekhex.c   type.h utils.h tekhex.h
tldldm.o:   tldldm.c   type.h utils.h tldldm.h
syms.o:     syms.c     type.h syms.h common.h
as1750.o:   as1750.c   type.h syms.h common.h
main.o:     main.c     type.h syms.h common.h version.h

# Cleaning up
clean:
	rm -f ${OBJ} as1750

