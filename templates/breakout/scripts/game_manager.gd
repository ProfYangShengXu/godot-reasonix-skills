extends Node2D

@export var ball_scene: PackedScene
@export var brick_scene: PackedScene

var score: int = 0
var lives: int = 3

@onready var score_label: Label = $UI/ScoreLabel
@onready var lives_label: Label = $UI/LivesLabel
@onready var ball: CharacterBody2D = $Ball


func _ready() -> void:
	# Create bricks grid
	for row in 5:
		for col in 8:
			var brick: StaticBody2D = brick_scene.instantiate()
			brick.position = Vector2(60 + col * 85, 60 + row * 30)
			add_child(brick)
			brick.body_entered.connect(_on_brick_hit.bind(brick))
	
	update_ui()


func _on_brick_hit(_body: Node, brick: StaticBody2D) -> void:
	brick.queue_free()
	score += 10
	update_ui()


func update_ui() -> void:
	score_label.text = "Score: " + str(score)
	lives_label.text = "Lives: " + str(lives)
