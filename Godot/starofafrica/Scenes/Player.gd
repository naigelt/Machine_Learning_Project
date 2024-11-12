extends Node

var RAY_LENGTH : float = 10.0
var target_position

# Called when the node enters the scene tree for the first time.
func _ready():
	target_position = $PlayerRB.translation
	print("testing" + str(target_position))


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func _input(event):
	# if mouse left button was clicked
	if event is InputEventMouseButton and event.pressed and event.button_index == 1:
		# raycast from MainCamera to clicked position to get position on map
		var camera3d = $"../MainCamera"
		var from = camera3d.project_ray_origin(event.position)
		var to = from + camera3d.project_ray_normal(event.position) * RAY_LENGTH
		
		to.y = 0.0
		print("mouseclick: " + str(to))
		# change target position
		target_position = to
		$PlayerRB.translation = target_position
