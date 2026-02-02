bin/e_chat: main.c server_core.c chat.c 
	mkdir -p bin
	gcc -Wall -std=c99 -o bin/e_chat main.c server_core.c chat.c