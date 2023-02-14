
#pragma once

#include "PhWidget.h"

namespace paf
{
	class PhPlane : public PhWidget
	{
	public:
		PhPlane(){}
		PhPlane(PhWidget* parent, void* PhAppear = nullptr);
		~PhPlane();
	};
}