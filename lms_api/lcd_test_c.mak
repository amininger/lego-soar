PROGRAM=lcd_test_c
DOBJECTS=ev3_lcd.o ev3_timer.o ev3_command.o 
TOOLPREFIX=arm-none-linux-gnueabi-

all:: realclean $(DOBJECTS) $(PROGRAM)

download:: all
	pscp -scp -pw "%PW%" lcd_test_c root@%IPADDR%:%FOLDER%

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=

CC=$(TOOLPREFIX)gcc

# how to link executable
lcd_test_c: lcd_test_c.c
	$(CC) $(FLAGS) $< -o$@ ev3_lcd.o ev3_timer.o ev3_command.o 

# how to compile source
%.o: %.c
	$(CC) $(FLAGS) -c $< -o$@

