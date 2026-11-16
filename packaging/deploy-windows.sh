#!/usr/bin/env bash
# Copy the Qt and runtime DLLs next to a cross-compiled Windows executable
# (a Linux-friendly stand-in for windeployqt).
#
# Usage: deploy-windows.sh <dir-containing-exe> <qt-windows-prefix>
set -euo pipefail

DIR="$1"
QT="$2"
SEARCH_PATHS=("${QT}/bin" /opt/llvm-mingw/x86_64-w64-mingw32/bin)
PLUGINS=(platforms/qwindows.dll styles/qmodernwindowsstyle.dll iconengines/qsvgicon.dll imageformats/qsvg.dll)

queue=("${DIR}"/*.exe)
for plugin in "${PLUGINS[@]}"; do
  mkdir -p "${DIR}/$(dirname "${plugin}")"
  cp "${QT}/plugins/${plugin}" "${DIR}/${plugin}"
  queue+=("${DIR}/${plugin}")
done

# Copy every non-system DLL referenced (transitively)
while [[ ${#queue[@]} -gt 0 ]]; do
  binary="${queue[0]}"
  queue=("${queue[@]:1}")
  for dll in $(llvm-objdump -p "${binary}" | sed -n 's/.*DLL Name: //p'); do
    [[ -e "${DIR}/${dll}" ]] && continue
    for path in "${SEARCH_PATHS[@]}"; do
      if [[ -e "${path}/${dll}" ]]; then
        echo "Bundling ${dll}"
        cp "${path}/${dll}" "${DIR}/${dll}"
        queue+=("${DIR}/${dll}")
        break
      fi
    done
  done
done
