# Playbook: 从模板创建项目

**用户意图**: "我想做一个打砖块游戏，就用模板吧"

**工具链**:

```yaml
- step: 1
  tool: ListTemplates
  input: {}
  # → Agent 选择 "breakout"

- step: 2
  tool: ForkTemplate
  input:
    template_id: breakout
    target_path: ./my-breakout
    project_name: "My Breakout"

- step: 3
  tool: CheckGodotVersion
  input: {}
  # 确认 Godot 版本兼容

- step: 4
  tool: RunGodot
  input:
    project_root: ./my-breakout
    args: ["--headless", "--check"]
  # 验证模板可加载
```

**变体**: 用户说"把打砖块改成红色主题"
```yaml
- step: CustomizeTemplate
  input:
    project_root: ./my-breakout
    overrides: { speed: 600, gravity: 1200 }
```
