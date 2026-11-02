
import socket
import threading

HOST = "127.0.0.1"
PORT = 5000


def receive_messages(sock):
    """Receive messages from the Qt TCP server."""
    buffer = ""

    try:
        while True:
            data = sock.recv(4096)

            if not data:
                print("\nServer disconnected.")
                break

            buffer += data.decode("utf-8", errors="replace")

            # Messages are terminated with CRLF (\r\n).
            while "\r\n" in buffer:
                message, buffer = buffer.split("\r\n", 1)
                if message:
                    print(f"\nQt Server: {message}")

                print("You: ", end="", flush=True)

    except OSError:
        pass


def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        print(f"Connecting to {HOST}:{PORT}...")

        sock.connect((HOST, PORT))
        print("Connected to Qt TCP Server!")
        print("Type a message and press Enter.")
        print("Type 'quit' to disconnect.\n")

        receiver = threading.Thread(
            target=receive_messages,
            args=(sock,),
            daemon=True
        )
        receiver.start()

        while True:
            message = input("You: ")

            if message.lower() == "quit":
                break

            if message:
                sock.sendall((message + "\r\n").encode("utf-8"))

    except ConnectionRefusedError:
        print("Connection refused. Is your Qt server listening on port 5000?")

    except OSError as error:
        print(f"Socket error: {error}")

    finally:
        sock.close()
        print("Python client closed.")


if __name__ == "__main__":
    main()
