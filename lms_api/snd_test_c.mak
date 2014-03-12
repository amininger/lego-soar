PROGRAM=snd_test_c
DOBJECTS=ev3_sound.o 
TOOLPREFIX=arm-none-linux-gnueabi-

all:: realclean $(DOBJECTS) $(PROGRAM)

download:: all
	pscp -scp -pw "" snd_test_c root@192.168.2.128:/media/card

clean::
	rm -f *.o *.ppu *.rst

realclean:: clean
	rm -f $(PROGRAM)

FLAGS=

CC=$(TOOLPREFIX)gcc

# how to link executable
snd_test_c: snd_test_c.c
	$(CC) $(FLAGS) $< -o$@ ev3_sound.o 

# how to compile source
%.o: %.c
	$(CC) $(FLAGS) -c $< -o$@

