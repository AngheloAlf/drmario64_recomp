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
- Idk, I haven't gotten this far yet

## Acknowledgements

- [Zelda64Recomp](https://github.com/Mr-Wiseguy/Zelda64Recomp)
  (commit 452d2af06767d82be960e027dd14b9c4f0e8bf31)
  most files were copied from here.
