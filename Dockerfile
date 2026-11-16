# syntax=docker/dockerfile:1
#
# Stages:
#   dev      Linux toolchain + Qt 6 (VS Code devcontainer, base of all builds)
#   linux    -> dist/linux/IntegrationGui-<arch>.AppImage
#   windows  -> dist/windows/IntegrationGui/IntegrationGui.exe
#   macos    -> dist/macos/IntegrationGui.app (needs the `macos-sdk` build context)
#
# Use `docker compose up <linux|windows|macos>` instead of building directly.

ARG QT_VERSION=6.10.1
ARG LLVM_MINGW_VERSION=20261020
ARG LLVM_VERSION=20

# ---------------------------------------------------------------------------
# dev: toolchain + Qt 6 (official build via aqtinstall, same version everywhere)
# ---------------------------------------------------------------------------
FROM ubuntu:24.04 AS dev

ARG DEBIAN_FRONTEND=noninteractive
ARG QT_VERSION
RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        gdb \
        git \
        doxygen \
        graphviz \
        ca-certificates \
        curl \
        file \
        xz-utils \
        python3-venv \
        libgl-dev \
        libegl-dev \
        libfontconfig1 \
        libfreetype6 \
        libdbus-1-3 \
        libglib2.0-0t64 \
        libxkbcommon0 \
        libxkbcommon-x11-0 \
        libxcb-cursor0 \
        libxcb-icccm4 \
        libxcb-image0 \
        libxcb-keysyms1 \
        libxcb-randr0 \
        libxcb-render-util0 \
        libxcb-shape0 \
        libxcb-xinerama0 \
        libxcb-xkb1 \
    && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/aqt && /opt/aqt/bin/pip install --no-cache-dir aqtinstall
RUN case "$(uname -m)" in \
        x86_64)  /opt/aqt/bin/aqt install-qt linux desktop ${QT_VERSION} linux_gcc_64 -m qtcharts -O /opt/qt ;; \
        aarch64) /opt/aqt/bin/aqt install-qt linux_arm64 desktop ${QT_VERSION} linux_gcc_arm64 -m qtcharts -O /opt/qt ;; \
    esac \
    && ln -s /opt/qt/${QT_VERSION}/gcc_* /opt/qt/host
ENV CMAKE_PREFIX_PATH=/opt/qt/host \
    PATH=/opt/qt/host/bin:$PATH

# ---------------------------------------------------------------------------
# Linux: AppImage built with linuxdeploy
# ---------------------------------------------------------------------------
FROM dev AS linux-build

RUN ARCH="$(uname -m)" \
    && curl -fsSL -o /usr/local/bin/linuxdeploy \
        "https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${ARCH}.AppImage" \
    && curl -fsSL -o /usr/local/bin/linuxdeploy-plugin-qt \
        "https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-${ARCH}.AppImage" \
    && chmod +x /usr/local/bin/linuxdeploy /usr/local/bin/linuxdeploy-plugin-qt

WORKDIR /src
COPY code/ code/
COPY packaging/ packaging/
RUN cmake -S code -B /build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr \
    && cmake --build /build \
    && DESTDIR=/AppDir cmake --install /build \
    && rm -rf /AppDir/usr/include /AppDir/usr/lib /AppDir/usr/share

ENV APPIMAGE_EXTRACT_AND_RUN=1 \
    QMAKE=/opt/qt/host/bin/qmake \
    LD_LIBRARY_PATH=/opt/qt/host/lib
