# Godot 4.x 格式参考 — 官方文档整合

**用途**：技能集开发时的规范依据。所有工具生成的 Godot 文件必须符合以下格式。

---

## 1. project.godot 文件格式

### 基础结构

```ini
config_version=5

[application]
config/name="Project Name"
run/main_scene="res://scenes/Main.tscn"
config/features=PackedStringArray("4.7")
config/icon="res://icon.svg"

[display]
window/size/viewport_width=800
window/size/viewport_height=600

[rendering]
renderer/rendering_method="forward_plus"
```

### [input] 段 — 输入映射

**官方依据**: `InputMap` class docs (add_action deadzone default = 0.2)

每个 action 是一个对象，含 `deadzone` 和 `events` 数组：

```ini
[input]

action_name={
"deadzone": 0.5,
"events": [{
"keycode": 65,
"type": 0
}]
}
```

- `"type": 0` = `InputEventKey`
- `"type": 1` = `InputEventMouseButton`  
- `"type": 3` = `InputEventJoypadButton`
- `"type": 4"` = `InputEventJoypadMotion`

**正确格式 — 单层 events 数组**（两层嵌套会导致 Godot 编辑器保存时损坏）：

```ini
ui_left={
"deadzone": 0.5,
"events": [{
"keycode": 65,
"type": 0
}, {
"keycode": 4194319,
"type": 0
}]
}
```

**Key 常量与数值对照**（来自 `Key` enum）：

| 键 | 常量 | 数值 |
|----|------|------|
| A | `KEY_A` | 65 |
| D | `KEY_D` | 68 |
| W | `KEY_W` | 87 |
| S | `KEY_S` | 83 |
| Space | `KEY_SPACE` | 32 |
| Shift | `KEY_SHIFT` | 4194306 |
| Ctrl | `KEY_CTRL` | 4194307 |
| Alt | `KEY_ALT` | 4194308 |
| Enter | `KEY_ENTER` | 4194309 |
| Escape | `KEY_ESCAPE` | 4194305 |
| Left Arrow | `KEY_LEFT` | 4194319 |
| Right Arrow | `KEY_RIGHT` | 4194321 |
| Up Arrow | `KEY_UP` | 4194320 |
| Down Arrow | `KEY_DOWN` | 4194322 |

### 默认输入映射（Godot 4.7 内置）

**实测结果**（通过 `InputMap.get_actions()` 在 Godot 4.7 验证）：

- `ui_left`: 左方向键 (4194319)
- `ui_right`: 右方向键 (4194321)
- `ui_up`: 上方向键 (4194320)
- `ui_down`: 下方向键 (4194322)
- `ui_accept`: Enter (4194309), 数字键盘Enter (4194310), Space (32)
- `ui_cancel`: Escape (4194305)
- `ui_select`: Space (32)
- `ui_focus_next`: Tab (4194306)
- `ui_focus_prev`: Shift+Tab (4194306)

**⚠️ 重要**: Godot 4.7 的默认输入映射**不包含 WASD**！
  - 如果需要 WASD 移动，必须在 `[input]` 段手动添加
  - 或者用 `InputMap.action_add_event()` 在 GDScript 中运行时添加

---

## 2. .tscn 场景文件格式

### 完整结构示例

```tscn
[gd_scene load_steps=5 format=3 uid="uid://b1c2a3d4e5f6g"]

[ext_resource type="Script" path="res://scripts/player.gd" id="1_pl1234"]
[ext_resource type="PackedScene" path="res://scenes/Other.tscn" id="2_other"]

[sub_resource type="RectangleShape2D" id="RectangleShape2D_abcd1"]
size = Vector2(32, 48)

[node name="Player" type="CharacterBody2D"]
script = ExtResource("1_pl1234")

[node name="CollisionShape" type="CollisionShape2D" parent="."]
shape = SubResource("RectangleShape2D_abcd1")

[node name="Camera2D" type="Camera2D" parent="."]

[node name="Sprite" type="Sprite2D" parent="Player"]
texture = ExtResource("2_other")

[connection signal="body_entered" from="." to="." method="_on_body_entered"]
```

### 头部 `[gd_scene]`

| 属性 | 说明 |
|------|------|
| `format=3` | Godot 4.x 文本格式版本号（固定 3） |
| `load_steps=N` | 总加载步骤数 = 1 + ext资源数 + sub资源数 + 节点数 |
| `uid` | 场景的唯一 ID（可选，Godot 自动生成） |

### 外部资源 `[ext_resource]`

```tscn
[ext_resource type="Script" path="res://scripts/player.gd" id="1_pl1234"]
[ext_resource type="PackedScene" uid="uid://abc123" path="res://scenes/Player.tscn" id="2_scene"]
[ext_resource type="Texture2D" path="res://assets/textures/icon.png" id="3_tex"]
```

- `type`: Godot 资源类名（Script, PackedScene, Texture2D, FontFile 等）
- `id`: Godot 内部引用 ID，格式 `数字_6字符标识` (如 `1_pl1234`)
- `uid`: 可选，资源的目标 UID。**未经 Godot 验证的自定义 UID 会导致警告**，建议留空让 Godot 自动分配
- `path`: 资源的 `res://` 路径

### 子资源 `[sub_resource]`

