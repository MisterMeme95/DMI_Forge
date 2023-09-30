# Variables
CC=gcc
CFLAGS=-I.

DMI_Forge1: main.o DMIParse.o DMI_Struct.o Icon_State.o
	$(CC) -L./zlib -o ./cmake-build-debug/png/__Execute main.o DMIParse.o DMI_Struct.o Icon_State.o  png/libpng.a -l../zlib/libz -lm

main.o: main.c
	$(CC) -I ./png -c main.c

DMIParse.o: DMIParse.c
	$(CC) -I ./png -c DMIParse.c

Icon_State.o: Icon_State.c
	$(CC) -I ./png -c Icon_State.c

DMI_Struct.o: DMI_Struct.c
	$(CC) -I ./png -c DMI_Struct.c

# Clean up
clean:
	rm -f *.o output


