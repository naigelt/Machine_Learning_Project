extends Node

## Globals

# nodeId to gameObject list
var nodeIds : Array

# Playerbodies (RigidBody)
var player1RB : RigidBody
var player2RB : RigidBody
var currentPlayer : int

# UI label connections for displaying text
# Player 1
var p1MoneyLabel : Label
var p1WinConditionLabel : Label
var p1LastDiceRollLabel : Label
# Player 2
var p2MoneyLabel : Label
var p2WinConditionLabel : Label
var p2LastDiceRollLabel : Label
# Board Status
var emptiesLeftLabel : Label
var horseshoesLeftLabel : Label
var robbersLeftLabel : Label
var topazesLeftLabel : Label
var emeraldsLeftLabel : Label
var rubiesLeftLabel : Label
var starFoundLabel : Label

func _ready():
	var root = get_tree().root
	
	# assign variables here
	player1RB = root.get_node("/root/GameBoard/Player1/PlayerRB")
	player2RB = root.get_node("/root/GameBoard/Player2/PlayerRB")
	currentPlayer = 1	# initialize as Player 2, so the first turn starts as if turn just changed
	#print(player1RB.translation)	# test print to check that the right object was grabbed
	p1MoneyLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player1VBoxContainer/Player1Money")
	p1WinConditionLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player1VBoxContainer/Player1WinCondition")
	p1LastDiceRollLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player1VBoxContainer/Player1LastDiceRoll")
	p2MoneyLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player2VBoxContainer/Player2Money")
	p2WinConditionLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player2VBoxContainer/Player2WinCondition")
	p2LastDiceRollLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/Player2VBoxContainer/Player2LastDiceRoll")
	emptiesLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Empty")
	horseshoesLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Horseshoe")
	robbersLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Robber")
	topazesLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Topaz")
	emeraldsLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Emerald")
	rubiesLeftLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Ruby")
	starFoundLabel = root.get_node("/root/GameBoard/UI/PlayerInfosHBoxContainer/BoardVBoxContainer/Star")
	
	

# function to handle parsed JSON and do appropriate things with the content
func handleJSON(jsonData : JSONParseResult):
	# just testing if connection works:
	#print("doing stuff with parsed data")
	#print(jsonData.result["city discs"])
	
	# update all the labels in GUI
	p1MoneyLabel.text = "Money: " + str(jsonData.result["players"]["player1"]["money"])
	p1LastDiceRollLabel.text = "Latest dice roll: " + str(jsonData.result["players"]["player1"]["diceroll"])
	
	p2MoneyLabel.text = "Money: " + str(jsonData.result["players"]["player2"]["money"])
	p2LastDiceRollLabel.text = "Latest dice roll: " + str(jsonData.result["players"]["player2"]["diceroll"])
	
	#print(jsonData.result["remaining discs"])	# just for testing to see what "remaining discs" contain. Left here just in case
	# Types are: Empty = 0, Horseshoe = 1, Robber = 2, Topaz = 3, Emerald = 4, Ruby = 5, StarOfAfrica = 6
	var empties = 30
	for discData in jsonData.result["remaining discs"]:
		match int(discData["type"]):
			1:
				horseshoesLeftLabel.text = "Horseshoes left: " + str(discData["count"])
			2:
				robbersLeftLabel.text = "Robbers remaining: " + str(discData["count"])
			3:
				topazesLeftLabel.text = "Topazes remaining: " + str(discData["count"])
			4:
				emeraldsLeftLabel.text = "Emeralds left: " + str(discData["count"])
			5:
				rubiesLeftLabel.text = "Rubies remaining: " + str(discData["count"])
			6:
				if discData["count"] == 0:
					starFoundLabel.text = "Star of Africa found"
				else:
					starFoundLabel.text = "Star not found yet"
			_:
				# for debugging and error handling.
				# For now only print.
				print(str(discData["type"]) + " " + str(discData["count"]))
		empties -= discData["count"]
	emptiesLeftLabel.text = "Empty discs left: " + str(empties)
	
	# move players to their node locations with a helper function
	move_player_to_node(player1RB, int(jsonData.result["players"]["player1"]["location"]))
	move_player_to_node(player2RB, int(jsonData.result["players"]["player2"]["location"]))
	
	# hide nodes if tuen changed
	#print(str(jsonData.result["current player"]) + " " + str(currentPlayer))
	if int(jsonData.result["current player"]) != currentPlayer:
		hide_all_nodes()
		currentPlayer = jsonData.result["current player"]
	
	# show available nodes (only)
	if int(jsonData.result["current player"]) == 0:
		for nodeId in jsonData.result["players"]["player1"]["possible moves"]:
			show_node(int(nodeId))
	else:
		for nodeId in jsonData.result["players"]["player2"]["possible moves"]:
			show_node(int(nodeId))
	
	# hide city discs when they are revealed
	for city in jsonData.result["city discs"]:
		if !jsonData.result["city discs"][city][0]:
			hide_city_disc(int(jsonData.result["city discs"][city][1]))

# move player function
func move_player_to_node(var playerRB : RigidBody, var nodeId : int):
	for nodeObject in nodeIds:
		if nodeObject[0] == nodeId:
			playerRB.translation = nodeObject[1].translation + nodeObject[1].player_offset_vector
			break

# make available nodes visible
func show_node(var nodeId : int):
	for nodeObject in nodeIds:
		if nodeObject[0] == nodeId:
			nodeObject[1].get_child(0).visible = true
			break

# hide all nodes
func hide_all_nodes():
	for nodeObject in nodeIds:
		nodeObject[1].get_child(0).visible = false

# hide city disc when the disc is revealed
func hide_city_disc(var nodeId : int):
	for nodeObject in nodeIds:
		if nodeObject[0] == nodeId:
			nodeObject[1].get_child(1).visible = false

# separate display data function?

# update nodeIdList after GameBoard has loaded them
func update_nodeIds():
	var root = get_tree().root
	nodeIds = root.get_node("/root/GameBoard").nodeIdArray
	#print("GlobalThings.nodeIds.size(): " + str(nodeIds.size()))
