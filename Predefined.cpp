#include "StdAfx.h"
#include "Predefined.h"
#include "Definition.h"
#include "Expression.h"
#include "Calculator.h"
#include <iostream>

using std::cout;

int32 Predefined::instanceCount(0);

Predefined::Predefined()
{
	++instanceCount;
}

Predefined::~Predefined()
{
	--instanceCount;
}

void Predefined::Define(Definition *definition)
{
	definition->Predef(this);
	definition->Attributes(Attributes());
}

void Predefined::PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode)
{
	cout << Name();
	cout << "[";
	ExprVector &leaves = expression->Leaves();
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		if(leaf != leaves.begin())
			cout << ", ";
		(*leaf)->Print(calculator, mode);
	}
	cout << "]";
}
