
#include "ImportsExports.h"

namespace Memory
	{
		opd_s* FindExportByName(const char* module, uint32_t fnid)
		{
			uint32_t* segment15 = *reinterpret_cast<uint32_t**>(0x1008C); // 0x1008C or 0x10094
			uint32_t exportAdressTable = segment15[0x984 / sizeof(uint32_t)];
			exportStub_s* exportStub = reinterpret_cast<exportStub_s*>(exportAdressTable);

			while (exportStub->ssize == 0x1C00)
			{
				if (!strcmp(module, exportStub->name))
				{
					for (int16_t i = 0; i < exportStub->exportsCount; i++)
					{
						if (exportStub->fnid[i] == fnid)
							return exportStub->stub[i];
					}
				}
				exportStub++;
			}

			return nullptr;
		}

		opd_s* FindImportByName(const char* module, uint32_t fnid)
		{
			uint32_t* segment15 = *reinterpret_cast<uint32_t**>(0x1008C); // 0x1008C or 0x10094
			uint32_t importAdressTable = segment15[0x984 / sizeof(uint32_t)];
			importStub_s* importStub = reinterpret_cast<importStub_s*>(importAdressTable);

			while (importStub->ssize == 0x2C00)
			{
				if (!strcmp(module, importStub->name))
				{
					for (int16_t i = 0; i < importStub->importsCount; i++)
					{
						if (importStub->fnid[i] == fnid)
							return importStub->stub[i];
					}
				}
				importStub++;
			}

			return nullptr;
		}

		ImportExportDetour::ImportExportDetour(HookType type, const std::string& libaryName, uint32_t fnid, uintptr_t fnCallback)
			: Detour(), m_LibaryName(libaryName), m_Fnid(fnid)
		{
			HookByFnid(type, libaryName, fnid, fnCallback);
		}

		ImportExportDetour::~ImportExportDetour()
		{
			UnHook();
		}

		void ImportExportDetour::Hook(uintptr_t fnAddress, uintptr_t fnCallback, uintptr_t tocOverride)
		{
			// not implemented
		}

		bool ImportExportDetour::UnHook()
		{
			// not implemented
			return false;
		}

		void ImportExportDetour::HookByFnid(HookType type, const std::string& libaryName, uint32_t fnid, uintptr_t fnCallback)
		{
			opd_s* fnOpd = nullptr;

			switch (type)
			{
			case HookType::Import:
				fnOpd = FindImportByName(libaryName.c_str(), fnid);
				break;

			case HookType::Export:
				fnOpd = FindExportByName(libaryName.c_str(), fnid);
				break;
			}

			if (fnOpd == nullptr)
				return;

			Detour::Hook(fnOpd->sub, fnCallback, fnOpd->toc);
		}
	}