
#include "PhPlane.h"
#include <vsh/paf.h>

namespace paf
{
	PhPlane::PhPlane(PhWidget* parent, void* PhAppear)
	{
		paf_D0197A7D(this, parent, PhAppear);
	}

	PhPlane::~PhPlane()
	{
		if (IsAttached())
			paf_0C16A258(this);
	}
}