FILES=./build/start.o ./build/garyos.o
INCLUDES=
all: ${FILES}
	i686-elf-ld -m elf_i386 -relocatable ${FILES} -o ./stdlib.elf
	
./build/start.o: ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.o

./build/garyos.o: ./src/garyos.asm
	nasm -f elf ./src/garyos.asm -o ./build/garyos.o

clean:
	rm -rf ${FILES}
	rm ./stdlib.elf