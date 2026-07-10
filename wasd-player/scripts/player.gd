extends CharacterBody2D

@export var speed: int = 300


func _ready() -> void:
	# 把 WASD 加到 Godot 内置的 ui_ 映射上
	_add_key_to_action("ui_left", KEY_A)
	_add_key_to_action("ui_right", KEY_D)
	_add_key_to_action("ui_up", KEY_W)
	_add_key_to_action("ui_down", KEY_S)


func _add_key_to_action(action: StringName, keycode: int) -> void:
	# 检查这个键是否已经绑定了
	for event in InputMap.action_get_events(action):
		if event is InputEventKey and event.keycode == keycode:
			return  # 已存在，跳过
	var event = InputEventKey.new()
	event.keycode = keycode
	InputMap.action_add_event(action, event)


func _physics_process(_delta: float) -> void:
	var direction = Vector2(
		Input.get_axis("ui_left", "ui_right"),
		Input.get_axis("ui_up", "ui_down")
	)
	
	if direction.length() > 0:
		direction = direction.normalized()
	
	velocity = direction * speed
	move_and_slide()
