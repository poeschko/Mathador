/*
**
** EvaluationControl.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "EvaluationControl.h"
#include "Expression.h"

void Unevaluated::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	// Simply make the arguments the whole expression, resp. a sequence of arguments.
	// TODO: Maybe this should be changed to only having effect when inside
	// a function argument list.
	if(expression->LeafCount() == 1)
		expression->AssignLeaf(0);
	else
	{
		delete expression->Head();
		expression->Head(new Expression("Sequence"));
	}
}
