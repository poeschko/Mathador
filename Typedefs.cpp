/*
**
** Typedefs.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Typedefs.h"

int64 abs(int64 x)
{
	return x >= 0 ? x : -x;
}

int32 max(const int32 x, const int32 y)
{
	return x > y ? x : y;
}

int64 max(const int64 x, const int64 y)
{
	return x > y ? x : y;
}

uint32 max(const uint32 x, const uint32 y)
{
	return x > y ? x : y;
}

uint64 max(const uint64 x, const uint64 y)
{
	return x > y ? x : y;
}

/*size_t max(const size_t x, const size_t y)
{
	return x > y ? x : y;
}*/

int64 min(const int64 x, const int64 y)
{
	return x < y ? x : y;
}
