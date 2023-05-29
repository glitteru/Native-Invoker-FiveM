#include <Windows.h>
#include <iostream>
#include <vector>

#pragma once

class Helper {
	public:
		static DWORD GetPointerAddress(DWORD ptr, std::vector<DWORD> offets);
};