---
name: godot-docs
description: 📄 [Godot] 产品/技术文档生成 — 读取产品输入，输出 HTML 产品 PRD + YAML 技术 PRD + 架构文档
runAs: inline
profiles: balanced
cost: medium
---

# godot-docs — Godot 产品与架构文档生成

**先读** `docs/product/` 下已有 PRD 文件（如有）。

## 产出物

| 产出 | 路径 | 格式 | 说明 |
|------|------|------|------|
| 产品设计文档 | `docs/product/<项目>-product-prd.html` | HTML | 浏览器打开，含背景/用户画像/MVP/2B2C/设计理念/风险 |
| 技术 PRD | `docs/prd/v1/prd.yaml` | YAML | 结构化、版本化、机器可读，含模块接口定义 + acceptance + 风险评估 |
| 架构文档 | `docs/prd/v1/architecture.html` | HTML | 人读的架构总览、模块分解、依赖关系、任务清单 |

## 产品 PRD 模板（`docs/product/`）

必须包含：
1. 背景与目标（解决什么问题、一句话定位、成功标准）
2. 用户画像（≥3 类，每类含身份/痛点/频率/诉求）
3. MVP 定义（must-have / nice-to-have / 明确不做什么）
4. 2B / 2C 分析
5. 设计理念（体验关键词、核心原则、视觉方向）
6. 非技术风险（市场/运营/增长/变现）

## 技术 PRD 模板（`docs/prd/v1/`）

必须包含：
1. 模块划分（高内聚低耦合，说明划分理由）
2. 接口定义（输入/输出/副作用）
3. 验收标准（每个 acceptance 含 desc/check/layer）
4. 风险评估（7 维：技术债/回滚/灰度/熔断/监控/容错/安全）
5. 依赖关系（depends_on）
6. 架构决策记录（ADR）

## 快速入口

```bash
# 查看已有产品文档
ls docs/product/

# 查看已有技术 PRD
cat docs/prd/v1/prd.yaml | head -50
```
