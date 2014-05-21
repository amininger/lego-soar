PROGRAM=choose_soar_file
#DOBJECTS=ev3_lcd.o ev3_timer.o ev3_command.o 
#TOOLPREFIX=arm-none-linux-gnueabi-

all:: realclean $(DOBJECTS) $(PROGRAM)

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=

CC=$(TOOLPREFIX)g++

# how to link executable
choose_soar_file: choose_soar_file.cpp
	$(CC) $(FLAGS) $< -o$@ #ev3_lcd.o ev3_timer.o ev3_command.o 

# how to compile source
#%.o: %.cpp
#	$(CC) $(FLAGS) -c $< -o$@

