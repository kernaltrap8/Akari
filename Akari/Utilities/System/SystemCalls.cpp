
#include "SystemCalls.h"
#include <sys/prx.h>
#include <vsh/stdc.h>

namespace Syscall
{
	int sys_sm_shutdown(uint16_t flags)
	{
		system_call_3(379, (uint64_t)flags, 0, 0);
		return_to_user_prog(int);
	}

	int sys_sm_control_led(uint8_t led, uint8_t action)
	{
		system_call_2(386, (uint64_t)led, (uint64_t)action);
		return_to_user_prog(int);
	}

	int sys_sm_get_system_info(platform_info_s* platform_info)
	{
		system_call_1(387, (uint64_t)platform_info);
		return_to_user_prog(int);
	}

	int sys_sm_ring_buzzer(uint16_t param)
	{
		system_call_3(392, 0x1007, 0xA, (uint64_t)param);
		return_to_user_prog(int);
	}

	int sys_sm_get_fan_policy(uint8_t id, uint8_t *st, uint8_t *mode, uint8_t *speed, uint8_t *duty)
	{
		system_call_5(409, (uint64_t)id, (uint64_t)st, (uint64_t)mode, (uint64_t)speed, (uint64_t)duty);
		return_to_user_prog(int);
	}

	void sys_storage_eject_insert(bool insert)
	{
		uint8_t atapi_cmd[56];
		int32_t dev_id;

		stdc::memset(atapi_cmd, 0, 56);

		atapi_cmd[0x00] = 0x1b;
		atapi_cmd[0x01] = 0x01;
		atapi_cmd[0x04] = insert ? 0x03 : 0x02;
		atapi_cmd[0x23] = 0x0c;

		{system_call_4(600, 0x101000000000006ULL, 0, (uint64_t)&dev_id, 0); }	// sys_storage_open
		{system_call_7(616, dev_id, 1, (uint64_t)atapi_cmd, 56, 0, 0, 0); }		// sys_storage_execute_device_command
		{system_call_1(601, dev_id); }											// sys_storage_close
	}

	int sys_game_get_temperature(int dev_id, unsigned int* temperature)
	{
		unsigned int temp;
		system_call_2(383, (uint64_t)dev_id, (uint64_t)&temp);
		*temperature = (temp >> 24);
		return_to_user_prog(int);
	}

	int sys_dbg_write_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size)
	{
		system_call_4(905, (uint64_t)pid, (uint64_t)address, (uint64_t)size, (uint64_t)data);
		return_to_user_prog(int);
	}

	int sys_dbg_read_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size)
	{
		system_call_4(904, (uint64_t)pid, (uint64_t)address, (uint64_t)size, (uint64_t)data);
		return_to_user_prog(int);
	}

	ConsoleType sys_dbg_get_console_type()
	{
		uint64_t type;
		system_call_1(985, (uint64_t)&type);
		return (ConsoleType)type;
	}

	int sys_mapi_write_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size)
	{
		system_call_6(8, 0x7777, 0x32, (uint64_t)pid, (uint64_t)address, (uint64_t)data, (uint64_t)size);
		return_to_user_prog(int);
	}

	int sys_mapi_read_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size)
	{
		system_call_6(8, 0x7777, 0x31, (uint64_t)pid, (uint64_t)address, (uint64_t)data, (uint64_t)size);
		return_to_user_prog(int);
	}

	int sys_mapi_get_version(unsigned int* version)
	{
		system_call_2(8, 0x7000, (uint64_t)version);
		return_to_user_prog(int);
	}

	int sys_mapi_get_idps(uint64_t out[2])
	{
		system_call_3(8, 0x7777, 0x81, (uint64_t)out);
		return_to_user_prog(int);
	}

	int sys_mapi_set_idps(uint64_t idps[2])
	{
		system_call_4(8, 0x7777, 0x82, idps[0], idps[1]);
		return_to_user_prog(int);
	}

	int sys_mapi_get_psid(uint64_t out[2])
	{
		system_call_3(8, 0x7777, 0x83, (uint64_t)out);
		return_to_user_prog(int);
	}

	int sys_mapi_set_psid(uint64_t psid[2])
	{
		system_call_4(8, 0x7777, 0x84, psid[0], psid[1]);
		return_to_user_prog(int);
	}

	bool sys_mapi_is_cobra_based()
	{
		unsigned int version = 0x99999999;

		if (sys_mapi_get_version(&version) & 0x80000000)
			return false;

		return (version != 0x99999999);
	}

	bool sys_mapi_is_hen()
	{
		system_call_1(8, 0x1337);
		return (p1 == 0x1337);
	}

	void finalize_module()
	{
		uint64_t meminfo[5];

		sys_prx_id_t prx = sys_prx_get_my_module_id();

		meminfo[0] = 0x28;
		meminfo[1] = 2;
		meminfo[3] = 0;

		system_call_3(482, prx, 0, (uint64_t)(uint32_t)meminfo);
	}

	void _sys_ppu_thread_exit(uint64_t exitCode)
	{
		system_call_1(41, exitCode);
	}
}