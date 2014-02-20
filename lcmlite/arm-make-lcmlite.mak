PROGRAM=../lib/arm/liblcmlite.a
SOURCES=lcmlite.c LcmliteWrapper.cpp
OBJECTS=lcmlite.o LcmliteWrapper.o
TOOLPREFIX=arm-none-linux-gnueabi-

CC=$(TOOLPREFIX)g++

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f *.o $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	ar rcs $@ $^

lcmlite.o: lcmlite.c
	$(CC) -o $@ -c $<

LcmliteWrapper.o: LcmliteWrapper.cpp
	$(CC) -o $@ -c $<
