# EIM  - Instant Messenger 

## Overview

EIM is a simple TCP-based instant messaging project. Currently, it supports text-only messages sent over a raw TCP connections. All messages are appended to a server-side `chat_log.txt` file. This system will allow you to chat with yourself from any device in your home network.

### Current limitations 

- Only works within the same machine or local network
- Messages are not broadcast to other clients

### Working on

- Broadcasting messages to all connected client-side

### How to run

To run the system, clone the repository and run `make -f server.mk` and `make -f client.mk` in the project directory. The compiled binaries will  appear in the `bin/` folder. Start the server with `./bin/eim` and start the client with `./bin/eim-client`.
