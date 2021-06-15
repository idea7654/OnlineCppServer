#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
public:
	static void&* operator new(std::size_t alloclLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) == sizeof(UCHAR*));
	}
};