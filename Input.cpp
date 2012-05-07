/*
**
** Input.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Input.h"
#include <iostream>
using std::cin;

Input::Input(): text("")
{
	position = text.c_str();
}

Input::~Input()
{
}

void Input::ReadInput()
{
	getline(cin, text, cin.widen('\n'));
	position = text.c_str();
}

bool Input::StartsWith(char start, string &following)
{
	if(text.length() > 0 && text.at(0) == start)
	{
		// Return substring starting at second character
		following = text.substr(1);	
		return true;
	}
	else
	{
		following = "";
		return false;
	}
}

string Input::GetIdentifier()
{
	string result("");
	while(position && (
		*position >= 'a' && *position <= 'z' ||
		*position >= 'A' && *position <= 'Z' ||
		*position >= '0' && *position <= '9'))
	{
		result += *position;
		position++;
	}
	return result;
}

string Input::GetOperator()
{
	static const int Operator_Characters_Count = 22;
	static const char Operator_Characters[Operator_Characters_Count] = {
		'+', '-', '*', '^', '/', '=', '@', '&', '|', '<', '>', '!', '?', '~', '.', ':', '%',
		';', '(', '{', '[', '#'
	};
	string result("");
	while(position)
	{
		bool isValid(false);
		for(int i=0; i<Operator_Characters_Count; i++)
			if(*position == Operator_Characters[i])
			{
				isValid = true;
				break;
			}
		if(isValid)
			result += *position;
		else
			break;
		position++;
	}
	return result;
}
