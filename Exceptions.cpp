/*
**
** Exceptions.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Exceptions.h"
#include <iostream>

using std::cout;
using std::endl;

Exception::Exception(): message("")
{
}

Exception::Exception(string msg): message(msg)
{
}

Exception::~Exception() throw()
{
}

const string &Exception::Message() const
{
	return message;
}

void Exception::Print() const
{
	if(Message() == "")
		cout << Name() << "." << endl;
	else
		cout << Name() << ": " << Message() << endl;
}

void ParseException::Print() const
{
	if(Message() == "")
		cout << "Parser error. Check your syntax!" << endl;
	else
		cout << "Parser error: " << Message() << " Check your syntax!" << endl;
}
