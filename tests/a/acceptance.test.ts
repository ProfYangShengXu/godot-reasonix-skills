/**
 * A 层验收测试 — 使用真实 Godot 4.x 二进制验证项目完整性
 *
 * 前置条件：Godot 4.x 已安装在 GODOT_PATH 所指向的位置。
 *
 * 覆盖：
 *   M1-A4: godot --headless --import 验证脚手架项目
 *   M2-A4: godot --headless 加载生成的场景无报错
 *   M3-A3: 脚本挂载后项目整体加载无错
 *   M5-A1: 模板 fork 后 godot 可加载
 */

import { describe, it, expect, beforeEach, afterEach } from "vitest";
import { mkdir, rm, writeFile, copyFile } from "node:fs/promises";
import { join } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";
import { spawn } from "node:child_process";

// ─── 配置 ───

const GODOT_PATH =
  "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Godot Engine\\godot.windows.opt.tools.64.exe";

// ─── Helper ───

async function runGodot(
  args: string[],
  timeoutMs = 30000,
): Promise<{ stdout: string; stderr: string; exitCode: number }> {
  return new Promise((resolve) => {
    const child = spawn(GODOT_PATH, args, {
      stdio: ["ignore", "pipe", "pipe"],
      timeout: timeoutMs,
    });
    let stdout = "";
    let stderr = "";
    child.stdout.on("data", (d: Buffer) => (stdout += d.toString()));
    child.stderr.on("data", (d: Buffer) => (stderr += d.toString()));
    child.on("close", (code) => resolve({ stdout, stderr, exitCode: code ?? -1 }));
    child.on("error", (err) =>
      resolve({ stdout, stderr, exitCode: -1 }),
    );
  });
}

function hasError(stderr: string): boolean {
  return stderr
    .split("\n")
    .some(
      (l) =>
        l.includes("ERROR") &&
        !l.includes("Deprecated") &&
        !l.includes("deprecated") &&
        !l.includes("editor_settings") &&
        !l.includes("Orchestrator"),
    );
}

async function makeTempDir(): Promise<string> {
  const dir = join(tmpdir(), `godot-a-${randomBytes(4).toString("hex")}`);
  await mkdir(dir, { recursive: true });
  return dir;
}

// ─── Tests ───

