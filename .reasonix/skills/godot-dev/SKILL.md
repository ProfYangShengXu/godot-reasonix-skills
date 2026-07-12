---
name: godot-dev
description: 🎮 [Godot] 游戏开发工具集 — 脚手架/场景/脚本/资源/模板/调试工具，Agent 可调用的 M1-M11 全套接口
runAs: inline
profiles: balanced, delivery
cost: high
---

# godot-dev — Godot 游戏开发工具集

**先读** `docs/playbooks/` 下的 playbook 了解自然语言→工具链的映射示例。

## 能力边界

### ✅ 能做
- 创建 Godot 4.x 项目骨架 + `project.godot` 配置
- 生成/编辑 `.tscn` 场景文件（增删节点、改属性）
- 生成 `.gd` GDScript 并挂载到场景节点
- 管理资源导入和路径解析
- 三个内置模板可 Fork（平台跳跃/打砖块/点击收集）
- godot CLI 调用（运行/验证/导出）
- **游戏运行日志捕获与结构化解析**
- **键盘输入模拟（跨平台）**
- **调试闭环编排 + LLM 分析**
- **生成 GDScript 日志工具模板**

### ❌ 不做
- 美术资产生成（图片/模型/音效）
- 可视化编辑器替代品
- Godot 3.x 兼容

## 工具清单

| 模块 | 工具 | 说明 |
|------|------|------|
| M1 | `ScaffoldProject` | 创建项目骨架 + project.godot |
| M2 | `CreateScene` / `AddNode` / `RemoveNode` / `SetNodeProperty` | 场景文件操作 |
| M3 | `CreateScript` / `AttachScript` / `ConnectSignal` | GDScript 管理 |
| M4 | `ImportResource` / `ResolvePath` | 资源路径管理 |
| M5 | `ListTemplates` / `ForkTemplate` / `CustomizeTemplate` | 模板库 |
| M6 | `RunGodot` / `CheckGodotVersion` | Godot CLI 桥 |
| M7 | `GetSkillDoc` / `FindPlaybook` / `GetScopeBoundary` | 文档 |
| M8 | `CaptureLogs` / `ParseLogText` / `SummarizeLogs` | 日志捕获与解析 |
| M9 | `SimulateKeySequence` / `CreateInputScript` / `ReplayInputScript` | 输入模拟 |
| M10 | `RunDebugSession` / `AnalyzeLogsWithLLM` / `ListDebugSessions` | 调试编排 |
| M11 | `GenerateLogScript` / `InjectLogStatements` | GDScript 日志工具 |

## 快速入口

```bash
# 从模板创建游戏
npx tsx tools/template_library.ts fork platformer ./my-game "MyGame"

# 运行日志捕获
npx tsx tools/log_capture.ts ./my-game --args --headless

# 启动调试会话
npx tsx tools/debug_orchestrator.ts run ./my-game --duration 15

# 生成日志工具
npx tsx tools/log_utils.ts generate --project-root ./my-game
```

## 相关文档
- `tools/` — TypeScript 源码实现
- `docs/prd/v1/prd.yaml` — 结构化技术 PRD
- `docs/product/` — 产品设计文档
- `docs/playbooks/` — 自然语言→工具链映射
