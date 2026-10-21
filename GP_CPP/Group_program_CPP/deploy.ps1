$QtDir = "D:\Qt\6.6.1\mingw_64\bin"
$BinDir = ".\build\bin"

# 复制 Qt DLL
Copy-Item "$QtDir\Qt6Core.dll" $BinDir
Copy-Item "$QtDir\Qt6Gui.dll" $BinDir
Copy-Item "$QtDir\Qt6Widgets.dll" $BinDir
Copy-Item "$QtDir\Qt6Network.dll" $BinDir

# 复制平台插件
New-Item -ItemType Directory -Force -Path "$BinDir\platforms"
Copy-Item "$QtDir\..\plugins\platforms\qwindows.dll" "$BinDir\platforms\"

# 复制样式插件
New-Item -ItemType Directory -Force -Path "$BinDir\styles"
Copy-Item "$QtDir\..\plugins\styles\qwindowsvistastyle.dll" "$BinDir\styles\"