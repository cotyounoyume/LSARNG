# a little sexy apparel replacer Plugin NG ver.

This is the CommonLibSSE-NG version of the SKSE Plugin (`LSARNG.dll`) for
[a little sexy apparel replacer](https://www.nexusmods.com/skyrimspecialedition/mods/15326).

## Requirements

- Skyrim Special Edition / Anniversary Edition (tested on 1.7.104)
- SKSE64 (2.3.1 or newer for 1.7.x)
- [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

## Building

```
git clone --recursive https://github.com/cotyounoyume/LSARNG
cmake --preset release
cmake --build build/release
```

Run these from a Visual Studio 2022 Developer Shell with `VCPKG_ROOT` set.
CommonLibSSE-NG ([alandtse fork](https://github.com/alandtse/CommonLibSSE-NG)) is included as a git submodule in `lib/commonlibsse-ng`.

## License

Copyright (C) 2018-2026 cotyounoyume

The SKSE plugin source code in this repository (everything except `scripts/`)
and `LSARNG.dll` are licensed under the GNU General Public License v3.0 or
later (see [LICENSE](LICENSE)).
This follows the license of CommonLibSSE-NG, which the DLL statically links.

Third-party components and their licenses are listed in
[THIRD-PARTY-NOTICES.txt](THIRD-PARTY-NOTICES.txt).

The GPL applies only to the SKSE plugin. The other files of the mod
(`.esp`, meshes, textures, and the Papyrus scripts in `scripts/`) are separate
works and are distributed under the permissions shown on the Nexus Mods page.

### Distributing the DLL

When you distribute `LSARNG.dll`, include `LICENSE`, `THIRD-PARTY-NOTICES.txt`,
and a link to this repository (the source code of the exact version you ship).
