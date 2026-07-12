/**
 * M11 · Log Utilities — 单元测试
 *
 * 测试 GenerateLogScript 和 InjectLogStatements。
 * 不依赖 Godot 进程。
 *
 * @test tests/u/log_utils.test.ts
 */

import { describe, it, expect } from "vitest";
import { GenerateLogScript, InjectLogStatements } from "../../tools/log_utils";
import { writeFileSync, unlinkSync, mkdtempSync, existsSync, readFileSync } from "node:fs";
import { join } from "node:path";
import { tmpdir } from "node:os";

// ─── GenerateLogScript ───

describe("GenerateLogScript", () => {
  // Normal: 默认级别生成脚本含三个核心函数
  it("normal 级别应包含 log_info / log_warn / log_error", async () => {
    const result = await GenerateLogScript({});
    expect(result.error).toBeUndefined();
    expect(result.script_content).toContain("func log_info");
    expect(result.script_content).toContain("func log_warn");
    expect(result.script_content).toContain("func log_error");
  });

  // Normal: verbose 级别应包含 log_debug
  it("verbose 级别应包含 log_debug", async () => {
    const result = await GenerateLogScript({ log_level: "verbose" });
    expect(result.script_content).toContain("func log_debug");
    expect(result.script_content).toContain('"DEBUG"');
  });

  // Normal: minimal 级别应不包含 push_error
  it("minimal 级别不应包含 push_error", async () => {
    const result = await GenerateLogScript({ log_level: "minimal" });
    expect(result.script_content).not.toContain("push_error");
  });

  // Normal: 输出格式为 LOG:[LEVEL]:[message]
  it("日志输出格式应为 LOG:[LEVEL]:[message]", async () => {
    const result = await GenerateLogScript({});
    expect(result.script_content).toContain("LOG");
    expect(result.script_content).toContain("\"INFO\"");
    expect(result.script_content).toContain("\"ERROR\"");
    expect(result.script_content).toContain("\"WARN\"");
  });

  // Boundary: 无效级别应返回错误
  it("无效级别应返回错误", async () => {
    const result = await GenerateLogScript({ log_level: "invalid" as any });
    expect(result.error).toBeDefined();
    expect(result.error).toContain("无效日志级别");
  });

  // Boundary: 写入到指定项目目录
  it("指定 project_root 应返回 script_path", async () => {
    const tmpDir = mkdtempSync(join(tmpdir(), "log-utils-test-"));
    // 创建 scripts 目录
    const scriptsDir = join(tmpDir, "scripts");
    const { mkdirSync } = await import("node:fs");
    mkdirSync(scriptsDir, { recursive: true });

    const result = await GenerateLogScript({ project_root: tmpDir });
    expect(result.script_path).toBe(join(tmpDir, "scripts", "log_utils.gd"));
    expect(existsSync(result.script_path!)).toBe(true);

    // 清理
    unlinkSync(result.script_path!);
  });

  // Boundary: 不存在的 project_root 应返回错误
  it("不存在的项目目录应返回错误", async () => {
    const result = await GenerateLogScript({
      project_root: "/tmp/nonexistent-project-12345/log-utils",
    });
    expect(result.error).toBeDefined();
  });
});

// ─── InjectLogStatements ───

describe("InjectLogStatements", () => {
  // Normal: 在 _ready 函数后插入日志
  it("应在 _ready 函数后插入日志语句", async () => {
    const tmpDir = mkdtempSync(join(tmpdir(), "log-inject-test-"));
    const testFile = join(tmpDir, "test.gd");
    writeFileSync(testFile, `extends Node2D

func _ready() -> void:
	pass
`);

    const result = await InjectLogStatements({
      file_path: testFile,
      inject_points: [{ function: "_ready", level: "info", message: "Node ready" }],
    });
    expect(result.error).toBeUndefined();
    expect(result.modified).toBe(true);

    const content = readFileSync(testFile, "utf-8");
    expect(content).toContain("LOG:INFO:Node ready");

    // 清理
    unlinkSync(testFile);
  });

  // Normal: 多函数注入
  it("支持多个函数注入", async () => {
    const tmpDir = mkdtempSync(join(tmpdir(), "log-inject-test-"));
    const testFile = join(tmpDir, "multi.gd");
    writeFileSync(testFile, `extends Node2D

func _ready() -> void:
	pass

func _process(delta: float) -> void:
	pass
`);

    const result = await InjectLogStatements({
      file_path: testFile,
      inject_points: [
        { function: "_ready", level: "info", message: "Ready" },
        { function: "_process", level: "info", message: "Process" },
      ],
    });
    expect(result.modified).toBe(true);

    const content = readFileSync(testFile, "utf-8");
    expect(content).toContain("LOG:INFO:Ready");
    expect(content).toContain("LOG:INFO:Process");

    // 清理
    unlinkSync(testFile);
  });

  // Boundary: 不存在的文件应返回错误
  it("不存在的文件应返回错误", async () => {
    const result = await InjectLogStatements({
      file_path: "/tmp/nonexistent_file_12345.gd",
      inject_points: [{ function: "_ready", level: "info", message: "test" }],
    });
    expect(result.error).toBeDefined();
    expect(result.modified).toBe(false);
  });

  // Boundary: 不存在的函数应不修改文件
  it("不存在的函数不应修改文件", async () => {
    const tmpDir = mkdtempSync(join(tmpdir(), "log-inject-test-"));
    const testFile = join(tmpDir, "nofunc.gd");
    writeFileSync(testFile, "extends Node2D\n");
    const originalContent = readFileSync(testFile, "utf-8");

    const result = await InjectLogStatements({
      file_path: testFile,
      inject_points: [{ function: "_nonexistent", level: "info", message: "test" }],
    });
    expect(result.modified).toBe(false);

    const content = readFileSync(testFile, "utf-8");
    expect(content).toBe(originalContent);

    // 清理
    unlinkSync(testFile);
  });
});
