default: asm.c
	gcc -g -o asm asm.c -lm
build:
	gcc -g -o asm asm.c -lm
test:
	gcc -g -o asm asm.c -lm  && ./asm main.S
