/*
**
** List.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "List.h"
#include "Exceptions.h"
#include "Expression.h"
#include <iostream>

using std::cout;

List::List()
{
}

List::~List()
{
}

void List::PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode)
{
	ExprVector &leaves = expression->Leaves();
	cout << "{";
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		if(leaf != leaves.begin())
			cout << ", ";
		(*leaf)->Print(calculator, mode);
	}
	cout << "}";
}


void Length::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Length expects 1 argument.");
	ExprPtr result(new Expression(expression->Leaf(0)->LeafCount()));
	expression->MoveNotCloned(result);
}
