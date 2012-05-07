/*
**
** Rule.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Rule.h"
#include "Exceptions.h"
#include "Expression.h"

void OpRule::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Rule expects 2 arguments.");
}

void OpRuleDelayed::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("RuleDelayed expects 2 arguments.");
}

void OpReplace::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Replace expects 2 arguments.");
	string leafFunction = expression->Leaf(1)->FunctionName();
	if(leafFunction != "Rule" && leafFunction != "RuleDelayed" && leafFunction != "List")
		throw ArgumentException("Replace expects its second argument to be a rule or a list of rules.");
	if(leafFunction == "List")
	{
		for(ExprVector::const_iterator item = expression->Leaf(1)->Leaves().begin(); item != expression->Leaf(1)->Leaves().end(); ++ item)
			if((*item)->FunctionName() != "Rule" && (*item)->FunctionName() != "RuleDelayed")
				throw ArgumentException("Replace expects its second argument to be a rule or a list of rules.");
		for(ExprVector::const_iterator rule = expression->Leaf(1)->Leaves().begin(); rule != expression->Leaf(1)->Leaves().end(); ++ rule)
		{
			if((*rule)->LeafCount() != 2)
				throw ArgumentException("Rule with 2 arguments expected.");
			if(expression->Leaf(0)->Replace(*rule, calculator))
				break;
		}
	}
	else
	{
		if(expression->Leaf(1)->LeafCount() != 2)
			throw ArgumentException("Rule with 2 arguments expected.");
		expression->Leaf(0)->Replace(expression->Leaf(1), calculator);
	}
	expression->AssignLeaf(0);
	expression->Evaluate(calculator, recursions);
}

void OpReplaceAll::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("ReplaceAll expects 2 arguments.");
	string leafFunction = expression->Leaf(1)->FunctionName();
	if(leafFunction != "Rule" && leafFunction != "RuleDelayed" && leafFunction != "List")
		throw ArgumentException("ReplaceAll expects its second argument to be a rule or a list of rules.");
	if(leafFunction == "List")
	{
		for(ExprVector::const_iterator item = expression->Leaf(1)->Leaves().begin(); item != expression->Leaf(1)->Leaves().end(); ++ item)
			if((*item)->FunctionName() != "Rule" && (*item)->FunctionName() != "RuleDelayed")
				throw ArgumentException("ReplaceAll expects its second argument to be a rule or a list of rules.");
		expression->Leaf(0)->ReplaceAll(expression->Leaf(1)->Leaves(), calculator);
	}
	else
	{
		ExprVector rules;
		rules.push_back(expression->Leaf(1));
		expression->Leaf(0)->ReplaceAll(rules, calculator);
	}
	expression->AssignLeaf(0);
	expression->Evaluate(calculator, recursions);
}

void OpReplaceRepeated::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("ReplaceRepeated expects 2 arguments.");
	if(expression->LeafCount() != 2)
		throw ArgumentException("ReplaceRepeated expects 2 arguments.");
	string leafFunction = expression->Leaf(1)->FunctionName();
	if(leafFunction != "Rule" && leafFunction != "RuleDelayed" && leafFunction != "List")
		throw ArgumentException("ReplaceRepeated expects its second argument to be a rule or a list of rules.");
	ExprVector rules;
	if(leafFunction == "List")
	{
		for(ExprVector::const_iterator item = expression->Leaf(1)->Leaves().begin(); item != expression->Leaf(1)->Leaves().end(); ++ item)
			if((*item)->FunctionName() != "Rule" && (*item)->FunctionName() != "RuleDelayed")
				throw ArgumentException("ReplaceRepeated expects its second argument to be a rule or a list of rules.");
		rules = expression->Leaf(1)->Leaves();
	}
	else
		rules.push_back(expression->Leaf(1));
	int32 iterations(0);
	Expression *result = expression->Leaf(0);
	while(true)
	{
		bool changed(false);
		if(!result->ReplaceAll(rules, calculator, &changed))
			break;
		if(!changed)
			break;
		++iterations;
		if(iterations >= Max_ReplaceRepeated_Iterations)
			throw LimitationException("Maximum number of iterations reached.");
	}
	expression->AssignLeaf(0);
	expression->Evaluate(calculator, recursions);
}
