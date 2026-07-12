extends CharacterBody2D

@export var initial_speed: float = 400.0

var direction: Vector2 = Vector2.ZERO


func _ready() -> void:
	print("LOG:INFO:Ball ready at ", position)
	reset_ball()


func _physics_process(delta: float) -> void:
	velocity = direction * initial_speed
	move_and_slide()
	
	# Wall bouncing
	if position.x < 10 or position.x > 790:
		direction.x *= -1
		print("LOG:INFO:Ball bounced off wall (x=" + str(position.x) + ")")
	if position.y < 10:
		direction.y *= -1
		print("LOG:INFO:Ball bounced off ceiling")
	
	# Bottom boundary — lose ball
	if position.y > 610:
		print("LOG:INFO:Ball lost at bottom (pos=" + str(position.y) + ")")
		reset_ball()


func reset_ball() -> void:
	position = Vector2(400, 400)
	direction = Vector2(1, -1).normalized()
	print("LOG:INFO:Ball reset to center")


func _on_body_entered(body: Node) -> void:
	if body.is_in_group("paddle"):
		# Bounce based on where ball hits paddle
		var hit_offset: float = (position.x - body.position.x) / 64.0
		direction = Vector2(hit_offset, -1).normalized()
		print("LOG:INFO:Ball hit paddle at offset " + str(hit_offset))
	elif body.is_in_group("brick"):
		direction.y *= -1
		print("LOG:INFO:Ball hit brick, bouncing")
