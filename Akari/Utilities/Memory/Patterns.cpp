
#include "Patterns.h"
#include <string.h>

#define getBits(x)	((x >= '0' && x <= '9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte(x)	(getBits(x[0]) << 4 | getBits(x[1]))

namespace Memory
{
	static const int memoryMaxPatternSize = 512;
	static char memoryPatternBytes[memoryMaxPatternSize];
	static char memoryPatternMask[memoryMaxPatternSize];

	bool DataCompare(const uint8_t* pbData, const uint8_t* pbMask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++pbData, ++pbMask)
			if (*szMask == 'x' && *pbData != *pbMask)
				return false;

		return (*szMask == 0);
	}

	uint32_t FindPattern(uint32_t rangeStart, uint32_t rangeEnd, uint8_t* bytes, const char* mask)
	{
		for (uint32_t i = rangeStart; i < rangeEnd; i++)
			if (DataCompare((uint8_t*)(i), bytes, mask))
				return i;

		return 0;
	}

	uint32_t FindIDAPattern(uint32_t rangeStart, uint32_t rangeEnd, const char* pattern)
	{
		std::memset(memoryPatternBytes, 0, memoryMaxPatternSize);
		std::memset(memoryPatternMask, 0, memoryMaxPatternSize);

		char* bytes = memoryPatternBytes;
		char* mask = memoryPatternMask;

		while (*pattern)
		{
			if (bytes >= (memoryPatternBytes + memoryMaxPatternSize))
			{
				*bytes = *mask = 0;
				break;
			}
			else if (*pattern != ' ')
			{
				if (*pattern == '?')
				{
					*(mask++) = '?';
					bytes++;
					while (*(pattern + 1) == '?')
						pattern++;
				}
				else
				{
					*(bytes++) = getByte(pattern);
					*(mask++) = 'x';
					pattern++;
				}
			}
			pattern++;
		}

		return FindPattern(rangeStart, rangeEnd, (uint8_t*)memoryPatternBytes, (const char*)memoryPatternMask);
	}

	uint32_t ReadHighLow(uint32_t address, uint32_t highAdditive, uint32_t lowAdditive)
	{
		uint32_t returnAddr = (((uint16_t)(*(uint32_t*)(address + highAdditive)) << 16) | (uint16_t)(*(uint32_t*)(address + lowAdditive)));
		uint32_t returnFinal = (returnAddr & 0x8000) ? returnAddr - 0x10000 : returnAddr;
		return returnFinal;
	}

	uint32_t ResolveBranch(uint32_t branchAddress)
	{
		uint32_t instruction = *(uint32_t*)(branchAddress);
		int32_t offset = instruction & 0x3FFFFFC;

		if (offset & (1 << 25))
			offset |= ~0x03FFFFFF;

		return branchAddress + offset;
	}
}