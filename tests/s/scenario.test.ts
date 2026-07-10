/**
 * S 层场景测试 — 端到端创建可运行游戏项目
 *
 * 用 M1-M5 工具从零创建一个完整的 Godot 项目，
 * 验证项目结构、文件完整性、内容正确性。
 *
 * 注意：此测试不依赖 godot 可执行文件。
 * 在安装了 Godot 的环境中可以额外跑 godot --headless --check 验证。
 */

import { describe, it, expect, beforeEach, afterEach } from "vitest";
import { mkdir, rm, readFile, access, readdir } from "node:fs/promises";
import { join } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";

// ─── Helpers ───

async function makeTempDir(): Promise<string> {
  const dir = join(tmpdir(), `godot-s-test-${randomBytes(6).toString("hex")}`);
  await mkdir(dir, { recursive: true });
  return dir;
}

/** 递归列出目录下所有文件（相对路径） */
async function listFilesRecursive(dir: string): Promise<string[]> {
  const entries: string[] = [];
  const stack = [""];
  while (stack.length > 0) {
    const prefix = stack.pop()!;
    const fullDir = prefix ? join(dir, prefix) : dir;
    let children: string[];
    try {
      children = await readdir(fullDir, { withFileTypes: true });
    } catch {
      continue;
    }
    for (const child of children) {
      const rel = prefix ? `${prefix}/${child.name}` : child.name;
      if (child.isDirectory()) {
        stack.push(rel);
      } else {
        entries.push(rel);
      }
    }
  }
  return entries.sort();
}

// ─── Tests ───

describe("S 层场景测试 — 端到端游戏项目创建", () => {
  let tmpDir: string;
  let projectRoot: string;

  beforeEach(async () => {
    tmpDir = await makeTempDir();
    projectRoot = join(tmpDir, "end-to-end-game");
  });

  afterEach(async () => {
    await rm(tmpDir, { recursive: true, force: true });
  });

  it("完整流程: 脚手架 → 场景 → 脚本 → 资源 → 验证", async () => {
    // ── 1. M1: Scaffold ──
    const { ScaffoldProject } = await import("../../tools/scaffolder.js");
    const scaffold = await ScaffoldProject({
      root_path: projectRoot,
      project_name: "EndToEndGame",
      resolution: { width: 800, height: 600 },
      renderer: "gl_compatibility",
    });
    expect(scaffold.error).toBeUndefined();

    // ── 2. M2: Create Player Scene ──
    const { CreateScene } = await import("../../tools/scene_generator.js");

    const playerScene = await CreateScene({
      path: join(projectRoot, "scenes", "Player.tscn"),
      root_node: {
        type: "CharacterBody2D",
        name: "Player",
        position: { x: 400, y: 300 },
        children: [
          { type: "CollisionShape2D", name: "CollisionShape" },
          { type: "Sprite2D", name: "Visual" },
        ],
      },
    });
    expect(playerScene.error).toBeUndefined();

    // ── 3. M2: Create Level Scene ──
    const levelScene = await CreateScene({
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
    expect(levelScene.error).toBeUndefined();

    // ── 4. M3: Create Script ──
    const { CreateScript, AttachScript } = await import(
      "../../tools/script_manager.js"
    );

    const playerScript = await CreateScript({
      path: join(projectRoot, "scripts", "player.gd"),
      extends: "CharacterBody2D",
      variables: [
        { name: "speed", type: "int", export: true, default: "300" },
        { name: "jump_velocity", type: "float", export: true, default: "-400.0" },
      ],
      functions: [
        {
          name: "_physics_process",
          args: "delta: float",
          body: [
            "var direction: float = Input.get_axis('ui_left', 'ui_right')",
            "if direction:",
            "\tvelocity.x = direction * speed",
            "else:",
            "\tvelocity.x = move_toward(velocity.x, 0, speed)",
            "move_and_slide()",
          ].join("\n"),
        },
      ],
    });
    expect(playerScript.error).toBeUndefined();

    // ── 5. M3: Attach Script ──
    const attach = await AttachScript({
      scene_path: join(projectRoot, "scenes", "Player.tscn"),
      node_path: "Player",
      script_path: "res://scripts/player.gd",
    });
    expect(attach.error).toBeUndefined();

    // ── 6. M4: Import Resource ──
    const { ImportResource } = await import(
      "../../tools/resource_resolver.js"
    );
    const { writeFile: fsWriteFile } = await import("node:fs/promises");

    const imgPath = join(tmpDir, "icon_coin.png");
    await fsWriteFile(imgPath, "dummy-png-content");

    const resource = await ImportResource({
      source_path: imgPath,
      dest_type: "texture",
      project_root: projectRoot,
    });
    expect(resource.error).toBeUndefined();
    expect(resource.resource_path).toBe("res://assets/textures/icon_coin.png");

    // ── 7. 验证项目结构 ──
    const files = await listFilesRecursive(projectRoot);
    expect(files).toContain("project.godot");
    expect(files).toContain("default_env.tres");
    expect(files).toContain("icon.svg");
    expect(files).toContain("scenes/Player.tscn");
    expect(files).toContain("scenes/Level.tscn");
    expect(files).toContain("scripts/player.gd");
    expect(files).toContain("assets/textures/icon_coin.png");

    // ── 8. 验证文件内容 ──

    // project.godot
    const godotContent = await readFile(
      join(projectRoot, "project.godot"),
      "utf-8",
    );
    expect(godotContent).toContain('config/name="EndToEndGame"');
    expect(godotContent).toContain('renderer/rendering_method="gl_compatibility"');
    expect(godotContent).toContain("viewport_width=800");
    expect(godotContent).toContain("viewport_height=600");

    // Player.tscn
    const playerTscn = await readFile(
      join(projectRoot, "scenes", "Player.tscn"),
      "utf-8",
    );
    expect(playerTscn).toContain('[node name="Player" type="CharacterBody2D"]');
    expect(playerTscn).toContain("script = ExtResource"); // script attached

    // player.gd
    const playerGd = await readFile(
      join(projectRoot, "scripts", "player.gd"),
      "utf-8",
    );
    expect(playerGd).toContain("extends CharacterBody2D");
    expect(playerGd).toContain("@export var speed: int = 300");
    expect(playerGd).toContain("func _physics_process(delta: float):");
  }, 30000);

  it("Fork 模板并定制", async () => {
    const { ForkTemplate, CustomizeTemplate } = await import(
      "../../tools/template_library.js"
    );

    // Fork clicker template
    const forkResult = await ForkTemplate({
      template_id: "clicker",
      target_path: projectRoot,
      project_name: "MyCustomClicker",
    });
    expect(forkResult.error).toBeUndefined();

    // Verify fork structure
    const files = await listFilesRecursive(projectRoot);
    expect(files).toContain("project.godot");
    expect(files).toContain("scenes/Main.tscn");
    expect(files).toContain("scenes/Collectible.tscn");
    expect(files).toContain("scripts/collectible.gd");
    expect(files).toContain("scripts/game_manager.gd");

    // Verify project name updated
    const godotContent = await readFile(
      join(projectRoot, "project.godot"),
      "utf-8",
    );
    expect(godotContent).toContain('config/name="MyCustomClicker"');

    // Verify template metadata match
    const { ListTemplates } = await import("../../tools/template_library.js");
    const templates = await ListTemplates();
    const clicker = templates.find((t) => t.id === "clicker");
    expect(clicker).toBeDefined();
    expect(clicker!.complexity).toBe("低");
    expect(clicker!.tags).toContain("mouse-input");
  }, 30000);
});
