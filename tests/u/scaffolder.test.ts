/**
 * M1 · Project Scaffolder — U 层测试
 *
 * 覆盖 M1-A1（正常路径）和 M1-A2（异常路径）。
 * 使用临时目录隔离文件系统副作用。
 */

import { describe, it, expect, beforeEach, afterEach } from "vitest";
import { mkdir, rm, readFile, access, writeFile } from "node:fs/promises";
import { join } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";
import { ScaffoldProject } from "../../tools/scaffolder.js";

// ─── Helper: 创建独立临时目录 ───

async function makeTempDir(): Promise<string> {
  const dir = join(tmpdir(), `godot-skills-test-${randomBytes(6).toString("hex")}`);
  await mkdir(dir, { recursive: true });
  return dir;
}

async function removeTempDir(dir: string): Promise<void> {
  await rm(dir, { recursive: true, force: true });
}

// ─── Helper: 验证文件存在 ───

async function fileExists(path: string): Promise<boolean> {
  try {
    await access(path);
    return true;
  } catch {
    return false;
  }
}

// ─── Tests ───

describe("M1 · ScaffoldProject — U 层", () => {
  let tmpDir: string;

  beforeEach(async () => {
    tmpDir = await makeTempDir();
  });

  afterEach(async () => {
    await removeTempDir(tmpDir);
  });

  // ── M1-A1: 正常路径 ──

  describe("M1-A1: 正常路径 — 创建完整项目", () => {
    it("应创建目录结构并返回 project_root", async () => {
      const projectPath = join(tmpDir, "my-game");

      const result = await ScaffoldProject({
        root_path: projectPath,
        project_name: "My Game",
      });

      expect(result.error).toBeUndefined();
      expect(result.project_root).toBe(projectPath);

      // 验证目录存在
      const dirs = [
        projectPath,
        join(projectPath, "scenes"),
        join(projectPath, "scripts"),
        join(projectPath, "assets", "textures"),
        join(projectPath, "assets", "audio"),
        join(projectPath, "assets", "fonts"),
      ];
      for (const dir of dirs) {
        await expect(access(dir)).resolves.toBeUndefined();
      }
    });

    it("应生成 project.godot 且内容包含项目名和渲染器配置", async () => {
      const projectPath = join(tmpDir, "config-check");

      await ScaffoldProject({
        root_path: projectPath,
        project_name: "ConfigCheck",
        resolution: { width: 1920, height: 1080 },
        renderer: "mobile",
      });

      const content = await readFile(join(projectPath, "project.godot"), "utf-8");
      expect(content).toContain('config/name="ConfigCheck"');
      expect(content).toContain('renderer/rendering_method="mobile"');
      expect(content).toContain("viewport_width=1920");
      expect(content).toContain("viewport_height=1080");
    });

    it("应生成 default_env.tres、icon.svg 和默认场景", async () => {
      const projectPath = join(tmpDir, "file-check");

      await ScaffoldProject({
        root_path: projectPath,
        project_name: "FileCheck",
      });

      await expect(access(join(projectPath, "default_env.tres"))).resolves.toBeUndefined();
      await expect(access(join(projectPath, "icon.svg"))).resolves.toBeUndefined();
      await expect(access(join(projectPath, "scenes", "Main.tscn"))).resolves.toBeUndefined();
    });

    it("默认场景 Main.tscn 应为合法格式", async () => {
      const projectPath = join(tmpDir, "scene-check");

      await ScaffoldProject({
        root_path: projectPath,
        project_name: "SceneCheck",
      });

      const scene = await readFile(join(projectPath, "scenes", "Main.tscn"), "utf-8");
      expect(scene).toContain("[gd_scene");
      expect(scene).toContain('[node name="Main" type="Node2D"]');
    });
  });

  // ── M1-A2: 异常路径 ──

  describe("M1-A2: 异常路径 — 错误处理", () => {
    it("root_path 为空应返回错误", async () => {
      const result = await ScaffoldProject({
        root_path: "",
        project_name: "Test",
      });
      expect(result.error).toBeTruthy();
    });

    it("project_name 为空应返回错误", async () => {
      const result = await ScaffoldProject({
        root_path: join(tmpDir, "test"),
        project_name: "   ",
      });
      expect(result.error).toBeTruthy();
    });

    it("路径含 ../ 穿越应拒绝", async () => {
      const result = await ScaffoldProject({
        root_path: "some/../../evil",
        project_name: "Evil",
      });
      expect(result.error).toContain("非法路径");
    });

    it("分辨率越界应拒绝", async () => {
      const result = await ScaffoldProject({
        root_path: join(tmpDir, "res-check"),
        project_name: "ResCheck",
        resolution: { width: 0, height: 100 },
      });
      expect(result.error).toBeTruthy();
    });

    it("不支持的版本应拒绝", async () => {
      const result = await ScaffoldProject({
        root_path: join(tmpDir, "ver-check"),
        project_name: "VersionCheck",
        version: "3.x",
      });
      expect(result.error).toContain("不支持的 Godot 版本");
    });

    it("已存在合法 Godot 项目时应拒绝覆盖", async () => {
      // 先创建一个合法项目
      const projectPath = join(tmpDir, "existing");
      await ScaffoldProject({
        root_path: projectPath,
        project_name: "Original",
      });

      // 再次创建同一路径 → 应返回错误
      const result = await ScaffoldProject({
        root_path: projectPath,
        project_name: "Overwrite",
      });
      expect(result.error).toContain("已包含 Godot 项目");
    });

    it("已存在 project.godot 时不覆盖", async () => {
      const projectPath = join(tmpDir, "existing2");
      await mkdir(projectPath, { recursive: true });
      await writeFile(join(projectPath, "project.godot"), "; existing", "utf-8");

      const result = await ScaffoldProject({
        root_path: projectPath,
        project_name: "Overwrite2",
      });
      // 验证未被覆盖
      const content = await readFile(join(projectPath, "project.godot"), "utf-8");
      expect(content).toBe("; existing");
    });
  });
});
