CFLAGS = -lrt -lm

all: gethz rdtscbench.o

gethz:
	./gethz.sh

rdtscbench.o: rdtscbench.h rdtscbench.c

clean:
	rm -f *~
	rm -f *.o
	rm -f hzdef.h
	rm -f rdtscbench.o
