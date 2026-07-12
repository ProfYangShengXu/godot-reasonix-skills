# Godot 挑刺大王附加检查项

> 在 Bobanana 11 项 checklist 完成后，如果当前 pipeline 涉及 Godot 项目，
> 追加以下 5 项 Godot 专属检查。

## 附加检查

| # | 检查项 | pass | fail |
|---|--------|------|------|
| G1 | **所有 .gd 脚本已挂接到场景节点** — grep script 在 .tscn 中出现 | ✅ | ❌ |
| G2 | **.tscn 格式合法** — Godot --headless 可加载无报错 | ✅ | ❌ |
| G3 | **资源引用路径正确** — tests/resource_resolver.test.ts 通过 | ✅ | ❌ |
| G4 | **日志埋点覆盖核心玩法** — 关键函数有 LOG:INFO 输出 | ✅ | ❌ |
| G5 | **调试会话可运行** — RunDebugSession 不崩溃 | ✅ | ❌ |

## 快速检查命令

```bash
# G1: 脚本挂接检查
grep -r "script/res://" templates/*/scenes/*.tscn | head -20

# G2: .tscn 格式检查（需要 Godot）
godot --headless --check my-game/project.godot 2>&1 | grep -c "ERROR"

# G3: 资源测试
npx vitest run tests/u/resource_resolver.test.ts

# G4: 日志埋点检查
grep -rn "LOG:" templates/*/scripts/*.gd

# G5: 调试会话（需要 Godot）
npx tsx tools/debug_orchestrator.ts run ./my-game --duration 3 --no-llm
```
