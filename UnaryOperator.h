/*
**
** UnaryOperator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Operator.h"

//enum UnaryOperatorPosition {upUnknown, upPrefix, upPostfix};

class UnaryOperator : public Operator
{
//private:
	//UnaryOperatorPosition position;
public:
	UnaryOperator();
	virtual ~UnaryOperator();
	//virtual OperatorPosition Position() { return opPrefix; }
	//virtual OperatorGrouping Grouping() { return ogNone; }
	//virtual void StructurePostfix(Stack &stack);
	/*virtual void SetPosition(const UnaryOperatorPosition thePosition)
	{
		position = thePosition;
	}*/
	//virtual Expression *Group(PartsIterator part);
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
