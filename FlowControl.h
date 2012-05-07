#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"
//#include "MultiOperator.h"

class CompoundExpression : public LeftInfixOperator
{
public:
	virtual std::string Name()
	{
		return "CompoundExpression";
	}
	virtual std::string OperatorSymbol()
	{
		return ";";
	}
	virtual OperatorPrecedence Precedence()
	{
		return 10;
	}
	virtual AttributeSet Attributes()
	{
		// Different from Mathematica: Flat attribute!
		return AttributeSet(atFlat, atHoldAll, atProtected, atReadProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpIf : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "If";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldRest, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpFor : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "For";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldAll, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
