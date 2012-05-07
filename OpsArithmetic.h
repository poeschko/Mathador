/*
**
** OpsArithmetic.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"
#include "UnaryOperator.h"

class Plus : public FlatInfixOperator
{
public:
	virtual string Name() { return "Plus"; }
	virtual string OperatorSymbol() { return "+"; }
	virtual OperatorPrecedence Precedence() { return 310; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atFlat, atListable, atNumericFunction, atOneIdentity, atOrderless, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Times : public FlatInfixOperator
{
public:
	virtual string Name() { return "Times"; }
	virtual string OperatorSymbol() { return "*"; }
	virtual OperatorPrecedence Precedence() { return 400; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atFlat, atListable, atNumericFunction, atOneIdentity, atOrderless, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Power : public RightInfixOperator
{
public:
	virtual string Name() { return "Power"; }
	virtual string OperatorSymbol() { return "^"; }
	virtual OperatorPrecedence Precedence() { return 590; }
	//virtual OperatorGrouping Grouping() { return ogRight; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atOneIdentity, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Minus : public PrefixOperator
{
public:
	virtual string Name() { return "Minus"; }
	virtual string OperatorSymbol() { return "-"; }
	virtual OperatorPrecedence Precedence() { return 480; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Subtract : public LeftInfixOperator
{
public:
	virtual string Name() { return "Subtract"; }
	virtual string OperatorSymbol() { return "-"; }
	virtual OperatorPrecedence Precedence() { return 310; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class Divide : public LeftInfixOperator
{
public:
	virtual string Name() { return "Divide"; }
	virtual string OperatorSymbol() { return "/"; }
	virtual OperatorPrecedence Precedence() { return 470; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class NonCommutativeMultiply : public FlatInfixOperator
{
public:
	virtual string Name() { return "NonCommutativeMultiply"; }
	virtual string OperatorSymbol() { return "**"; }
	virtual OperatorPrecedence Precedence() { return 510; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atFlat, atOneIdentity, atProtected);
	}
};
