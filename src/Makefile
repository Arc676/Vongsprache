CC=gcc
FLAGS=-c
OBJS=main.o stream.o hashtable.o lexer.o
OUT=vongsprache

vongsprache: $(OBJS)
    $(CC) $(OBJS) -o vongsprache

main.o:
    $(CC) $(FLAGS) main.c

stream.o:
    $(CC) $(FLAGS) stream.c

hashtable.o:
    $(CC) $(FLAGS) hashtable.c

lexer.o:
	$(CC) $(FLAGS) lexer.c

clean:
        rm -f $(OBJS) $(OUT)
