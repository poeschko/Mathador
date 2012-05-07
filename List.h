/*
**
** List.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"

class List : public PredefinedFunc
{
public:
	List();
	virtual ~List();
	virtual string Name()
	{
		return "List";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atLocked, atProtected);
	}
	virtual void PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode);
};

class Length : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "Length";
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
