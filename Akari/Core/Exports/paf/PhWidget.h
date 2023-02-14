
#pragma once

#include "PhHandler.h"
#include "Surface.h"
#include "../../../Utilities/Math.h"
#include "../../../Utilities/Memory.h"
#include <string>

constexpr float VIEWPORT_WIDTH = 1280.0;
constexpr float VIEWPORT_HEIGHT = 720.0;

namespace paf
{
	class PhWidget : public PhHandler
	{
	public:
		enum Style
		{
			Anchor = 0x12,
			TextAlignment = 0x31,
			TextShadow = 0x38 // true by default
		};

		enum TextAlignmentStyle
		{
			TextAlignCenter,
			TextAlignLeft = (1 << 0),
			TextAlignRight = (1 << 1)
		};

		enum AnchorStyle
		{
			AnchorCenter,
			AnchorLeft = (1 << 0),
			AnchorRight = (1 << 1),
			AnchorTop = (1 << 4),
			AnchorBottom = (1 << 5)
		};

	public:
		PhWidget() {};

		PhWidget* FindChild(const std::string& childName);
		void RegistChild(PhWidget* child);
		void RemoveChild(PhWidget* child);

		bool IsAttached();
		void Attach(PhWidget* parent);
		void Detach();

		PhWidget& SetName(const std::string& name);
		PhWidget& SetLayoutPos(int positionLayoutFactorX, int positionLayoutFactorY, int positionLayoutFactorZ, vec4 position);
		PhWidget& SetLayoutSize(int sizeLayoutFactorX, int sizeLayoutFactorY, int sizeLayoutFactorZ, vec4 size);
		PhWidget& SetLayoutStyle(int r4, int r5, float f1);
		PhWidget& SetPosition(vec2 position);
		PhWidget& SetSize(vec2 size);
		PhWidget& SetRotation(vec4 rotation, PhWidget* widget, bool useRadian);
		PhWidget& SetRotation(float rollAngle, bool useRadian);
		PhWidget& SetColor(Color rgba);
		PhWidget& SetColor32(uint32_t rgba);
		PhWidget& SetTexture(SurfacePtr& texture);
		PhWidget& SetSystemTexture(const std::string& textureName);

		PhWidget& SetStyle(int style, bool value);
		PhWidget& SetStyle(int style, float value);
		PhWidget& SetStyle(int style, int value);
		PhWidget& SetStyle(int style, vec4 const& value);

		PhWidget& UpdateLayoutPos();
		PhWidget& UpdateLayoutSize();
		PhWidget& UpdatePrepare();

		template <typename R, typename... Args>
		R CallVirtualMethod(int methodIndex, Args... args)
		{
			R(*method)(Args...) = (R(*)(Args...))(this->m_Data.vtable[methodIndex]);
			return method(args...);
		}

		static void ViewportScaleHorizontal(float& value);
		static void ViewportScaleVertical(float& value);
		static void ViewportScale(vec2& vec);

		static void RevertViewportScaleHorizontal(float& value);
		static void RevertViewportScaleVertical(float& value);
		static void RevertViewportScale(vec2& vec);

	public:
		struct
		{
			Memory::opd_s** vtable;				// 0x000
			std::string name;					// 0x004
			std::uint8_t __padding_0x20[0xC0];	// 0x020
			PhWidget* parent;					// 0x0E0
			std::uint8_t __padding_0xE4[0xC];	// 0x0E4
			struct								// 0x0F0
			{
				Memory::opd_s** vtable;				// 0x000 | 0x006D9188
				std::uint8_t __padding_0x4[0x24];	// 0x004
				struct								// 0x028
				{
					Memory::opd_s** vtable;				// 0x000
					std::uint8_t __padding_0x4[0x28];	// 0x004
					std::wstring text;					// 0x02C
					std::uint8_t __padding_0x48[0x160];	// 0x048
					vec2 shadowPosition;				// 0x1A8
					vec2 scale;							// 0x1B0
					// ...
				} *PhSText; // Size: 0x200+

				template <typename R, typename... Args>
				R CallVirtualMethod(int methodIndex, Args... args)
				{
					R(*method)(Args...) = (R(*)(Args...))(this->vtable[methodIndex]);
					return method(args...);
				}
			} *sRender; // Size: ~0x40
			std::uint8_t __padding_0xF4[0x18];	// 0x0F4
			float metaAlpha;					// 0x10C
			std::uint8_t __padding_0x110[0x10];	// 0x110
			vec4 colorScaleRGBA;				// 0x120
			vec4 positionOnTimer;				// 0x130
			vec4 scaleOnTimer;					// 0x140
			vec4 rotationOnTimer;				// 0x150
			std::uint8_t __padding_0x160[0x4C];	// 0x160
			float verticalItalicAmount;			// 0x1AC
			float scaleY;						// 0x1B0
			std::uint8_t __padding_0x1B4[0xC];	// 0x1B4
			float horizontalItalicAmount;		// 0x1C0
			float scaleX;						// 0x1C4
			std::uint8_t __padding_0x1C8[0x18];	// 0x1C8
			vec4 position;						// 0x1E0
			std::uint8_t __padding_0x1F0[0x40];	// 0x1F0
			vec4 sizeOnTimer;					// 0x230
			std::uint8_t __padding_0x240[0x10];	// 0x240
			int positionLayoutFactorX;			// 0x250
			int positionLayoutFactorY;			// 0x254
			int positionLayoutFactorZ;			// 0x258
			vec4 positionLayout;				// 0x25C
			int sizeLayoutFactorX;				// 0x26C
			int sizeLayoutFactorY;				// 0x270
			int sizeLayoutFactorZ;				// 0x274
			vec4 sizeLayout;					// 0x278
			std::uint8_t __padding_0x288[0x8];	// 0x288

		} m_Data; // Size: 0x290
	}; 


}