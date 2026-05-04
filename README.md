# Engine

Legacy Win32 game engine (Visual Studio solution under **`Code/`**).

## Quick links

- **Build on Windows:** [`docs/WINDOWS_QUICKSTART.md`](docs/WINDOWS_QUICKSTART.md) — clone, PhysX env, Boost bootstrap, run **Test**
- **Mac only (prep, no native run):** [`docs/MACOS_WITHOUT_WINDOWS.md`](docs/MACOS_WITHOUT_WINDOWS.md)
- **Details:** [`BUILDING.md`](BUILDING.md), [`ROADMAP.md`](ROADMAP.md)
- **Local paths template:** copy `.dev.vars.example` → `.dev.vars`

## Third-party fetch (Boost)

- Windows: `.\scripts\bootstrap-3rdparty.ps1`
- macOS/Linux: `./scripts/bootstrap-3rdparty.sh`

## Windows: install tools + optional build/run

- `powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1`  
  See **`docs/WINDOWS_QUICKSTART.md`** for flags (`-LaunchVS`, `-Build`, `-Run`).

PhysX 2.8.1 is **not** downloaded by scripts; set **`PHYSX281_SDK`** on Windows (see docs).
