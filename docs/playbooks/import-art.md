# Playbook: 导入美术资源

**用户意图**: "我下了一张角色的 png 图片，帮我用到游戏里"

**工具链**:

```yaml
- step: 1
  tool: ImportResource
  input:
    source_path: /Users/me/Downloads/character.png
    dest_type: texture
    project_root: ./my-game

- step: 2
  tool: SetNodeProperty
  input:
    scene_path: "./my-game/scenes/Player.tscn"
    node_path: "Player/Sprite2D"
    properties:
      texture: "res://assets/textures/character.png"
```

**完整流程**: 导入 + 创建使用资源的场景
```yaml
- step:
  tool: ImportResource
  input: { source_path: "hero.png", dest_type: texture, project_root: "./game" }
  
- step:
  tool: CreateScene
  input:
    path: "./game/scenes/Hero.tscn"
    root_node:
      type: CharacterBody2D
      name: Hero
      children:
        - type: Sprite2D
          name: Sprite
          properties: { texture: "res://assets/textures/hero.png" }
```
