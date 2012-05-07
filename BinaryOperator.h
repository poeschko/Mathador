/*
**
** BinaryOperator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Operator.h"
#include "Typedefs.h"

class BinaryOperator : public Operator
{
public:
	virtual void PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode);
	virtual Expression *Group(PartsIterator part);
};

class LeftInfixOperator : public BinaryOperator
{
public:
	virtual OperatorType Type() 
	{
		return otLeftInfix;
	}
};

class RightInfixOperator : public BinaryOperator
{
public:
	virtual OperatorType Type() 
	{
		return otRightInfix;
	}
	virtual bool ReverseGroup()
	{
		return true;
	}
};

class FlatInfixOperator : public BinaryOperator
{
public:
	virtual OperatorType Type() 
	{
		return otFlatInfix;
	}
};
