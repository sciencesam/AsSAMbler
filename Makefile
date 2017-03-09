default: AsSambler.c
	gcc -g -o AsSAMbler AsSAMbler.c
build:
	gcc -g -o AsSAMbler AsSAMbler.c
test:
	gcc -g -o AsSAMbler AsSAMbler.c && ./AsSAMbler main.S
