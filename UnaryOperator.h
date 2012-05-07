/*
**
** UnaryOperator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Operator.h"

class UnaryOperator : public Operator
{
public:
	UnaryOperator();
	virtual ~UnaryOperator();
};

class PrefixOperator : public UnaryOperator
{
public:
	virtual OperatorType Type() 
	{
		return otPrefix;
	}
	virtual Expression *Group(PartsIterator part);
};

class PostfixOperator : public UnaryOperator
{
public:
	virtual OperatorType Type() 
	{
		return otPostfix;
	}
	virtual Expression *Group(PartsIterator part);
};
