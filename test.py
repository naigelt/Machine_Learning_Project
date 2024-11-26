import socket
import json

def receive_and_display_data():
    """
    Connect to the server, receive JSON data, and print it.
    """
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(('localhost', 12345))  # Adjust the host and port if needed

    try:
        while True:
            # Receive data from the server
            response = client_socket.recv(4096).decode()
            if not response:
                print("Server disconnected.")
                break

            # Parse and print the JSON data
            try:
                data = json.loads(response)
                print("\n--- Game State Update ---")
                print(json.dumps(data, indent=4))  # Pretty print JSON data
                print("\n")
            except json.JSONDecodeError:
                print("Received invalid JSON data from the server.")
    except KeyboardInterrupt:
        print("Exiting client...")
    finally:
        client_socket.close()

if __name__ == "__main__":
    receive_and_display_data()
