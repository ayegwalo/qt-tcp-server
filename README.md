# Qt TCP Server

A graphical TCP server application developed using **C++, Qt 6 and CMake**, demonstrating network programming, socket communication and client-server interaction.

The application provides a Qt Widgets interface for managing a TCP server, accepting client connections, exchanging messages and monitoring communication activity.

## Project Overview

This project explores TCP/IP networking through Qt's networking framework.

It uses `QTcpServer` to listen for incoming IPv4 connections and `QTcpSocket` to establish bidirectional communication with a connected client.

The graphical interface allows users to configure the listening port, start or stop the server, exchange messages and monitor connection events.

## Features

- Start and stop the TCP server through a graphical interface.
- Configure the listening port (default: 5000).
- Listen on available IPv4 network interfaces.
- Accept incoming TCP client connections.
- Support one active client connection at a time.
- Reject additional clients while an existing client is connected.
- Send messages from the server to the connected client.
- Receive and display messages from the client.
- Automatically send a welcome message when a client connects.
- Display connection status, timestamps and communication logs.
- Python TCP client included for testing.

## Technologies Used

| Technology | Purpose |
|---|---|
| C++ | Application logic |
| Qt 6 | Application framework |
| Qt Widgets | Graphical interface |
| QTcpServer | Listening for TCP connections |
| QTcpSocket | Network communication |
| CMake | Build configuration |
| Python 3 | Client-side testing |

## Application Architecture

The application is organised around the `MainWindow` class, which inherits from `QMainWindow`.

### Core networking components

**QTcpServer**

Responsible for:
- Listening for incoming TCP connections.
- Monitoring pending client connections.
- Accepting connection requests.

**QTcpSocket**

Responsible for:
- Establishing communication with a connected client.
- Sending messages to the client.
- Receiving incoming data.
- Monitoring connection state and socket errors.

### Main application functions

| Function | Description |
|---|---|
| `toggleServer()` | Starts or stops the TCP server. |
| `acceptConnection()` | Accepts incoming client connections. |
| `sendMessage()` | Sends messages to the connected client. |
| `readIncomingData()` | Reads and displays incoming client data. |
| `appendLog()` | Adds server events and messages to the application log. |

## How It Works

1. Launch the Qt TCP Server application.
2. Enter a listening port (default: `5000`).
3. Click **Listen** to start the server.
4. A TCP client connects using the server's IP address and listening port.
5. The server accepts the connection and sends a welcome message.
6. Both sides can exchange messages.
7. Connection events and messages appear in the server's output log.
8. Click **Stop Listening** to stop the server.

## Project Structure

```text
TcpServer/
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── tcp_client.py
├── CMakeLists.txt
├── .gitignore
└── README.md
```

## Getting Started

### Prerequisites

- Qt 6
- Qt Creator (recommended)
- CMake
- C++ compiler
- Python 3 (for the Python client)

### Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/qt-tcp-server.git

cd qt-tcp-server
```

### Build the Application

Configure the project:

```bash
cmake -S . -B build
```

Compile:

```bash
cmake --build build
```

Alternatively, open `CMakeLists.txt` in Qt Creator and build the project.

## Testing Client Communication

The project includes a Python TCP client (`tcp_client.py`) for testing communication with the Qt server.

1. Start the Qt TCP server.
2. Configure the Python client to use the server's IP address and port.
3. Connect the Python client.
4. Exchange messages between the client and server.
5. Observe the messages and connection events in the Qt application's output log.

The default server port is `5000`.

## Current Limitations

- Only one active client connection is supported at a time.
- Additional incoming connections are rejected while a client is connected.
- The application currently uses a simple text-based communication approach.
- TCP message framing and handling of fragmented or combined messages can be improved.

## Future Improvements

- Multiple simultaneous client connections.
- Individual client management.
- Improved message framing and protocol handling.
- Enhanced connection monitoring.
- Message history and exportable logs.
- Improved network error handling.

## Author

**Christopher Ayegwalo**

C++ | Qt | Python | Software Engineering | Networking

GitHub: https://github.com/ayegwalo

---

*Part of my ongoing software engineering and technology projects portfolio.*
