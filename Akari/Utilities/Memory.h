
#pragma once

#include "System/SystemCalls.h"
#include "Memory/Patterns.h"
#include "Memory/Detours.h"
#include "Memory/ImportsExports.h"
#include <sys/process.h>

namespace Memory
{
	struct ElfHeader
	{
		uint8_t e_ident[16];
		uint16_t e_type;
		uint16_t e_machine;
		uint32_t e_version;
		uint64_t e_entry;
		uint64_t e_phoff;
		uint64_t e_shoff;
		uint32_t e_flags;
		uint16_t e_ehsize;
		uint16_t e_phentsize;
		uint16_t e_phnum;
		uint16_t e_shentsize;
		uint16_t e_shnum;
		uint16_t e_shstrndx;
	};

	uint32_t GetCurrentToc();

	int ReadProcessMemory(sys_pid_t pid, uint32_t address, void* data, size_t size);
	int WriteProcessMemory(sys_pid_t pid, uint32_t address, const void* data, size_t size);

	void nop(uint32_t address);
	void blr(uint32_t address);
	void bl(uint32_t from, uint32_t to);

	template<typename T>
	inline T Read(uint32_t address)
	{
		T data;
		ReadProcessMemory(sys_process_getpid(), address, &data, sizeof(T));
		return data;
	}

	template<typename T>
	inline void Write(uint32_t address, T data)
	{
		WriteProcessMemory(sys_process_getpid(), address, &data, sizeof(T));
	}

	template <typename R, typename... Args>
	inline R Call(uint32_t address, Args... args)
	{
		volatile opd_s opd = { address, GetCurrentToc() };
		if (opd.sub && opd.toc)
		{
			R(*fn)(Args...) = (R(*)(Args...))&opd;
			return fn(args...);
		}
		return (R)0;
	}
}