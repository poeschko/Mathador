/*
**
** OpsTests.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"

class OpEqual : public FlatInfixOperator
{
public:
	virtual string Name() { return "Equal"; }
	virtual string OperatorSymbol() { return "=="; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpUnequal : public FlatInfixOperator
{
public:
	virtual string Name() { return "Unequal"; }
	virtual string OperatorSymbol() { return "!="; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpLess : public FlatInfixOperator
{
public:
	virtual string Name() { return "Less"; }
	virtual string OperatorSymbol() { return "<"; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpGreater : public FlatInfixOperator
{
public:
	virtual string Name() { return "Greater"; }
	virtual string OperatorSymbol() { return ">"; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpLessEqual : public FlatInfixOperator
{
public:
	virtual string Name() { return "LessEqual"; }
	virtual string OperatorSymbol() { return "<="; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpGreaterEqual : public FlatInfixOperator
{
public:
	virtual string Name() { return "GreaterEqual"; }
	virtual string OperatorSymbol() { return ">="; }
	virtual OperatorPrecedence Precedence() { return 290; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
