/*
**
** Input.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <string>
using std::string;

enum InputMode {imInfix, imPostfix};

class Input
{
private:
	string text;
	const char *position;
public:
	Input();
	~Input();
	virtual void ReadInput();
	virtual bool IsString(string compared)
	{
		return text == compared;
	}
	virtual bool StartsWith(char start, string &following);
	virtual string GetIdentifier();
	virtual string GetOperator();
	virtual void SkipBlanks()
	{
		if(position)
			while(*position && (*position == ' '))
				position++;
	}
	virtual char Current() const
	{
		if(position)
			return *position;
		else
			return '\0';
	}
	virtual char NextChar() const
	{
		if(position && *(position+1))
			return *(position+1);
		else
			return '\0';
	}
	virtual void Next()
	{
		position++;
	}
	virtual void Back()
	{
		position--;
	}
	virtual bool AtEnd() const
	{
		return !position || !*position;
	}
};
