# Integration GUI

A Qt 6 desktop application for numerical integration. It has a consistent dark interface with the system font on macOS, Windows and Linux. Pick one of three functions and one of three integration methods (lower Darboux sum, upper Darboux sum, trapezoidal rule), set the integration bounds, the number of subintervals and the function parameters, and the result is computed and plotted.

The integration library is decoupled from the GUI: function parameters are passed as strings, so any function implemented and registered in the library can be integrated without touching the interface.

## Background

This project was written during the software development internship (Softwareentwicklungspraktikum) of the Bachelor in Computational Engineering Science at RWTH Aachen University in summer semester 2017. Students work in small groups on a software project for one of the university's institutes, learning the Qt framework, teamwork, maintainability and documentation. This application was the warm-up project before the main institute project.

Authors:

- Simon Schäfer
- Markus Klaus Mizael Moser
- Sören Mehnert
- Sharon Michael Rouf-Chowdhury

## Try it: build a native app with Docker

All you need is [Docker](https://docs.docker.com/get-docker/). A single command builds a self-contained application for your operating system and writes it to `dist/`:

| Your OS | Command | Result |
| ------- | ------- | ------ |
| macOS   | `docker compose up macos`   | `dist/macos/IntegrationGui.app` (universal: Apple Silicon + Intel, macOS 13+) |
| Windows | `docker compose up windows` | `dist/windows/IntegrationGui/IntegrationGui.exe` (x86_64, Windows 10+) |
| Linux   | `docker compose up linux`   | `dist/linux/IntegrationGui-<arch>.AppImage` |

`docker compose up` without a service name builds Linux and Windows.

The first build downloads the toolchains and Qt and takes a few minutes. Later builds are cached.

### macOS notes

The macOS build cross-compiles with clang/lld against the official Qt frameworks. It uses the macOS SDK from your Xcode Command Line Tools, which Docker reads from your Mac at build time. That's why the macOS build only works on a Mac. Install the tools once with:

```bash
xcode-select --install
```

To use a different SDK, set `MACOS_SDK`, for example `MACOS_SDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk docker compose up macos`.

The app is ad-hoc signed. A locally built copy opens normally. If you copy it to another Mac (for example via AirDrop or a download), Gatekeeper will block it. In that case, remove the quarantine flag:

```bash
xattr -dr com.apple.quarantine IntegrationGui.app
```

### Linux notes

The AppImage is built for the architecture of the machine running Docker and needs glibc 2.39 or newer (Ubuntu 24.04, Debian 13, Fedora 40 or later). Make it executable and run it:

```bash
chmod +x dist/linux/IntegrationGui-*.AppImage
./dist/linux/IntegrationGui-*.AppImage
```

## Development

### VS Code devcontainer

The repository includes a devcontainer with GCC, CMake, Ninja, GDB, Doxygen and Qt 6.10. In VS Code, run **Dev Containers: Reopen in Container**. CMake Tools is already configured to use `code/` as the source directory. The devcontainer is meant for editing and building. To run the GUI, build a native app with `docker compose up <os>`.

### Building natively

Requirements:

- Qt 6.8 or newer (Core, Gui, Widgets, Charts, Svg)
- CMake 3.16 or newer
- A C++17 compiler

```bash
cmake -S code -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/src/app/IntegrationGui          # Linux/Windows
open build/src/app/IntegrationGui.app   # macOS
```

If CMake can't find Qt, pass its location, for example `-DCMAKE_PREFIX_PATH=$(brew --prefix qt)` or `-DCMAKE_PREFIX_PATH=~/Qt/6.10.1/macos`.

### Icons

All icons are SVGs in `code/res/icons/`, so they stay sharp on Retina and other high-DPI screens. The platform app icons in `code/res/app/` (`.icns` for macOS, `.ico` for Windows, `.png` for Linux) are generated from `app.svg`. After changing it, regenerate them with:

```bash
docker run --rm -v "$PWD:/w" -w /w debian:trixie-slim packaging/generate-icons.sh
```

### Documentation

Generate the API documentation with Doxygen:

```bash
cd code && doxygen Doxyfile   # output in code/doxygen/
```

## Project layout

```
code/
  src/library/   integration library (functions, integration methods)
  src/app/       Qt GUI (model / view / controller)
  res/icons/     SVG icons (UI and app icon)
  res/app/       platform app icons, Info.plist, Windows resource file
packaging/       cross-compilation toolchains and deployment scripts
Dockerfile       build stages for the devcontainer and all platforms
```

## License

[GPL-3.0](LICENSE)
