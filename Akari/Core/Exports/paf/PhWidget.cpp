
#include "PhWidget.h"
#include "View.h"
#include <vsh/paf.h>

namespace paf
{
	PhWidget* PhWidget::FindChild(const std::string& childName)
	{
		return (PhWidget*)paf_D557F850(this, childName.c_str(), 0);
	}

	void PhWidget::RegistChild(PhWidget* child)
	{
		paf_190D4130(this, child);
	}

	void PhWidget::RemoveChild(PhWidget* child)
	{
		paf_593FB1E5(this, child);
	}

	bool PhWidget::IsAttached()
	{
		return (m_Data.parent) ? ((m_Data.name.c_str()[0]) ? m_Data.parent->FindChild(m_Data.name) : false) : false;
	}

	void PhWidget::Attach(PhWidget* parent)
	{
		if (!parent || IsAttached())
			return;

		parent->RegistChild(this);
		m_Data.parent = parent;
	}

	void PhWidget::Detach()
	{
		if (!m_Data.parent || !IsAttached())
			return;

		m_Data.parent->RemoveChild(this);
		m_Data.parent = nullptr;
	}

	PhWidget& PhWidget::SetName(const std::string& name)
	{
		m_Data.name = name;
		return *this;
	}

	PhWidget& PhWidget::SetLayoutPos(int positionLayoutFactorX, int positionLayoutFactorY, int positionLayoutFactorZ, vec4 position)
	{
		position.w = 0.0;
		m_Data.positionLayoutFactorX = positionLayoutFactorX;
		m_Data.positionLayoutFactorY = positionLayoutFactorY;
		m_Data.positionLayoutFactorZ = positionLayoutFactorZ;
		m_Data.positionLayout = position;
		return UpdateLayoutPos();
	}

	PhWidget& PhWidget::SetLayoutSize(int sizeLayoutFactorX, int sizeLayoutFactorY, int sizeLayoutFactorZ, vec4 size)
	{
		size.w = 0.0;
		m_Data.sizeLayoutFactorX = sizeLayoutFactorX;
		m_Data.sizeLayoutFactorY = sizeLayoutFactorY;
		m_Data.sizeLayoutFactorZ = sizeLayoutFactorZ;
		m_Data.sizeLayout = size;
		return UpdateLayoutSize();
	}

	PhWidget& PhWidget::SetLayoutStyle(int r4, int r5, float f1)
	{
		if (!m_Data.sRender)
			return *this;
		// sub_3F1164 calls paf::PhSText::SetLayoutStyle(int, int, float)
		// 006D6918 PhSText vtable
		m_Data.sRender->CallVirtualMethod<void>(29, m_Data.sRender, r4, r5, f1);
		return *this;
	}

	PhWidget& PhWidget::SetPosition(vec2 position)
	{
		ViewportScale(position);
		return SetLayoutPos(6, 5, 0, vec4(position.x, position.y, 0.0, 0.0));
	}

	PhWidget& PhWidget::SetSize(vec2 size)
	{
		ViewportScale(size);
		return SetLayoutSize(6, 5, 0, vec4(size.x, size.y, 0.0, 0.0));
	}

	PhWidget& PhWidget::SetRotation(vec4 rotation, PhWidget* widget, bool useRadian)
	{
		if (!useRadian)
			rotation *= (3.1415926535897 / 180.0);

		paf_67437488(this, rotation, widget, 0);
		KillTimerCB(PhHandler::RotationHandler);
		return *this;
	}

	PhWidget& PhWidget::SetRotation(float rollAngle, bool useRadian)
	{
		return SetRotation(vec4(0.0, 0.0, rollAngle, 0.0), nullptr, useRadian);
	}

	PhWidget& PhWidget::SetColor(Color rgba)
	{
		m_Data.colorScaleRGBA = rgba.clamp(0.0, 1.0);
		KillTimerCB(PhHandler::ColorHandler);
		return *this;
	}

	PhWidget& PhWidget::SetColor32(uint32_t rgba)
	{
		Color color =
		{
			static_cast<float>((rgba & 0xFF000000) >> 24) / 255,
			static_cast<float>((rgba & 0x00FF0000) >> 16) / 255,
			static_cast<float>((rgba & 0x0000FF00) >> 8) / 255,
			static_cast<float>(rgba & 0x000000FF) / 255
		};

		return SetColor(color);
	}

