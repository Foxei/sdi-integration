#!/usr/bin/env bash
# Build a universal IntegrationGui.app natively on macOS (used by CI, works
# locally too). Needs only Python 3 and the Xcode Command Line Tools; Qt,
# CMake and Ninja are installed into the work directory.
#
# Usage: packaging/build-macos-native.sh [output-dir]   (default: dist/macos)
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:-${ROOT}/dist/macos}"
WORK="${WORK_DIR:-${ROOT}/build-macos}"
QT_VERSION="$(sed -n 's/^ARG QT_VERSION=//p' "${ROOT}/Dockerfile")"

# Tools
python3 -m venv "${WORK}/venv"
"${WORK}/venv/bin/pip" install --quiet aqtinstall cmake ninja
export PATH="${WORK}/venv/bin:${PATH}"

# Qt (same version as the Docker builds)
QT="${WORK}/qt/${QT_VERSION}/macos"
if [[ ! -d "${QT}" ]]; then
  aqt install-qt mac desktop "${QT_VERSION}" clang_64 -m qtcharts -O "${WORK}/qt"
fi

# Build arm64 + x86_64 in one go
cmake -S "${ROOT}/code" -B "${WORK}/build" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH="${QT}" \
    -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=13.0
cmake --build "${WORK}/build"

# Bundle Qt into the app
mkdir -p "${OUT}"
rm -rf "${OUT}/IntegrationGui.app"
cp -R "${WORK}/build/src/app/IntegrationGui.app" "${OUT}/"
"${ROOT}/packaging/deploy-macos.sh" "${OUT}/IntegrationGui.app" "${QT}"

# Ad-hoc sign the complete bundle (no Apple Developer ID needed)
codesign --force --deep --sign - "${OUT}/IntegrationGui.app"
codesign --verify --deep --strict "${OUT}/IntegrationGui.app"
echo "Built ${OUT}/IntegrationGui.app"
