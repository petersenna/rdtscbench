CFLAGS = -lrt -lm
LDFLAGS = -lm 

all: gethz rdtscbench.o

gethz:
	./gethz.sh

rdtscbench.o: rdtscbench.h rdtscbench.c $(LDFLAGS)

clean:
	rm -f *~
	rm -f *.o
	rm -f hzdef.h
	rm -f rdtscbench.o
