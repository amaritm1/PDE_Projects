OBJECTS = xmalloc.o
TARGOBJ = umfpack-Hilbert-demo.o
#--{{{-------------------------------------------------------------------------#
TARGETS = $(TARGOBJ:.o=)
SHELL = /bin/sh
LDLIBS = -lumfpack

CFLAGS = -Wall -std=c99 -pedantic -O2 
LDFLAGS =

.PHONY: clean all
all: $(TARGETS)
$(TARGETS): $(OBJECTS)
clean:
	$(RM) $(OBJECTS) $(TARGOBJ) $(TARGETS)
#--}}}-------------------------------------------------------------------------#

#--output of `gcc -MM *.c`-----------------------------------------------------#
umfpack-Hilbert-demo.o: umfpack-Hilbert-demo.c array.h xmalloc.h
xmalloc.o: xmalloc.c xmalloc.h
