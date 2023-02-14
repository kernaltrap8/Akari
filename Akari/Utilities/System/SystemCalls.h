
#pragma once

#include <sys/syscall.h>

namespace Syscall
{
	enum shutdown_flags_t : uint16_t
	{
		lpar_id_1 = (1 << 0),
		lpar_id_2 = (1 << 1),
		lpar_id_3 = (1 << 2),
		sys_shutdown = (1 << 8),
		sys_reboot = (1 << 9),
		sys_hard_shutdown = (1 << 12),
		sys_load_lpar = (1 << 15)
	};

	enum led_t : uint8_t
	{
		led_green = 1,
		led_red = 2
	};

	enum led_action_t : uint8_t
	{
		led_action_off,
		led_action_on,
		led_action_blink_fast,
		led_action_blink_slow
	};

	enum ring_buzzer_params_t : uint16_t
	{
		ring_buzzer_single = 0x0006,
		ring_buzzer_double = 0x0036,
		ring_buzzer_triple = 0x01B6,
		ring_buzzer_continuous = 0x0FFF
	};

	struct platform_info_s
	{
		char firmware_version[7];
		char platform_id[9];
		uint32_t build_number;
		uint32_t unknown;
	}; // size: 0x18

	enum class ConsoleType
	{
		CEX = 1,
		DEX,
		DEH
	};

	int sys_sm_shutdown(uint16_t flags);
	int sys_sm_control_led(uint8_t led, uint8_t action);
	int sys_sm_get_system_info(platform_info_s* platform_info);
	int sys_sm_ring_buzzer(uint16_t param);
	int sys_sm_get_fan_policy(uint8_t id, uint8_t* st, uint8_t* mode, uint8_t* speed, uint8_t* duty);

	void sys_storage_eject_insert(bool insert);

	int sys_game_get_temperature(int dev_id, unsigned int* temperature);

	int sys_dbg_write_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size);
	int sys_dbg_read_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size);
	ConsoleType sys_dbg_get_console_type();

	int sys_mapi_write_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size);
	int sys_mapi_read_process_memory(sys_pid_t pid, uint32_t address, const void* data, size_t size);
	int sys_mapi_get_version(unsigned int* version);
	int sys_mapi_get_idps(uint64_t out[2]);
	int sys_mapi_set_idps(uint64_t idps[2]);
	int sys_mapi_get_psid(uint64_t out[2]);
	int sys_mapi_set_psid(uint64_t psid[2]);
	bool sys_mapi_is_cobra_based();
	bool sys_mapi_is_hen();

	void finalize_module();
	void _sys_ppu_thread_exit(uint64_t exitCode);
}