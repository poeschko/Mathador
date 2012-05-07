/*
**
** Test.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Test.h"
#include "Integer.h"
#include <iostream>

using std::cout;
using std::endl;

Test::Test()
{
}

Test::~Test()
{
}

bool Test::Run()
{
	cout << "Tests:" << endl;
	Integer::IntVector digits;
	digits.push_back(0x341535f4);
	digits.push_back(0x53415345);
	Integer int1(false, digits);
	Integer int2(false, Integer::IntVector(1, 10));
	auto_ptr<Integer> result(int1.Divide(&int2));
	result->Print(0, pmStandard);
	cout << endl;
	cout << endl;
	return true;
}
