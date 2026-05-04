# Load KEY=value pairs from .dev.vars into the current PowerShell session.
# Usage: . .\scripts\load-dev-vars.ps1
#        . .\scripts\load-dev-vars.ps1 -Path C:\path\to\custom.vars

param(
    [string] $Path = (Join-Path (Split-Path $PSScriptRoot -Parent) ".dev.vars")
)

if (-not (Test-Path -LiteralPath $Path)) {
    Write-Error "Missing file: $Path — copy .dev.vars.example to .dev.vars and fill in paths."
    return
}

Get-Content -LiteralPath $Path | ForEach-Object {
    $line = $_.Trim()
    if ($line -eq "" -or $line.StartsWith("#")) { return }
    $eq = $line.IndexOf("=")
    if ($eq -lt 1) { return }
    $key = $line.Substring(0, $eq).Trim()
    $val = $line.Substring($eq + 1).Trim()
    if ($key -match '^[A-Za-z_][A-Za-z0-9_]*$') {
        Set-Item -Path "Env:$key" -Value $val
    }
}

Write-Host "load-dev-vars: loaded $(Split-Path $Path -Leaf)"
