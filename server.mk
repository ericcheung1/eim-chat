bin/eim: src/server/main.c src/server/server_core.c src/server/chat.c 
	mkdir -p bin
	gcc -Wall -std=c99 -o bin/eim src/server/main.c src/server/server_core.c src/server/chat.c 