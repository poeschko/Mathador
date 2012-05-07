/*
**
** EvalutionControl.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"

class Unevaluated : public PredefinedFunc
{
public:
	virtual string Name() { return "Unevaluated"; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAllComplete, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
