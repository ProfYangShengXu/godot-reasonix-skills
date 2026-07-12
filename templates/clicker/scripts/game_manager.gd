extends Node2D

@export var collectible_scene: PackedScene
@export var spawn_interval: float = 2.0

var score: int = 0
var spawn_timer: float = 0.0

@onready var score_label: Label = $UI/ScoreLabel


func _ready() -> void:
	print("LOG:INFO:Clicker game started")
	# Spawn initial items
	for i in 3:
		spawn_item()


func _process(delta: float) -> void:
	spawn_timer += delta
	if spawn_timer >= spawn_interval:
		spawn_timer = 0.0
		spawn_item()


func spawn_item() -> void:
	if collectible_scene:
		var item: Area2D = collectible_scene.instantiate()
		item.collected.connect(_on_item_collected)
		add_child(item)


func _on_item_collected(points: int) -> void:
	score += points
	print("LOG:INFO:Item collected, score=" + str(score))
	score_label.text = "Score: " + str(score)
