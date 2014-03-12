PROGRAM=button_test_c
DOBJECTS=ev3_button.o ev3_timer.o 
TOOLPREFIX=arm-none-linux-gnueabi-

all:: realclean $(DOBJECTS) $(PROGRAM)

download:: all
	pscp -scp -pw "" button_test_c root@192.168.2.128:/media/card

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=

CC=$(TOOLPREFIX)gcc

# how to link executable
button_test_c: button_test_c.c
	$(CC) $(FLAGS) $< -o$@ ev3_button.o ev3_timer.o 

# how to compile source
%.o: %.c
	$(CC) $(FLAGS) -c $< -o$@

