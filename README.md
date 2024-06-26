# drmario64_recomp

> [!IMPORTANT]
> This is just a repository for testing the recompilation technology.
> I won't make a release for this recompiled game. Don't ask for feature requests

## Known issues

- The pills do not get drawn correctly when falling during the game. Meaning
  that the game is mostly unplayable for most humans (the CPU bot doesn't care
  and will play anyways).
- There's no multiple controller support. So just 1 player at a time.
- The extended aspect ratio is very jank.
- Application segfaults when it is closed.
- Very untested.

## Dependencies

- git
- C (c17) and C++ (c++20) compilers.
  - Clang 15 or newer is recommended.
- SDL2
  - This has to be installed from source under Linux.
  - For example:

    ```bash
    wget https://www.libsdl.org/release/SDL2-2.26.1.tar.gz
    tar -xzf SDL2-2.26.1.tar.gz
    cd SDL2-2.26.1
    ./configure
    make -j $(nproc)
    sudo make install
    ```

- libgtk-3-dev

## Build

The listed commits are known to be able to build this repository.

Note that the following are Linux instructions. For building on Windows you'll
need Visual Studio and such. Refer to the Zelda recomp repo for Windows
instruction. You are on your own there.

- Clone with submodules (`git clone --recurse-submodules` or
  `git submodule update --init --recursive`).
- Build the decomp repo (<https://github.com/AngheloAlf/drmario64>,
  commit `91dab37987bdad4d100958685cc10a011d4917dd`)
  - Once it is built successfully, grab `build/us/drmario64.us.elf` and
    `build/us/drmario64_uncompressed.us.z64` and put them in the root of this
    project.
- Build the N64Recomp repo (<https://github.com/Mr-Wiseguy/N64Recomp>, commit
  `2a2df89349ff25a3afb3a09617deb3a166efe2f3`) and copy the `build/N64Recomp` and
  `build/RSPRecomp` binaries to the root of this project.
- Run `N64Recomp drmario64.us.toml` on the root of this
  project.
- Run `RSPRecomp aspMain.us.toml` on the root of this
  project.
- Build this repo by running:

```bash
cmake -B build/ -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Release
cmake --build build -j $(nproc) --config Release
```

Possible options for the build type are `Debug`, `Release`, `RelWithDebInfo` and
`MinSizeRel`

This game expects a vanilla US ROM to run, don't use the uncompressed one from
the decomp.

## Libraries Used and Projects Referenced

- [Zelda64Recomp](https://github.com/Mr-Wiseguy/Zelda64Recomp)
  (commit 452d2af06767d82be960e027dd14b9c4f0e8bf31)
  most files were copied from here.
- [Dr. Mario 64 decompilation](https://github.com/AngheloAlf/drmario64)
- [RT64](https://github.com/rt64/rt64) for the project's rendering engine
- [RmlUi](https://github.com/mikke89/RmlUi) for building the menus and launcher
- [lunasvg](https://github.com/sammycage/lunasvg) for SVG rendering, used by
  RmlUi
- [FreeType](https://freetype.org/) for font rendering, used by RmlUi
- [moodycamel::ConcurrentQueue](https://github.com/cameron314/concurrentqueue)
  for semaphores and fast, lock-free MPMC queues
- [Gamepad Motion Helpers](https://github.com/JibbSmart/GamepadMotionHelpers)
  for sensor fusion and calibration algorithms to implement gyro aiming
