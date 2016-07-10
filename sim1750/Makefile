#############################################################################
#                                                                           #
#                Makefile for 1750 Simulator (UNIX version)                 #
#                                                                           #
#############################################################################

CC=      gcc

# Some playing around with CFLAGS may be needed for your host system

## CFLAGS for Sun/Solaris with GNU readline
# CFLAGS= -D__STDC__=0 -DSTRDUP -DSTRNCASECMP -DLONGLONG  # -DGNU_READLINE
CFLAGS= -DSTRDUP -DSTRNCASECMP -DLONGLONG  # -DGNU_READLINE

# CFLAGS for Linux
# CFLAGS= -DSTRDUP -DSTRNCASECMP  # -DLONGLONG


PROJ_DIR=.
SRC=$(PROJ_DIR)/src
OBJ=$(PROJ_DIR)/obj

OBJECTS= $(OBJ)/arith.o		\
	 $(OBJ)/break.o		\
	 $(OBJ)/cmd.o		\
	 $(OBJ)/cpu.o		\
	 $(OBJ)/dism1750.o	\
	 $(OBJ)/do_xio.o	\
	 $(OBJ)/exec.o		\
	 $(OBJ)/flt1750.o	\
	 $(OBJ)/ldm.o		\
	 $(OBJ)/lic.o		\
	 $(OBJ)/loadfile.o	\
	 $(OBJ)/main.o		\
	 $(OBJ)/phys_mem.o	\
	 $(OBJ)/peekpoke.o	\
	 $(OBJ)/sdisasm.o	\
	 $(OBJ)/smemacc.o	\
	 $(OBJ)/status.o	\
	 $(OBJ)/tekhex.o	\
	 $(OBJ)/tekops.o	\
	 $(OBJ)/utils.o		\
	 $(OBJ)/load_coff.o	\
	 $(OBJ)/xiodef.o


SOURCES= $(OBJECTS:$(OBJ).o=$(SRC).c)

sim1750: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROJ_DIR)/sim1750 $(OBJECTS) -lm
#	$(CC) $(CFLAGS) -o $(PROJ_DIR)/sim1750 $(OBJECTS) -lm -lreadline -ltermcap

all:
	@for i in $(OBJECTS:$(OBJ).o=$(SRC).c); do \
		( touch $$i )          \
				    done
	-@echo "all sources touched, now rerun MAKE for desired target"

profile: $(OBJECTS)
	$(CC) -p -o $(PROJ_DIR)/sim $(OBJECTS)

debug:
	@$(MAKE) "CFLAGS= -g -Wall $(CFLAGS)"

opt:
	@$(MAKE) "CFLAGS= -O $(CFLAGS)"

