/*
**
** Help.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <string>
using std::string;

class Help
{
public:
	Help();
	virtual ~Help();
	virtual void Print(string topic);
};
