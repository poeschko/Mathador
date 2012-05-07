/*
**
** OpsLogical.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "UnaryOperator.h"
#include "BinaryOperator.h"

class Not : public PrefixOperator
{
public:
	virtual string Name() { return "Not"; }
	virtual string OperatorSymbol() { return "!"; }
	virtual OperatorPrecedence Precedence() { return 230; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpAnd : public FlatInfixOperator
{
public:
	virtual string Name() { return "And"; }
	virtual string OperatorSymbol() { return "&&"; }
	virtual OperatorPrecedence Precedence() { return 215; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atFlat, atHoldAll, atOneIdentity, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpOr : public FlatInfixOperator
{
public:
	virtual string Name() { return "Or"; }
	virtual string OperatorSymbol() { return "||"; }
	virtual OperatorPrecedence Precedence() { return 215; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atFlat, atHoldAll, atOneIdentity, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
