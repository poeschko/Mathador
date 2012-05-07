/*
**
** OpsTests.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsTests.h"
#include "Exceptions.h"
#include "Number.h"

void OpEqual::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Equal expects 2 arguments.");
	Expression *leaf1 = expression->Leaf(0);
	Expression *leaf2 = expression->Leaf(1);
	// First test if the leaves have the completely same structure
	if(leaf1->SameExpression(leaf2))
	{
		expression->MoveNotCloned(ExprPtr(new Expression("True")));
		return;
	}
	// Secondly, we can also compare numbers (esp. an Integer with a Real)
	Number *number1 = leaf1->NumberHead();
	Number *number2 = leaf2->NumberHead();
	if(number1 && number2)
	{
		CompareResult cmp = number1->Compare(number2);
		if(cmp == crEqual)
			expression->MoveNotCloned(ExprPtr(new Expression("True")));
		else
			expression->MoveNotCloned(ExprPtr(new Expression("False")));
	}
}

void OpUnequal::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Unequal expects 2 arguments.");
	// a != b = !(a == b)
	delete expression->Head();
	Expression *negation;
    negation = new Expression("Not");
	expression->Head(negation);
	Expression *equal = new Expression("Equal", 2);
	equal->MoveLeaves(expression);
	expression->AppendLeaf(equal);
	expression->Evaluate(calculator, recursions);
}

void OpLess::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Less expects 2 arguments.");
	Number *number1 = expression->Leaf(0)->NumberHead();
	Number *number2 = expression->Leaf(1)->NumberHead();
	if(number1 && number2)
	{
		CompareResult cmp = number1->Compare(number2);
		if(cmp == crLess)
			expression->MoveNotCloned(ExprPtr(new Expression("True")));
		else
			expression->MoveNotCloned(ExprPtr(new Expression("False")));
	}
}

void OpGreater::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Greater expects 2 arguments.");
	Number *number1 = expression->Leaf(0)->NumberHead();
	Number *number2 = expression->Leaf(1)->NumberHead();
	if(number1 && number2)
	{
		CompareResult cmp = number1->Compare(number2);
		if(cmp == crGreater)
			expression->MoveNotCloned(ExprPtr(new Expression("True")));
		else
			expression->MoveNotCloned(ExprPtr(new Expression("False")));
	}
}

void OpLessEqual::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("LessEqual expects 2 arguments.");
	// a <= b = (a < b) || (a == b)
	delete expression->Head();
	expression->Head(new Expression("Or"));
	Expression *less = new Expression("Less", 2);
	less->MoveLeaves(expression);
	Expression *equal = new Expression("Equal", 2);
	equal->AppendLeaf(less->Leaf(0)->Clone());
	equal->AppendLeaf(less->Leaf(1)->Clone());
	expression->AppendLeaf(less);
	expression->AppendLeaf(equal);
	expression->Evaluate(calculator, recursions);
}

void OpGreaterEqual::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("GreaterEqual expects 2 arguments.");
	// a >= b = (a > b) || (a == b)
	delete expression->Head();
	expression->Head(new Expression("Or"));
	Expression *less = new Expression("Greater", 2);
	less->MoveLeaves(expression);
	Expression *equal = new Expression("Equal", 2);
	equal->AppendLeaf(less->Leaf(0)->Clone());
	equal->AppendLeaf(less->Leaf(1)->Clone());
	expression->AppendLeaf(less);
	expression->AppendLeaf(equal);
	expression->Evaluate(calculator, recursions);
}
