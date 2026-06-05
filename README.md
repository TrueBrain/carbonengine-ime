# IME

IME is a small Carbon/Blue extension that exposes native input method editor functionality to Python. It provides keyboard layout and composition helpers for Windows and macOS builds.

## Building

Initialize the vendored build dependencies, then configure and build with CMake:

```sh
git submodule update --init --recursive
cmake --preset x64-windows-release
cmake --build .cmake-build-x64-windows-release --config Release
```

On macOS, use `arm64-osx-release` or `x64-osx-release` instead of the Windows preset.

## 📄 License and Legal Notices

© 2026 CCP Games 

This software is provided by CCP Games and does not include or distribute any third-party libraries or frameworks. 

This software is a small Carbon/Blue extension that exposes native input method editor functionality to Python.

Trademark Notice: CCP Games is a trademark of CCP ehf. 

This project is licensed under the [MIT License](LICENSE.md). Nothing in the [MIT License](LICENSE.md) grants any rights to CCP Games' trademarks or game content.