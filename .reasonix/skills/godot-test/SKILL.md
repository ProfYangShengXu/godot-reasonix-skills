---
name: godot-test
description: 🧪 [Godot] 测试运行器 — 四层测试 (U/I/S/A) 的 vitest 执行入口，含覆盖率检查和三路径验证
runAs: inline
profiles: balanced, delivery
cost: medium
---

# godot-test — Godot 测试运行器

执行 Godot 工具集的四层测试体系。每层必须覆盖 normal / boundary / adversarial 三路径。

## 命令速查

```bash
# 全量运行
npm test                    # 等价于 vitest run

# 按层运行
npm run test:u              # vitest run tests/u/   — 单元测试
npx vitest run tests/i/     # 集成测试
npx vitest run tests/s/     # 场景测试（需要 Godot 4.x）
npx vitest run tests/a/     # 验收测试（需要 Godot 4.x）

# 运行单个文件
npx vitest run tests/u/log_parser.test.ts
npx vitest run tests/i/log_pipeline.test.ts

# 覆盖率
npx vitest run --coverage

# 持续监听
npm run test:watch
```

## 测试层说明

| 层 | 目录 | 依赖 | 目的 |
|----|------|------|------|
| U | `tests/u/` | 无 | 纯逻辑，Mock 数据，不依赖 Godot |
| I | `tests/i/` | M6 (CLI) | 模块间交互，可模拟 godot 进程 |
| S | `tests/s/` | Godot 4.x | 端到端场景，真实 godot --headless |
| A | `tests/a/` | Godot 4.x | 验收测试，完整游戏项目验证 |

## 质量门

1. 每次变更后必须跑对应层的测试
2. U 层测试必须通过才能进入 I 层
3. I 层测试必须通过才能进入 S 层
4. A 层测试需包含 adversarial 路径（错误输入、边界条件）
