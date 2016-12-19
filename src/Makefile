# To use high-bandwidth memory on Cori at NERSC (2nd-gen Intel Xeon Phi,
# "Knights Landing"), one needs to load the "memkind" module, and also one must
# link dynamically. If you try to link statically with memkind you will get
# lots of errors.

USE_HBM=FALSE

CC=cc

default: resist-mini-app.x

RESIST_MINI_APP_OBJ=resist-mini-app.o resist-config.o resist-context.o resist-memory.o

ifeq ($(USE_HBM),TRUE)
  LDFLAGS=-dynamic -lmemkind
  CFLAGS=-DUSE_HBM=TRUE
endif

CFLAGS+=-O0 -g -Wall -Werror

# Resist uses a number of C99 features.
CFLAGS+=-std=c99

LDFLAGS+=-lm

resist-mini-app.x: $(RESIST_MINI_APP_OBJ)
	$(CC) $(RESIST_MINI_APP_OBJ) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

format: 
	uncrustify -c uncrustify.cfg --check *.c *.h

realclean: clean
	rm -rf *.x

clean:
	rm -rf *.o
	rm -rf *.uncrustify

