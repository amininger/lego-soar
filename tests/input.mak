PROGRAM=input_test
DOBJECTS=input_test.cpp

INCLUDEPATH=-I$(EV3)/include -I$(EV3)/CodeSource/arm-2009q1/arm-none-linux-gnueabi/include 
LIBRARYPATH=-L$(EV3)/CodeSource/arm-2009q1/arm-none-linux-gnueabi/lib

LIBRARIES=

TOOLPREFIX=arm-none-linux-gnueabi-
CC=$(TOOLPREFIX)g++

FLAGS=

all:: realclean $(DOBJECTS) $(PROGRAM)

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

# how to link executable
$(PROGRAM): $(DOBJECTS)
	$(CC) $(FLAGS) -o$@ $(DOBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES) 

