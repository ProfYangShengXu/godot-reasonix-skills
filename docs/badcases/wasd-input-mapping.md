# Badcase: WASD 输入映射不生效

## 症状

角色可见，但 WASD 按键无响应。

## 排查过程

### 第 1 轮：怀疑 project.godot [input] 段被 Godot 编辑器写坏

**操作**：删除 `[input]` 段，依赖 Godot 内置默认输入映射  
**结果**：❌ Godot 4.7 默认只有方向键无 WASD，WASD 仍不工作

### 第 2 轮：重写 project.godot [input] 段

**操作**：用以下格式写入 W/A/S/D 映射：
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
**结果**：❌ Action 存在但 keycodes 为空（`InputMap.action_get_events()` 返回空数组）
→ Godot 4.7 解析此格式时丢弃了 events 数组

### 第 3 轮：脚本运行时添加

**操作**：在 `_ready()` 中用 `InputMap.action_add_event()` 动态绑定 WASD  
**结果**：✅ WASD 正常工作

## 根因

`project.godot` 的 `[input]` 段 JSON 格式与 Godot 4.7 实际解析器不兼容。
Godot 4.7 将 action 创建出来（`has_action()=true`）但丢弃了 events 数组，
导致 action 存在却零绑定。

## 修复

放弃在 `project.godot` 中定义 `[input]`，改为在脚本 `_ready()` 中动态绑定：

```gdscript
func _ready() -> void:
    _add_key_to_action("ui_left", KEY_A)
    _add_key_to_action("ui_right", KEY_D)
    _add_key_to_action("ui_up", KEY_W)
    _add_key_to_action("ui_down", KEY_S)

func _add_key_to_action(action: StringName, keycode: int) -> void:
    for event in InputMap.action_get_events(action):
        if event is InputEventKey and event.keycode == keycode:
            return
    var event = InputEventKey.new()
    event.keycode = keycode
    InputMap.action_add_event(action, event)
```

## 教训

1. **`project.godot` 的 `[input]` 段格式不可靠** — Godot 4.x 各小版本解析行为不同
2. **优先用 GDScript API 操作 InputMap** — `action_add_event()` 是官方推荐方式
3. **Godot 4.7 默认输入映射只有方向键**，不包含 WASD
4. **调试方法**：用 `extends SceneTree` 脚本 + `--script` 启动，打印 `InputMap.action_get_events()`

## 关联问题

- 相机跟随 + 纯色背景 → 无法感知移动 → 需固定相机 + 地面着色
- CollisionShape2D.shape 需 SubResource 而非 Vector2
- ext_resource 的 uid 属性须用 hex 格式，否则 Godot 警告
