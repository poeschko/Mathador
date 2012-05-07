/*
**
** NumericEvaluation.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "NumericEvaluation.h"
#include "Exceptions.h"
#include "Expression.h"

void OpN::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("N expects 1 argument.");
	expression->Leaf(0)->ApplyN(calculator);
	expression->AssignLeaf(0);
}
