
#include "Memory.h"
#include "System/SystemCalls.h"

namespace Memory
{
	static bool memoryUseMapi = false;

	uint32_t GetCurrentToc()
	{
		ElfHeader* header = (ElfHeader*)0x10000;
		opd_s* entry = (opd_s*)(uint32_t)header->e_entry;
		return entry ? entry->toc : 0;
	}

	int ReadProcessMemory(sys_pid_t pid, uint32_t address, void* data, size_t size)
	{
		if (!memoryUseMapi && Syscall::sys_dbg_get_console_type() > Syscall::ConsoleType::CEX)
		{
			uint32_t returnValue = Syscall::sys_dbg_read_process_memory(pid, address, data, size);
			if (!(returnValue & 0x80000000))
				return 0;
		}

		memoryUseMapi = true;
		return Syscall::sys_mapi_read_process_memory(pid, address, data, size);
	}

	int WriteProcessMemory(sys_pid_t pid, uint32_t address, const void* data, size_t size)
	{
		if (!memoryUseMapi && Syscall::sys_dbg_get_console_type() > Syscall::ConsoleType::CEX)
		{
			uint32_t returnValue = Syscall::sys_dbg_write_process_memory(pid, address, data, size);
			if (!(returnValue & 0x80000000))
				return 0;
		}

		memoryUseMapi = true;
		return Syscall::sys_mapi_write_process_memory(pid, address, data, size);
	}

	void nop(uint32_t address)
	{
		Write<uint32_t>(address, 0x60000000);
	}

	void blr(uint32_t address)
	{
		Write<uint32_t>(address, 0x4E800020);
	}

	void bl(uint32_t from, uint32_t to)
	{
		Write<uint32_t>(from, (0x48000001 + ((to - from) & 0x3FFFFFF)));
	}
}