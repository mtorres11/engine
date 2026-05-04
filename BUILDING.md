# Building and local setup

This repository is a legacy **Visual Studio 2008** solution (`Code/videojuego.sln`, `.vcproj` projects) targeting **Win32**. A future **CMake** workflow may read the same environment variables documented here.

## Prerequisites

- **Windows** with a toolchain capable of building VS2008-era C++ (Visual Studio with MFC/Win32 SDK, or migration to a newer VS—out of scope for this document).
- **NVIDIA PhysX SDK 2.8.1** (legacy). Only `Code/Test/Test.vcproj` embeds hardcoded include/lib paths today; you need this SDK installed to link the Test target.
- **Boost** headers: project files reference `3dParty/boost_1_45_0`, which is **not** shipped in this repo. Extract [Boost 1.45](https://www.boost.org/users/history/version_1_45_0.html) there or set `BOOST_ROOT` for future tooling.
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

Visual Studio **does not** read `.dev.vars` automatically. Until project files are updated to use MSBuild properties or CMake, keep using the IDE’s include/lib directories—or replace the hardcoded PhysX paths in `Code/Test/Test.vcproj` with `$(PHYSX281_SDK)\...` after defining that macro in a shared `.props` file.

## Debugger working directory

Per `Code/Readme.txt`, set the **working directory** for the startup project to `Run/<ProjectName>` (relative to repo root), e.g. `Run/Test` for the Test executable.

The keys `ENGINE_RUN_*` in `.dev.vars.example` are reminders only.

## Hardcoded path audit (`.vcproj`)

| File | Issue |
|------|--------|
| `Code/Test/Test.vcproj` | Absolute PhysX 2.8.1 include and lib paths (`E:\...`, `C:\Archivos de programa\...`) |
| Other `*.vcproj` under `Code/` | No drive-letter paths found; use `$(SolutionDir)\..\3dParty\...` |

## Git

`.dev.vars` is listed in `.gitignore` so machine-specific paths are not committed. Commit **`.dev.vars.example`** only.

## Next steps (not done yet)

- Replace PhysX hardcoding in `Test.vcproj` with `$(PHYSX281_SDK)` via `Directory.Build.props` or CMake.
- Add a CMake preset that reads `ENV{PHYSX281_SDK}` and `ENV{BOOST_ROOT}`.
