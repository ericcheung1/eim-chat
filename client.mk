bin/eim-client: src/client/main.c
	mkdir -p bin
	gcc -Wall -std=c99 -o bin/eim-client src/client/main.c