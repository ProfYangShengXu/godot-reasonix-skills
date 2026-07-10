# Godot Reasonix Skills

让 AI 编程 Agent 操控 Godot 引擎，辅助游戏开发。

[![Tests](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml/badge.svg)](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml)
![Godot](https://img.shields.io/badge/Godot-4.7-478cbf?logo=godot-engine&logoColor=white)
![TypeScript](https://img.shields.io/badge/TypeScript-5.4-3178c6?logo=typescript&logoColor=white)

## 概述

一套工具集，让编程 Agent（Cursor、Windsurf、Reasonix 等）能够通过命令行和文件操作来创建和管理 Godot 4.x 项目。用户用自然语言描述游戏想法，Agent 自动完成项目搭建、场景编排、脚本编写和资源管理。

不需要手动操作 Godot 编辑器即可完成基础项目搭建。

## 快速开始

```bash
# 1. 从模板创建一个平台跳跃游戏
npx tsx tools/template_library.ts fork platformer ./my-game "MyGame"

# 2. 用 Godot 打开项目
godot my-game/project.godot

# 3. 按 F5 运行
```

## 工具模块

| 模块 | 功能 | 工具函数 |
|------|------|---------|
| **Project Scaffolder** | 创建 Godot 项目骨架 | `ScaffoldProject` |
| **Scene Generator** | 生成和编辑 .tscn 场景文件 | `CreateScene`, `AddNode`, `RemoveNode`, `SetNodeProperty` |
| **Script Manager** | 生成 GDScript 并绑定到场景 | `CreateScript`, `AttachScript`, `ConnectSignal` |
| **Resource Resolver** | 管理纹理/音频/字体等资源路径 | `ImportResource`, `ResolvePath` |
| **Template Library** | 内置 3 个游戏模板，可 fork 定制 | `ListTemplates`, `ForkTemplate`, `CustomizeTemplate` |
| **Godot CLI Bridge** | 封装 godot 命令行调用 | `RunGodot`, `CheckGodotVersion` |
| **Documentation** | Agent 可读的 API 文档和示例 | `GetSkillDoc`, `FindPlaybook`, `GetScopeBoundary` |

## 内置模板

| 模板 | 描述 | 包含内容 |
|------|------|---------|
| `platformer` | 2D 平台跳跃游戏 | CharacterBody2D 角色、物理碰撞、跳跃机制 |
| `breakout` | 经典打砖块游戏 | 挡板控制、小球物理、砖块碰撞、计分系统 |
| `clicker` | 点击收集游戏 | 随机生成物品、点击交互、计时消失机制 |

```bash
# 列出可用模板
npx tsx tools/template_library.ts list

# Fork 模板到新项目
npx tsx tools/template_library.ts fork clicker ./my-clicker "MyClicker"
```

## 测试

项目包含四层测试，覆盖单元测试到 Godot 真实环境验收：

```
tests/
├── u/          # 单元测试（56 用例）
├── i/          # 集成测试（4 用例）
├── s/          # 场景测试（2 用例）
└── a/          # 验收测试（7 用例，需 Godot 4.x 环境）
```

```bash
npm test        # 运行全部测试
npm run test:u  # 仅单元测试
npm run test:a  # 仅验收测试（需要 Godot 4.x）
```

## 项目结构

```
├── tools/                  # 工具模块
├── templates/              # 游戏模板
│   ├── platformer/
│   ├── breakout/
│   └── clicker/
├── tests/                  # 测试
├── docs/
│   ├── product/            # 产品设计文档
│   ├── prd/                # 技术 PRD
│   ├── playbooks/          # 使用示例
│   ├── reference/          # Godot 格式参考
│   └── badcases/           # 踩坑记录
├── wasd-player/            # 示例项目
├── SKILL.md                # 技能集入口文档
└── package.json
```

## 前置依赖

- Node.js 18+
- Godot 4.x（用于验收测试和运行生成的项目）

## 文档

- [技能集入口文档](SKILL.md) — 能力边界、工具清单、快速上手
- [产品设计文档](docs/product/godot-reasonix-product-prd.html) — 产品定位与设计理念
- [技术 PRD](docs/prd/v1/prd.yaml) — 模块接口定义与验收标准
- [Godot 格式参考](docs/reference/godot-4-format-reference.md) — 官方文档整合

## 许可证

MIT
