
#pragma once

#include <stdint.h>

namespace paf
{
	struct Surface
	{
		std::uint8_t __padding_0x0[0x68];
	};

	template<class _Ty>
	class SurfaceRCPtr
	{
	public:
		_Ty* ptr;

		SurfaceRCPtr() : ptr(nullptr) {};
		SurfaceRCPtr(_Ty* p) : ptr(p) {};
		~SurfaceRCPtr() {};
	};

	using SurfacePtr = SurfaceRCPtr<Surface>;
}