
#pragma once

#include <stdint.h>

// By iMoD1998, edited by TheRouletteBoi

#define MARK_AS_EXECUTABLE __attribute__((section(".text")))

namespace Memory
{
	class Detour
	{
	public:
		Detour();
		Detour(uint32_t fnAddress, uint32_t fnCallback);
		Detour(Detour const&) = delete;
		Detour(Detour&&) = delete;
		Detour& operator=(Detour const&) = delete;
		Detour& operator=(Detour&&) = delete;
		virtual ~Detour();

		virtual void Hook(uint32_t fnAddress, uint32_t fnCallback, uint32_t tocOverride = 0);
		virtual bool UnHook();

		template <typename R, typename... TArgs>
		R CallOriginal(TArgs... args)
		{
			R(*original)(TArgs...) = (R(*)(TArgs...))m_TrampolineOpd;
			return original(args...);
		}

	private:
		size_t Jump(void* destination, const void* branchTarget, bool linked, bool preserveRegister);
		size_t JumpWithOptions(void* destination, const void* branchTarget, bool linked, bool preserveRegister,
			uint32_t branchOptions, uint8_t conditionRegisterBit, uint8_t registerIndex);

		size_t RelocateBranch(uint32_t* destination, uint32_t* source);
		size_t RelocateCode(uint32_t* destination, uint32_t* source);
		size_t GetHookSize(const void* branchTarget, bool linked, bool preserveRegister);

	protected:
		const void* m_HookTarget;
		void* m_HookAddress;
		uint8_t* m_TrampolineAddress;
		uint32_t m_TrampolineOpd[2];
		uint8_t m_OriginalInstructions[30];
		size_t m_OriginalLength;

		// Shared
		MARK_AS_EXECUTABLE static uint8_t   s_TrampolineBuffer[2048];
		static size_t                       s_TrampolineSize;
	};
}