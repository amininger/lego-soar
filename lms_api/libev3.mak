PROGRAM=libev3.so
DOBJECTS=ev3_sound.c ev3_output.c ev3_button.c ev3_timer.c ev3_command.c ev3_lcd.c
TOOLPREFIX=arm-none-linux-gnueabi-

all:: realclean $(DOBJECTS) $(PROGRAM)

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=-fpic -shared

CC=$(TOOLPREFIX)gcc

# how to link executable
libev3.so: $(DOBJECTS)
	$(CC) $(FLAGS) -o$@ $(DOBJECTS) 

# how to compile source
%.o: %.c
	$(CC) $(FLAGS) -c $< -o$@

