CFLAGS = -W -Wall -g -O2 --std=c99 -fshort-enums
LDFLAGS = -lgmp

all : wisp_tests
test : all
	./wisp_tests

OBJ = ../symtab.o ../object.o ../cons.o ../common.o ../number.o \
      ../lisp_math.o ../eval.o ../hashtab.o ../mem.o ../lisp.o \
      ../reader.o ../str.o

wisp_tests : wisp_tests.o $(OBJ)

wisp_tests.o : wisp_tests.c

.PHONY : clean all test
clean :
	$(RM) wisp_tests wisp_tests.o
