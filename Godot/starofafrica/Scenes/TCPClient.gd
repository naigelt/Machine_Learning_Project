extends Node

# using StreamPeerTCP class

signal connected	# connected to server
signal disconnected	# disconnected from the server
signal data			# received some data
signal error		# something went wrong

var _status: int = 0
var _stream: StreamPeerTCP = StreamPeerTCP.new()

func _ready() -> void:
	_status = _stream.get_status()

# listen to any data sent from the server
# if status changes, something was sent (error or data). Emit corresponding signal
func _process(_delta: float) -> void:
	var new_status: int = _stream.get_status()
	if new_status != _status:
		_status = new_status
		match _status:
			_stream.STATUS_NONE:
				print("Disconnected from host.")
				emit_signal("disconnected")
			_stream.STATUS_CONNECTING:
				print("Connecting to host. (_process())")
			_stream.STATUS_CONNECTED:
				print("Connected to host.")
				emit_signal("connected")
			_stream.STATUS_ERROR:
				print("Error with socket stream.")
				emit_signal("error")

	if _status == _stream.STATUS_CONNECTED:
		var available_bytes: int = _stream.get_available_bytes()
		if available_bytes > 0:
			print("available bytes: ", available_bytes)
			var data: Array = _stream.get_partial_data(available_bytes)
			# Check for read error.
			if data[0] != OK:
				print("Error getting data from stream: ", data[0])
				emit_signal("error")
			else:
				# this should be a JSON that the server sent
				# parse it somewhere somehow
				emit_signal("data", data[1])

# establish connection
func connect_to_host(host: String, port: int) -> void:
	print("Connecting to %s:%d (connect_to_host())" % [host, port])
	
	# Reset status to none, so we can tell if something went wrong
	_status = _stream.STATUS_NONE
	if _stream.connect_to_host(host, port) != OK:
		print("Error connecting to host.")
		emit_signal("error")

# send data as a pool of nonfragmented bytes to TCP stream (server)
# returns true if everything worked, false if there was an error, such as disconnect
func send(data: PoolByteArray) -> bool:
	if _status != _stream.STATUS_CONNECTED:
		print("Error: Stream not connected.")
		return false
	var error: int = _stream.put_data(data)
	if error != OK:
		print("Writing to stream failed: ", error)
		return false
	return true
