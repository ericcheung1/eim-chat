bin/eim-client: src/client/main.c src/client/client_core.c
	mkdir -p bin
	gcc -Wall -std=c99 -o bin/eim-client src/client/main.c src/client/client_core.c