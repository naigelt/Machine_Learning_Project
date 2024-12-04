extends Spatial

var player_offset_vector : Vector3 = Vector3(0, 1.5, 0)
export var nodeId : int

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _on_Area_input_event(_camera, event, _position, _normal, _shape_idx):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		print("Clicked a city or a road point: " + name)
		$"../../Player1/PlayerRB".translation = translation + player_offset_vector
