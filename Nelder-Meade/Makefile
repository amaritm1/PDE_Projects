OBJECTS = nelder-mead.o xmalloc.o
TARGOBJ = demo-2D.o demo-1D.o demo-constrained.o demo-energy.o
#--{{{-------------------------------------------------------------------------#
TARGETS = $(TARGOBJ:.o=)
SHELL = /bin/sh
LIBS =

CFLAGS = -Wall -Wextra -pedantic -std=c99 -O2
LDFLAGS =

.PHONY: clean all
all: $(TARGETS)
$(TARGETS): $(OBJECTS)
clean:
	$(RM) $(OBJECTS) $(TARGOBJ) $(TARGETS)
#--}}}-------------------------------------------------------------------------#

#--output of `gcc -MM *.c`-----------------------------------------------------#
demo-1D.o: demo-1D.c nelder-mead.h
demo-2D.o: demo-2D.c nelder-mead.h
demo-energy.o: demo-energy.c nelder-mead.h
nelder-mead.o: nelder-mead.c array.h xmalloc.h nelder-mead.h
rank-vertices-test.o: rank-vertices-test.c
xmalloc.o: xmalloc.c xmalloc.h
