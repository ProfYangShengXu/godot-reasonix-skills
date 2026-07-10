extends CharacterBody2D

@export var speed: int = 500

func _physics_process(_delta: float) -> void:
	var direction: float = Input.get_axis("ui_left", "ui_right")
	
	if direction:
		velocity.x = direction * speed
	else:
		velocity.x = move_toward(velocity.x, 0, speed)
	
	move_and_slide()
