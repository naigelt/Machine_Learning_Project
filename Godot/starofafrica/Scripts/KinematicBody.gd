extends KinematicBody

var speed = 5.0
var current_city

func _physics_process(delta):
	var input_dir = Input.get_vector("move_left", "move_right", "move_up", "move_down")
	var velocity = Vector3(input_dir.x, 0, input_dir.y) * speed
	move_and_slide(velocity)

func move_to(city):
	global_transform.origin = city.global_transform.origin
	current_city = city