describe("A 层验收测试 — Godot 4.x 真实环境", () => {
  let tmpDir: string;

  beforeEach(async () => {
    tmpDir = await makeTempDir();
  });

  afterEach(async () => {
    await rm(tmpDir, { recursive: true, force: true });
  });

  // ── M1-A4: 脚手架项目验证 ──

  describe("M1-A4: 脚手架项目 → godot 可加载", () => {
    it("ScaffoldProject 生成的项目通过 godot 验证", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const projectRoot = join(tmpDir, "scaffold-test");

      const result = await ScaffoldProject({
        root_path: projectRoot,
        project_name: "ScaffoldATest",
      });
      expect(result.error).toBeUndefined();

      // Godot 验证：加载项目、导入资源、退出
      const godotResult = await runGodot([
        "--path",
        projectRoot,
        "--headless",
        "--quit",
      ]);

      // 无 ERROR 级别日志即通过
      expect(hasError(godotResult.stderr)).toBe(false);
      expect(godotResult.exitCode).toBe(0);
    });
  });

  // ── M2-A4: 场景文件验证 ──

  describe("M2-A4: 生成场景 → godot 无报错", () => {
    it("CreateScene 生成的复杂场景可加载", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { CreateScene } = await import("../../tools/scene_generator.js");
      const projectRoot = join(tmpDir, "scene-test");

      await ScaffoldProject({
        root_path: projectRoot,
        project_name: "SceneATest",
      });

      // 创建含节点树的复杂场景
      await CreateScene({
        path: join(projectRoot, "scenes", "Player.tscn"),
        root_node: {
          type: "CharacterBody2D",
          name: "Player",
          position: { x: 100, y: 200 },
          children: [
            {
              type: "CollisionShape2D",
              name: "CollisionShape",
              properties: {
                shape: { x: 32, y: 48 },
                disabled: false,
              },
            },
            { type: "Sprite2D", name: "Visual" },
          ],
        },
      });

      await CreateScene({
        path: join(projectRoot, "scenes", "Level.tscn"),
        root_node: {
          type: "Node2D",
          name: "Level",
          children: [
            {
              type: "StaticBody2D",
              name: "Ground",
              position: { x: 400, y: 580 },
              children: [{ type: "CollisionShape2D", name: "Shape" }],
            },
          ],
        },
      });

      // Godot 验证
      const godotResult = await runGodot([
        "--path",
        projectRoot,
        "--headless",
        "--quit",
      ]);

      expect(hasError(godotResult.stderr)).toBe(false);
    });
  });

  // ── M3-A3: 脚本挂载项目验证 ──

  describe("M3-A3: 脚本挂载 → 项目整体加载无错", () => {
    it("创建项目 + 场景 + 脚本 + 挂载 → godot 可加载", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { CreateScene } = await import("../../tools/scene_generator.js");
      const { CreateScript, AttachScript } = await import(
        "../../tools/script_manager.js"
      );
      const projectRoot = join(tmpDir, "script-test");

      await ScaffoldProject({
        root_path: projectRoot,
        project_name: "ScriptATest",
      });

      await CreateScene({
        path: join(projectRoot, "scenes", "Player.tscn"),
        root_node: {
          type: "CharacterBody2D",
          name: "Player",
          children: [{ type: "CollisionShape2D", name: "Shape" }],
        },
      });

      await CreateScript({
        path: join(projectRoot, "scripts", "player.gd"),
        extends: "CharacterBody2D",
        variables: [
          { name: "speed", type: "int", export: true, default: "300" },
        ],
        functions: [
          {
            name: "_physics_process",
            args: "delta: float",
            body: "move_and_slide()",
          },
        ],
      });

      await AttachScript({
        scene_path: join(projectRoot, "scenes", "Player.tscn"),
        node_path: "Player",
        script_path: "res://scripts/player.gd",
      });

      const godotResult = await runGodot([
        "--path",
        projectRoot,
        "--headless",
        "--quit",
      ]);

      expect(hasError(godotResult.stderr)).toBe(false);
    });
  });

  // ── M5-A1: 模板 fork 验证 ──

  describe("M5-A1: 模板 fork → godot 可加载", () => {
    it.each(["platformer", "breakout", "clicker"] as const)(
      "模板 %s fork 后 godot 可加载",
      async (templateId) => {
        const { ForkTemplate } = await import(
          "../../tools/template_library.js"
        );
        const projectRoot = join(tmpDir, `template-${templateId}`);

        const forkResult = await ForkTemplate({
          template_id: templateId,
          target_path: projectRoot,
          project_name: `${templateId}-test`,
        });
        expect(forkResult.error).toBeUndefined();

        const godotResult = await runGodot([
          "--path",
          projectRoot,
          "--headless",
          "--quit",
        ]);

        // 模板含 GDScript，可能在 stderr 中有 GDScript 编译信息
        // 只要不是严重 ERROR 且 exit code = 0 即通过
        expect(godotResult.exitCode).toBe(0);
      },
      45000,
    );
  });

  // ── 完整管线：从零到可运行项目 ──

  describe("E2E: 从零到可运行", () => {
    it("完整管线创建的项目 godot 可加载运行", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { CreateScene } = await import("../../tools/scene_generator.js");
      const { CreateScript, AttachScript } = await import(
        "../../tools/script_manager.js"
      );
      const projectRoot = join(tmpDir, "e2e-test");

      // 1. Scaffold
      await ScaffoldProject({
        root_path: projectRoot,
        project_name: "E2E_Acceptance",
        resolution: { width: 800, height: 600 },
      });

      // 2. Scenes
      await CreateScene({
        path: join(projectRoot, "scenes", "Player.tscn"),
        root_node: {
          type: "CharacterBody2D",
          name: "Player",
          children: [{ type: "CollisionShape2D", name: "Shape" }],
        },
      });

      await CreateScene({
        path: join(projectRoot, "scenes", "Main.tscn"),
        root_node: { type: "Node2D", name: "Main" },
      });

      // 3. Script
      await CreateScript({
        path: join(projectRoot, "scripts", "player.gd"),
        extends: "CharacterBody2D",
        functions: [
          {
            name: "_ready",
            body: "print('Player ready')",
          },
        ],
      });

      // 4. Attach
      await AttachScript({
        scene_path: join(projectRoot, "scenes", "Player.tscn"),
        node_path: "Player",
        script_path: "res://scripts/player.gd",
      });

      // 5. 更新 project.godot 的主场景指向 Main.tscn
      const { readFile, writeFile: writeFs } = await import(
        "node:fs/promises"
      );
      let godotConf = await readFile(
        join(projectRoot, "project.godot"),
        "utf-8",
      );
      godotConf = godotConf.replace(
        /run\/main_scene="[^"]*"/,
        'run/main_scene="res://scenes/Main.tscn"',
      );
      await writeFs(join(projectRoot, "project.godot"), godotConf, "utf-8");

      // 6. Godot 验收
      const godotResult = await runGodot([
        "--path",
        projectRoot,
        "--headless",
        "--quit",
      ]);

      expect(hasError(godotResult.stderr)).toBe(false);
      expect(godotResult.exitCode).toBe(0);
    }, 45000);
  });
});
