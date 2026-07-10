/**
 * M2 · Scene Generator — U 层测试
 *
 * 覆盖 M2-A1（正常路径）、M2-A2（异常路径）、M2-A4（对抗路径）。
 * 使用 RenderTscn / ParseTscn 纯函数，避免文件系统 IO。
 */

import { describe, it, expect, beforeEach, afterEach } from "vitest";
import { mkdir, rm, writeFile } from "node:fs/promises";
import { join } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";
import {
  RenderTscn,
  ParseTscn,
  type NodeDesc,
} from "../../tools/scene_generator.js";

// ─── Helpers ───

function countNodes(tscn: string): number {
  return (tscn.match(/^\[node /gm) || []).length;
}

function findNodeProperty(tscn: string, nodeName: string, propKey: string): string | null {
  const lines = tscn.split("\n");
  let inNode = false;
  for (const line of lines) {
    if (line.startsWith(`[node name="${nodeName}"`)) {
      inNode = true;
      continue;
    }
    if (inNode) {
      if (line.startsWith("[")) break; // next section
      if (line.startsWith(`${propKey} = `)) {
        return line.slice(propKey.length + 3).trim();
      }
    }
  }
  return null;
}

// ─── Tests ───

describe("M2 · Scene Generator — U 层", () => {
  // ── M2-A1: 正常路径 ──

  describe("M2-A1: 正常路径 — 创建场景", () => {
    it("应生成含根节点的合法 .tscn", () => {
      const tscn = RenderTscn({
        path: "scenes/Main.tscn",
        root_node: { type: "Node2D", name: "Main" },
      });

      expect(tscn).toContain("[gd_scene");
      expect(tscn).toContain('[node name="Main" type="Node2D"]');
      expect(tscn).toContain("format=3");
    });

    it("应创建含 CharacterBody2D + Sprite2D + CollisionShape2D 的完整场景", () => {
      const tree: NodeDesc = {
        type: "CharacterBody2D",
        name: "Player",
        position: { x: 100, y: 200 },
        children: [
          {
            type: "Sprite2D",
            name: "Sprite",
            properties: { texture: "" },
          },
          {
            type: "CollisionShape2D",
            name: "CollisionShape",
            properties: { disabled: false },
          },
        ],
      };

      const tscn = RenderTscn({
        path: "scenes/Player.tscn",
        root_node: tree,
      });

      expect(countNodes(tscn)).toBe(3);
      expect(tscn).toContain('[node name="Player" type="CharacterBody2D"]');
      expect(tscn).toContain('[node name="Sprite" type="Sprite2D" parent="Player"]');
      expect(tscn).toContain('[node name="CollisionShape" type="CollisionShape2D" parent="Player"]');
    });

    it("位置属性应正确序列化为 Vector2", () => {
      const tscn = RenderTscn({
        path: "scenes/Test.tscn",
        root_node: {
          type: "Node2D",
          name: "Test",
          position: { x: 150, y: 300 },
        },
      });

      expect(findNodeProperty(tscn, "Test", "position")).toBe("Vector2(150, 300)");
    });

    it("布尔属性应序列化为 true/false", () => {
      const tscn = RenderTscn({
        path: "scenes/Test.tscn",
        root_node: {
          type: "Node2D",
          name: "Test",
          visible: false,
          properties: { disabled: true },
        },
      });

      expect(findNodeProperty(tscn, "Test", "visible")).toBe("false");
      expect(findNodeProperty(tscn, "Test", "disabled")).toBe("true");
    });

    it("应生成完整的 round-trip（序列化→解析→再序列化）", () => {
      const original = RenderTscn({
        path: "scenes/RoundTrip.tscn",
        root_node: {
          type: "Node2D",
          name: "Root",
          position: { x: 10, y: 20 },
          children: [
            { type: "Sprite2D", name: "Child", position: { x: 0, y: 0 } },
          ],
        },
      });

      const parsed = ParseTscn(original);
      expect(parsed.nodes).toHaveLength(2);
      expect(parsed.nodes[0].name).toBe("Root");
      expect(parsed.nodes[1].name).toBe("Child");
      expect(parsed.nodes[1].parent).toBe("Root");
    });
  });

  // ── M2-A2: 异常路径 ──

  describe("M2-A2: 异常路径 — 错误处理", () => {
    it("空 path 应返回错误", async () => {
      const { CreateScene } = await import("../../tools/scene_generator.js");
      const result = await CreateScene({
        path: "",
        root_node: { type: "Node2D", name: "Test" },
      });
      expect(result.error).toBeTruthy();
    });

    it("空根节点名应返回错误", async () => {
      const { CreateScene } = await import("../../tools/scene_generator.js");
      const result = await CreateScene({
        path: "/tmp/test.tscn",
        root_node: { type: "Node2D", name: "" },
      });
      expect(result.error).toBeTruthy();
    });

    it("不存在的父节点添加子节点应返回错误（运行时）", async () => {
      const { AddNode } = await import("../../tools/scene_generator.js");
      const result = await AddNode({
        scene_path: "/tmp/nonexistent.tscn",
        parent_path: "NonExistent",
        node: { type: "Sprite2D", name: "Child" },
      });
      expect(result.error).toBeTruthy();
    });
  });

  // ── M2-A3: 边界路径 ──

  describe("M2-A3: 边界路径 — 边界情况", () => {
    it("含中文节点名应正确编码", () => {
      const tscn = RenderTscn({
        path: "scenes/游戏.tscn",
        root_node: { type: "Node2D", name: "游戏主场景" },
      });

      expect(tscn).toContain('name="游戏主场景"');
    });

    it("大量属性不应丢失", () => {
      const props: Record<string, number | string | boolean> = {};
      for (let i = 0; i < 20; i++) {
        props[`prop_${i}`] = i;
      }

      const tscn = RenderTscn({
        path: "scenes/BigProps.tscn",
        root_node: { type: "Node2D", name: "Big", properties: props },
      });

      for (let i = 0; i < 20; i++) {
        expect(findNodeProperty(tscn, "Big", `prop_${i}`)).toBe(String(i));
      }
    });

    it("特殊字符在字符串属性中应转义", () => {
      const tscn = RenderTscn({
        path: "scenes/Escape.tscn",
        root_node: {
          type: "Label",
          name: "Label",
          properties: { text: 'hello "world" \\test' },
        },
      });

      const val = findNodeProperty(tscn, "Label", "text");
      expect(val).toBe('"hello \\"world\\" \\\\test"');
    });
  });

  // ── M2-A4: 对抗路径 ──

  describe("M2-A4: 对抗路径 — 异常场景内容", () => {
    it("解析空字符串不应崩溃", () => {
      const result = ParseTscn("");
      expect(result.nodes).toEqual([]);
      expect(result.extResources).toEqual([]);
    });

    it("解析格式错误的场景不应崩溃", () => {
      const garbled = `[gd_scene format=3 uid="uid://test"]
garbage content
[node name="Broken" type="Node2D"
 missing close bracket
`;
      const result = ParseTscn(garbled);
      // 解析器容错：header 不完整的节点被忽略，不崩溃即可
      expect(Array.isArray(result.nodes)).toBe(true);
      expect(Array.isArray(result.extResources)).toBe(true);
    });

    it("移除不存在的节点应返回错误", async () => {
      // 先创建临时场景
      const tmpDir = join(tmpdir(), `scene-test-${randomBytes(4).toString("hex")}`);
      await mkdir(tmpDir, { recursive: true });
      const scenePath = join(tmpDir, "test.tscn");
      const { CreateScene, RemoveNode } = await import("../../tools/scene_generator.js");
      await CreateScene({
        path: scenePath,
        root_node: { type: "Node2D", name: "Root" },
      });

      const result = await RemoveNode({
        scene_path: scenePath,
        node_path: "NonExistent",
      });
      expect(result.error).toContain("不存在");

      await rm(tmpDir, { recursive: true, force: true });
    });
  });
});
