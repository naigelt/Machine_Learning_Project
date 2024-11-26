extends Node

# server connections here

var address := "127.0.0.1"
var port := 12345

const TCPClient = preload("res://Scenes/TCPClient.gd")
var _tcp_client: TCPClient = TCPClient.new()

func _ready():
	# return values not needed here, so just taking them to a variable to get rid of warnings
	var retVal
	retVal = _tcp_client.connect("connected", self, "_connected_to_server")
	retVal += _tcp_client.connect("disconnected", self, "_server_disconnected")
	retVal += _tcp_client.connect("error", self, "connection_failed")
	retVal += _tcp_client.connect("data", self, "_handle_received_data")
	if retVal != 0:
		print("Something went wrong trying to establish server connections.")
	
	# this is done using StreamPeerTCP class
	add_child(_tcp_client)
	_tcp_client.connect_to_host(address, port)

# another way of joining with NetworkedMultiplayerENet class (both ways obviously not needed
func join_server():
	# make the client
	var serverClient = NetworkedMultiplayerENet.new()
	# listen to local port 12345
	var err = serverClient.create_client(address, port)
	
	if err != OK:
		print("Client cannot connect to server.")
		return
	
	# set the client if everything worked
	get_tree().network_peer = serverClient
	print("Listening to " + address + ":" + str(port))
	
func _connected_to_server():
	print("Connected to server.")

func _server_disconnected():
	print("Server disconnected.")

func connection_failed():
	print("Connection failed.")

# print received data to console and send acknowledgement "ack" to server
func _handle_received_data(data: PoolByteArray) -> void:
	print("Client data: ", data.get_string_from_utf8())
	var message: PoolByteArray = [97, 99, 107] # "ack" in ASCII
	var retSend = _tcp_client.send(message)
	if !retSend:
		print("Sending data (ack) failed")
