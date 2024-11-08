extends Camera

var cameraSpeed : int = 10

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	if Input.is_action_pressed("ui_left"):
		translation.x -= cameraSpeed * delta
	else:
		if Input.is_action_pressed("ui_right"):
			translation.x += cameraSpeed * delta
	if Input.is_action_pressed("ui_up"):
		translation.z -= cameraSpeed * delta
	else:
		if Input.is_action_pressed("ui_down"):
			translation.z += cameraSpeed * delta
