/*
**
** Typedefs.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include <memory>

// Global typedefs
typedef signed int int32;
typedef signed long long int int64;
typedef unsigned int uint32;
typedef unsigned long long int uint64;

// Some constants
const int32 Max_Evaluate_Recursions = 256;
const int32 Max_ReplaceRepeated_Iterations = 50;

template <class Target, class Source>
std::auto_ptr<Target> auto_ptr_cast(std::auto_ptr<Source> source)
{
	return std::auto_ptr<Target>(dynamic_cast<Target*>(source.release()));
}

int64 abs(int64 x);

template <typename Type>
Type max(const Type x, const Type y)
{
	return x > y ? x : y;
}

template <typename Type>
Type min(const Type x, const Type y)
{
	return x < y ? x : y;
}
