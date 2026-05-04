# Runtime DLLs (Windows)

After building **Test** (or **Viewer** / **Videogame**), the executable usually needs **native DLLs** next to it (or on `PATH`), in addition to setting the **working directory** to `Run/Test` (etc.).

Paths below are typical for **PhysX 2.8.1 Win32**; your SDK layout may differ slightly—search under `%PHYSX281_SDK%` for these names.

## PhysX 2.8.1 (common)

Copy from the SDK **`bin\win32`** (or **`Bin\win32`**) / shipping folders, depending on your package:

| DLL | Purpose |
|-----|---------|
| `PhysXLoader.dll` | Loads PhysX implementation |
| `PhysXCore.dll` | Core simulation |
| `PhysXCooking.dll` | Mesh cooking (if used at runtime) |

Some builds also ship **`cudart32_*.dll`** or other CUDA-related DLLs if GPU PhysX paths are enabled—match what your installed redist provides.

## Audio

| DLL | Notes |
|-----|--------|
| `bass.dll` | From your **BASS** package under `3dParty/BASS` (or official BASS download). |

If you use **BASS_FX**, add the corresponding DLL as required by that add-on.

## Direct3D 9 / D3DX

The project links **`d3d9`** and **`d3dx9`** (debug vs release variants in project settings).

| DLL | Notes |
|-----|--------|
| `d3d9.dll` | Part of Windows; usually not copied |
| `d3dx9_43.dll` (and friends) | Install **DirectX End-User Runtimes** on the target PC if missing (legacy D3DX). |

Use the same **architecture** (x86 vs x64) as your build. This solution historically targeted **Win32**.

## libxml2 / iconv

If your build links these **DLL** (not only static `.lib`), copy the matching runtime DLLs from the same vendor/build you linked against (`3dParty\libxml2-2.7.2.win32`, etc.).

## Quick check

If the app fails to start with “missing DLL”:

1. Use **Dependencies** (lucasg) or similar on the `.exe` to see the failing name.
2. Copy that DLL beside the `.exe` or install the official runtime package.
