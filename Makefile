default: asm.c
	gcc -g -o asm asm.c
build:
	gcc -g -o asm asm.c
test:
	gcc -g -o asm asm.c && ./asm main.S