# linuxdeploy-plugin-qt skips the SVG icon engine (sharp icons at any scale)
RUN mkdir -p /out /AppDir/usr/plugins/iconengines \
    && cp /opt/qt/host/plugins/iconengines/libqsvgicon.so /AppDir/usr/plugins/iconengines/ \
    && cd /out \
    && linuxdeploy --appdir /AppDir \
        --desktop-file /src/packaging/IntegrationGui.desktop \
        --icon-file /src/code/res/app/IntegrationGui.png \
        --plugin qt \
        --output appimage \
    && mv /out/*.AppImage "/out/IntegrationGui-$(uname -m).AppImage"

# ---------------------------------------------------------------------------
# cross: clang/lld and llvm-mingw on top of the host Qt (moc/rcc/uic)
# ---------------------------------------------------------------------------
FROM dev AS cross

ARG DEBIAN_FRONTEND=noninteractive
ARG LLVM_VERSION
RUN apt-get update && apt-get install -y --no-install-recommends \
        clang-${LLVM_VERSION} \
        lld-${LLVM_VERSION} \
        llvm-${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*
# Cross builds find their target Qt through the toolchain files instead
ENV PATH=/usr/lib/llvm-${LLVM_VERSION}/bin:/opt/llvm-mingw/bin:$PATH \
    CMAKE_PREFIX_PATH=
WORKDIR /src
COPY code/ code/
COPY packaging/ packaging/

# ---------------------------------------------------------------------------
# Windows: cross-compiled with llvm-mingw against Qt's llvm-mingw build
# ---------------------------------------------------------------------------
FROM cross AS windows-build

ARG QT_VERSION
ARG LLVM_MINGW_VERSION
RUN curl -fsSL "https://github.com/mstorsjo/llvm-mingw/releases/download/${LLVM_MINGW_VERSION}/llvm-mingw-${LLVM_MINGW_VERSION}-ucrt-ubuntu-22.04-$(uname -m).tar.xz" \
        | tar -xJ -C /opt \
    && mv /opt/llvm-mingw-* /opt/llvm-mingw \
    && /opt/aqt/bin/aqt install-qt windows desktop ${QT_VERSION} win64_llvm_mingw -m qtcharts -O /opt/qt \
    && ln -s /opt/qt/${QT_VERSION}/llvm-mingw_64 /opt/qt/windows

RUN cmake -S code -B /build -G Ninja -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=/src/packaging/windows-toolchain.cmake \
        -DQT_TARGET_PATH=/opt/qt/windows \
        -DCMAKE_PREFIX_PATH=/opt/qt/windows \
        -DQT_HOST_PATH=/opt/qt/host \
    && cmake --build /build \
    && mkdir -p /out/IntegrationGui \
    && cp /build/src/app/IntegrationGui.exe /out/IntegrationGui/ \
    && packaging/deploy-windows.sh /out/IntegrationGui /opt/qt/windows

# ---------------------------------------------------------------------------
# macOS: cross-compiled with clang/lld against the official Qt frameworks,
# arm64 + x86_64 merged into a universal binary
# ---------------------------------------------------------------------------
FROM cross AS macos-build

ARG QT_VERSION
RUN /opt/aqt/bin/aqt install-qt mac desktop ${QT_VERSION} clang_64 -m qtcharts -O /opt/qt \
    && ln -s /opt/qt/${QT_VERSION}/macos /opt/qt/macos
COPY --from=macos-sdk . /opt/macos-sdk
# CMake asks `sw_vers` for the macOS version to enable @rpath support
RUN printf '#!/bin/sh\necho 13.0\n' > /usr/local/bin/sw_vers && chmod +x /usr/local/bin/sw_vers

RUN for arch in arm64 x86_64; do \
        cmake -S code -B /build-${arch} -G Ninja -DCMAKE_BUILD_TYPE=Release \
            -DCMAKE_TOOLCHAIN_FILE=/src/packaging/macos-toolchain.cmake \
            -DCMAKE_OSX_ARCHITECTURES=${arch} \
            -DQT_TARGET_PATH=/opt/qt/macos \
            -DCMAKE_PREFIX_PATH=/opt/qt/macos \
            -DQT_HOST_PATH=/opt/qt/host \
        && cmake --build /build-${arch} || exit 1; \
    done \
    && mkdir -p /out \
    && cp -a /build-arm64/src/app/IntegrationGui.app /out/ \
    && llvm-lipo -create \
        /build-arm64/src/app/IntegrationGui.app/Contents/MacOS/IntegrationGui \
        /build-x86_64/src/app/IntegrationGui.app/Contents/MacOS/IntegrationGui \
        -output /out/IntegrationGui.app/Contents/MacOS/IntegrationGui \
    && packaging/deploy-macos.sh /out/IntegrationGui.app /opt/qt/macos

# ---------------------------------------------------------------------------
# Export stages: copy the packaged application into the mounted /dist
# ---------------------------------------------------------------------------
FROM busybox:stable AS export
CMD ["sh", "-c", "cp -a /out/. /dist/ && chown -R \"$(stat -c %u:%g /dist)\" /dist && echo \"Done: $(ls /out)\""]

FROM export AS linux
COPY --from=linux-build /out /out

FROM export AS windows
COPY --from=windows-build /out /out

FROM export AS macos
COPY --from=macos-build /out /out
