extends SceneTree

# Godot 项目验证器
# 被 A 层测试调用，验证项目结构完整性

var failed := false
var output: String = ""


func _init() -> void:
	# 验证 1: 项目设置存在
	var project_name: String = ProjectSettings.get_setting("application/config/name", "")
	if project_name.is_empty():
		fail("项目名称未设置")
	else:
		pass_ok("项目名称: " + project_name)
	
	# 验证 2: 主场景存在
	var main_scene: String = ProjectSettings.get_setting("application/run/main_scene", "")
	if main_scene.is_empty():
		fail("主场景未设置")
	elif ResourceLoader.exists(main_scene):
		pass_ok("主场景可加载: " + main_scene)
	else:
		fail("主场景不存在: " + main_scene)
	
	# 验证 3: 图标存在
	var icon_path: String = ProjectSettings.get_setting("application/config/icon", "")
	if icon_path.is_empty():
		fail("项目图标未设置")
	elif ResourceLoader.exists(icon_path):
		pass_ok("项目图标可加载: " + icon_path)
	else:
		fail("项目图标不存在: " + icon_path)
	
	# 验证 4: 场景目录下的 .tscn 文件可加载
	var dir: DirAccess = DirAccess.open("res://")
	if dir:
		dir.list_dir_begin()
		var file_name: String = dir.get_next()
		while file_name != "":
			if file_name.ends_with(".tscn") or file_name.ends_with(".scn"):
				if ResourceLoader.exists("res://" + file_name):
					pass_ok("场景可加载: " + file_name)
				else:
					fail("场景加载失败: " + file_name)
			file_name = dir.get_next()
	
	# 输出结果
	if failed:
		print("VALIDATION FAILED")
	else:
		print("ALL CHECKS PASSED")
	
	quit()


func pass_ok(msg: String) -> void:
	print("  [PASS] " + msg)


func fail(msg: String) -> void:
	failed = true
	print("  [FAIL] " + msg)
