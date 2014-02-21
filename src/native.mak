PROGRAM=../bin/native_ev3
DOBJECTS=native_ev3.cpp

INCLUDEPATH=-I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/newsoarev3 -I$(SOAR_HOME)/include -I$(ARM_GCC)/arm-none-linux-gnueabi/include 
LIBRARYPATH=-L$(ARM_GCC)/arm-none-linux-gnueabi/lib -L$(EV3)/lib/arm

LIBRARIES=-lsoarev3 -llcmlite -lrt -ldl -lpthread

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

