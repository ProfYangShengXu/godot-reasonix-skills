/**
 * M8 · Log Parser — 单元测试
 *
 * 测试 ParseLogText 和 SummarizeLogs 的核心逻辑。
 * 使用模拟的日志文本，不依赖 godot 进程。
 *
 * 覆盖路径：
 *   - Normal:  标准 Godot 错误日志、print 输出、结构化 LOG 格式
 *   - Boundary: 空日志、大量条目、中文/特殊字符
 *   - Adversarial: 日志风暴截断、非标准格式
 *
 * @test tests/u/log_parser.test.ts
 */

import { describe, it, expect } from "vitest";
import {
  ParseLogText,
  SummarizeLogs,
} from "../../tools/log_parser";

// ─── ParseLogText ───

describe("ParseLogText", () => {
  // Normal: 标准 Godot 引擎错误
  it("应解析 Godot 引擎 ERROR 格式", () => {
    const result = ParseLogText({
      raw_text: "ERROR: Condition '!is_inside_tree()' is true.\n",
      source: "stderr",
    });
    expect(result.entries.length).toBeGreaterThanOrEqual(1);
    const entry = result.entries[0]!;
    expect(entry.level).toBe("error");
    expect(entry.category).toBe("engine");
  });

  // Normal: 标准 Godot 引擎 WARNING
  it("应解析 Godot 引擎 WARNING 格式", () => {
    const result = ParseLogText({
      raw_text: "WARNING: Script is not configured.\n",
      source: "stderr",
    });
    expect(result.entries.length).toBeGreaterThanOrEqual(1);
    expect(result.entries[0]!.level).toBe("warning");
  });

  // Normal: GDScript SCRIPT ERROR
  it("应解析 GDScript SCRIPT ERROR 格式", () => {
    const result = ParseLogText({
      raw_text: "SCRIPT ERROR: Parse Error: Unexpected token\n",
      source: "stderr",
    });
    expect(result.entries.length).toBeGreaterThanOrEqual(1);
    expect(result.entries[0]!.level).toBe("error");
    expect(result.entries[0]!.category).toBe("script");
  });

  // Normal: 结构化 LOG 格式
  it("应解析结构化 LOG: 格式", () => {
    const result = ParseLogText({
      raw_text: "LOG:INFO:Player ready\nLOG:ERROR:Collision failed\nLOG:WARN:Low fps\n",
      source: "stdout",
    });
    expect(result.entries.length).toBe(3);
    expect(result.entries[0]!.level).toBe("info");
    expect(result.entries[0]!.message).toBe("Player ready");
    expect(result.entries[1]!.level).toBe("error");
    expect(result.entries[1]!.message).toBe("Collision failed");
    expect(result.entries[2]!.level).toBe("warning");
    expect(result.entries[2]!.message).toBe("Low fps");
  });

  // Normal: traceback 文件行号关联
  it("应关联 traceback 中的文件和行号到上一错误", () => {
    const result = ParseLogText({
      raw_text: `SCRIPT ERROR: Invalid call. Target not found.
   at res://scripts/player.gd:42
   at res://scripts/game.gd:10
`,
      source: "stderr",
    });
    const errorEntry = result.entries.find((e) => e.level === "error");
    expect(errorEntry).toBeDefined();
    expect(errorEntry!.file).toBe("res://scripts/player.gd");
    expect(errorEntry!.line).toBe(42);
  });

  // Normal: 普通 print 输出
  it("应捕获 stdout 上的普通 print 输出", () => {
    const result = ParseLogText({
      raw_text: "Hello World\nScore: 100\n",
      source: "stdout",
    });
    expect(result.entries.length).toBe(2);
    expect(result.entries[0]!.level).toBe("info");
    expect(result.entries[0]!.message).toBe("Hello World");
  });

  // Boundary: 空输入
  it("空日志应返回空条目列表", () => {
    const result = ParseLogText({ raw_text: "", source: "stdout" });
    expect(result.entries.length).toBe(0);
  });

  // Boundary: 仅空行和 Godot 启动横幅
  it("应过滤 Godot 启动横幅和空行", () => {
    const result = ParseLogText({
      raw_text: "\n\nGodot Engine v4.3.stable - https://godotengine.org\n\n",
      source: "stdout",
    });
    // 可能没有条目，或只有横幅被忽略
    expect(result.entries.length).toBe(0);
  });

  // Boundary: 中文和特殊字符
  it("应正确编码中文和特殊字符", () => {
    const result = ParseLogText({
      raw_text: "LOG:ERROR:角色碰撞检测失败\nLOG:INFO:测试: @#$%^&\n",
      source: "stdout",
    });
    expect(result.entries.length).toBe(2);
    expect(result.entries[0]!.message).toContain("角色碰撞检测失败");
    expect(result.entries[1]!.message).toContain("@#$%^&");
  });

  // Boundary: LOG 级别映射
  it("应正确映射 LOG:DEBUG 到 debug 级别", () => {
    const result = ParseLogText({
      raw_text: "LOG:DEBUG:Frame 120\n",
      source: "stdout",
    });
    expect(result.entries[0]!.level).toBe("debug");
  });

  // Adversarial: 日志风暴截断
  it("超过 100000 行的日志应被截断", () => {
    const manyLines = Array.from({ length: 150_000 }, (_, i) => `LOG:INFO:Line ${i}`).join("\n");
    const result = ParseLogText({ raw_text: manyLines, source: "stdout" });
    // MAX_LINES = 100_000，但还有 MAX_ENTRIES 限制
    expect(result.entries.length).toBeLessThanOrEqual(50_000);
  });
});

