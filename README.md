# EIM  - Instant Messenger 

## Overview

EIM is a simple TCP-based instant messaging experiment built for learning. Currently, it supports text-only messages sent over a raw TCP connection. All messages are appended to a server-side `chat_log.txt` file. This tiny chat system will allow you to chat with yourself from any device in your home network.

### Currently limitations 

- Only the server-side program exists
- Clients must use an external tool (e.g., `nc`) to send messages
- Only works within the same machine or local network
- Only one client can write to the chat log at a time

### Working on

- Support for multiple simultaneous clients

### Coming additions 

- A dedicated client program for sending messages

### How to run

To run the server, clone the repository and run `make -f server.mk` in the project directory. The compiled binary will appear in a `bin/` folder. Start the server with `./bin/eim`. Use a TCP tool like `nc` to connect and send messages to the server
