#!/usr/bin/env bash
# Load KEY=value pairs from a .dev.vars-style file into the current shell.
# Usage: source scripts/load-dev-vars.sh
#        source scripts/load-dev-vars.sh /path/to/other.vars
set -euo pipefail

_VARS_FILE="${1:-$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)/.dev.vars}"

if [[ ! -f "$_VARS_FILE" ]]; then
  echo "load-dev-vars: missing file: $_VARS_FILE" >&2
  echo "  Copy .dev.vars.example to .dev.vars and fill in paths." >&2
  return 1 2>/dev/null || exit 1
fi

while IFS= read -r line || [[ -n "$line" ]]; do
  [[ "$line" =~ ^[[:space:]]*# ]] && continue
  [[ -z "${line//[[:space:]]/}" ]] && continue
  if [[ "$line" =~ ^[[:space:]]*([A-Za-z_][A-Za-z0-9_]*)=(.*)$ ]]; then
    export "${BASH_REMATCH[1]}=${BASH_REMATCH[2]}"
  fi
done < "$_VARS_FILE"

echo "load-dev-vars: loaded $(basename "$_VARS_FILE")"
