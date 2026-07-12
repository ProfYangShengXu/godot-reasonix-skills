/**
 * M8 + M11 集成测试：日志管线端到端
 *
 * 测试流程：
 *   1. 用 M11 GenerateLogScript 生成 log_utils.gd
 *   2. 用 M11 InjectLogStatements 在测试 .gd 中注入日志
 *   3. 用 M8 ParseLogText 解析模拟运行时输出
 *   4. 用 M8 SummarizeLogs 生成摘要
 *
 * @test tests/i/log_pipeline.test.ts
 */

import { describe, it, expect } from "vitest";
import { GenerateLogScript, InjectLogStatements } from "../../tools/log_utils";
import { ParseLogText, SummarizeLogs } from "../../tools/log_parser";
import { writeFileSync, unlinkSync, mkdtempSync, readFileSync } from "node:fs";
import { join } from "node:path";
import { tmpdir } from "node:os";

describe("M8+M11 日志管线集成", () => {
  // 集成: 生成 → 注入 → 模拟运行 → 解析
  it("应完整走通: 生成日志模板 → 注入 → 模拟 print → 解析为结构化条目", async () => {
    // Step 1: 生成 log_utils.gd 模板
    const templateResult = await GenerateLogScript({ log_level: "normal" });
    expect(templateResult.error).toBeUndefined();
    expect(templateResult.script_content).toContain("func log_info");
    expect(templateResult.script_content).toContain("LOG:");

    // Step 2: 创建测试 .gd 文件并注入日志
    const tmpDir = mkdtempSync(join(tmpdir(), "log-pipeline-"));
    const testFile = join(tmpDir, "test_player.gd");
    writeFileSync(testFile, `extends CharacterBody2D

var speed: int = 300

func _ready() -> void:
	pass

func _physics_process(delta: float) -> void:
	pass
`);

    const injectResult = await InjectLogStatements({
      file_path: testFile,
      inject_points: [
        { function: "_ready", level: "info", message: "Player ready" },
        { function: "_physics_process", level: "debug", message: "Processing frame" },
      ],
    });
    expect(injectResult.modified).toBe(true);

    const injectedContent = readFileSync(testFile, "utf-8");
    expect(injectedContent).toContain("LOG:INFO:Player ready");
    expect(injectedContent).toContain("LOG:DEBUG:Processing frame");

    // Step 3: 模拟游戏运行输出（模拟 godot 运行时）
    const simulatedOutput = [
      'Godot Engine v4.3.stable - https://godotengine.org',
      '',
      'LOG:INFO:Player ready',
      'LOG:INFO:Ball spawned at (400, 300)',
      'WARNING: Low performance mode activated',
      'LOG:DEBUG:Processing frame',
      'LOG:ERROR:Collision detected with null body',
      '   at res://scripts/test_player.gd:12',
      'LOG:INFO:Frame complete',
      'ERROR: Condition "!is_inside_tree()" is true.',
    ].join("\n");

    const parseResult = ParseLogText({
      raw_text: simulatedOutput,
      source: "stdout",
    });
    expect(parseResult.error).toBeUndefined();

    // 验证解析结果
    const infoEntries = parseResult.entries.filter((e) => e.level === "info");
    expect(infoEntries.length).toBeGreaterThanOrEqual(3);
    expect(infoEntries.some((e) => e.message.includes("Player ready"))).toBe(true);

    const errorEntries = parseResult.entries.filter((e) => e.level === "error");
    expect(errorEntries.length).toBeGreaterThanOrEqual(2);

    const debugEntries = parseResult.entries.filter((e) => e.level === "debug");
    expect(debugEntries.length).toBeGreaterThanOrEqual(1);

    // 验证 traceback 关联
    const collisionError = errorEntries.find((e) => e.message.includes("Collision"));
    expect(collisionError).toBeDefined();
    // traceback 行应该关联文件行号
    const tracebackEntry = parseResult.entries.find(
      (e) => e.message.includes("test_player.gd:12"),
    );
    expect(tracebackEntry).toBeDefined();

    // Step 4: 生成摘要
    const summaryResult = SummarizeLogs({ entries: parseResult.entries });
    expect(summaryResult.summary.stats.total).toBeGreaterThan(0);
    expect(summaryResult.summary.stats.errors).toBeGreaterThanOrEqual(2);
    expect(summaryResult.summary.diagnosis_hints.length).toBeGreaterThan(0);

    // 清理
    unlinkSync(testFile);
  });

  // 对抗: 空的/无效的日志不崩溃
  it("空日志输入应优雅处理", () => {
    const result = ParseLogText({ raw_text: "", source: "stdout" });
    expect(result.entries.length).toBe(0);
    expect(result.error).toBeUndefined();
  });

  // 边界: 日志中包含非常规格式
  it("应处理混合格式日志不崩溃", () => {
    const messyLog = `
LOG:INFO:Normal entry
Some random text
ERROR: without colon
  indented error line
LOG:WARN:Warning message
---
LOG:ERROR:Script crash
   at res://game.gd:99
`;
    const result = ParseLogText({ raw_text: messyLog, source: "stderr" });
    // 不应崩溃，应返回合理的条目数
    expect(result.entries.length).toBeGreaterThan(0);
    expect(result.error).toBeUndefined();

    const summary = SummarizeLogs({ entries: result.entries });
    expect(summary.summary.stats.total).toBe(result.entries.length);
  });
});
