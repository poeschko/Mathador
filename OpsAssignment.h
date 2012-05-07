/*
**
** OpsAssignment.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"
#include "UnaryOperator.h"

class Set : public RightInfixOperator
{
public:
	virtual string Name() { return "Set"; }
	virtual string OperatorSymbol() { return "="; }
	virtual OperatorPrecedence Precedence() { return 40; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class SetDelayed : public RightInfixOperator
{
public:
	virtual string Name() { return "SetDelayed"; }
	virtual string OperatorSymbol() { return ":="; }
	virtual OperatorPrecedence Precedence() { return 40; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class UpSet : public RightInfixOperator
{
public:
	virtual string Name() { return "UpSet"; }
	virtual string OperatorSymbol() { return "^="; }
	virtual OperatorPrecedence Precedence() { return 40; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class UpSetDelayed : public RightInfixOperator
{
public:
	virtual string Name() { return "UpSetDelayed"; }
	virtual string OperatorSymbol() { return "^:="; }
	virtual OperatorPrecedence Precedence() { return 40; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Increment : public PostfixOperator
{
public:
	virtual string Name() { return "Increment"; }
	virtual string OperatorSymbol() { return "++"; }
	virtual OperatorPrecedence Precedence() { return 660; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected, atReadProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class PreIncrement : public PrefixOperator
{
public:
	virtual string Name() { return "PreIncrement"; }
	virtual string OperatorSymbol() { return "++"; }
	virtual OperatorPrecedence Precedence() { return 660; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected, atReadProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
