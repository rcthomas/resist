
CC=gcc

default: resist-mini-app.x

resist-mini-app.x: resist-mini-app.o resist-config.o
	$(CC) resist-mini-app.o resist-config.o -o $@

%.o: %.c
	$(CC) -c $< -o $@

realclean: clean
	rm -rf *.x

clean:
	rm -rf *.o

