
#pragma once

#include <stdint.h>

namespace Memory
{
	bool DataCompare(const uint8_t* pbData, const uint8_t* pbMask, const char* szMask);
	uint32_t FindPattern(uint32_t rangeStart, uint32_t rangeEnd, uint8_t* bytes, const char* mask);
	uint32_t FindIDAPattern(uint32_t rangeStart, uint32_t rangeEnd, const char* pattern);

	uint32_t ReadHighLow(uint32_t address, uint32_t highAdditive, uint32_t lowAdditive);
	uint32_t ResolveBranch(uint32_t branchAddress);
}