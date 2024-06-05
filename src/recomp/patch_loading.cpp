#include <unordered_map>
#include <algorithm>
#include <vector>
#include "recomp.h"

#if 0
#include "../../RecompiledPatches/recomp_overlays.inl"

void load_special_overlay(const SectionTableEntry& section, int32_t ram);
#endif

void load_patch_functions() {
#if 0
	load_special_overlay(section_table[0], section_table[0].ram_addr);
#endif
}
