/*
**
** OpsFunctional.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "UnaryOperator.h"
#include "BinaryOperator.h"
#include "Exceptions.h"

class Function : public PostfixOperator
{
public:
	virtual string Name()
	{
		return "Function";
	}
	virtual string OperatorSymbol()
	{
		return "&";
	}
	virtual OperatorPrecedence Precedence()
	{
		return 90;
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atProtected);
	}
};

class Slot : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "Slot";
	}
};

class SlotSequence : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "SlotSequence";
	}
};

class OpApply : public RightInfixOperator
{
public:
	virtual string Name()
	{
		return "Apply";
	}
	virtual string OperatorSymbol()
	{
		return "@@";
	}
	virtual OperatorPrecedence Precedence()
	{
		return 620;
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpMap : public RightInfixOperator
{
public:
	virtual string Name()
	{
		return "Map";
	}
	virtual string OperatorSymbol()
	{
		return "/@";
	}
	virtual OperatorPrecedence Precedence()
	{
		return 620;
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
