/*
**
** OpsExpressionStructure.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsExpressionStructure.h"
#include "Exceptions.h"
#include "Atom.h"

void OpHead::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Head expects 1 argument.");
	Expression *leaf = expression->Leaf(0);
	if(leaf->IsAtom())
	{
		Atom *atom = leaf->AtomHead();
		expression->MoveNotCloned(ExprPtr(new Expression(atom->AtomName())));
	}
	else
	{
		ExprPtr head(leaf->Head()->Clone());
		expression->MoveNotCloned(head);
	}
}

void FullForm::PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("FullForm expects 1 argument.");
	expression->Leaf(0)->Print(calculator, pmFullForm);
}
