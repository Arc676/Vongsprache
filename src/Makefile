CC=gcc
FLAGS=-c
OBJS=main.o stream.o hashtable.o lexer.o token.o
OUT=vongsprache

vongsprache: $(OBJS)
	$(CC) -o $(OUT) $(OBJS)

main.o:
	$(CC) $(FLAGS) main.c

stream.o:
	$(CC) $(FLAGS) stream.c

hashtable.o:
	$(CC) $(FLAGS) hashtable.c

lexer.o:
	$(CC) $(FLAGS) lexer.c

token.o:
	$(CC) $(FLAGS) token.c

clean:
	rm -f $(OBJS) $(OUT)
