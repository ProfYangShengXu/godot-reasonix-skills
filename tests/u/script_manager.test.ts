/**
 * M3 · Script Manager — U 层测试
 *
 * 覆盖 M3-A1（正常路径）、M3-A2（异常路径）、M3-A4（对抗路径）。
 */

import { describe, it, expect } from "vitest";
import { RenderGdScript } from "../../tools/script_manager.js";

// ─── Helpers ───

/** 检查 .gd 内容中是否包含某行（trim 后比较） */
function gdContains(content: string, line: string): boolean {
  return content.split("\n").some((l) => l.trim() === line.trim());
}

// ─── Tests ───

describe("M3 · Script Manager — U 层", () => {
  // ── M3-A1: 正常路径 ──

  describe("M3-A1: 正常路径 — GDScript 生成", () => {
    it("应生成 extends CharacterBody2D 的脚本", () => {
      const gd = RenderGdScript({
        path: "scripts/player.gd",
        extends: "CharacterBody2D",
      });

      expect(gd).toContain("extends CharacterBody2D");
      expect(gd.endsWith("\n")).toBe(true);
    });

    it("应包含 _ready 和 _process 函数", () => {
      const gd = RenderGdScript({
        path: "scripts/player.gd",
        extends: "CharacterBody2D",
        functions: [
          {
            name: "_ready",
            body: "print('ready')",
          },
          {
            name: "_process",
            args: "delta: float",
            body: "print(delta)",
          },
        ],
      });

      expect(gd).toContain("func _ready():");
      expect(gd).toContain("\tprint('ready')");
      expect(gd).toContain("func _process(delta: float):");
      expect(gd).toContain("\tprint(delta)");
    });

    it("应生成信号定义", () => {
      const gd = RenderGdScript({
        path: "scripts/enemy.gd",
        extends: "Node2D",
        signals: [
          { name: "hit", args: [{ name: "damage", type: "int" }] },
          { name: "died" },
        ],
      });

      expect(gd).toContain("signal hit(damage: int)");
      expect(gd).toContain("signal died");
    });

    it("应生成 @export 变量", () => {
      const gd = RenderGdScript({
        path: "scripts/player.gd",
        extends: "CharacterBody2D",
        variables: [
          { name: "speed", type: "int", export: true, default: "200" },
          { name: "jump_velocity", type: "float", export: true, default: "-400.0" },
          { name: "gravity", type: "float", default: "980.0" },
        ],
      });

      expect(gd).toContain("@export var speed: int = 200");
      expect(gd).toContain("@export var jump_velocity: float = -400.0");
      expect(gd).toContain("var gravity: float = 980.0");
    });

    it("应支持 class_name 和 @tool", () => {
      const gd = RenderGdScript({
        path: "scripts/global.gd",
        extends: "Node",
        is_tool: true,
        class_name: "GlobalHelper",
      });

      expect(gd).toContain("@tool");
      expect(gd).toContain("class_name GlobalHelper");
    });

    it("空函数体应生成 pass", () => {
      const gd = RenderGdScript({
        path: "scripts/empty.gd",
        extends: "Node",
        functions: [{ name: "_ready", body: "" }],
      });

      expect(gd).toContain("\tpass");
    });
  });

  // ── M3-A2: 异常路径 ──

  describe("M3-A2: 异常路径 — 错误处理", () => {
    it("空 path 应返回 error", async () => {
      const { CreateScript } = await import("../../tools/script_manager.js");
      const result = await CreateScript({ path: "", extends: "Node" });
      expect(result.error).toBeTruthy();
    });

    it("空 extends 应返回 error", async () => {
      const { CreateScript } = await import("../../tools/script_manager.js");
      const result = await CreateScript({
        path: "/tmp/test.gd",
        extends: "",
      });
      expect(result.error).toBeTruthy();
    });

    it("extends 为不合法类型也应创建（语法由 Godot 检查，本模块只生成文本）", () => {
      // M3 只负责生成文本，不验证 GDScript 语义
      const gd = RenderGdScript({
        path: "scripts/test.gd",
        extends: "NonExistentType",
      });
      expect(gd).toContain("extends NonExistentType");
    });
  });

  // ── M3-A4: 对抗路径 ──

  describe("M3-A4: 对抗路径 — 边界与安全", () => {
    it("大量信号和函数不应丢失", () => {
      const signals: Array<{ name: string; args?: Array<{ name: string; type: string }> }> = [];
      const functions: Array<{ name: string; body: string }> = [];

      for (let i = 0; i < 10; i++) {
        signals.push({ name: `sig_${i}` });
        functions.push({ name: `func_${i}`, body: `print(${i})` });
      }

      const gd = RenderGdScript({
        path: "scripts/big.gd",
        extends: "Node",
        signals,
        functions,
      });

      for (let i = 0; i < 10; i++) {
        expect(gd).toContain(`signal sig_${i}`);
        expect(gd).toContain(`func func_${i}():`);
        expect(gd).toContain(`\tprint(${i})`);
      }
    });

    it("脚本内容含特殊字符不破坏格式", () => {
      const gd = RenderGdScript({
        path: "scripts/escape.gd",
        extends: "Node",
        functions: [
          {
            name: "test",
            body: 'print("hello\\nworld")',
          },
        ],
      });

      expect(gd).toContain('print("hello');
      expect(gd).toContain('world")');
    });

    it("函数体含多行缩进正确", () => {
      const gd = RenderGdScript({
        path: "scripts/multi.gd",
        extends: "Node",
        functions: [
          {
            name: "complex",
            body: [
              "var x = 1",
              "if x > 0:",
              "\tprint('positive')",
              "else:",
              "\tprint('negative')",
            ].join("\n"),
          },
        ],
      });

      const lines = gd.split("\n");
      const funcIdx = lines.findIndex((l) => l.includes("func complex"));
      expect(lines[funcIdx + 1].startsWith("\t")).toBe(true);
      expect(lines[funcIdx + 2].startsWith("\t")).toBe(true);
    });
  });
});
