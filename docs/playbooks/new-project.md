# Playbook: 从零创建新项目

**用户意图**: "帮我创建一个新的 Godot 平台跳跃游戏项目"

**工具链**:

```yaml
- step: 1
  tool: ScaffoldProject
  input:
    root_path: ./my-platformer
    project_name: "My Platformer"
    resolution: { width: 1152, height: 648 }
    renderer: forward_plus
  output: project_root

- step: 2
  tool: CreateScript
  input:
    path: "{project_root}/scripts/player.gd"
    extends: CharacterBody2D
    variables:
      - { name: speed, type: int, export: true, default: "300" }
      - { name: jump_velocity, type: float, export: true, default: "-400.0" }
    functions:
      - name: _physics_process
        args: "delta: float"
        body: |
          if not is_on_floor():
            velocity.y += gravity * delta
          if Input.is_action_just_pressed("ui_accept") and is_on_floor():
            velocity.y = jump_velocity
          var direction = Input.get_axis("ui_left", "ui_right")
          velocity.x = direction * speed if direction else move_toward(velocity.x, 0, speed)
          move_and_slide()
  output: script_path

- step: 3
  tool: CreateScene
  input:
    path: "{project_root}/scenes/Player.tscn"
    root_node:
      type: CharacterBody2D
      name: Player
      children:
        - type: CollisionShape2D
          name: CollisionShape
          properties: { shape: RectangleShape2D }

- step: 4
  tool: AttachScript
  input:
    scene_path: "{project_root}/scenes/Player.tscn"
    node_path: "Player"
    script_path: "res://scripts/player.gd"

- step: 5
  tool: CreateScene
  input:
    path: "{project_root}/scenes/Level.tscn"
    root_node:
      type: Node2D
      name: Level
      children:
        - type: StaticBody2D
          name: Ground
          position: { x: 576, y: 632 }
          children:
            - type: CollisionShape2D
              name: Shape

- step: 6
  tool: RunGodot
  input:
    project_root: "{project_root}"
    args: ["--headless", "--check"]
  output: "验证项目加载无误"
```

**边界声明**: 此 Playbook 生成的是最简结构，不含 UI、分数、音效。用户可在此基础上增量添加。
