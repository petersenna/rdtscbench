all:
	$(MAKE) -C ./rdtscbench
	$(MAKE) -C ./getpid
	$(MAKE) -C ./matrix
	$(MAKE) -C ./moreExamples

clean:
	$(MAKE) -C ./rdtscbench clean
	$(MAKE) -C ./getpid clean
	$(MAKE) -C ./matrix clean
	$(MAKE) -C ./moreExamples clean
	rm -f *~
	rm -f *.o

