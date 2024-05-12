# drmario64_recomp

## Build

- Clone with submodules (`git clone --recurse-submodules` or
  `git submodule update --init --recursive`).
- Build the decomp repo (<https://github.com/AngheloAlf/drmario64>) and grab
  the elf from `build/us/drmario64.us.elf`, put it on the root of this project.
- Build the N64Recomp repo (<https://github.com/Mr-Wiseguy/N64Recomp>, commit
  `TODO`).
- Run `path/to/N64Recomp/build/N64Recomp drmario64.us.toml` on the root of this
  project.
- Idk, I haven't gotten this far yet.
  - Maybe: `cmake -B build/` and then `cmake --build build`.

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
