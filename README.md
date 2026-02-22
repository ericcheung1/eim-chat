# EIM  - Instant Messenger 

## Overview

EIM is a simple TCP-based instant messaging system. It supports multi-client text-only messages sent over a raw TCP connections. All messages are appended to a server-side `chat_log.txt` file and broadcast to all other connected clients. This system allows you to chat with yourself from any device on your home network.

### Limitations 

- Only works within the same machine or local network

### Potential Additions

- A way to explicitly bind client to a specific IP 
- Timestamps in message logs

### How to run

To run the system, clone the repository and run `make -f server.mk` and `make -f client.mk` in the project directory. The compiled binaries will  appear in the `bin/` folder. Start the server with `./bin/eim` and start the client with `./bin/eim-client`.
