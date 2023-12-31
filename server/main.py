import socket
import threading
import time

current_socket: socket.socket = None


def receive_thread(client_socket: socket.socket):
    global current_socket
    while current_socket is not None:
        try:
            data = client_socket.recv(1000)
            if len(data) == 0:
                print("Client Terminated...")
                client_socket.close()
                current_socket = None
                break
            strdata = data.decode()
            if (strdata == "_pong\0"):
                continue
            print(f"Client Response -> {strdata}")
        except TimeoutError:
            print("Client Timed Out...")
            client_socket.close()
            current_socket = None
            break
        except ConnectionResetError:
            print("Client Disconnected...")
            client_socket.close()
            current_socket = None
            break
        except OSError:
            client_socket.close()
            current_socket = None
            break


def client_ping(client_socket: socket.socket):
    global current_socket
    while current_socket is not None:
        try:
            client_socket.sendall("_ping".encode())
            # sleep for 5 seconds
            time.sleep(5)
        except TimeoutError:
            print("Client Timed Out...")
            client_socket.close()
            current_socket = None
            break
        except ConnectionResetError:
            print("Client Disconnected...")
            client_socket.close()
            current_socket = None
            break
        except OSError:
            client_socket.close()
            current_socket = None
            break


def connect_req_handler(server_socket: socket.socket):
    while (True):
        client_socket, client_address = server_socket.accept()
        global current_socket
        if (current_socket is not None):
            print("A client attempted to connect but the connection already exist with another client")
            client_socket.close()
            continue
        print(f"Accepted connection from {client_address}")
        client_socket.settimeout(10)
        current_socket = client_socket
        thread = threading.Thread(target=receive_thread, args=(client_socket,))
        thread.start()
        pthread = threading.Thread(target=client_ping, args=(client_socket,))
        pthread.start()


def tcp_server():
    server_ip = "0.0.0.0"  # Listen on all available interfaces
    server_port = 42069

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((server_ip, server_port))
    server_socket.listen(5)

    print(f"Server listening on {server_ip}:{server_port}")

    thread = threading.Thread(target=connect_req_handler, args=(server_socket,))
    thread.start()
    print("Enter command:")
    while (True):
        command = input()
        if (current_socket is None):
            print("Wait for client to connect before sending a command...")
            continue
        command = command+'\0'
        current_socket.sendall(command.encode())


if __name__ == "__main__":
    tcp_server()
