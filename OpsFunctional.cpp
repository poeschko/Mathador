/*
**
** OpsFunctional.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsFunctional.h"
#include "Expression.h"

void OpApply::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw EvaluateException("Apply expects 2 operands.");
	// If second operand is Atom, return that Atom. Otherwise replace head of second operand
	// by first operand.
	if(!expression->Leaf(1)->IsAtom())
	{
		delete expression->Leaf(1)->Head();
		expression->Leaf(1)->Head(expression->Leaf(0));
	}
	expression->Leaves().erase(expression->Leaves().begin());
	expression->AssignLeaf(0);
	expression->Evaluate(calculator, recursions);
}

void OpMap::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw EvaluateException("Map expects 2 operands.");
	Expression *head = expression->Leaf(0);
	Expression *list = expression->Leaf(1);
	for(ExprVector::iterator leaf = list->Leaves().begin(); leaf != list->Leaves().end(); ++leaf)
	{
		ExprPtr operation(new Expression());
		operation->Head(head->Clone());
		operation->AppendLeaf(*leaf);
		*leaf = operation.release();
	}
	expression->AssignLeaf(1);
	expression->Evaluate(calculator, recursions);
}
