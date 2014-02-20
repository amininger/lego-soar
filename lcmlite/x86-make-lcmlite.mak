PROGRAM=../lib/x86/liblcmlite.so
SOURCES=lcmlite.c LcmliteWrapper.cpp
OBJECTS=lcmlite.o LcmliteWrapper.o
TOOLPREFIX=

CC=$(TOOLPREFIX)g++

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
