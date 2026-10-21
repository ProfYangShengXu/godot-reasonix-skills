@tool
extends EditorPlugin

# Godox MCP Bridge — Main EditorPlugin entry point
# Loads on editor startup, starts MCP stdio server for AI Agent communication.
# Godot 4.7+
#
# MCP protocol: JSON-RPC 2.0 over stdin/stdout
# Methods: get_scene_tree, add_node, remove_node, set_node_property, attach_script, query_version

var mcp_server: Reference = null
var input_relay: Reference = null
var log_channel: Reference = null
var validator: Reference = null

# Load submodules on demand
const MCP_SERVER_SCRIPT := preload("res://addons/godox/mcp_server.gd")
const INPUT_RELAY_SCRIPT := preload("res://addons/godox/input_relay.gd")
const LOG_CHANNEL_SCRIPT := preload("res://addons/godox/log_channel.gd")
const VALIDATOR_SCRIPT := preload("res://addons/godox/validator.gd")


func _enter_tree() -> void:
	print("[godox] Plugin loading...")
	_start_mcp_server()
	print("[godox] Plugin ready — MCP server running on stdio")


func _exit_tree() -> void:
	_stop_mcp_server()
	print("[godox] Plugin unloaded")


# ─── G1: MCP Server ───

func _start_mcp_server() -> void:
	if mcp_server != null:
		return
	mcp_server = MCP_SERVER_SCRIPT.new()
	mcp_server.plugin = self
	mcp_server.start()


func _stop_mcp_server() -> void:
	if mcp_server == null:
		return
	mcp_server.stop()
	mcp_server = null


# ─── G2: Input Relay ───

func start_input_relay(port: int = 42069) -> bool:
	if input_relay != null:
		return true
	input_relay = INPUT_RELAY_SCRIPT.new()
	input_relay.plugin = self
	return input_relay.start(port)


func stop_input_relay() -> void:
	if input_relay == null:
		return
	input_relay.stop()
	input_relay = null


# ─── G3: Log Channel ───

func start_log_channel(port: int = 42070) -> bool:
	if log_channel != null:
		return true
	log_channel = LOG_CHANNEL_SCRIPT.new()
	log_channel.plugin = self
	return log_channel.start(port)


func stop_log_channel() -> void:
	if log_channel == null:
		return
	log_channel.stop()
	log_channel = null


# ─── G4: Scene Validator ───

func validate_scene(scene_path: String) -> Dictionary:
	if validator == null:
		validator = VALIDATOR_SCRIPT.new()
		validator.plugin = self
	return validator.validate(scene_path)


# ─── MCP Method Handlers ───

# Return the edited scene's root node
func get_scene_tree() -> Dictionary:
	var root := EditorInterface.get_edited_scene_root()
	if root == null:
		return {"nodes": []}
	return {"nodes": [_serialize_node(root)]}


func _serialize_node(node: Node) -> Dictionary:
	var data := {
		"name": node.name,
		"type": node.get_class(),
		"path": EditorInterface.get_edited_scene_root().get_path_to(node),
		"children": [],
		"properties": _get_node_properties(node)
	}
	for child in node.get_children():
		data["children"].append(_serialize_node(child))
	return data


func _get_node_properties(node: Node) -> Dictionary:
	var props := {}
	var property_list := node.get_property_list()
	for p in property_list:
		var name: String = p["name"]
		# Skip internal and script properties
		if name.begins_with("__") or name in ["script", "unique_name_in_owner"]:
			continue
		if p["usage"] & PROPERTY_USAGE_EDITOR:
			props[name] = var_to_str(node.get(name))
	return props


func add_node(parent_path: String, node_type: String, node_name: String, properties: Dictionary = {}) -> Dictionary:
	var root := EditorInterface.get_edited_scene_root()
	if root == null:
		return {"error": "No scene open"}

	var parent: Node
	if parent_path.is_empty():
		parent = root
	else:
		parent = root.get_node_or_null(parent_path)
		if parent == null:
			return {"error": "Parent node not found: " + parent_path}

	var new_node: Node
	var type_class = ClassDB.instantiate(node_type)
	if type_class == null:
		return {"error": "Unknown node type: " + node_type}
	new_node = type_class
	new_node.name = node_name

	for key in properties:
		var val = str_to_var(properties[key])
		if val != null:
			new_node.set(key, val)

	parent.add_child(new_node)
	new_node.set_owner(root)

	var path := root.get_path_to(new_node)
	return {"node_path": path}


func remove_node(node_path: String) -> Dictionary:
	var root := EditorInterface.get_edited_scene_root()
	if root == null:
		return {"error": "No scene open", "success": false}

	var node := root.get_node_or_null(node_path)
	if node == null:
		return {"error": "Node not found: " + node_path, "success": false}

	node.queue_free()
	return {"success": true}


func set_node_property(node_path: String, properties: Dictionary) -> Dictionary:
	var root := EditorInterface.get_edited_scene_root()
	if root == null:
		return {"error": "No scene open", "success": false}

	var node := root.get_node_or_null(node_path)
	if node == null:
		return {"error": "Node not found: " + node_path, "success": false}

	for key in properties:
		var val = str_to_var(properties[key])
		if val != null:
			node.set(key, val)

	return {"success": true}


func attach_script(node_path: String, script_path: String) -> Dictionary:
	var root := EditorInterface.get_edited_scene_root()
	if root == null:
		return {"error": "No scene open", "success": false}

	var node := root.get_node_or_null(node_path)
	if node == null:
		return {"error": "Node not found: " + node_path, "success": false}

	var script_res := ResourceLoader.load(script_path)
	if script_res == null:
		return {"error": "Script not found: " + script_path, "success": false}

	node.set_script(script_res)
	return {"success": true}
