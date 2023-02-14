
#include "View.h"

namespace paf
{
	View* View::Find(const std::string& pluginName)
	{
		return (View*)paf_F21655F3(pluginName.c_str());
	}

	PhWidget* View::FindWidget(const std::string& widgetName)
	{
		return (PhWidget*)paf_794CEACB((int)this, widgetName.c_str());
	}

	const wchar_t* View::GetString(const std::string& stringName)
	{
		return paf_89B67B9C((int)this, stringName.c_str());
	}

	SurfacePtr View::GetTexture(const std::string& textureName)
	{
		SurfacePtr texture = SurfacePtr();
		if (textureName != "")
			paf_3A8454FC((int*)&texture, (int)this, textureName.c_str());
		return texture;
	}

	void View::PlaySound(const std::string& soundName)
	{
		paf_B93AFE7E((int)this, soundName.c_str(), 1.0, 0);
	}

	void View::SetInterface(uint32_t interfaceId, void* interfaceStruct)
	{
		paf_0A1DC401(this, interfaceId, interfaceStruct);
	}
}