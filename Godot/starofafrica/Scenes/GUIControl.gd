extends Control


# Called when the node enters the scene tree for the first time.
func _ready():
	# let mouse clicks go through the GUI layer to actually get to click gameobjects
	mouse_filter = MOUSE_FILTER_IGNORE

# this should never be called since mouse clicks are set to ignore
func _gui_input(event):
	if event is InputEventMouseButton and event.button_index == BUTTON_LEFT and event.pressed:
		print("Left mouse button was pressed on GUI (_gui_input()).")


func _on_Player1DiceButton_pressed():
	print("Player 1 button pressed")


func _on_Player2DiceButton_pressed():
	print("Player 2 button pressed")
