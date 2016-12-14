
CC=cc

default: resist-mini-app.x

RESIST_MINI_APP_OBJ=resist-mini-app.o resist-config.o resist-context.o

CFLAGS+=-O0 -Wall -Werror

resist-mini-app.x: $(RESIST_MINI_APP_OBJ)
	$(CC) $(RESIST_MINI_APP_OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

format: 
	uncrustify -c uncrustify.cfg --check *.c *.h

realclean: clean
	rm -rf *.x

clean:
	rm -rf *.o
	rm -rf *.uncrustify

