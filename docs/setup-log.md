# boti UE5 Setup Log

**Date:** 2026-08-05  
**Engine:** Unreal Engine 5.8.1 (Changelist 56057345) at `/opt/Linux_Unreal_Engine_5.8.1`  
**Platform:** Linux (Fedora), clang 20.1.8 (bundled), RTX 5090

---

## 1. Engine Binary Check

`UnrealEditor` binary found at `/opt/Linux_Unreal_Engine_5.8.1/Engine/Binaries/Linux/UnrealEditor`.

- `ldd ... | grep "not found"` → **no missing shared libraries**
- Version confirmed: 5.8.1, Changelist 56057345, Branch `++UE5+Release-5.8`

**Status: PASS**

---

## 2. Project File Creation

Created `boti.uproject` with:
- `EngineAssociation`: `/opt/Linux_Unreal_Engine_5.8.1` (absolute path for source-built engine)
- Module: `boti` (Runtime, Default loading phase)
- Plugins: `ModelContextProtocol` (MCP server) + `MCPClientToolset` (MCP client adapter)

---

## 3. C++ Module Source Files

Created minimal boilerplate under `Source/boti/`:

| File | Purpose |
|------|---------|
| `boti.Build.cs` | UBT module rules; depends on Core, CoreUObject, Engine, InputCore, EnhancedInput |
| `boti.h` | Primary module header |
| `boti.cpp` | `IMPLEMENT_PRIMARY_GAME_MODULE` |
| `botiGameModeBase.h` | `AbotiGameModeBase : AGameModeBase` |
| `botiGameModeBase.cpp` | Empty implementation |

Also created required Target files under `Source/`:

| File | Purpose |
|------|---------|
| `boti.Target.cs` | Game target, `BuildSettingsVersion.V7`, `IncludeOrderVersion.Latest` |
| `botiEditor.Target.cs` | Editor target (same settings) |

**Errors encountered and resolved:**
- `EngineIncludeOrderVersion.Unreal5Latest` → does not exist in 5.8; correct value is `EngineIncludeOrderVersion.Latest` (alias for `Unreal5_8`)
- `BuildSettingsVersion.V5` → UBT warned to use V7 (Latest in 5.8) to match engine defaults and avoid property conflict with `UnrealEditor` shared build products

---

## 4. Project File Generation

```
/opt/Linux_Unreal_Engine_5.8.1/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh boti.uproject -game -engine
```

- Make project files: **SUCCESS**
- VS Code project files: failed with `ArgumentOutOfRangeException` in `VSCodeProjectFileGenerator.WriteWorkspaceIgnoreFile` (known UBT 5.8 bug, non-blocking)
- Android SDK warning: expected (not installed), non-blocking

---

## 5. Build Result

```
/opt/Linux_Unreal_Engine_5.8.1/Engine/Build/BatchFiles/Linux/Build.sh botiEditor Linux Development \
  -Project=/home/narfman0/workspace/boti/boti.uproject
```

**Result: Succeeded** (35.78 seconds)

Actions compiled:
1. SharedPCH.UnrealEd.Project.ValApi.ValExpApi.Cpp20.h
2. boti.cpp
3. botiGameModeBase.cpp
4. PerModuleInline.gen.cpp
5. Module.boti.gen.cpp
6. Link libUnrealEditor-boti.so
7. WriteMetadata botiEditor.target

Output: `libUnrealEditor-boti.so` linked into engine Binaries.  
Toolchain: bundled clang 20.1.8 (UE's `v26_clang-20.1.8-rockylinux8`).

---

## 6. MCP Plugin Situation

### Plugin 1: `ModelContextProtocol` ← **MCP SERVER** (what we want)

**Path:** `/opt/Linux_Unreal_Engine_5.8.1/Engine/Plugins/Experimental/ModelContextProtocol/`  
**Description:** "Anthropic MCP (Model Context Protocol) server implementation for Unreal Engine."

This plugin runs an HTTP server **inside** the UE editor. Claude Code (or any MCP client) connects to it.

| Property | Value |
|----------|-------|
| Default port | **8000** |
| Default URL path | `/mcp` |
| Full URL | `http://localhost:8000/mcp` |
| Protocol versions | `2025-11-25`, `2025-06-18`, `2024-11-05` |
| Transport | Streamable HTTP (MCP 2025-03-26 spec) |
| Auto-start | Configurable via `bAutoStartServer` in Editor Preferences |
| Tool search | On by default (`bEnableToolSearch = true`) — LLM discovers tools on demand |

**Modules:** `ModelContextProtocol` (core), `ModelContextProtocolEngine` (settings/auto-start), `ModelContextProtocolEditor` (editor UI)

The server exposes UE tools as MCP tools. Other plugins (like `ModelContextProtocolEngine` built-ins, Editor toolsets) register tools that Claude can call via MCP.

### Plugin 2: `MCPClientToolset` ← **MCP CLIENT** (inverse direction)

**Path:** `/opt/Linux_Unreal_Engine_5.8.1/Engine/Plugins/Experimental/Toolsets/MCPClientToolset/`  
**Description:** "An adapter that allows toolset registry customers (like the EDA) to connect to **local/private MCP servers**."

This plugin is for the UE editor's AI assistant (EDA — Epic's internal AI) to connect OUT to external MCP servers. Not relevant for Claude Code controlling UE.

---

## 7. Next Steps: Wiring Claude Code as MCP Client

The `ModelContextProtocol` plugin is already enabled in `boti.uproject`. To connect Claude Code:

### Step 1 — Enable auto-start in editor preferences

In the running editor: **Edit → Editor Preferences → Plugins → Model Context Protocol**
- Check `bAutoStartServer = true`

Or add to `Saved/Config/LinuxEditor/EditorPerProjectUserSettings.ini`:
```ini
[/Script/ModelContextProtocolEngine.ModelContextProtocolSettings]
bAutoStartServer=True
ServerPortNumber=8000
ServerUrlPath=/mcp
```

### Step 2 — Add MCP server to Claude Code config

In `~/.claude/mcp_servers.json` (or project `.claude/mcp_servers.json`):
```json
{
  "unreal-editor": {
    "type": "http",
    "url": "http://localhost:8000/mcp"
  }
}
```

Or with `claude mcp add`:
```bash
claude mcp add unreal-editor --transport http --url http://localhost:8000/mcp
```

### Step 3 — Launch the editor and verify

```bash
/opt/Linux_Unreal_Engine_5.8.1/Engine/Binaries/Linux/UnrealEditor \
  /home/narfman0/workspace/boti/boti.uproject
```

Once open, Claude Code should see Unreal Editor tools via MCP and can author Blueprints, place actors, edit materials, etc.

---

## Summary

| Check | Result |
|-------|--------|
| Engine binary | ✅ Runs, no missing libs |
| Module compiled | ✅ `libUnrealEditor-boti.so` built in 35s |
| MCP server plugin | ✅ `ModelContextProtocol` — HTTP server on `localhost:8000/mcp` |
| MCP client plugin | ✅ `MCPClientToolset` — for UE→external-server direction |
| Claude Code wiring | ⏳ Needs editor running + MCP config (see Next Steps) |
