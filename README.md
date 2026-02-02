# tcp-e-chat

## Overview

This is a text-only chatting program that is implented via a TCP client-server model. User(s) can send messages that will be recorded to a server-side `chat_log.txt` text file. 

### Currently limitations 

- Only the server side code is supported
- Clients need to use to use an external program such as `nc` to send messages
- Clients can only join from within the same machine/local area network

it supports one user writting messages to a text file to the root directory. 

### Working on

- Adding support for mutiple clients

### Coming additions 

- Client side program for sending messages

### How to run

Clone the repository and run `make` in the project directory. The compiled binary will appear in the `bin/` folder. Use a program like that can send TCP packets like `nc` to send messages. 