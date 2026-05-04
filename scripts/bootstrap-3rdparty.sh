#!/usr/bin/env bash
# Fetch third-party sources that are not redistributed in this repo (macOS / Linux / Git Bash on Windows).
# Usage: from repo root — bash scripts/bootstrap-3rdparty.sh
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
TP_DIR="${REPO_ROOT}/3dParty"
DL_DIR="${TP_DIR}/.downloads"
BOOST_DIR="${TP_DIR}/boost_1_45_0"
BOOST_TAR="boost_1_45_0.tar.bz2"
# Official Boost archives (redirects OK)
BOOST_URL_PRIMARY="https://archives.boost.io/release/1.45.0/source/${BOOST_TAR}"
BOOST_URL_FALLBACK="https://downloads.sourceforge.net/project/boost/boost/1.45.0/${BOOST_TAR}"

mkdir -p "${DL_DIR}"

if [[ -f "${BOOST_DIR}/boost/version.hpp" ]]; then
  echo "bootstrap-3rdparty: Boost 1.45 already present at ${BOOST_DIR}"
else
  echo "bootstrap-3rdparty: downloading Boost 1.45.0 (headers; large archive)…"
  if ! curl -fsSL -o "${DL_DIR}/${BOOST_TAR}" "${BOOST_URL_PRIMARY}"; then
    echo "bootstrap-3rdparty: primary URL failed, trying SourceForge mirror…"
    curl -fsSL -o "${DL_DIR}/${BOOST_TAR}" "${BOOST_URL_FALLBACK}"
  fi
  echo "bootstrap-3rdparty: extracting…"
  rm -rf "${BOOST_DIR}"
  tar -xjf "${DL_DIR}/${BOOST_TAR}" -C "${TP_DIR}"
  if [[ ! -f "${BOOST_DIR}/boost/version.hpp" ]]; then
    echo "bootstrap-3rdparty: error: expected ${BOOST_DIR}/boost/version.hpp after extract" >&2
    exit 1
  fi
  echo "bootstrap-3rdparty: Boost 1.45.0 ready at ${BOOST_DIR}"
fi

echo "bootstrap-3rdparty: done."
echo ""
echo "Still manual (license / login): NVIDIA PhysX SDK 2.8.1 — see docs/PHYSX281_SDK_LAYOUT.md"
echo "When you have Windows: set PHYSX281_SDK, open Code/videojuego.sln, build. See BUILDING.md"
