#pragma once

#include "BinaryOperator.h"

class FunctionOperator : public LeftInfixOperator
{
public:
	FunctionOperator();
	virtual ~FunctionOperator();
	virtual Expression *Group(PartsIterator part);
	virtual string Name() { return "FunctionOperator"; }
	virtual string OperatorSymbol() { return ""; }
	virtual OperatorPrecedence Precedence() { return 670; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet();
	}
};
