/*
**
** OpsElementary.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsElementary.h"
#include "Exceptions.h"
#include "Real.h"

void Sin::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Sin expects 1 argument.");
	// Apply Sin only when argument is Real number
	/*Real *real = expression->Leaf(0)->RealHead();
	if(real)
	{
		real->Sin();
		expression->AssignLeaf(0);
	}*/
	Number *number = expression->Leaf(0)->NumberHead();
	if(number && number->IsNumeric())
	{
		NumberPtr result(number->Sin());
		expression->MoveNotCloned(ExprPtr(new Expression(result)));
	}
}

void Cos::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Cos expects 1 argument.");
	// Apply Sin only when argument is Real number
	/*Real *real = expression->Leaf(0)->RealHead();
	if(real)
	{
		real->Cos();
		expression->AssignLeaf(0);
	}*/
	Number *number = expression->Leaf(0)->NumberHead();
	if(number && number->IsNumeric())
	{
		NumberPtr result(number->Cos());
		expression->MoveNotCloned(ExprPtr(new Expression(result)));
	}
}
