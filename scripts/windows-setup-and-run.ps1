#Requires -Version 5.1
<#
.SYNOPSIS
  On Windows: install common prerequisites (winget), fetch Boost, optionally build/run Test.

.DESCRIPTION
  - Installs Git and Visual Studio 2022 Community with "Desktop development with C++" via winget (if available).
  - Optionally runs the DirectX End-User Runtime web installer (helps with d3dx9_43.dll at runtime).
  - Runs scripts\bootstrap-3rdparty.ps1 for Boost.
  - Does NOT install NVIDIA PhysX SDK 2.8.1 (set PHYSX281_SDK yourself — see docs\PHYSX281_SDK_LAYOUT.md).

  The solution is legacy VS2008 .vcproj. After installing VS, you usually must OPEN Code\videojuego.sln once
  and let Visual Studio RETARGET / MIGRATE projects, then build from the IDE or re-run with -Build.

.PARAMETER SkipPrereqInstall
  Skip winget installs (Git, VS 2022, DirectX web setup).

.PARAMETER LaunchVS
  Open Code\videojuego.sln in Visual Studio after prerequisites (for one-time migration).

.PARAMETER Build
  Try MSBuild on the solution (may fail until projects are migrated to .vcxproj in VS).

.PARAMETER Run
  If Test_d.exe exists, run it with working directory Run\Test.

.EXAMPLE
  # From repo root (Administrator PowerShell often not required):
  powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1

.EXAMPLE
  powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1 -LaunchVS

.EXAMPLE
  powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1 -Build -Run
#>

param(
    [switch]$SkipPrereqInstall,
    [switch]$SkipPhysXCheck,
    [switch]$SkipDirectXWebSetup,
    [switch]$LaunchVS,
    [switch]$Build,
    [switch]$Run,
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",
    [string]$StartupProject = "Test"
)

$ErrorActionPreference = "Stop"
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path

function Write-Step($msg) { Write-Host "`n=== $msg ===" -ForegroundColor Cyan }

function Test-Command($name) {
    return [bool](Get-Command $name -ErrorAction SilentlyContinue)
}

function Get-MsBuildPath {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path -LiteralPath $vswhere) {
        $installationPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath 2>$null
        if ($installationPath) {
            $candidates = @(
                (Join-Path $installationPath "MSBuild\Current\Bin\MSBuild.exe"),
                (Join-Path $installationPath "MSBuild\15.0\Bin\MSBuild.exe")
            )
            foreach ($p in $candidates) {
                if (Test-Path -LiteralPath $p) { return $p }
            }
        }
    }
    $fallback = "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
    if (Test-Path -LiteralPath $fallback) { return $fallback }
    return $null
}

function Invoke-WingetInstall {
    param([string]$Id, [string]$OverrideArgs = "")
    if (-not (Test-Command "winget")) {
        throw "winget not found. Install 'App Installer' from Microsoft Store or update Windows 10/11."
    }
    Write-Host "winget install $Id ..."
    if ($OverrideArgs) {
        winget install --id $Id -e --source winget --accept-package-agreements --accept-source-agreements --silent --override $OverrideArgs
    } else {
        winget install --id $Id -e --source winget --accept-package-agreements --accept-source-agreements --silent
    }
}

Write-Step "Repository root: $RepoRoot"

if (-not $SkipPhysXCheck) {
    $px = [Environment]::GetEnvironmentVariable("PHYSX281_SDK", "User")
    if (-not $px) { $px = [Environment]::GetEnvironmentVariable("PHYSX281_SDK", "Machine") }
    if (-not $px) { $px = $env:PHYSX281_SDK }
    if (-not $px) {
        Write-Host @"

WARNING: PHYSX281_SDK is not set. The engine will NOT compile until you:
  1) Install NVIDIA PhysX SDK 2.8.1
  2) Set user env var PHYSX281_SDK to the SDK root (folder containing SDKs\)
     Example (PowerShell as current user):
       [System.Environment]::SetEnvironmentVariable('PHYSX281_SDK', 'C:\Path\To\NVIDIA PhysX SDK\v2.8.1', 'User')
  See: docs\PHYSX281_SDK_LAYOUT.md

Continuing to install other tools...
"@ -ForegroundColor Yellow
    } else {
        Write-Host "PHYSX281_SDK = $px" -ForegroundColor Green
    }
}

