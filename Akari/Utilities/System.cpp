
#include "System.h"
#include <vsh/allocator.h>

void* operator new(size_t size)
{
	return _sys_malloc(size);
}

void* operator new(size_t size, size_t align)
{
	return _sys_memalign(align, size);
}

void* operator new[](size_t size)
{
	return _sys_malloc(size);
}

void* operator new[](size_t size, size_t align)
{
	return _sys_memalign(align, size);
}

void operator delete(void* mem)
{
	_sys_free(mem);
}

void operator delete[](void* mem)
{
	_sys_free(mem);
}