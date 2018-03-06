CC=gcc
FLAGS=-c
OBJS=main.o stream.o hashtable.o lexer.o token.o parser.o
OUT=vongsprache

debug: FLAGS += -g -O0
debug: vongsprache

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

parser.o:
	$(CC) $(FLAGS) parser.c

clean:
	rm -f $(OBJS) $(OUT)
