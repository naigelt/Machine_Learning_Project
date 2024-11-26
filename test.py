import socket
import json

def send_commands():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 12345))
    
    try:
        while True:
            # Receive server response
            response = client_socket.recv(4096).decode()
            print("Response from server:", response)

            data = json.loads(response)
            if "reachableNodes" in data:
                reachable_nodes = data["reachableNodes"]

                # For now, pick the first option
                selected_node = reachable_nodes[0]
                print(f"Selecting node {selected_node} to move.")
                
                # Send the move command
                move_command = f"MOVE_PLAYER {selected_node}"
                client_socket.sendall(move_command.encode())
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        client_socket.close()

send_commands()
