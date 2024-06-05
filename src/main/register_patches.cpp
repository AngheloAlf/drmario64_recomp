#include "ovl_patches.hpp"
#if 0
#include "../../RecompiledPatches/patches_bin.h"
#include "../../RecompiledPatches/recomp_overlays.inl"
#endif

#include "librecomp/overlays.hpp"
#include "librecomp/game.hpp"

void zelda64::register_patches() {
#if 0
    // TODO: This is a bit awful, maybe provide only one functions that does both in librecomp?
    recomp::register_patch(mm_patches_bin, sizeof(mm_patches_bin));
    recomp::register_patch_section(section_table);
#endif
}
