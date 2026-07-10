/**
 * I 层集成测试 — 模块组合工作
 *
 * 测试 M1→M2→M3→M4 的完整协作流程。
 * 每个测试在临时目录中创建项目，使用真实文件系统。
 */

import { describe, it, expect, beforeEach, afterEach } from "vitest";
import { mkdir, rm, readFile, access } from "node:fs/promises";
import { join } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";

// ─── Helpers ───

async function makeTempDir(): Promise<string> {
  const dir = join(tmpdir(), `godot-i-test-${randomBytes(6).toString("hex")}`);
  await mkdir(dir, { recursive: true });
  return dir;
}

// ─── Tests ───

describe("I 层集成测试 — 模块间协作", () => {
  let tmpDir: string;
  let projectRoot: string;

  beforeEach(async () => {
    tmpDir = await makeTempDir();
    projectRoot = join(tmpDir, "my-game");
  });

  afterEach(async () => {
    await rm(tmpDir, { recursive: true, force: true });
  });

  // ── M1 + M2: 脚手架 + 场景 ──

  describe("M1 + M2: 脚手架 + 场景生成", () => {
    it("应创建项目并添加复杂场景", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { CreateScene } = await import(
        "../../tools/scene_generator.js"
      );
      // eslint-disable-next-line @typescript-eslint/consistent-type-imports
      type NodeDesc = import("../../tools/scene_generator.js").NodeDesc;

      // 1. Scaffold
      const scaffold = await ScaffoldProject({
        root_path: projectRoot,
        project_name: "IntegrationTest",
      });
      expect(scaffold.error).toBeUndefined();

      // 2. 创建 Player 场景（含子节点树）
      const playerTree: NodeDesc = {
        type: "CharacterBody2D",
        name: "Player",
        position: { x: 100, y: 200 },
        children: [
          {
            type: "CollisionShape2D",
            name: "CollisionShape",
            properties: { disabled: false },
          },
          {
            type: "Sprite2D",
            name: "Sprite",
            properties: { texture: "" },
          },
        ],
      };

      const playerScene = await CreateScene({
        path: join(projectRoot, "scenes", "Player.tscn"),
        root_node: playerTree,
      });
      expect(playerScene.error).toBeUndefined();

      // 验证生成的场景文件
      const content = await readFile(
        join(projectRoot, "scenes", "Player.tscn"),
        "utf-8",
      );
      expect(content).toContain('[node name="Player" type="CharacterBody2D"]');
      expect(content).toContain(
        '[node name="CollisionShape" type="CollisionShape2D" parent="Player"]',
      );
      expect(content).toContain(
        '[node name="Sprite" type="Sprite2D" parent="Player"]',
      );

      // 验证项目目录完整
      await expect(
        access(join(projectRoot, "project.godot")),
      ).resolves.toBeUndefined();
    });
  });

  // ── M1 + M2 + M3: 完整管线 ──

  describe("M1 + M2 + M3: 脚手架 + 场景 + 脚本", () => {
    it("应创建项目、场景、脚本并挂载", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { CreateScene, ParseTscn } = await import(
        "../../tools/scene_generator.js"
      );
      const { CreateScript, AttachScript } = await import(
        "../../tools/script_manager.js"
      );

      // 1. Scaffold
      await ScaffoldProject({
        root_path: projectRoot,
        project_name: "ScriptTest",
      });

      // 2. Create scene
      await CreateScene({
        path: join(projectRoot, "scenes", "Player.tscn"),
        root_node: { type: "CharacterBody2D", name: "Player" },
      });

      // 3. Create script
      const scriptPath = join(projectRoot, "scripts", "player.gd");
      const script = await CreateScript({
        path: scriptPath,
        extends: "CharacterBody2D",
        variables: [
          { name: "speed", type: "int", export: true, default: "200" },
        ],
        functions: [
          {
            name: "_physics_process",
            args: "delta: float",
            body: "move_and_slide()",
          },
        ],
      });
      expect(script.error).toBeUndefined();

      // 4. Attach script to scene node
      const attach = await AttachScript({
        scene_path: join(projectRoot, "scenes", "Player.tscn"),
        node_path: "Player",
        script_path: "res://scripts/player.gd",
      });
      expect(attach.error).toBeUndefined();

      // 5. 验证场景文件已含 script 引用
      const sceneContent = await readFile(
        join(projectRoot, "scenes", "Player.tscn"),
        "utf-8",
      );
      expect(sceneContent).toContain("script = ExtResource");
      expect(sceneContent).toContain("[ext_resource type=\"Script\"");

      // 6. 验证 .gd 文件内容
      const scriptContent = await readFile(scriptPath, "utf-8");
      expect(scriptContent).toContain("extends CharacterBody2D");
      expect(scriptContent).toContain("@export var speed: int = 200");
      expect(scriptContent).toContain("func _physics_process(delta: float):");
    });
  });

  // ── M1 + M4: 资源导入 ──

  describe("M1 + M4: 脚手架 + 资源导入", () => {
    it("应导入资源到脚手架项目中", async () => {
      const { ScaffoldProject } = await import("../../tools/scaffolder.js");
      const { ImportResource } = await import(
        "../../tools/resource_resolver.js"
      );

      // 1. Scaffold
      await ScaffoldProject({
        root_path: projectRoot,
        project_name: "ResourceTest",
      });

      // 2. Create a test image file
      const imgPath = join(tmpDir, "hero.png");
      // 创建一个最小有效的 PNG（仅用于测试路径，不是真正的图片）
      const { writeFile: fsWriteFile } = await import("node:fs/promises");
      await fsWriteFile(imgPath, "fake-png-bytes");

      // 3. Import into project
      const importResult = await ImportResource({
        source_path: imgPath,
        dest_type: "texture",
        project_root: projectRoot,
      });
      expect(importResult.error).toBeUndefined();
      expect(importResult.resource_path).toBe("res://assets/textures/hero.png");

      // 4. 验证文件已复制
      await expect(
        access(join(projectRoot, "assets", "textures", "hero.png")),
      ).resolves.toBeUndefined();
    });
  });

  // ── ForkTemplate + 验证 ──

  describe("M5 ForkTemplate", () => {
    it("应复制模板并更新项目名", async () => {
      const { ForkTemplate, ListTemplates } = await import(
        "../../tools/template_library.js"
      );

      const templates = await ListTemplates();
      expect(templates.length).toBeGreaterThanOrEqual(3);
      expect(templates[0].id).toBe("platformer");

      // Fork the clicker template (simplest)
      const forkDir = join(tmpDir, "forked-game");
      const result = await ForkTemplate({
        template_id: "clicker",
        target_path: forkDir,
        project_name: "MyClicker",
      });
      expect(result.error).toBeUndefined();
      expect(result.project_root).toBe(forkDir);

      // Verify project files
      await expect(
        access(join(forkDir, "project.godot")),
      ).resolves.toBeUndefined();
      await expect(
        access(join(forkDir, "scenes", "Main.tscn")),
      ).resolves.toBeUndefined();

      // Verify project name was updated
      const godotContent = await readFile(
        join(forkDir, "project.godot"),
        "utf-8",
      );
      expect(godotContent).toContain('config/name="MyClicker"');
    });
  });
});
