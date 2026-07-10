extends CharacterBody2D

@export var initial_speed: float = 400.0

var direction: Vector2 = Vector2.ZERO


func _ready() -> void:
	reset_ball()


func _physics_process(delta: float) -> void:
	velocity = direction * initial_speed
	move_and_slide()
	
	# Wall bouncing
	if position.x < 10 or position.x > 790:
		direction.x *= -1
	if position.y < 10:
		direction.y *= -1
	
	# Bottom boundary — lose ball
	if position.y > 610:
		reset_ball()


func reset_ball() -> void:
	position = Vector2(400, 400)
	direction = Vector2(1, -1).normalized()


func _on_body_entered(body: Node) -> void:
	if body.is_in_group("paddle"):
		# Bounce based on where ball hits paddle
		var hit_offset: float = (position.x - body.position.x) / 64.0
		direction = Vector2(hit_offset, -1).normalized()
	elif body.is_in_group("brick"):
		direction.y *= -1
