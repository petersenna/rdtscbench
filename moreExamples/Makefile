NAME = moreExamples
SRCDIR = ../rdtscbench
CFLAGS = -lrt -lm
OBJ = $(SRCDIR)/rdtscbench.o $(NAME).o


all: 
	$(MAKE) -C $(SRCDIR)
	$(MAKE) -C . $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

$(SRCDIR)/rdtscbench.o: $(SRCDIR)/rdtscbench.h $(SRCDIR)/rdtscbench.c

clean:
	rm -f *~
	rm -f *.o
	rm -f hzdef.h
	rm -f $(NAME)
#	$(MAKE) -C $(SRCDIR) clean

