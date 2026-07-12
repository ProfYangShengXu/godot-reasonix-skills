extends CharacterBody2D

@export var speed: int = 300
@export var jump_velocity: float = -400.0

# Get the gravity from project settings
var gravity: int = ProjectSettings.get_setting("physics/2d/default_gravity")


func _ready() -> void:
	print("LOG:INFO:Player ready at position ", position)


func _physics_process(delta: float) -> void:
	# Add gravity
	if not is_on_floor():
		velocity.y += gravity * delta

	# Handle jump
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = jump_velocity
		print("LOG:INFO:Player jumped (velocity=" + str(jump_velocity) + ")")

	# Get horizontal input
	var direction: float = Input.get_axis("ui_left", "ui_right")

	# Apply horizontal movement
	if direction:
		velocity.x = direction * speed
	else:
		velocity.x = move_toward(velocity.x, 0, speed)

	move_and_slide()
	
	# Log wall/floor collision
	if is_on_wall():
		print("LOG:INFO:Player touching wall")
	if is_on_floor():
		print("LOG:INFO:Player on floor (pos=" + str(position.y) + ")")
