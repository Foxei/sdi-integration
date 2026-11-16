#!/usr/bin/env bash
# Regenerate the platform app icons from code/res/icons/app.svg.
#
# Usage (from the repository root, needs only Docker):
#   docker run --rm -v "$PWD:/w" -w /w debian:trixie-slim packaging/generate-icons.sh
set -euo pipefail

if ! command -v rsvg-convert >/dev/null; then
  apt-get update -qq && apt-get install -y -qq --no-install-recommends librsvg2-bin python3-pil >/dev/null
fi

SVG=code/res/icons/app.svg
OUT=code/res/app
TMP="$(mktemp -d)"
mkdir -p "${OUT}"

for size in 16 24 32 48 64 128 256 512 1024; do
  rsvg-convert -w "${size}" -h "${size}" "${SVG}" -o "${TMP}/${size}.png"
done

cp "${TMP}/256.png" "${OUT}/IntegrationGui.png"   # Linux (AppImage / .desktop)

python3 - "${TMP}" "${OUT}" <<'PY'
import sys
from PIL import Image
tmp, out = sys.argv[1], sys.argv[2]
icon = lambda s: Image.open(f"{tmp}/{s}.png")
# Windows: multi-resolution .ico for all common DPI scales
icon(256).save(f"{out}/IntegrationGui.ico",
               sizes=[(s, s) for s in (16, 24, 32, 48, 64, 128, 256)],
               append_images=[icon(s) for s in (16, 24, 32, 48, 64, 128)])
# macOS: .icns with every @1x/@2x slot (PNG payloads), 16 up to 512@2x
import struct
entries = [(b"icp4", 16), (b"icp5", 32), (b"ic11", 32), (b"ic12", 64),
           (b"ic07", 128), (b"ic13", 256), (b"ic08", 256), (b"ic14", 512),
           (b"ic09", 512), (b"ic10", 1024)]
chunks = b""
for kind, size in entries:
    data = open(f"{tmp}/{size}.png", "rb").read()
    chunks += kind + struct.pack(">I", len(data) + 8) + data
with open(f"{out}/IntegrationGui.icns", "wb") as f:
    f.write(b"icns" + struct.pack(">I", len(chunks) + 8) + chunks)
PY

rm -rf "${TMP}"
echo "Icons written to ${OUT}"
