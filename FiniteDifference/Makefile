OBJECTS = problem-spec.o xmalloc.o plot-with-geomview.o
TARGOBJ = heat-implicit.o heat-explicit.o heat-crank-nicolson.o
#--{{{-------------------------------------------------------------------------#
TARGETS = $(TARGOBJ:.o=)
SHELL = /bin/sh

CFLAGS = -Wall -Wextra -Wpedantic -std=c99
LDFLAGS =
LDLIBS = -lm

.PHONY: clean all
all: $(TARGETS)
$(TARGETS): $(OBJECTS)
clean:
	$(RM) $(OBJECTS) $(TARGOBJ) $(TARGETS)
#--}}}-------------------------------------------------------------------------#

problem-spec.o: problem-spec.c problem-spec.h
plot-with-geomview.o: plot-with-geomview.c plot-with-geomview.h mesh.h \
 problem-spec.h
problem-spec.o: problem-spec.c problem-spec.h
heat-implicit.o: heat-implicit.c array.h xmalloc.h problem-spec.h
heat-exlicit.o: heat-explicit.c array.h xmalloc.h problem-spec.h
heat-crank-nicolson.o: heat-crank-nicolson.c array.h xmalloc.h problem-spec.h
