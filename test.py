import socket

def send_commands(commands):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 12345))
    try:
        for command in commands:
            client_socket.sendall(command.encode())
            response = client_socket.recv(4096).decode()
            print("Response from server:", response)
            if command == "QUIT":
                break
    finally:
        client_socket.close()

# Example usage
send_commands(["ROLL_DICE", "MOVE PLAYER 3", "QUIT"])
