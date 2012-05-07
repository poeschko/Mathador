/*
**
** Symbol.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Symbol.h"
#include <iostream>
using std::cout;

Symbol::Symbol(): name("")
{
}

Symbol::Symbol(string itsName): name(itsName)
{
}

Symbol::~Symbol()
{
}

void Symbol::Print(Calculator *calculator, PrintMode mode)
{
	cout << name;
}