// ─── SummarizeLogs ───

describe("SummarizeLogs", () => {
  // Normal: 正常摘要统计
  it("应正确统计各级别日志数量", () => {
    const entries = [
      { level: "error" as const, message: "Error 1", timestamp: 0, raw: "", category: "engine" as const },
      { level: "error" as const, message: "Error 2", timestamp: 5, raw: "", category: "script" as const },
      { level: "warning" as const, message: "Warn 1", timestamp: 10, raw: "", category: "engine" as const },
      { level: "info" as const, message: "Info 1", timestamp: 15, raw: "", category: "print" as const },
      { level: "info" as const, message: "Info 2", timestamp: 20, raw: "", category: "print" as const },
      { level: "debug" as const, message: "Debug 1", timestamp: 25, raw: "", category: "system" as const },
    ];
    const result = SummarizeLogs({ entries });
    expect(result.summary.stats.total).toBe(6);
    expect(result.summary.stats.errors).toBe(2);
    expect(result.summary.stats.warnings).toBe(1);
    expect(result.summary.stats.infos).toBe(2);
    expect(result.summary.stats.debug).toBe(1);
  });

  // Normal: top_errors 按频率聚合
  it("应聚合 top_errors 按文件行号分组", () => {
    const entries = [
      { level: "error" as const, message: "Null ref", timestamp: 0, raw: "", category: "script" as const, file: "res://player.gd", line: 42 },
      { level: "error" as const, message: "Null ref", timestamp: 5, raw: "", category: "script" as const, file: "res://player.gd", line: 42 },
      { level: "error" as const, message: "Div by 0", timestamp: 10, raw: "", category: "script" as const, file: "res://math.gd", line: 10 },
    ];
    const result = SummarizeLogs({ entries });
    expect(result.summary.top_errors.length).toBe(2);
    expect(result.summary.top_errors[0]!.file).toBe("res://player.gd");
    expect(result.summary.top_errors[0]!.count).toBe(2);
  });

  // Normal: 诊断提示
  it("有错误时应生成诊断提示", () => {
    const entries = [
      { level: "error" as const, message: "Crash", timestamp: 0, raw: "", category: "engine" as const, file: "res://main.gd", line: 1 },
    ];
    const result = SummarizeLogs({ entries });
    expect(result.summary.diagnosis_hints.length).toBeGreaterThan(0);
    expect(result.summary.diagnosis_hints.some((h) => h.includes("错误"))).toBe(true);
  });

  // Boundary: 空条目列表
  it("空列表应返回完整但全零的统计", () => {
    const result = SummarizeLogs({ entries: [] });
    expect(result.summary.stats.total).toBe(0);
    expect(result.summary.stats.errors).toBe(0);
    expect(result.summary.top_errors.length).toBe(0);
    expect(result.summary.diagnosis_hints.length).toBe(1);
    expect(result.summary.diagnosis_hints[0]).toContain("未捕获到");
  });

  // Boundary: 大量条目性能边界
  it("1000 条日志的生成时间应在合理范围", () => {
    const entries = Array.from({ length: 1000 }, (_, i) => ({
      level: (i % 3 === 0 ? "error" : i % 3 === 1 ? "warning" : "info") as "error" | "warning" | "info",
      message: `Log ${i}`,
      timestamp: i * 10,
      raw: `raw ${i}`,
      category: "print" as const,
      file: i % 2 === 0 ? `res://file${i % 10}.gd` : undefined,
      line: i % 2 === 0 ? i % 100 : undefined,
    }));
    const start = Date.now();
    const result = SummarizeLogs({ entries });
    const elapsed = Date.now() - start;
    expect(elapsed).toBeLessThan(1000); // 应在 1 秒内完成
    expect(result.summary.stats.total).toBe(1000);
  });

  // Adversarial: 无 file/line 的错误不应影响统计
  it("无文件信息的错误不应计入 top_errors", () => {
    const entries = [
      { level: "error" as const, message: "Generic error", timestamp: 0, raw: "", category: "engine" as const },
      { level: "error" as const, message: "Another error", timestamp: 5, raw: "", category: "engine" as const },
    ];
    const result = SummarizeLogs({ entries });
    // top_errors 只包含有 file 的条目
    expect(result.summary.top_errors.length).toBe(0);
    expect(result.summary.stats.errors).toBe(2);
  });
});
