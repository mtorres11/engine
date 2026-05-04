# Working on macOS without a Windows PC yet

You **cannot compile or run** this engine natively on macOS today (Win32 + D3D9 + PhysX 2.8). You can still **prepare the repo** so the first day you have Windows (or a VM), you only install PhysX + Visual Studio and build.

## Do this on your Mac now

1. **Boost headers (automated)** — from the repo root:

   ```bash
   chmod +x scripts/bootstrap-3rdparty.sh   # once
   ./scripts/bootstrap-3rdparty.sh
   ```

   This creates `3dParty/boost_1_45_0/` (gitignored; large). Requires network.

2. **Local paths template**

   ```bash
   cp .dev.vars.example .dev.vars
   # When you have PhysX installed on Windows, put that machine’s path in PHYSX281_SDK
   # (on Mac you can leave it blank or note the future path for your VM)
   ```

3. **Read the layout docs**

   - `docs/PHYSX281_SDK_LAYOUT.md` — what to install later  
   - `docs/RUNTIME_DLLS.md` — DLLs to copy next to the exe when you get a build  

4. **Optional: CI** — push the repo and use **GitHub Actions** (`.github/workflows/bootstrap-third-party.yml`) to verify **Boost bootstrap** on `macos-latest` / `windows-latest`. Full game build still needs PhysX on a self-hosted runner or manual SDK install.

## When you get Windows (or a VM)

1. Install **Visual Studio** (with C++ desktop workload) — consider upgrading from VS2008 projects when ready.  
2. Install **PhysX SDK 2.8.1**, set **`PHYSX281_SDK`**.  
3. Run `scripts\bootstrap-3rdparty.ps1` if `3dParty\boost_1_45_0` is missing.  
4. Open `Code\videojuego.sln`, build, set working dir to `Run\Test`, copy DLLs per `docs/RUNTIME_DLLS.md`.

## Running “on Mac” without porting

Use a **Windows VM** (Parallels, VMware Fusion, UTM) and follow the Windows steps there. That is still the lowest-effort way to see the original engine alive.
