/**
 * A 层验收测试辅助 — 用 Godot CLI 验证项目
 *
 * 创建项目 → 运行 godot --headless 验证 → 解析输出
 */

import { execSync } from "node:child_process";
import { copyFile, mkdir, writeFile, rm } from "node:fs/promises";
import { join, resolve } from "node:path";
import { tmpdir } from "node:os";
import { randomBytes } from "node:crypto";

const GODOT_PATH =
  "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Godot Engine\\godot.windows.opt.tools.64.exe";

export interface ValidationResult {
  passed: boolean;
  stdout: string;
  stderr: string;
  exitCode: number;
  errors: string[];
}

/**
 * 用 Godot 验证项目的完整性
 * 使用 godot --headless --import 加载项目，检查 stderr 中是否有 ERROR 级别输出
 */
export async function validateProject(projectRoot: string): Promise<ValidationResult> {
  const result = await runGodot([
    "--path", projectRoot,
    "--headless",
    "--quit",
  ]);

  // 解析 stderr 中的错误
  const errors: string[] = [];
  const stderrLines = result.stderr.split("\n");
  for (const line of stderrLines) {
    if (line.includes("ERROR") || line.includes("error:")) {
      // 忽略 GDScript 编译中的无害信息
      if (!line.includes("Deprecated") && !line.includes("deprecated")) {
        errors.push(line.trim());
      }
    }
  }

  return {
    ...result,
    passed: errors.length === 0,
    errors,
  };
}

/**
 * 用 Godot 运行验证脚本
 */
export async function runValidatorScript(
  projectRoot: string,
  validatorPath: string,
): Promise<ValidationResult> {
  const result = await runGodot([
    "--path", projectRoot,
    "--headless",
    "--script", validatorPath,
  ]);

  const passed = result.stdout.includes("ALL CHECKS PASSED");
  const errors: string[] = [];
  for (const line of result.stderr.split("\n")) {
    if (line.includes("ERROR") && !line.includes("Deprecated")) {
      errors.push(line.trim());
    }
  }

  return { ...result, passed, errors };
}

async function runGodot(args: string[]): Promise<{
  stdout: string;
  stderr: string;
  exitCode: number;
}> {
  return new Promise((resolve, reject) => {
    const { spawn } = require("node:child_process");
    const child = spawn(GODOT_PATH, args, {
      stdio: ["ignore", "pipe", "pipe"],
      timeout: 30000,
    });

    let stdout = "";
    let stderr = "";

    child.stdout.on("data", (data: Buffer) => {
      stdout += data.toString("utf-8");
    });
    child.stderr.on("data", (data: Buffer) => {
      stderr += data.toString("utf-8");
    });

    child.on("close", (code: number | null) => {
      resolve({ stdout, stderr, exitCode: code ?? -1 });
    });
    child.on("error", (err: Error) => {
      reject(err);
    });
  });
}

/**
 * 创建临时项目并返回路径
 */
export async function createTempProject(
  tool: "scaffolder" | "template",
  templateId?: string,
): Promise<{ path: string; cleanup: () => Promise<void> }> {
  const tmpPath = join(
    tmpdir(),
    `godot-a-test-${randomBytes(4).toString("hex")}`,
  );
  const projectRoot = join(tmpPath, "project");

  if (tool === "scaffolder") {
    // 使用 scaffolder
    const { ScaffoldProject } = await import("../../tools/scaffolder.js");
    const result = await ScaffoldProject({
      root_path: projectRoot,
      project_name: "ATest",
      resolution: { width: 800, height: 600 },
    });
    if (result.error) throw new Error(`Scaffold failed: ${result.error}`);
  } else if (tool === "template" && templateId) {
    // 使用模板库 fork
    const { ForkTemplate } = await import("../../tools/template_library.js");
    const result = await ForkTemplate({
      template_id: templateId,
      target_path: projectRoot,
      project_name: "ATest",
    });
    if (result.error) throw new Error(`Fork failed: ${result.error}`);
  }

  return {
    path: projectRoot,
    cleanup: () => rm(tmpPath, { recursive: true, force: true }),
  };
}
