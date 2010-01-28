CFLAGS = -W -Wall -g -O2 --std=c99 -fshort-enums
LDFLAGS = -lgmp

default : wisp

all : wisp test doc
test : wisp
	make -C test test
doc :
	make -C doc

OBJ = wisp.o cons.o common.o object.o symtab.o eval.o hashtab.o mem.o \
      lisp.o str.o reader.o number.o lisp_math.o vector.o

wisp : $(OBJ)

wisp.o : wisp.c cons.h object.h common.h symtab.h eval.h number.h
cons.o : cons.c object.h symtab.h cons.h mem.h
object.o : object.c object.h common.h mem.h symtab.h cons.h str.h number.h
symtab.o : symtab.c symtab.h object.h common.h symtab.h
eval.o : eval.c eval.h common.h cons.h symtab.h number.h
mem.o : mem.c mem.h common.h
lisp.o : lisp.c lisp.h object.h symtab.h cons.h str.h number.h
lisp_math.o : lisp_math.c lisp.h
common.o : common.c common.h
str.o : str.c str.h object.h mem.h
reader.o : reader.c reader.h object.h eval.h number.h
number.o : number.c number.h object.h
vector.o : vector.c vector.h object.h mem.h

# Hash table
hashtab.o : hashtab.c hashtab.h

.PHONY : all clean test doc

clean :
	$(RM) $(OBJ) wisp y.tab.h
	make -C test clean
	make -C doc clean
