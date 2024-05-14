# drmario64_recomp

## Dependencies

- git
- C (c17) and C++ (c++20) compilers
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

- Clone with submodules (`git clone --recurse-submodules` or
  `git submodule update --init --recursive`).
- Build the decomp repo (<https://github.com/AngheloAlf/drmario64>)
  - Once it is built successfully, grab `build/us/drmario64.us.elf` and
    `build/us/drmario64_uncompressed.us.z64` and put them in the root of this
    project.
- Build the N64Recomp repo (<https://github.com/Mr-Wiseguy/N64Recomp>, commit
  `TODO`).
- Run `path/to/N64Recomp/build/N64Recomp drmario64.us.toml` on the root of this
  project.
- Run `path/to/N64Recomp/build/RSPRecomp aspMain.us.toml` on the root of this
  project.
- Open `RecompiledFuncs/Idle_ThreadEntry.c` and add `pause_self(rdram);` after
  line 83.
  - This is a manual fix, sorry.
- Build this repo by running `cmake -B build/` and then
  `cmake --build build --parallel $(nproc)`.

This game expects a vanilla US ROM to run, don't use the uncompressed one from
the decomp.

## Libraries Used and Projects Referenced

- [Zelda64Recomp](https://github.com/Mr-Wiseguy/Zelda64Recomp)
  (commit 452d2af06767d82be960e027dd14b9c4f0e8bf31)
  most files were copied from here.
- [Dr. Mario 64 decompilation](https://github.com/AngheloAlf/drmario64)
- [RT64](https://github.com/rt64/rt64) for the project's rendering engine
- [moodycamel::ConcurrentQueue](https://github.com/cameron314/concurrentqueue)
  for semaphores and fast, lock-free MPMC queues
- [Gamepad Motion Helpers](https://github.com/JibbSmart/GamepadMotionHelpers)
  for sensor fusion and calibration algorithms to implement gyro aiming
