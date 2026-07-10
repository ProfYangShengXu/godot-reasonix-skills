# Godot Reasonix 技能集

让编程 Agent 操控 Godot 引擎，零门槛开发游戏。

> **定位**: 像 VSCode Copilot 之于代码 —— 但之于 Godot 编辑器。
> **一句话**: 一套 Agent 可调用的工具集，用户用自然语言描述游戏想法，Agent 自动完成 Godot 项目的搭建、场景编排、脚本编写、资源管理。

## 能力边界（Scope Boundary）

### ✅ 能做
- 创建 Godot 4.x 项目骨架（`ScaffoldProject`）
- 生成/编辑 .tscn 场景文件（`CreateScene`, `AddNode`, `RemoveNode`, `SetNodeProperty`）
- 生成 .gd GDScript 并挂载到场景节点（`CreateScript`, `AttachScript`, `ConnectSignal`）
- 管理资源导入和路径解析（`ImportResource`, `ResolvePath`）
- 从三个内置模板 Fork 完整游戏项目（`ListTemplates`, `ForkTemplate`, `CustomizeTemplate`）
- 调用 godot CLI 进行验证和导出（`RunGodot`, `CheckGodotVersion`）

### ❌ 不做
- **不做** 可视化编辑器替代品 — Agent 通过文件操作 + CLI 交互
- **不做** 美术资产生成（生成图片/模型/音效）
- **不做** 游戏逻辑引擎 — 不替代 Godot 的 Physics/Animation/Audio 运行时
- **不做** 低代码拖拽界面
- **不做** Godot 3.x 兼容 — MVP 锁定 Godot 4.x

## 工具清单

| 模块 | 工具名 | 说明 |
|------|--------|------|
| M1 | `ScaffoldProject` | 创建项目骨架 + project.godot + 目录结构 |
| M2 | `CreateScene` | 创建 .tscn 场景文件（含节点树） |
| M2 | `AddNode` | 向已有场景添加子节点 |
| M2 | `RemoveNode` | 从场景移除节点 |
| M2 | `SetNodeProperty` | 修改节点属性 |
| M3 | `CreateScript` | 创建 .gd GDScript 脚本 |
| M3 | `AttachScript` | 将脚本挂载到场景节点 |
| M3 | `ConnectSignal` | 建立信号连接 |
| M4 | `ImportResource` | 导入外部资源到项目 |
| M4 | `ResolvePath` | 双向路径解析（res:// ↔ fs） |
| M5 | `ListTemplates` | 列出内置模板 |
| M5 | `ForkTemplate` | 复制模板到目录 |
| M5 | `CustomizeTemplate` | 定制模板参数 |
| M6 | `RunGodot` | 运行 godot CLI 命令 |
| M6 | `CheckGodotVersion` | 检测 Godot 版本兼容性 |
| M7 | `GetSkillDoc` | 获取技能集完整文档 |
| M7 | `FindPlaybook` | 查询匹配的 playbook 示例 |
| M7 | `GetScopeBoundary` | 获取能力边界声明 |

## 快速上手指南

### 1. 从模板开始
```
用户说: "做个平台跳跃游戏"
Agent 做:
  ForkTemplate({ template_id: "platformer", target_path: "./my-platformer", project_name: "My Platformer" })
  RunGodot({ project_root: "./my-platformer", args: ["--headless", "--check"] })
  → 项目就绪，用户可直接在 Godot 编辑器中打开运行
```

### 2. 从零构建
```
用户说: "帮我创建一个新的 Godot 项目，加一个能左右移动的蓝色方块"
Agent 做:
  1. ScaffoldProject({ root_path: "./my-game", project_name: "My Game" })
  2. CreateScript({ path: "my-game/scripts/player.gd", extends: "CharacterBody2D", ... })
  3. CreateScene({ path: "my-game/scenes/Player.tscn", root_node: { ... } })
  4. AttachScript({ scene_path: "my-game/scenes/Player.tscn", node_path: "Player", script_path: "res://scripts/player.gd" })
```

### 3. 迭代修改
```
用户说: "把跳跃力调高一点"
Agent 做:
  SetNodeProperty({ scene_path: "...", node_path: "Player", properties: { jump_velocity: -500 } })
  或者修改 .gd 文件中的 @export 默认值
```

## 相关文档
- `docs/playbooks/` — 自然语言→工具链映射示例
- `docs/prd/v1/prd.yaml` — 结构化技术 PRD
- `docs/product/godot-reasonix-product-prd.html` — 产品设计文档
