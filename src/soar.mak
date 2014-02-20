PROGRAM=soar_ev3
DOBJECTS=soar_ev3.cpp

INCLUDEPATH=-I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/newsoarev3 
LIBRARYPATH=-L$(EV3)/lib/x86
LIBRARIES=-lsoarev3 -llcmlite -lrt -ldl -lpthread

TOOLPREFIX=
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

