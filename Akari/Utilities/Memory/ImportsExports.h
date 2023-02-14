
#pragma once

#include "Detours.h"
#include <string>

namespace Memory
{
	struct opd_s
	{
		uint32_t sub, toc;
	};

	struct importStub_s
	{
		int16_t ssize;
		int16_t header1;
		int16_t header2;
		int16_t importsCount;
		int32_t zero1;
		int32_t zero2;
		const char* name;
		uint32_t* fnid;
		opd_s** stub;
		int32_t zero3;
		int32_t zero4;
		int32_t zero5;
		int32_t zero6;
	};

	struct exportStub_s
	{
		int16_t ssize;
		int16_t header1;
		int16_t header2;
		int16_t exportsCount;
		int32_t zero1;
		int32_t zero2;
		const char* name;
		uint32_t* fnid;
		opd_s** stub;
	};

	opd_s* FindExportByName(const char* module, uint32_t fnid);
	opd_s* FindImportByName(const char* module, uint32_t fnid);

	class ImportExportDetour : public Detour
	{
	public:
		enum HookType
		{
			Import,
			Export
		};

		ImportExportDetour(HookType type, const std::string& libaryName, uint32_t fnid, uint32_t fnCallback);
		virtual ~ImportExportDetour();

		virtual void Hook(uint32_t fnAddress, uint32_t fnCallback, uint32_t tocOverride = 0) override;
		virtual bool UnHook() override;

	private:
		void HookByFnid(HookType type, const std::string& libaryName, uint32_t fnid, uint32_t fnCallback);

	protected:
		std::string m_LibaryName;
		uint32_t m_Fnid;
	};
}