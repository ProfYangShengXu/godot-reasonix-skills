# Pipeline Workflow — 管线工作流示例

> 展示一条完整的 Godot 游戏开发管线：用户说需求 → Bobanana 管线角色自动推进 → 交付可运行的游戏。
> 适用 `/pipeline` 入口。

## 场景：做一个平台跳跃游戏

```
用户: /pipeline 做一个平台跳跃游戏，角色能左右移动和跳跃，有地面碰撞
```

### Session 1 · 🏗️ 架构师

**输入**：用户目标（一字不改）
**产出**：
- `docs/prd/v1/prd.yaml` — M1 ScaffoldProject + M2 CreateScene + M3 CreateScript
- `docs/prd/v1/architecture.html` — 节点树设计、场景结构、依赖关系

**queue_next_prompt**:
```
[GOAL] 做一个平台跳跃游戏，角色能左右移动和跳跃，有地面碰撞
[PHASE] arch-done
[ROLE] 🏗️ 架构师
[DONE] - 输出技术 PRD，含 3 个模块定义
       - 产出架构文档
       Artifacts: docs/prd/v1/prd.yaml, docs/prd/v1/architecture.html
[STATE] task_list: 项目脚手架⬜ 场景搭建⬜ 脚本编写⬜
[NEXT] 按 PRD M1→M2→M3 顺序实现
```

### Session 2 · 🔧 开发（M1）

**输入**：`prd.yaml` M1 模块定义
**任务**：调用 `ScaffoldProject` 创建项目骨架
**输出**：`my-game/` 目录 + `project.godot`

### Session 3 · 🔧 开发（M2）

**任务**：调用 `CreateScene` 创建 `Player.tscn`（CharacterBody2D + Sprite2D + CollisionShape2D）
**输出**：`scenes/Player.tscn`

### Session 4 · 🔧 开发（M3）

**任务**：调用 `CreateScript` 创建 `player.gd`（_physics_process + 跳跃/移动逻辑），`AttachScript` 挂载
**输出**：`scripts/player.gd`

### Session 5 · 🧪 测试 U

**任务**：`npx vitest run tests/u/`
**输出**：trace（84 tests passed）

### Session 6 · 🧪 测试 I

**任务**：`npx vitest run tests/i/`
**输出**：trace（7 tests passed）

### Session 7 · 🧪 测试 S

**任务**：`npx tsx tools/debug_orchestrator.ts run ./my-game --duration 5`
**输出**：日志摘要 + 错误统计

### Session 8 · 📋 评判

**任务**：对照 prd.yaml acceptance 逐条验证
**输出**：PASS/FAIL 表 + badcase（如有）

### Session 9 · 👿 挑刺大王

**任务**：11 项 checklist 逐条检查
**输出**：11/11 pass → `signal_done`

---

## 场景：修复打砖块游戏的碰撞 bug

```
用户: /loop 打砖块小球的碰撞有时候会穿过砖块，帮我修
```

### Session 1 · 🔧 开发

**任务**：
1. `RunDebugSession` 启动调试，捕获碰撞日志
2. 从日志中发现 `direction.y *= -1` 在某些角度下反复翻转导致穿模
3. 修改 `ball.gd`，增加角度校验

### Session 2 · 🧪 测试

**任务**：创建测试用例验证碰撞角度范围
**输出**：trace

---

## 场景：并行开发多个模块

```
用户: /cycle 给我的游戏加金币收集系统和计分板
```

### Session 1 · 并行 3 个 task

- Task A：创建 `Coin.tscn`（Area2D + CollisionShape2D + 动画）
- Task B：创建 `ScoreManager.gd`（全局信号 + UI 更新）
- Task C：创建 `HUD.tscn`（ScoreLabel + LivesLabel）

**验证**：合并后跑 `npx vitest run tests/i/`

### Session 2（如有剩余任务）· 继续并行
