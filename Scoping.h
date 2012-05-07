#pragma once
#include "PredefinedFunc.h"

class Module : public PredefinedFunc
{
private:
	// Static counter, incremented everytime a module is evaluated
	// (thus unique for each call).
	static int32 moduleNumber;
public:
	virtual string Name()
	{
		return "Module";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
