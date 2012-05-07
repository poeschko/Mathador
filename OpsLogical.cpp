/*
**
** OpsLogical.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsLogical.h"
#include "Exceptions.h"
#include "Expression.h"

void Not::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Not expects 1 argument.");
	if(expression->Leaf(0)->SymbolName() == "True")
		expression->MoveNotCloned(ExprPtr(new Expression("False")));
	else if(expression->Leaf(0)->SymbolName() == "False")
		expression->MoveNotCloned(ExprPtr(new Expression("True")));
}

void OpAnd::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	bool allBools(true);
	bool result(true);
	for(ExprVector::iterator leaf = expression->Leaves().begin(); leaf != expression->Leaves().end(); )
	{
		(*leaf)->Evaluate(calculator, recursions);
		string symbolName = (*leaf)->SymbolName();
		// All true values can be deleted from the leaves list
		if(symbolName == "True")
		{
			expression->DeleteLeaf(leaf);
			// Need not increment iterator
		}
		else if(symbolName == "False")
		{
			result = false;
			break;
		}
		else
		{
			allBools = false;
			++leaf;
		}
	}
	if(!result)
		expression->MoveNotCloned(ExprPtr(new Expression("False")));
	else if(allBools)
		expression->MoveNotCloned(ExprPtr(new Expression("True")));
	else
		expression->ApplyOneIdentity();
}

void OpOr::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	bool allBools(true);
	bool result(false);
	for(ExprVector::iterator leaf = expression->Leaves().begin(); leaf != expression->Leaves().end(); )
	{
		(*leaf)->Evaluate(calculator, recursions);
		string symbolName = (*leaf)->SymbolName();
		// All false values can be deleted from the leaves list
		if(symbolName == "False")
		{
			expression->DeleteLeaf(leaf);
			// Need not increment iterator
		}
		else if(symbolName == "True")
		{
			result = true;
			break;
		}
		else
		{
			allBools = false;
			++leaf;
		}
	}
	if(result)
		expression->MoveNotCloned(ExprPtr(new Expression("True")));
	else if(allBools)
		expression->MoveNotCloned(ExprPtr(new Expression("False")));
	else
		expression->ApplyOneIdentity();
}
