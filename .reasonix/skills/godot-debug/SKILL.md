---
name: godot-debug
description: 🔍 [Godot] 游戏调试闭环 — 日志捕获→解析→LLM分析，输入模拟，一键调试会话
runAs: inline
profiles: balanced, delivery
cost: high
---

# godot-debug — Godot 游戏调试与测试

**先读** `docs/product/godot-reasonix-debugger-product-prd.html` 了解产品定位。

## 能力

| 模块 | 工具 | 说明 |
|------|------|------|
| M8 | `CaptureLogs` / `ParseLogText` / `SummarizeLogs` | 运行 godot → 捕获 stdout/stderr → 结构化解析 |
| M9 | `SimulateKeySequence` / `CreateInputScript` / `ReplayInputScript` | 跨平台键盘输入模拟，可录制重放 |
| M10 | `RunDebugSession` / `AnalyzeLogsWithLLM` / `ListDebugSessions` | 编排完整调试：启动→输入→抓日志→LLM 分析 |
| M11 | `GenerateLogScript` / `InjectLogStatements` | 生成 log_utils.gd 模板，在脚本中注入日志 |

## 调试闭环

```
① 用户描述问题 → ② RunDebugSession → ③ godot --headless 启动
   → ④ SimulateKeySequence 模拟输入 → ⑤ CaptureLogs 抓日志
   → ⑥ ParseLogText 解析 → ⑦ AnalyzeLogsWithLLM 诊断
   → ⑧ Agent 根据诊断改代码 → ⑨ 重新 RunDebugSession 验证
```

## 快速入口

```bash
# 一键调试会话
npx tsx tools/debug_orchestrator.ts run ./my-game --duration 10

# 捕获日志
npx tsx tools/log_capture.ts ./my-game --args --headless

# 模拟按键
npx tsx tools/input_simulator.ts send Space

# 生成日志工具
npx tsx tools/log_utils.ts generate --project-root ./my-game
```
