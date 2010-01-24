CFLAGS = -W -Wall -g -O2 --std=c99

all : wisp
test : all
	make -C test test

OBJ = wisp.o cons.o common.o object.o symtab.o eval.o hashtab.o mem.o \
      lisp.o parser.o scanner.o

wisp : $(OBJ)

wisp.o : wisp.c cons.h object.h common.h symtab.h eval.h
cons.o : cons.c object.h symtab.h cons.h mem.h
object.o : object.c object.h common.h mem.h symtab.h cons.h
symtab.o : symtab.c symtab.h object.h common.h symtab.h
eval.o : eval.c eval.h common.h cons.h symtab.h
mem.o : mem.c mem.h common.h
lisp.o : lisp.c lisp.h object.h symtab.h cons.h
common.o : common.c common.h

parser.o : parser.y object.h common.h eval.h
scanner.o : scanner.l

# Hash table
hashtab.o : hashtab.c hashtab.h

.PHONY : all clean test

clean : 
	$(RM) $(OBJ) wisp y.tab.h
	make -C test clean
