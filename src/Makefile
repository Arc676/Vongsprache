CC=gcc
STD=c11
FLAGS=-c -std=$(STD)

ODIR=../obj

OBJS=main.o stream.o hashtable.o lexer.o token.o tokendata.o parser.o\
	scope.o error.o evaluation.o interactive.o builtins.o util.o
_OBJS=$(patsubst %, $(ODIR)/%, $(OBJS))

OUT=vongsprache

ifdef DEBUG
	FLAGS += -g -O0
endif

vongsprache: makedir $(_OBJS)
	$(CC) -o $(OUT) $(_OBJS) -lm

makedir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f $(ODIR)/*.o $(OUT)
