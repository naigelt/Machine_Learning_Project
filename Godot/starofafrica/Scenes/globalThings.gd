extends Node

## Globals

# UI label connections for displaying text
# Player 1
var player1MoneyText : Label
var player1WinConditionText : Label
var player1LastDiceRollText : Label

# Player 2
var player2MoneyText : Label
var player2WinConditionText : Label
var player2LastDiceRollText : Label

# Board status
var emptiesLeftText : Label
var horseshoesLeftText : Label
var robbersLeftText : Label
var topazesLeftText : Label
var emeraldsLeftText : Label
var rubiesLeftText : Label
var starFoundText : Label


func _ready():
	# assign variables here
	pass

# function to handle parsed JSON and do appropriate things with the content
func handleJSON(data : JSONParseResult):
	print("doing stuff with parsed data")
	
	"""
	GlobalThings.player1Container.get_child(1).text = "Money: " + str(jsonData.result["players"]["player1"]["money"])
	GlobalThings.player1Container.get_child(3).text = "Latest dice roll: " + str(jsonData.result["players"]["player1"]["diceroll"])
	
	GlobalThings.player2Container.get_child(1).text = "Money: " + str(jsonData.result["players"]["player2"]["money"])
	GlobalThings.player2Container.get_child(3).text = "Latest dice roll: " + str(jsonData.result["players"]["player2"]["diceroll"])
	
	# JSON arrays retain the order of their elements, so using indices here should be fine
	var horseshoes = jsonData.result["remaining discs"][3]["count"]
	var robbers = jsonData.result["remaining discs"][2]["count"]
	var topazes = jsonData.result["remaining discs"][0]["count"]
	var emeralds = jsonData.result["remaining discs"][4]["count"]
	var rubies = jsonData.result["remaining discs"][1]["count"]
	var star = jsonData.result["remaining discs"][5]["count"]
	var empties = 30 - (horseshoes + robbers + topazes + emeralds + rubies + star)
	GlobalThings.boardDataContainer.get_child(1).text = "Empty discs left: " + str(empties)
	GlobalThings.boardDataContainer.get_child(2).text = "Horseshoes left: " + str(horseshoes)
	GlobalThings.boardDataContainer.get_child(3).text = "Robbers remaining: " + str(robbers)
	GlobalThings.boardDataContainer.get_child(4).text = "Topazes remaining: " + str(topazes)
	GlobalThings.boardDataContainer.get_child(5).text = "Emeralds left: " + str(emeralds)
	GlobalThings.boardDataContainer.get_child(6).text = "Rubies remaining: " + str(rubies)
	if star == 0:
		GlobalThings.boardDataContainer.get_child(7).text = "Star of Africa found"
	else:
		GlobalThings.boardDataContainer.get_child(7).text = "Star not found yet"
	
	"""

# move player function?

# display data function?
