/*
**
** UnaryOperator.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "UnaryOperator.h"
#include "Exceptions.h"
#include "Expression.h"

UnaryOperator::UnaryOperator()
{
}

UnaryOperator::~UnaryOperator()
{
}

Expression *PrefixOperator::Group(PartsIterator part)
{
	if(part.IsLast())
		throw ParseException("No operand after prefix operator.");
	CompoundPart nextPart;
	for(PartsIterator next = part.Next(); !next.IsEnd(); (next++).Erase())
		if((*next)->Precedence() > Precedence() || (*next)->Precedence() == 0)
			nextPart.AppendSubPart(*next);
		else
			break;
	ExprPtr next(nextPart.Structure());
	ExprPtr op(new Expression(Name(), 1));
	op->AppendLeaf(next.release());
	delete this;
	*part = op.get();
	return op.release();
}

Expression *PostfixOperator::Group(PartsIterator part)
{
	if(part.IsFirst())
		throw ParseException("No operand before postfix operator.");
	CompoundPart prevPart;
	for(PartsIterator prev = part.Prev(); !prev.IsBegin(); (prev--).Erase())
		if((*prev)->Precedence() > Precedence() || (*prev)->Precedence() == 0)
			prevPart.PrependSubPart(*prev);
		else
			break;
	ExprPtr prev(prevPart.Structure());
	ExprPtr op(new Expression(Name(), 1));
	op->AppendLeaf(prev.release());
	delete this;
	*part = op.get();
	return op.release();
}
