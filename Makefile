CC=gcc
CFLAGS=-I.
LIBS=-lpthread
DEPS = module.h pedrolio.h
OBJ = main.o pedrolio.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

grupo7: $(OBJ) mod_grupo7/mod_grupo7.o
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

all: grupo7

PHONY: clean

clean:
	rm -f *.o */*.o grupo7
