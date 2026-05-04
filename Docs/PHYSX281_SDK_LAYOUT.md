# PhysX 2.8.1 SDK layout

The **Test** project expects the environment variable **`PHYSX281_SDK`** to point at the **root** of the legacy **NVIDIA PhysX SDK 2.8.1** installation (the folder that contains a `SDKs` directory).

## Expected layout

```
%PHYSX281_SDK%/
  SDKs/
    Foundation/include/
    Physics/include/
    NxCharacter/include/
    PhysXLoader/include/
    Cooking/include/
    lib/Win32/          ← .lib files (and often .dll siblings or under bin — see NVIDIA docs)
```

`Code/Test/Test.vcproj` adds:

- **Includes:** `$(PHYSX281_SDK)\SDKs\...\include` for Foundation, Physics, NxCharacter, PhysXLoader, Cooking.
- **Libs:** `$(PHYSX281_SDK)\SDKs\lib\Win32` (links `PhysXLoader.lib`, `NxCooking.lib`, `NxCharacter.lib`, etc.).

## Obtaining the SDK

NVIDIA no longer distributes **2.8.1** through current public portals the way they used to. Practical options:

- Use an **old installer** or backup from your original dev machine.
- Search **NVIDIA developer** / legacy SDK archives (account may be required).
- Do **not** commit the full SDK to git (size + license).

Once installed, set **`PHYSX281_SDK`** to that root and restart Visual Studio (see `BUILDING.md`).

## Runtime DLLs

See **`docs/RUNTIME_DLLS.md`** for what to copy next to the built `.exe`.
