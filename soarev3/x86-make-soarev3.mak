PROGRAM=../lib/x86/libsoarev3.so
SOURCES=$(shell cat soarev3.cxx)
#SOURCES=LcmUtil.cpp WMUtil.cpp comm/CommStructs.cpp comm/SoarCommunication.cpp soar/SoarManager.cpp soar/SoarMotor.cpp soar/SoarBrick.cpp soar/SoarColorSensor.cpp soar/SoarTouchSensor.cpp
OBJECTS=$(SOURCES:.cpp=.o)

OS := $(shell uname)
ifeq ($(OS),Darwin) 
	TOOLPREFIX=clang
else
	TOOLPREFIX=g
endif
CC=$(TOOLPREFIX)++

FLAGS=-shared -fpic

INCLUDEPATH=-I$(EV3)/soarev3 -I$(EV3)/lcmlite -I$(EV3)/include -I$(SOAR_HOME)/include 
LIBRARYPATH=-L$(SOAR_HOME) -L$(EV3)/lib/x86
LIBRARIES=-llcmlite -lSoar -ldl -lrt -lpthread

all:: clean $(OBJECTS) $(PROGRAM)

clean::
	rm -f $(PROGRAM) *.o comm/*.o ev3/*.o soar/*.o 

# how to link executable
$(PROGRAM): $(OBJECTS)
	$(CC) $(FLAGS) -o $@ $(OBJECTS) $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

%.o: %.cpp
	$(CC) $(FLAGS) -o $@ -c $< $(INCLUDEPATH) $(LIBRARYPATH) $(LIBRARIES)

