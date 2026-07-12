@echo off
chcp 65001 >nul
title Godot Reasonix Skills — 一键安装

echo.
echo  🍌 Godot Reasonix Skills 安装器
echo  =================================
echo.

:: 检查 Node.js
where node >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo  ❌ 未找到 Node.js。请先安装: https://nodejs.org/ (LTS 18+)
    pause
    exit /b 1
)
echo  ✅ Node.js: OK

:: 安装依赖
if not exist "node_modules" (
    echo  ⏳ 正在安装 npm 依赖...
    call npm install
    if %ERRORLEVEL% neq 0 (
        echo  ❌ npm install 失败
        pause
        exit /b 1
    )
)
echo  ✅ npm 依赖: OK

:: 确定 Reasonix 全局 skills 目录
if "%USERPROFILE%"=="" set "USERPROFILE=C:\Users\%USERNAME%"
set "REASONIX_SKILLS=%USERPROFILE%\.reasonix\skills"
set "GODOT_SKILLS=.reasonix\skills"

:: 检查是否有 skills 要安装
if not exist "%GODOT_SKILLS%" (
    echo  ⚠️  未找到技能目录 (.reasonix/skills/)，跳过全局安装
    echo  本项目的技能已直接可用。
    goto :done
)

echo  📂 安装技能到全局: %REASONIX_SKILLS%

:: 创建目标目录
if not exist "%REASONIX_SKILLS%" mkdir "%REASONIX_SKILLS%"

:: 复制每个 skill 目录
for /d %%i in ("%GODOT_SKILLS%\*") do (
    set "SKILL_NAME=%%~nxi"
    if not exist "%REASONIX_SKILLS%\%%~nxi" mkdir "%REASONIX_SKILLS%\%%~nxi"
    copy /y "%%i\SKILL.md" "%REASONIX_SKILLS%\%%~nxi\SKILL.md" >nul 2>&1
    if exist "%%i\*.md" copy /y "%%i\*.md" "%REASONIX_SKILLS%\%%~nxi\" >nul 2>&1
    echo   ✅ 已安装: %%~nxi
)

:: 复制 Bobanana.md
if exist "Bobanana.md" (
    copy /y "Bobanana.md" "%USERPROFILE%\.reasonix\Bobanana.md" >nul 2>&1
    echo   ✅ 已安装: Bobanana.md（工程原则）
)

echo.
echo  ⏳ 编译检查 TypeScript...
call npx tsc --noEmit 2>&1 | findstr /V "node_modules" | findstr /C:"error"
if %ERRORLEVEL% equ 0 (
    echo  ⚠️  存在 TypeScript 编译错误，请检查
) else (
    echo  ✅ TypeScript 编译: 通过
)

:done
echo.
echo  =================================
echo  🎉 安装完成！
echo.
echo  在 Reasonix 中使用:
echo    /pipeline 用 Godot 做一个平台跳跃游戏
echo    /run_skill godot-dev
echo    /run_skill godot-test
echo.
pause
