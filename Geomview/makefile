OFILES 	= mesh.o problem-spec.o twb-quad-demo.o twb-quad.o xmalloc.o \
		plot-with-geomview.o
TARGET 	= twb-quad-demo
CFLAGS 	= -Wall -pedantic -std=c99

$(TARGET): $(OFILES) triangle.o
	cc $(OFILES) triangle.o -o $@ -lm
clean:
	rm -f $(TARGET) $(OFILES)

mesh.o: mesh.c triangle.h xmalloc.h array.h mesh.h problem-spec.h
problem-spec.o: problem-spec.c problem-spec.h array.h xmalloc.h
twb-quad.o: twb-quad.c twb-quad.h
twb-quad-demo.o: twb-quad-demo.c twb-quad.h problem-spec.h mesh.h \
 plot-with-geomview.h
xmalloc.o: xmalloc.c xmalloc.h

