/*
**
** BinaryOperator.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "BinaryOperator.h"
#include "Expression.h"
#include "Exceptions.h"
#include <iostream>

using std::cout;

void BinaryOperator::PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode)
{
	string symbol = OperatorSymbol();
	if(symbol != "" && expression->LeafCount() >= 2)
	{
		ExprVector &leaves = expression->Leaves();
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		{
			if(leaf != leaves.begin())
				cout << " " << symbol << " ";
			Operator *op = (*leaf)->PredefOperator(calculator);
			bool inBrackets = op && (op->Precedence() <= Precedence());
			if(inBrackets)
				cout << "(";
			(*leaf)->Print(calculator, mode);
			if(inBrackets)
				cout << ")";
		}
	}
}

Expression *BinaryOperator::Group(PartsIterator part)
{
	if(part.IsFirst())
		throw ParseException("No operand on left side of infix operator.");
	if(part.IsLast())
		throw ParseException("No operand on right side of infix operator.");
	PartsIterator prevItr = part.Prev();
	PartsIterator nextItr = part.Next();
	Expression *prev = (*prevItr)->Group(prevItr);
	Expression *next = (*nextItr)->Group(nextItr);

	// "Clue" neighbors
	ExprPtr op(new Expression(Name(), 2));
	op->AppendLeaf(prev);
	op->AppendLeaf(next);
	prevItr.Erase();
	nextItr.Erase();
	delete this;
	*part = op.get();

	return op.release();
}