	PhWidget& PhWidget::SetTexture(SurfacePtr& texture)
	{
		if (!m_Data.sRender)
			return *this;

		m_Data.sRender->CallVirtualMethod<void>(40, m_Data.sRender, texture, 0);
		return UpdatePrepare();
	}

	PhWidget& PhWidget::SetSystemTexture(const std::string& textureName)
	{
		View* system_plugin = paf::View::Find("system_plugin");
		if (!system_plugin)
			return *this;

		SurfacePtr texture = system_plugin->GetTexture(textureName);
		SetTexture(texture);
		return *this;
	}

	PhWidget& PhWidget::SetStyle(int style, bool value)
	{
		if (!m_Data.sRender)
			return *this;

		m_Data.sRender->CallVirtualMethod<void>(11, m_Data.sRender, style, value);
		return UpdatePrepare();
	}

	PhWidget& PhWidget::SetStyle(int style, int value)
	{
		if (!m_Data.sRender)
			return *this;

		m_Data.sRender->CallVirtualMethod<void>(12, m_Data.sRender, style, value);
		return UpdatePrepare();
	}

	PhWidget& PhWidget::SetStyle(int style, float value)
	{
		if (!m_Data.sRender)
			return *this;

		m_Data.sRender->CallVirtualMethod<void>(13, m_Data.sRender, style, value);
		return UpdatePrepare();
	}

	PhWidget& PhWidget::SetStyle(int style, vec4 const& value)
	{
		if (!m_Data.sRender)
			return *this;

		m_Data.sRender->CallVirtualMethod<void>(14, m_Data.sRender, style, value);
		return UpdatePrepare();
	}

	PhWidget& PhWidget::UpdateLayoutPos()
	{
		paf_BF4B155C(this);
		return *this;
	}

	PhWidget& PhWidget::UpdateLayoutSize()
	{
		paf_DF031EDD(this);
		return *this;
	}

	PhWidget& PhWidget::UpdatePrepare()
	{
		paf_384F93FC(this);
		return *this;
	}

	void PhWidget::ViewportScaleHorizontal(float& value)
	{
		vec2 vec = { value, 0 };
		ViewportScale(vec);
		value = vec.x;
	}

	void PhWidget::ViewportScaleVertical(float& value)
	{
		vec2 vec = { 0, value };
		ViewportScale(vec);
		value = vec.y;
	}

	void PhWidget::ViewportScale(vec2& vec)
	{
		float screenWidth = static_cast<float>(paf_F476E8AA());
		float screenHeight = static_cast<float>(paf_AC984A12());

		// 720 / 480 = 1.5
		// 720 / 576 = 1.25
		// 1280 / 720 = 1.777777777777777
		// 1280 / 1080 = 1.185185185185
		// 1920 / 1080 = 1.77777777777777
		float aspectRatio = screenWidth / screenHeight;

		if (aspectRatio < 1.7)
			return;

		vec.x *= (screenWidth / VIEWPORT_WIDTH);
		vec.y *= (screenHeight / VIEWPORT_HEIGHT);
	}

	void PhWidget::RevertViewportScaleHorizontal(float& value)
	{
		vec2 vec = { value, 0 };
		RevertViewportScale(vec);
		value = vec.x;
	}

	void PhWidget::RevertViewportScaleVertical(float& value)
	{
		vec2 vec = { 0, value };
		RevertViewportScale(vec);
		value = vec.y;
	}

	void PhWidget::RevertViewportScale(vec2& vec)
	{
		float screenWidth = static_cast<float>(paf_F476E8AA());
		float screenHeight = static_cast<float>(paf_AC984A12());

		// 720 / 480 = 1.5
		// 720 / 576 = 1.25
		// 1280 / 720 = 1.777777777777777
		// 1280 / 1080 = 1.185185185185
		// 1920 / 1080 = 1.77777777777777
		float aspectRatio = screenWidth / screenHeight;

		if (aspectRatio < 1.7)
			return;

		vec.x *= (VIEWPORT_WIDTH / screenWidth);
		vec.y *= (VIEWPORT_HEIGHT / screenHeight);
	}
}