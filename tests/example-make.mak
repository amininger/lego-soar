PROGRAM=motor_test
DOBJECTS=motor_test.cpp
TOOLPREFIX=arm-none-linux-gnueabi-
INCLUDEPATH=-I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/CodeSource/arm-2009q1/arm-none-linux-gnueabi/include 
LIBRARYPATH=-L$(EV3)/CodeSource/arm-2009q1/arm-none-linux-gnueabi/lib -L$(EV3)/lib
LIBRARIES=-llcmlitearm -lrt -ldl -lpthread

all:: realclean $(DOBJECTS) $(PROGRAM)

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=-static

CC=$(TOOLPREFIX)g++

# how to link executable
motor_test: $(DOBJECTS)
	$(CC) $(FLAGS) -o$@ $(DOBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

