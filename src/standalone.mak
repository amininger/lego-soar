PROGRAM=../bin/standalone
DOBJECTS=standalone.cpp

INCLUDEPATH=-I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/newsoarev3 -I$(SOAR_HOME)/include -I$(ARM_GCC)/arm-none-linux-gnueabi/include 
LIBRARYPATH=-L$(ARM_GCC)/arm-none-linux-gnueabi/lib -L$(EV3)/lib/arm

LIBRARIES=-lsoarev3 -lSoar -llcmlite -lrt -ldl -lpthread -lstdc++

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

