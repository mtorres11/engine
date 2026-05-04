# Fetch Boost 1.45 into 3dParty (Windows PowerShell). Run from repo root:
#   powershell -ExecutionPolicy Bypass -File scripts\bootstrap-3rdparty.ps1

$ErrorActionPreference = "Stop"
# scripts/ -> repo root
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
if (-not (Test-Path (Join-Path $RepoRoot "Code\videojuego.sln"))) {
    throw "Run this script from the engine repo (missing Code\videojuego.sln under $RepoRoot)"
}
$TpDir = Join-Path $RepoRoot "3dParty"
$DlDir = Join-Path $TpDir ".downloads"
$BoostDir = Join-Path $TpDir "boost_1_45_0"
$TarName = "boost_1_45_0.tar.bz2"
$TarPath = Join-Path $DlDir $TarName
$UrlPrimary = "https://archives.boost.io/release/1.45.0/source/$TarName"
$UrlFallback = "https://downloads.sourceforge.net/project/boost/boost/1.45.0/$TarName"

New-Item -ItemType Directory -Force -Path $DlDir | Out-Null

$VersionHpp = Join-Path $BoostDir "boost\version.hpp"
if (Test-Path -LiteralPath $VersionHpp) {
    Write-Host "bootstrap-3rdparty: Boost 1.45 already present at $BoostDir"
    exit 0
}

Write-Host "bootstrap-3rdparty: downloading Boost 1.45.0…"
try {
    Invoke-WebRequest -Uri $UrlPrimary -OutFile $TarPath -UseBasicParsing
} catch {
    Write-Host "bootstrap-3rdparty: primary URL failed, trying SourceForge…"
    Invoke-WebRequest -Uri $UrlFallback -OutFile $TarPath -UseBasicParsing
}

Write-Host "bootstrap-3rdparty: extracting…"
if (Test-Path -LiteralPath $BoostDir) { Remove-Item -Recurse -Force $BoostDir }
New-Item -ItemType Directory -Force -Path $TpDir | Out-Null
# Windows 10+ built-in tar
tar -xjf $TarPath -C $TpDir
if (-not (Test-Path -LiteralPath $VersionHpp)) {
    throw "Extract failed: missing $VersionHpp"
}
Write-Host "bootstrap-3rdparty: Boost ready at $BoostDir"
Write-Host "bootstrap-3rdparty: done. PhysX 2.8.1 is still manual — see docs/PHYSX281_SDK_LAYOUT.md"
