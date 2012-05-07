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
	Number *number = expression->Leaf(0)->NumberHead();
	if(number && number->IsNumeric())
	{
		NumberPtr result(number->Cos());
		expression->MoveNotCloned(ExprPtr(new Expression(result)));
	}
}
