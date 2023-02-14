
#pragma once

#include "PhWidget.h"
#include <vsh/paf.h>

namespace paf
{
	class View
	{
	public:
		static View* Find(const std::string& pluginName);
		PhWidget* FindWidget(const std::string& widgetName);

		const wchar_t* GetString(const std::string& stringName);
		SurfacePtr GetTexture(const std::string& textureName);

		void PlaySound(const std::string& soundName);
		void SetInterface(uint32_t interfaceId, void* interfaceStruct);

		template<typename R>
		R GetInterface(uint32_t interfaceId)
		{
			return (R)paf_23AFB290((uint32_t)this, interfaceId);
		}
	};
}