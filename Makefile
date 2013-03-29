MAKEFILE    := Makefile.h350
MAKE        := make
MAKE_CLEAN  := make clean

subsystem:
	$(MAKE) -f $(MAKEFILE)

clean:
	$(MAKE_CLEAN) -f $(MAKEFILE)
