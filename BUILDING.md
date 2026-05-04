# Building and local setup

This repository is a legacy **Visual Studio 2008** solution (`Code/videojuego.sln`, `.vcproj` projects) targeting **Win32**. A future **CMake** workflow may read the same environment variables documented here.

**On macOS only?** See **`docs/MACOS_WITHOUT_WINDOWS.md`** — you can still fetch Boost and prep the repo; you cannot compile the game natively until you have Windows (or a VM).

**On Windows (step-by-step):** **`docs/WINDOWS_QUICKSTART.md`**

## Prerequisites

- **Windows** with a toolchain capable of building VS2008-era C++ (Visual Studio with MFC/Win32 SDK, or migration to a newer VS—out of scope for this document).
- **NVIDIA PhysX SDK 2.8.1** (legacy). The **Test** project uses the environment variable **`PHYSX281_SDK`** (root folder that contains `SDKs/`). Install the SDK, then set the variable **before** starting Visual Studio (see below). Layout: **`docs/PHYSX281_SDK_LAYOUT.md`**.
- **Boost** headers: project files reference `3dParty/boost_1_45_0` (not committed; **gitignored**). Fetch automatically:

  ```bash
  ./scripts/bootstrap-3rdparty.sh
  ```

  On Windows (PowerShell): `.\scripts\bootstrap-3rdparty.ps1`
- **Vendored deps** under `3dParty/`: Lua, luabind, Cal3D, BASS, libxml2 (win32), iconv (win32), etc.—verify they are present after clone.

## Local paths: `.dev.vars`

1. Copy the template and edit:

   ```bash
   cp .dev.vars.example .dev.vars
   ```

2. Set at least `PHYSX281_SDK` to the root of your PhysX 2.8.1 install (the directory that contains `SDKs/`).

3. Load variables into your shell (useful for CMake, scripts, or documentation):

   **bash / zsh**

   ```bash
   source scripts/load-dev-vars.sh
   ```

   **PowerShell**

   ```powershell
   . .\scripts\load-dev-vars.ps1
   ```

Visual Studio **does not** read `.dev.vars` automatically. For **`Test.vcproj`**, includes and libs use **`$(PHYSX281_SDK)\...`**, which resolves from the **process environment** when the build runs.

**Windows — set `PHYSX281_SDK` so VS sees it**

- **Option A:** *System properties → Environment variables* → User or System → New → `PHYSX281_SDK` = `C:\path\to\NVIDIA PhysX SDK\v2.8.1` (no trailing backslash required). Restart Visual Studio.
- **Option B:** Launch Visual Studio from a shell where the variable is already set, e.g. after `set PHYSX281_SDK=C:\path\to\...\v2.8.1` (cmd) or `. .\scripts\load-dev-vars.ps1` (PowerShell, from repo root with `.dev.vars` filled in).
- **Option C:** `setx PHYSX281_SDK "C:\path\to\...\v2.8.1"` then open a **new** terminal / VS instance.

If `PHYSX281_SDK` is empty, the compiler will look under invalid paths — ensure it is set.

## Debugger working directory

Per `Code/Readme.txt`, set the **working directory** for the startup project to `Run/<ProjectName>` (relative to repo root), e.g. `Run/Test` for the Test executable.

The keys `ENGINE_RUN_*` in `.dev.vars.example` are reminders only.

## Path audit (`.vcproj`)

| File | Notes |
|------|--------|
| `Code/Test/Test.vcproj` | PhysX uses **`$(PHYSX281_SDK)`** (env). |
| Other `*.vcproj` under `Code/` | No drive-letter paths found in prior audit; use `$(SolutionDir)\..\3dParty\...` |

## Git

`.dev.vars` is listed in `.gitignore` so machine-specific paths are not committed. Commit **`.dev.vars.example`** only.

## Next steps

See **`ROADMAP.md`** for revive milestones. Short term: run **`scripts/bootstrap-3rdparty`**, set **PhysX** env on Windows, build, then copy runtime DLLs per **`docs/RUNTIME_DLLS.md`**.

- CI: **`.github/workflows/bootstrap-third-party.yml`** checks Boost bootstrap on GitHub (push to `main`/`master` or **Run workflow**).
- Add **CMake** that reads `ENV{PHYSX281_SDK}` (planned).
