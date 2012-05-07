#include "StdAfx.h"
#include "FlowControl.h"
#include "Exceptions.h"
#include "Expression.h"

void CompoundExpression::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	ExprPtr returnValue(0);
	for(ExprVector::const_iterator leaf = expression->Leaves().begin(); leaf != expression->Leaves().end(); ++leaf)
	{
		(*leaf)->Evaluate(calculator, recursions);
		if((*leaf)->FunctionName() == "Return")
		{
			returnValue = (*leaf)->Clone();
			break;
		}
	}
	if(!returnValue.get())
	{
		if(expression->LeafCount() > 0)
			returnValue = expression->LastLeaf()->Clone();
		else
			returnValue = ExprPtr(new Expression("Null"));
	}
	expression->MoveNotCloned(returnValue);
}

void OpIf::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2 && expression->LeafCount() != 3)
		throw ArgumentException("If expects 2 or 3 arguments.");
	Expression *test = expression->Leaf(0);
	Expression *positive = expression->Leaf(1);
	Expression *negative = expression->Leaf(2);
	if(test->SymbolName() == "True")
	{
		positive->Evaluate(calculator, recursions);
		expression->MoveNotCloned(positive->Clone());
	}
	else
	{
		if(negative)
		{
			negative->Evaluate(calculator, recursions);
			expression->MoveNotCloned(negative->Clone());
		}
		else
			expression->MoveNotCloned(ExprPtr(new Expression("Null")));
	}
}

void OpFor::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 4)
		throw ArgumentException("For expects 4 arguments.");
	Expression *start = expression->Leaf(0);
	Expression *test = expression->Leaf(1);
	Expression *incr = expression->Leaf(2);
	Expression *body = expression->Leaf(3);
	// Start
	start->Evaluate(calculator, recursions);
	while(true)
	{
		// Test
		ExprPtr actualTest(test->Clone());
		actualTest->Evaluate(calculator, recursions);
		bool result = actualTest->SymbolName() == "True";
		if(!result)
			break;
		// Body
		ExprPtr actualBody(body->Clone());
		actualBody->Evaluate(calculator, recursions);
		// Increment
		ExprPtr actualIncr(incr->Clone());
		actualIncr->Evaluate(calculator, recursions);
	}
	// For returns Null by default
	expression->MoveNotCloned(ExprPtr(new Expression("Null")));
}
