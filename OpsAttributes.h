/*
**
** OpsAttributes.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "AttributeSet.h"

class OpAttributes : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "Attributes";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atListable, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpSetAttributes : public PredefinedFunc
{
public:
	virtual string Name()
	{ 
		return "SetAttributes";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpClearAttributes : public PredefinedFunc
{
public:
	virtual string Name()
	{ 
		return "ClearAttributes";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