clean:
	rm $(OBJ)/*.o $(PROJ_DIR)/sim1750


#  now dependencies of objects from sources

$(OBJ)/arith.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/flt1750.h $(SRC)/arith.c
	$(CC) -c $(CFLAGS) $(SRC)/arith.c	-o $(OBJ)/arith.o

$(OBJ)/break.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/utils.h \
	  $(SRC)/break.h $(SRC)/type.h $(SRC)/break.c
	$(CC) -c $(CFLAGS) $(SRC)/break.c	-o $(OBJ)/break.o

$(OBJ)/cmd.o: $(SRC)/utils.h $(SRC)/status.h \
	  $(SRC)/loadfile.h $(SRC)/tekops.h $(SRC)/phys_mem.h $(SRC)/break.h \
	  $(SRC)/exec.h $(SRC)/cmd.c
	$(CC) -c $(CFLAGS) $(SRC)/cmd.c	-o $(OBJ)/cmd.o

$(OBJ)/cpu.o: $(SRC)/xiodef.h $(SRC)/status.h \
	  $(SRC)/stime.h $(SRC)/flt1750.h $(SRC)/phys_mem.h $(SRC)/break.h \
	  $(SRC)/arith.h $(SRC)/cpu.c
	$(CC) -c $(CFLAGS) $(SRC)/cpu.c	-o $(OBJ)/cpu.o

$(OBJ)/dism1750.o: $(SRC)/type.h $(SRC)/xiodef.h \
	  $(SRC)/dism1750.c
	$(CC) -c $(CFLAGS) $(SRC)/dism1750.c	-o $(OBJ)/dism1750.o

$(OBJ)/do_xio.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/do_xio.c
	$(CC) -c $(CFLAGS) $(SRC)/do_xio.c	-o $(OBJ)/do_xio.o

$(OBJ)/exec.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/break.h $(SRC)/exec.c
	$(CC) -c $(CFLAGS) $(SRC)/exec.c	-o $(OBJ)/exec.o

$(OBJ)/flt1750.o:	$(SRC)/flt1750.h $(SRC)/flt1750.c
	$(CC) -c $(CFLAGS) $(SRC)/flt1750.c	-o $(OBJ)/flt1750.o

$(OBJ)/lic.o:	$(SRC)/lic.c
	$(CC) -c $(CFLAGS) $(SRC)/lic.c	-o $(OBJ)/lic.o

$(OBJ)/loadfile.o: $(SRC)/status.h $(SRC)/phys_mem.h \
	  $(SRC)/utils.h $(SRC)/tekhex.h $(SRC)/tekops.h $(SRC)/loadfile.c
	$(CC) -c $(CFLAGS) $(SRC)/loadfile.c	-o $(OBJ)/loadfile.o

$(OBJ)/main.o: $(SRC)/status.h $(SRC)/main.c
	$(CC) -c $(CFLAGS) $(SRC)/main.c	-o $(OBJ)/main.o

$(OBJ)/phys_mem.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/utils.h \
	  $(SRC)/tekhex.h $(SRC)/arch.h $(SRC)/phys_mem.c
	$(CC) -c $(CFLAGS) $(SRC)/phys_mem.c	-o $(OBJ)/phys_mem.o

$(OBJ)/peekpoke.o: $(SRC)/arch.h $(SRC)/peekpoke.c
	$(CC) -c $(CFLAGS) $(SRC)/peekpoke.c	-o $(OBJ)/peekpoke.o

$(OBJ)/sdisasm.o: $(SRC)/arch.h $(SRC)/sdisasm.c
	$(CC) -c $(CFLAGS) $(SRC)/sdisasm.c	-o $(OBJ)/sdisasm.o

$(OBJ)/smemacc.o: $(SRC)/arch.h $(SRC)/smemacc.c
	$(CC) -c $(CFLAGS) $(SRC)/smemacc.c	-o $(OBJ)/smemacc.o

$(OBJ)/status.o: $(SRC)/status.h $(SRC)/status.c
	$(CC) -c $(CFLAGS) $(SRC)/status.c	-o $(OBJ)/status.o

$(OBJ)/tekhex.o: $(SRC)/arch.h $(SRC)/status.h $(SRC)/utils.h $(SRC)/tekhex.c
	$(CC) -c $(CFLAGS) $(SRC)/tekhex.c	-o $(OBJ)/tekhex.o

$(OBJ)/tekops.o: $(SRC)/arch.h $(SRC)/utils.h $(SRC)/status.h $(SRC)/tekops.c
	$(CC) -c $(CFLAGS) $(SRC)/tekops.c	-o $(OBJ)/tekops.o

$(OBJ)/ldm.o: $(SRC)/arch.h $(SRC)/peekpoke.h $(SRC)/utils.h $(SRC)/ldm.c
	$(CC) -c $(CFLAGS) $(SRC)/ldm.c		-o $(OBJ)/ldm.o

$(OBJ)/load_coff.o: $(SRC)/arch.h $(SRC)/peekpoke.h $(SRC)/utils.h $(SRC)/load_coff.c
	$(CC) -c $(CFLAGS) $(SRC)/load_coff.c	-o $(OBJ)/load_coff.o

$(OBJ)/utils.o: $(SRC)/type.h $(SRC)/utils.h $(SRC)/utils.c
	$(CC) -c $(CFLAGS) $(SRC)/utils.c	-o $(OBJ)/utils.o

$(OBJ)/xiodef.o: $(SRC)/xiodef.h $(SRC)/xiodef.c
	$(CC) -c $(CFLAGS) $(SRC)/xiodef.c	-o $(OBJ)/xiodef.o

