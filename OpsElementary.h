/*
**
** OpsElementary.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"

class Sin : public PredefinedFunc
{
public:
	virtual string Name() { return "Sin"; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Cos : public PredefinedFunc
{
public:
	virtual string Name() { return "Cos"; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
