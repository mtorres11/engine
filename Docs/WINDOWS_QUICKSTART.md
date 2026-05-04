# Windows quickstart (clone → build → run)

Use this on the Windows PC when you **pull from Git** (work on Mac with Cursor, test on Windows).

## 0. Automated setup (optional)

From an **elevated or normal** PowerShell prompt at the **repo root** (Windows 10/11 with **winget**):

```powershell
Set-ExecutionPolicy -Scope CurrentUser RemoteSigned -Force   # once per machine, if scripts are blocked
powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1
```

This tries to install **Git**, **Visual Studio 2022 Community** (C++ desktop workload), run the **DirectX web installer**, and run **Boost bootstrap**. It does **not** install **PhysX 2.8.1** (set `PHYSX281_SDK` yourself).

Useful flags:

| Flag | Meaning |
|------|--------|
| `-LaunchVS` | Open `Code\videojuego.sln` after setup (for migration / first build). |
| `-Build` | Try **MSBuild** (often fails until VS has migrated `.vcproj` once). |
| `-Run` | Run `Bin\Test\Test_d.exe` with cwd `Run\Test` if the exe exists. |
| `-SkipPrereqInstall` | Only run Boost bootstrap (no winget). |
| `-SkipPhysXCheck` | Hide the PhysX env warning. |

Then continue with **§1** below if anything failed (corporate PCs often block `winget` or VS install).

---

## 0b. What you must install (one-time, if not using the script)

| Item | Why |
|------|-----|
| **Git** | `git pull` |
| **Visual Studio** with **Desktop development with C++** | Prefer **VS 2019/2022**; opening the old `.sln` will offer **migration** from `.vcproj` to `.vcxproj`. VS 2008 is obsolete on modern Windows. |
| **NVIDIA PhysX SDK 2.8.1** | Headers + libs for the **PhysX** engine project and **Test** / **Viewer**. See `docs/PHYSX281_SDK_LAYOUT.md`. |
| **DirectX** | Code uses D3D9 + D3DX. Install legacy **DirectX End-User Runtimes** if DLLs are missing at run time. |

Optional: **Windows SDK** / old **DirectX SDK (June 2010)** if the linker cannot find `d3dx9.lib` (depends on VS setup).

## 1. Environment variable (every PC user)

Set **`PHYSX281_SDK`** to the **root** of the PhysX 2.8.1 install (folder that contains `SDKs`).

**PowerShell (current user, persistent):**

```powershell
[System.Environment]::SetEnvironmentVariable("PHYSX281_SDK", "C:\Path\To\NVIDIA PhysX SDK\v2.8.1", "User")
```

Close and reopen **Visual Studio** (and any terminal) so the variable is picked up.

## 2. Clone / pull and fetch Boost

```powershell
cd C:\path\where\you\keep\repos
git clone <your-remote> engine
cd engine
git pull
powershell -ExecutionPolicy Bypass -File .\scripts\bootstrap-3rdparty.ps1
```

This creates `3dParty\boost_1_45_0` (not stored in git).

## 3. Open the solution

- Path: **`Code\videojuego.sln`**
- If Visual Studio asks to **upgrade** projects, accept (retarget Windows SDK / toolset as prompted).

After upgrade, verify **Additional Include** / **Library** paths still contain **`$(PHYSX281_SDK)\...`** on **Test**, **Viewer**, and **PhysX** projects. If a migrated project drops them, re-add using the same paths as in git’s `.vcproj` or compare with another machine.

## 4. Build order

1. Build **static libraries** first: **Base**, **Core**, **Graphics**, **Input**, **GUI**, **Sound**, **PhysX**, **Lua**, **LUABind**, **Cal3D**, **Network** (if used).
2. Then build **Test** (or **Viewer**).

**Configuration:** start with **Debug | Win32** (or **x86**) to match original **Win32** libs from PhysX 2.8.1 `SDKs\lib\Win32`.

## 5. Run **Test**

1. **Startup project:** right‑click **Test** → *Set as Startup Project*.
2. **Working directory:** Project → **Test** → Properties → **Debugging** → **Working Directory**:

   `$(SolutionDir)..\..\Run\Test`

   Or absolute: `C:\...\engine\Run\Test` (repo root `\Run\Test`).

3. **F5** or run `Bin\Test\Test_d.exe` (name may vary) from that working directory.

## 6. If the exe fails to start

Copy runtime DLLs beside the `.exe` or onto `PATH` — see **`docs/RUNTIME_DLLS.md`** (PhysX, BASS, D3DX).

## 7. Workflow with Mac

1. Commit and push on Mac.  
2. On Windows: `git pull`.  
3. Rebuild affected projects.  
4. If only docs/scripts changed, no rebuild.

## What was wrong before (now fixed in repo)

- **`Engine\PhysX\PhysX.vcproj`** did not add PhysX SDK **include** paths even though `PhysicsManager.cpp` includes `NxPhysics.h`. Builds would fail on a clean machine until **`PHYSX281_SDK`** was on the **global** include path.
- **`Viewer`** linked `PhysXLoader.lib` but did not add **`$(PHYSX281_SDK)\SDKs\lib\Win32`** to **Additional Library Directories**; Release used a bogus `3dParty\PhysX` include path.

You still must set **`PHYSX281_SDK`** and install the SDK locally — that cannot be pushed to git (license/size).
