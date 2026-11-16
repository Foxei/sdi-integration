#!/usr/bin/env bash
# Bundle Qt frameworks and plugins into a macOS .app (a Linux-friendly
# stand-in for macdeployqt).
#
# Usage: deploy-macos.sh <IntegrationGui.app> <qt-macos-prefix>
set -euo pipefail

APP="$1"
QT="$2"
CONTENTS="${APP}/Contents"
PLUGINS=(platforms/libqcocoa.dylib styles/libqmacstyle.dylib iconengines/libqsvgicon.dylib imageformats/libqsvg.dylib)

mkdir -p "${CONTENTS}/Frameworks" "${CONTENTS}/PlugIns" "${CONTENTS}/Resources"

queue=("${CONTENTS}/MacOS/"*)
for plugin in "${PLUGINS[@]}"; do
  mkdir -p "${CONTENTS}/PlugIns/$(dirname "${plugin}")"
  cp "${QT}/plugins/${plugin}" "${CONTENTS}/PlugIns/${plugin}"
  queue+=("${CONTENTS}/PlugIns/${plugin}")
done

# Copy every Qt framework referenced (transitively) via @rpath
while [[ ${#queue[@]} -gt 0 ]]; do
  binary="${queue[0]}"
  queue=("${queue[@]:1}")
  for framework in $(llvm-otool -L "${binary}" | grep -o '@rpath/Qt[A-Za-z]*\.framework' | sed 's|@rpath/||' | sort -u); do
    target="${CONTENTS}/Frameworks/${framework}"
    [[ -e "${target}" ]] && continue
    echo "Bundling ${framework}"
    cp -a "${QT}/lib/${framework}" "${target}"
    rm -rf "${target}/Headers" "${target}/Versions/A/Headers" "${target}"/*.prl "${target}/Versions/A/"*.prl
    queue+=("${target}/Versions/A/${framework%.framework}")
  done
done

printf '[Paths]\nPlugins = PlugIns\n' > "${CONTENTS}/Resources/qt.conf"
