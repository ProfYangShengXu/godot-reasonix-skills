# Godot Reasonix Skills

让 AI 编程 Agent 操控 Godot 引擎，辅助游戏开发
可 配套reasonix插件
https://github.com/ProfYangShengXu/bobanana4.0.git 使用

[![Tests](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml/badge.svg)](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml)
![Godot](https://img.shields.io/badge/Godot-4.7-478cbf?logo=godot-engine&logoColor=white)
![TypeScript](https://img.shields.io/badge/TypeScript-5.4-3178c6?logo=typescript&logoColor=white)
![Pipeline](https://img.shields.io/badge/Pipeline-Bobanana%204.0-8b5cf6)
![Tests](https://img.shields.io/badge/Tests-100%20passed-22c55e)

一套工具集，让编程 Agent（Cursor、Windsurf、Reasonix 等）通过命令行和文件操作来创建和管理 Godot 4.x 项目。用户用自然语言描述游戏想法，Agent 自动完成项目搭建、场景编排、脚本编写、资源管理、日志调试的完整闭环。

---

## 快速开始

```bash
# 1. 从模板创建一个平台跳跃游戏
npx tsx tools/template_library.ts fork platformer ./my-game "MyGame"

# 2. 用 Godot 打开项目
godot my-game/project.godot

# 3. 运行测试
npm test
```

## 工具模块

| 模块 | 功能 | 工具函数 |
|------|------|---------|
| **M1 · Project Scaffolder** | 创建 Godot 4.x 项目骨架 | `ScaffoldProject` |
| **M2 · Scene Generator** | 生成和编辑 .tscn 场景 | `CreateScene`, `AddNode`, `RemoveNode`, `SetNodeProperty` |
| **M3 · Script Manager** | 生成 GDScript 并挂载到场景 | `CreateScript`, `AttachScript`, `ConnectSignal` |
| **M4 · Resource Resolver** | 管理纹理/音频/字体路径 | `ImportResource`, `ResolvePath` |
| **M5 · Template Library** | 3 个内置游戏模板，可 fork 定制 | `ListTemplates`, `ForkTemplate`, `CustomizeTemplate` |
| **M6 · Godot CLI Bridge** | 封装 godot 命令行调用 | `RunGodot`, `CheckGodotVersion` |
| **M7 · Documentation** | Agent 可读的 API 文档 | `GetSkillDoc`, `FindPlaybook`, `GetScopeBoundary` |
| **M8 · Log Capture & Parser** | 捕获游戏日志，解析为结构化条目 | `CaptureLogs`, `ParseLogText`, `SummarizeLogs` |
| **M9 · Input Simulator** | 跨平台键盘输入模拟 | `SimulateKeySequence`, `CreateInputScript`, `ReplayInputScript` |
| **M10 · Debug Orchestrator** | 编排调试闭环 + LLM 分析 | `RunDebugSession`, `AnalyzeLogsWithLLM`, `ListDebugSessions` |
| **M11 · GDScript Log Utils** | 结构化日志模板和自动注入 | `GenerateLogScript`, `InjectLogStatements` |

## 内置模板

| 模板 | 类型 | 包含 |
|------|------|------|
| `platformer` | 2D 平台跳跃 | CharacterBody2D、物理碰撞、跳跃 |
| `breakout` | 经典打砖块 | 挡板、小球、砖块、计分 |
| `clicker` | 点击收集 | 随机生成、点击交互、计时消失 |

## 测试

100 个测试用例，4 层覆盖：

```
Layer    Dir           Tests    Depends
─────────────────────────────────────────
U        tests/u/      84       无
I        tests/i/      7        无 (mock)
S        tests/s/      2        Godot 4.x
A        tests/a/      7        Godot 4.x
```

```bash
npm test          # 全部 100 tests
npm run test:u    # 单元测试
npm run test:i    # 集成测试
npm run test:s    # 场景测试（需 Godot）
npm run test:a    # 验收测试（需 Godot）
```

## 调试闭环

调试模块（M8-M11）让 Agent 能「看见」游戏运行时：

```
描述问题 → RunDebugSession → godot --headless 运行
  → 捕获日志 → LLM 分析 → 定位根因 → 修复 → 再验证
```

```bash
# 一键调试
npx tsx tools/debug_orchestrator.ts run ./my-game --duration 10

# 捕获日志
npx tsx tools/log_capture.ts ./my-game --args --headless

# 模拟按键
npx tsx tools/input_simulator.ts send Space

# 生成日志工具脚本
npx tsx tools/log_utils.ts generate --project-root ./my-game
```

## 项目结构

```
├── tools/                  # 11 个工具模块 (M1-M11)
├── templates/              # 3 个游戏模板
├── tests/                  # 4 层测试 (U/I/S/A)
├── docs/
│   ├── product/            # 产品设计文档
│   ├── prd/                # 技术 PRD
│   ├── playbooks/          # 使用示例
│   └── reference/          # Godot 格式参考 + Judge 标准
├── input-scripts/          # 输入模拟脚本
├── wasd-player/            # 示例 Godot 项目
├── .reasonix/skills/       # Reasonix 技能注册
├── Bobanana.md             # 工程原则
├── SKILL.md                # 技能集入口文档
└── install.bat             # 一键安装
```

## 安装

### 前置依赖

- Node.js 18+
- Godot 4.x（可选，用于验收测试和运行项目）

### 从源码

```bash
git clone https://github.com/ProfYangShengXu/godot-reasonix-skills.git
cd godot-reasonix-skills
npm install
```

### Reasonix 管线集成（Bobanana 4.0）

```bash
# 一键安装到全局 ~/.reasonix/skills/
install.bat

# 在 Reasonix 中使用
/pipeline 用 Godot 做一个平台跳跃游戏
/run_skill godot-dev
/run_skill godot-test
```

## 文档

- [技能集入口](SKILL.md) — 能力边界、工具清单
- [产品设计文档](docs/product/godot-reasonix-product-prd.html)
- [调试模块产品设计](docs/product/godot-reasonix-debugger-product-prd.html)
- [技术 PRD](docs/prd/v1/prd.yaml) — 模块接口定义与验收标准
- [GDScript 评判标准](docs/reference/gdscript-judge-criteria.md)
- [管线工作流示例](docs/playbooks/pipeline-workflow.md)
- [Godot 格式参考](docs/reference/godot-4-format-reference.md)

## 许可证

MIT
