CC=gcc
FLAGS=-c

ODIR=../obj

OBJS=main.o stream.o hashtable.o lexer.o token.o tokendata.o parser.o scope.o error.o
_OBJS=$(patsubst %, $(ODIR)/%, $(OBJS))

OUT=vongsprache

ifdef DEBUG
	FLAGS += -g -O0
endif

vongsprache: makedir $(_OBJS)
	$(CC) -o $(OUT) $(_OBJS)

makedir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: %.c
	$(CC) $(FLAGS) -o $@ $<

clean:
	rm -f $(ODIR)/*.o $(OUT)
