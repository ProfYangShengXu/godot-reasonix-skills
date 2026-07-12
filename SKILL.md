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
- **日志捕获与结构化解析**（`CaptureLogs`, `ParseLogText`, `SummarizeLogs`）
- **键盘输入模拟**（`SimulateKeySequence`, `CreateInputScript`, `ReplayInputScript`）
- **调试闭环编排 + LLM 分析**（`RunDebugSession`, `AnalyzeLogsWithLLM`）
- **GDScript 日志工具模板**（`GenerateLogScript`, `InjectLogStatements`）

### ❌ 不做
- **不做** 可视化编辑器替代品 — Agent 通过文件操作 + CLI 交互
- **不做** 美术资产生成（生成图片/模型/音效）
- **不做** 游戏逻辑引擎 — 不替代 Godot 的 Physics/Animation/Audio 运行时
- **不做** 低代码拖拽界面
- **不做** Godot 3.x 兼容 — MVP 锁定 Godot 4.x

## 技能入口

```bash
# 本技能集注册了4个 Reasonix 子技能，可在管线/循环中按需调用:
#
#   /run_skill godot-dev     游戏开发工具集（M1-M11 全套接口）
#   /run_skill godot-test    测试运行器（U/I/S/A 四层 vitest）
#   /run_skill godot-docs    产品/架构文档生成
#   /run_skill godot-debug   调试闭环（日志+输入模拟+LLM分析）
#
# 上手:
#   /pipeline 用 Godot 做一个平台跳跃游戏
#   或直接:
#   npx tsx tools/template_library.ts fork platformer ./my-game "MyGame"
```

## 工具清单

| 模块 | 工具名 | 说明 |
|------|--------|------|
| M1 | `ScaffoldProject` | 创建项目骨架 + project.godot + 目录结构 |
| M2 | `CreateScene` / `AddNode` / `RemoveNode` / `SetNodeProperty` | .tscn 场景增删改 |
| M3 | `CreateScript` / `AttachScript` / `ConnectSignal` | GDScript 生成与挂载 |
| M4 | `ImportResource` / `ResolvePath` | 资源路径管理 |
| M5 | `ListTemplates` / `ForkTemplate` / `CustomizeTemplate` | 模板库 |
| M6 | `RunGodot` / `CheckGodotVersion` | Godot CLI 桥 |
| M7 | `GetSkillDoc` / `FindPlaybook` / `GetScopeBoundary` | 技能文档 |
| M8 | `CaptureLogs` / `ParseLogText` / `SummarizeLogs` | 日志捕获与解析 |
| M9 | `SimulateKeySequence` / `CreateInputScript` / `ReplayInputScript` | 输入模拟 |
| M10 | `RunDebugSession` / `AnalyzeLogsWithLLM` / `ListDebugSessions` | 调试编排 |
| M11 | `GenerateLogScript` / `InjectLogStatements` | GDScript 日志工具 |

## 快速上手指南

### 1. 从模板开始（配管线）
```
/pipeline 从平台跳跃模板开始，改成我的游戏
```
Agent 走 Bobanana 管线：架构师拆任务 → 开发 ForkTemplate → 测试 → 评判 → 挑刺。

### 2. 从零构建
```
用户说: "帮我创建一个新的 Godot 项目，加一个能左右移动的蓝色方块"
Agent 做:
  1. ScaffoldProject({ root_path: "./my-game", project_name: "My Game" })
  2. CreateScript({ path: "my-game/scripts/player.gd", extends: "CharacterBody2D", ... })
  3. CreateScene({ path: "my-game/scenes/Player.tscn", root_node: { ... } })
  4. AttachScript({ scene_path: "my-game/scenes/Player.tscn", node_path: "Player", script_path: "res://scripts/player.gd" })
```

### 3. 调试
```
/loop 角色跳起来卡在地板里，帮我找出问题
```
Agent 启动调试闭环：RunDebugSession → 捕获日志 → LLM 分析 → 定位根因 → 修复 → 再验证。

### 4. 迭代修改
```
用户说: "把跳跃力调高一点"
Agent 做:
  SetNodeProperty({ scene_path: "...", node_path: "Player", properties: { jump_velocity: -500 } })
  或者修改 .gd 文件中的 @export 默认值
```

## 相关文档
- `.reasonix/skills/godot-dev/SKILL.md` — 技能化入口
- `.reasonix/skills/godot-test/SKILL.md` — 测试入口
- `.reasonix/skills/godot-docs/SKILL.md` — 文档入口
- `.reasonix/skills/godot-debug/SKILL.md` — 调试入口
- `docs/playbooks/` — 自然语言→工具链映射示例
- `docs/prd/v1/prd.yaml` — 结构化技术 PRD
- `docs/product/godot-reasonix-product-prd.html` — 产品设计文档
- `docs/product/godot-reasonix-debugger-product-prd.html` — 调试模块产品设计文档
