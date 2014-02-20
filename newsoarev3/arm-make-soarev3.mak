PROGRAM=../lib/arm/libsoarev3.a
SOURCES=$(shell cat soarev3.cxx)
OBJECTS=$(SOURCES:.cpp=.o)

TOOLPREFIX=arm-none-linux-gnueabi-
CC=$(TOOLPREFIX)g++

INCLUDEPATH=-I$(EV3)/newsoarev3 -I$(EV3)/lcmlite -I$(EV3)/include -I$(EV3)/CodeSource/arm-2009q1/arm-none-linux-gnueabi/include 

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) *.o comm/*.o ev3/*.o soar/*.o

# how to link executable
$(PROGRAM): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(INCLUDEPATH) 

