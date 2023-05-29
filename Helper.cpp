#include "Helper.h"

DWORD Helper::GetPointerAddress(DWORD ptr, std::vector<DWORD> offsets) {
	DWORD addr = ptr;
	for (int i = 0; i < offsets.size(); i++) {
		addr = *(DWORD*)addr;
		addr += offsets[i];
	}
	return addr;
}