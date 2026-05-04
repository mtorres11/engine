# Revive engine — roadmap

Goal: see the project **running again**, ideally from **macOS and Windows** with **as little work as reasonable**.

**Reality check:** This codebase is **Win32 + Direct3D 9 + DirectInput + PhysX 2.8**. There is **no small change** that makes it native on macOS. You choose one of:

| Track | “Both environments” meaning | Effort |
|-------|-----------------------------|--------|
| **A — Pragmatic** | **Windows = native build/run.** **macOS = Windows VM** (Parallels, VMware, UTM) or remote Windows PC. | Days to first run (after SDKs). |
| **B — Dual native** | Real macOS + Windows binaries from one tree: new **window/input** layer (e.g. SDL2), **graphics** port off D3D9, **physics** off PhysX 2.8. | Weeks to months (POC smaller than full parity). |

This roadmap assumes you want **Track B eventually**, but uses **Track A milestones** so you get a **working game on Windows quickly** while the port proceeds.

---

## Phase 0 — Repo usable on any Windows PC (in progress)

**Outcome:** Clone → set env → build **Test** without editing `vcproj` paths.

| Step | Status | Notes |
|------|--------|--------|
| 0.1 `.dev.vars` + loader scripts + `BUILDING.md` | Done | |
| 0.2 PhysX paths use `$(PHYSX281_SDK)` | Done | On **Test**, **Engine/PhysX**, and **Viewer** `.vcproj` files. Set **`PHYSX281_SDK`** before opening Visual Studio. |
| 0.3 **Boost** into `3dParty/boost_1_45_0` | Done (automated) | Run `scripts/bootstrap-3rdparty.sh` (Mac/Git Bash) or `scripts/bootstrap-3rdparty.ps1` (Windows). Folder is **gitignored**. |
| 0.4 Runtime DLLs next to exe | Done (doc) | See **`docs/RUNTIME_DLLS.md`**. |

---

## Phase 1 — Reliable Windows “playable” build

**Outcome:** Double-click or F5 from VS; stable enough for a POC video.

1. **Working directory** = `Run/Test` (or chosen target). Already documented in `Code/Readme.txt`.
2. **Remove reliance on `/FORCE`** in linker when possible — fix real unresolved symbols.
3. **Upgrade toolchain** (recommended): migrate solution to **Visual Studio 2019/2022** (`vcxproj`) when you’re ready — fewer headaches than hunting VS2008 on modern Windows.
4. **DirectX**: ensure **D3DX9** runtimes / dev libs available on build and run machines.

---

## Phase 2 — Cross-platform skeleton (first real step toward native macOS)

**Outcome:** One **CMake** (or premake) build that still targets **Windows only**, but all third-party paths come from **env / vcpkg / submodule**.

1. Add **CMakeLists.txt** at repo root: Windows + Win32/x64, finds PhysX via `PHYSX281_SDK`, links same libs as Test.
2. Optionally introduce **SDL2** as a **second window** path behind `#ifdef` — still D3D9 first, or stub GL clear — *only if* you want incremental port.

---

## Phase 3 — Graphics abstraction (largest code change)

**Outcome:** Your game code talks to **`IRenderDevice`** (or similar), not `LPDIRECT3DDEVICE9`.

1. Wrap **D3D9** behind an interface; keep behavior on Windows.
2. Add **OpenGL 3.3+** or **bgfx** backend; implement only what **Test** needs first (clear, meshes, textures, basic shaders).
3. Replace **D3DX** helpers (math, texture load) with **glm**, **stb_image**, or **DirectXTex** on Windows-only path.

---

## Phase 4 — Input + platform

1. Replace **DirectInput / XInput** with **SDL2** (keyboard, mouse, gamepad) behind `IInputSystem`.
2. Replace **WinMain / message loop** with SDL2 loop; keep game update order the same.

---

## Phase 5 — Physics

1. **Short POC:** disable or stub physics-heavy scenes; or keep **PhysX 2.8** **Windows-only** behind interface while macOS uses **no-op** or **Jolt** with simplified levels.
2. **Full:** Reimplement `PhysicsManager` against **Jolt** or **PhysX 4/5** — large API change from `Nx*`.

---

## Phase 6 — Scripting / deps cleanup

1. **Lua 5.4** + **sol2** (or keep 5.1 for a while) replacing **luabind**.
2. **libxml2 / iconv**: vcpkg or system libs per OS.

---

## What to do next (order)

1. **You (any OS):** Run **`./scripts/bootstrap-3rdparty.sh`** so Boost exists under `3dParty/boost_1_45_0` (gitignored).
2. **You (Windows or VM):** Obtain **PhysX 2.8.1**, set **`PHYSX281_SDK`**, open **`Code/videojuego.sln`**, build, working dir **`Run/Test`**, runtime DLLs per **`docs/RUNTIME_DLLS.md`**.
3. **You (macOS, no Windows yet):** Follow **`docs/MACOS_WITHOUT_WINDOWS.md`**; optional GitHub Actions **bootstrap-third-party** workflow.
4. **We (repo):** CMake (Phase 2) when a Windows build has been confirmed once.

When Phase 1 is done, say so and we pick the next ticket (CMake vs first `IRenderDevice` spike).