```tscn
[sub_resource type="RectangleShape2D" id="RectangleShape2D_abcd1"]
size = Vector2(32, 48)
```

- `type`: Godot 资源类名（不继承 Node，而是 Resource 子类）
- `id`: 格式 `TypeName_6字符标识`

常见 SubResource 类型：

| 类型 | 属性 |
|------|------|
| `RectangleShape2D` | `size = Vector2(w, h)` |
| `CircleShape2D` | `radius = float` |
| `CapsuleShape2D` | `radius`, `height` |
| `WorldBoundaryShape2D` | `plane = Plane(...)` |
| `ConcavePolygonShape2D` | `segments = PackedVector2Array(...)` |
| `ConvexPolygonShape2D` | `points = PackedVector2Array(...)` |
| `AnimationNodeBlendTree` | 复杂结构 |
| `Gradient` | `offsets`, `colors` |

### 节点 `[node]`

```tscn
[node name="NodeName" type="NodeType" parent="ParentName"]
position = Vector2(100, 200)
scale = Vector2(2, 1)
script = ExtResource("1_script")
```

- `parent="."` 表示根节点
- `instance=ExtResource("id")` 表示实例化另一个场景
- 属性名=Godot 表达式（Vector2, Color, float, int, string, bool 等）
- 对 `CollisionShape2D.shape` 等资源类型属性，**必须使用 `SubResource` 或 `ExtResource`**，不能直接用 `Vector2(w, h)`

**实例化场景**：
```tscn
[node name="PlayerInstance" parent="." instance=ExtResource("1_main1")]
position = Vector2(400, 300)  # 可覆盖实例的位置
```

### 信号连接 `[connection]`

```tscn
[connection signal="body_entered" from="Area2D" to="." method="_on_body_entered"]
[connection signal="pressed" from="Button" to="." method="_on_button_pressed" flags=0]
```

---

## 3. GDScript 语法要点

### 官方参考
- GDScript reference: https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_basics.html
- GDScript exports: https://docs.godotengine.org/en/stable/tutorials/scripting/gdscript/gdscript_exports.html

### `@export` 语法（Godot 4.x）

```gdscript
@export var speed: int = 300
@export var jump_velocity: float = -400.0
@export var color: Color = Color(1, 0, 0, 1)
@export var node_ref: Node
```

⚠️ **Godot 4.x `@export` 语法**是 `@export var name: type = default`
- ❌ 旧版 `export var` 不再支持
- ❌ `@export int var speed` 是错误语法（C# 风格）

### 输入处理

```gdscript
# 推荐方式（Godot 4.x）:
var direction = Input.get_axis("ui_left", "ui_right")
var direction_2d = Vector2(
    Input.get_axis("ui_left", "ui_right"),
    Input.get_axis("ui_up", "ui_down")
)

# 手动创建输入映射（运行时）:
func _ready():
    if not InputMap.has_action("my_action"):
        InputMap.add_action("my_action")
        var event = InputEventKey.new()
        event.keycode = KEY_SPACE
        InputMap.action_add_event("my_action", event)
```

### `_draw()` 注意事项
- `_draw()` 在节点首次进入场景树时**自动调用**
- 每次修改后需要调用 `queue_redraw()`
- 在 `_ready()` 中调用 `queue_redraw()` 可以触发重绘
- 使用 `Polygon2D` 节点是更稳定的替代方案（不依赖 `_draw()`）

---

## 4. ResourceUID 格式

**官方依据**: `ResourceUID` class docs

- UID 是一个整数，用 `ResourceUID.create_id()` 生成
- 文本格式: `uid://<hex_string>`
- `ResourceUID.INVALID_ID = -1` 表示无效 UID
- UID 在项目中被映射到资源文件路径

**UID 生成规则**（Godot 4.x 内部）：
- 6 随机字节编码为 13 字符
- 字符集: `0-9a-z`（36 个字符）
- `ResourceUID.id_to_text(id)` 将 int 转为 `uid://...` 字符串

**最佳实践**：不在手动编写的 `.tscn` 或 `project.godot` 中写入 uid 属性，
让 Godot 编辑器第一次打开时自动分配。如需预置，使用 `ResourceUID.create_id_for_path(path)` 生成确定性 UID。

---

## 5. 常见错误与排查

| 错误 | 原因 | 修复 |
|------|------|------|
| 看不见角色 | 脚本未挂载 / 初始位置在墙内 / 无 Camera2D | `script = ExtResource("id")` + 安全出生点 + Camera2D |
| WASD 没反应 | 默认 input map 无 WASD | 在 `[input]` 段添加 W/A/S/D 映射 |
| `Parameter "which" is null` | ext_resource UID 格式不对 / 场景未正确实例化 | 去掉手动 UID 或使用正确的 hex 格式 |
| `invalid UID` 警告 | uid 字符串不符合 `^[0-9a-z]{13}$` | 去掉 `uid` 属性，或使用 `ResourceUID.create_id()` |
| `shape = Vector2(...)` 不生效 | CollisionShape2D.shape 需 SubResource | 改用 `shape = SubResource("Rect_xxx")` |
| Godot 改写 project.godot | 输入自定义格式 | 使用与 Godot 内部一致的 JSON 格式 |

---

*参考 Godot Engine 4.7 stable 官方文档，实测验证于 2026-07*
