extends Area2D

@export var value: int = 1
@export var lifetime: float = 5.0

signal collected(points: int)

var time_left: float


func _ready() -> void:
	time_left = lifetime
	# Random position within view
	position = Vector2(
		randf_range(50, 750),
		randf_range(50, 550)
	)
	
	# Animate appearance
	scale = Vector2.ZERO
	var tween: Tween = create_tween()
	tween.tween_property(self, "scale", Vector2.ONE, 0.3)


func _process(delta: float) -> void:
	time_left -= delta
	if time_left <= 0:
		# Fade out and remove
		var tween: Tween = create_tween()
		tween.tween_property(self, "modulate", Color.TRANSPARENT, 0.3)
		tween.tween_callback(queue_free)
		time_left = lifetime + 5  # prevent duplicate calls


func _on_input_event(_viewport: Node, event: InputEvent, _shape_idx: int) -> void:
	if event is InputEventMouseButton and event.pressed and event.button_index == MOUSE_BUTTON_LEFT:
		collected.emit(value)
		# Collect animation
		var tween: Tween = create_tween()
		tween.tween_property(self, "scale", Vector2(1.5, 1.5), 0.15)
		tween.tween_property(self, "modulate", Color.TRANSPARENT, 0.2)
		tween.tween_callback(queue_free)
