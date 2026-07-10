# 🍌 Godot Reasonix 技能集

> **让 AI Agent 帮你做 Godot 游戏，你只管想玩法。**

[![Tests](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml/badge.svg)](https://github.com/ProfYangShengXu/godot-reasonix-skills/actions/workflows/test.yml)
![Godot](https://img.shields.io/badge/Godot-4.7-478cbf?logo=godot-engine&logoColor=white)
![TypeScript](https://img.shields.io/badge/TypeScript-5.4-3178c6?logo=typescript&logoColor=white)

---

## 🎯 这玩意儿是干嘛的？

你有没有过这种经历：

- 🤔 "我想做个游戏，但 Godot 编辑器看着就头大"
- 😤 "我会写代码，但不想学拖拽节点树"
- 🤖 "如果能让 AI 直接帮我搭 Godot 项目就好了……"

**这就是答案。**

一套让编程 Agent（Cursor / Windsurf / Reasonix / 你家猫）操控 Godot 引擎的工具集。
你说人话，Agent 干活，Godot 出游戏。

## 🚀 三秒上手

```bash
# 1. 拿一个现成模板
npx tsx tools/template_library.ts fork platformer ./my-game "我的第一个游戏"

# 2. 用 Godot 打开
# （你的 Godot 安装路径在这里替换）
godot my-game/project.godot

# 3. 按 F5，你已经会了
```

## 🧰 工具箱（7 个模块，18 个工具）

| 模块 | 干嘛的 | 工具 |
|------|--------|------|
| 🏗️ **Scaffolder** | 创建项目骨架 | `ScaffoldProject` |
| 🎬 **Scene Generator** | 搭场景 | `CreateScene` `AddNode` `RemoveNode` `SetNodeProperty` |
| 📜 **Script Manager** | 写脚本 | `CreateScript` `AttachScript` `ConnectSignal` |
| 🗂️ **Resource Resolver** | 管资源 | `ImportResource` `ResolvePath` |
| 📦 **Template Library** | 抄作业 | `ListTemplates` `ForkTemplate` `CustomizeTemplate` |
| 🔧 **Godot CLI Bridge** | 调引擎 | `RunGodot` `CheckGodotVersion` |
| 📖 **Documentation** | 看说明书 | `GetSkillDoc` `FindPlaybook` `GetScopeBoundary` |

## 🎮 内置模板

| 模板 | 说明 | 难度 |
|------|------|------|
| `platformer` | 🏃 平台跳跃 — 左右跑 + 跳 | 中 |
| `breakout` | 🧱 打砖块 — 挡板接球碎砖 | 中 |
| `clicker` | 🖱️ 点击收集 — 点点点拿分 | 低 |

一键 fork：
```bash
npx tsx tools/template_library.ts fork clicker ./my-clicker "我的点击游戏"
```

## 🧪 质量声明

```
单元测试:  56 ✅  (覆盖每个工具的每条代码路径)
集成测试:   4 ✅  (M1+M2+M3+M4 组合协作)
场景测试:   2 ✅  (完整端到端管线)
验收测试:   7 ✅  (Godot 4.7 真实环境加载验证)
─────────────────────────────────────
总计:      69 ✅  全部通过，不跟你开玩笑
```

## 🐛 踩坑记录

想知道我们是怎么把 Godot 输入映射搞炸又修好的？见 [`docs/badcases/`](docs/badcases/)：

- 🔥 `[input]` 段在 `project.godot` 里写什么格式都会被 Godot 4.7 吃绑定
- 🔥 `CollisionShape2D.shape` 要的是 `SubResource` 不是 `Vector2`
- 🔥 Camera2D 跟在小人身上 + 纯灰背景 = 你以为游戏卡了
- 🔥 `ext_resource` 上的 uid 别手写，让 Godot 自己生成

## 🏗️ 项目结构

```
├── tools/              # 7 个工具模块
│   ├── scaffolder.ts
│   ├── scene_generator.ts
│   ├── script_manager.ts
│   ├── resource_resolver.ts
│   ├── template_library.ts
│   ├── godot_cli.ts
├── templates/          # 3 个游戏模板
│   ├── platformer/
│   ├── breakout/
│   └── clicker/
├── tests/              # 4 层测试
│   ├── u/              # 单元测试 (56)
│   ├── i/              # 集成测试 (4)
│   ├── s/              # 场景测试 (2)
│   └── a/              # 验收测试 (7)
├── docs/
│   ├── product/        # 产品 PRD
│   ├── prd/            # 技术 PRD
│   ├── playbooks/      # 使用示例
│   └── reference/      # Godot 格式参考
├── SKILL.md            # 技能集入口
└── package.json
```

## 📜 许可证

MIT — 做啥都行，别找我赔钱就行。

---

**Godot Reasonix 技能集** — *"我不会 Godot，但我的 Agent 会。"*
