PROGRAM=../lib/x86/liblcmlite.so
SOURCES=lcmlite.c LcmliteWrapper.cpp
OBJECTS=lcmlite.o LcmliteWrapper.o

OS := $(shell uname)
ifeq ($(OS),Darwin) 
	TOOLPREFIX=clang
else
	TOOLPREFIX=g
endif
CC=$(TOOLPREFIX)++

FLAGS=-shared -fpic

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f *.o $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS)

lcmlite.o: lcmlite.c
	$(CC) $(FLAGS) -o $@ -c $<

LcmliteWrapper.o: LcmliteWrapper.cpp
	$(CC) $(FLAGS) -o $@ -c $<
