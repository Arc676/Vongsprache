CC=gcc
FLAGS=-c
OBJS=main.o stream.o hashtable.o
OUT=vongsprache

vongsprache: $(OBJS)
        $(CC) $(OBJS) -o vongsprache

main.o:
        $(CC) $(FLAGS) node.cpp

stream.o:
        $(CC) $(FLAGS) graph.cpp

hashtable.o:
        $(CC) $(FLAGS) pathfinder.cpp

clean:
        rm -f $(OBJS) $(OUT)
