
#pragma once

#include "../paf/PhPlane.h"
#include "../paf/PhText.h"
#include "../paf/View.h"
#include <string>

namespace vsh
{
	class NavigationMessage
	{
	public:
		// NavigationMessage(paf::PhWidget* parent, const std::wstring& message, int unk1 = 4, int unk2 = 0); // sub_34D938

		void Revoke();

	public:
		struct
		{ // reversed with IDA pro, too lazy to check via debugger so i'm not sure if these are correct
			paf::PhWidget* phWidget_0x0;	// 0x00 child of page_notification that serves as a 'group'
			paf::PhPlane* phPlaneDiv_0x4;	// 0x04 shadow ?
			paf::PhPlane* phPlaneDiv_0x8;	// 0x08 background ?
			paf::PhText* phText_0xC;		// 0x0C message ?
			bool bValue_0x10 = true;		// 0x10
			char __padding_0x11[0xF];		// 0x11
			int iValue_0x20;				// 0x20
			float duration = 5000.0;		// 0x24
			int iValue_0x28;				// 0x28 (unk2 from constructor)
			void* pValue_0x2C;				// 0x2C
			float fValue_0x30;				// 0x30
			float fValue_0x34;				// 0x34
			char __padding_0x38[0x8];		// 0x38
		} m_Data;
	};

	NavigationMessage* ShowButtonNavigationMessage(const std::wstring& message);
	NavigationMessage* ShowButtonNavigationMessage(const std::string& message);

	void ShowSystemNotification(const std::wstring& message, const std::string& texture);
	void ShowSystemNotification(const std::string& message, const std::string& texture);
}