if (-not $SkipPrereqInstall) {
    Write-Step "Prerequisites (winget)"
    if (-not (Test-Command "winget")) {
        Write-Host "Install winget: https://learn.microsoft.com/windows/package-manager/winget/ then re-run." -ForegroundColor Red
        exit 1
    }

    if (-not (Test-Command "git")) {
        Invoke-WingetInstall -Id "Git.Git"
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path", "Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path", "User")
    } else {
        Write-Host "Git already installed."
    }

    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $hasVs = (Test-Path -LiteralPath $vswhere) -and (& $vswhere -latest -property installationPath 2>$null)
    if (-not $hasVs) {
        Write-Host "Installing Visual Studio 2022 Community (C++ desktop workload). This can take 20–40 minutes."
        # --passive shows progress; --quiet is fully silent (longer with no UI)
        $override = '--wait --passive --norestart --add Microsoft.VisualStudio.Workload.NativeDesktop --includeRecommended'
        try {
            Invoke-WingetInstall -Id "Microsoft.VisualStudio.2022.Community" -OverrideArgs $override
        } catch {
            Write-Host "winget VS install failed (common on SKU/policy). Install manually: https://visualstudio.microsoft.com/downloads/" -ForegroundColor Yellow
            Write-Host $_ -ForegroundColor Yellow
        }
    } else {
        Write-Host "Visual Studio already detected (vswhere)."
    }

    if (-not $SkipDirectXWebSetup) {
        Write-Step "DirectX End-User Runtime (web installer)"
        $dxUrl = "https://download.microsoft.com/download/1/7/1/1718CCC4-6315-4D8E-9543-8E28A4E18C4C/dxwebsetup.exe"
        $dxExe = Join-Path $env:TEMP "dxwebsetup.exe"
        if (-not (Test-Path -LiteralPath $dxExe)) {
            Invoke-WebRequest -Uri $dxUrl -OutFile $dxExe -UseBasicParsing
        }
        Write-Host "Running DirectX web setup (silent). May still prompt if components already present."
        Start-Process -FilePath $dxExe -ArgumentList "/Q" -Wait -NoNewWindow
    }
}

Write-Step "Boost (bootstrap)"
& (Join-Path $RepoRoot "scripts\bootstrap-3rdparty.ps1")

if ($LaunchVS) {
    Write-Step "Launch Visual Studio"
    $sln = Join-Path $RepoRoot "Code\videojuego.sln"
    if (-not (Test-Path -LiteralPath $sln)) { throw "Missing $sln" }
    Start-Process $sln
    Write-Host "Complete the migration/retarget in Visual Studio, then Build solution (Debug | Win32 or x86)." -ForegroundColor Green
}

if ($Build) {
    Write-Step "MSBuild ($Configuration)"
    $msb = Get-MsBuildPath
    if (-not $msb) {
        Write-Host "MSBuild not found. Open the solution in Visual Studio once, install workloads, then re-run with -Build." -ForegroundColor Yellow
    } else {
        Write-Host "Using: $msb"
        $sln = Join-Path $RepoRoot "Code\videojuego.sln"
        # Win32 is the legacy platform name; after migration VS may use Win32 or x86
        & $msb $sln /m /p:Configuration=$Configuration /p:Platform=Win32 /restore
        if ($LASTEXITCODE -ne 0) {
            Write-Host "MSBuild failed (expected if .vcproj not migrated yet). Use -LaunchVS and build inside IDE first." -ForegroundColor Yellow
        }
    }
}

if ($Run) {
    Write-Step "Run $StartupProject"
    $exeName = if ($Configuration -eq "Debug") { "${StartupProject}_d.exe" } else { "${StartupProject}.exe" }
    $exePath = Join-Path $RepoRoot "Bin\$StartupProject\$exeName"
    $runDir = Join-Path $RepoRoot "Run\$StartupProject"
    if (-not (Test-Path -LiteralPath $exePath)) {
        Write-Host "Executable not found: $exePath — build first." -ForegroundColor Yellow
    } else {
        if (-not (Test-Path -LiteralPath $runDir)) {
            Write-Host "Working directory missing: $runDir" -ForegroundColor Yellow
        }
        Push-Location $runDir
        try {
            Write-Host "Running: $exePath (cwd: $runDir)"
            & $exePath
        } finally {
            Pop-Location
        }
    }
}

Write-Step "Done"
Write-Host @"

Next steps if you have not built yet:
  1) Set PHYSX281_SDK and restart the terminal / VS
  2) Run:  powershell -ExecutionPolicy Bypass -File .\scripts\windows-setup-and-run.ps1 -LaunchVS
  3) In Visual Studio: Retarget solution, Build (Debug | Win32), set Test working directory to Run\Test
  4) See docs\WINDOWS_QUICKSTART.md and docs\RUNTIME_DLLS.md

"@ -ForegroundColor Green
