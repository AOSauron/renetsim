CC = gcc  -Wall -Wno-deprecated-declarations
LDFLAGS = -lm -ljson-c -L/usr/local/lib -lgvc -lcgraph -lcdt
CFLAGS = -I/usr/local/include/graphviz -I/usr/include/json-c
OBJECTS = main.o renetsim.o renderer.o pathfinding.o graphGeneration.o input.o

all : rns

rns : $(OBJECTS)
		$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

.c.o:
		$(CC) $(CFLAGS) -c $<

clean:
		rm -rf *.o

mrproper: clean
		rm -rf rns
