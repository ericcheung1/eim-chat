# EIM  - Instant Messenger 

## Overview

EIM is a simple TCP-based instant messaging project made for learning. Currently, it supports text-only messages sent over a raw TCP connections. All messages are appended to a server-side `chat_log.txt` file. This system will allow you to chat with yourself from any device in your home network.

### Current limitations 

- Only the server-side program exists
- Clients must use an external tool (e.g., `nc`) to send messages
- Only works within the same machine or local network

### Working on

- Support for multiple simultaneous clients - DONE
- Viewing messages on client-side

### Coming additions 

- A dedicated client program for sending messages

### How to run

To run the server, clone the repository and run `make -f server.mk` in the project directory. The compiled binary will appear in a `bin/` folder. Start the server with `./bin/eim`. Use a TCP tool like `nc` to connect and send messages to the server
