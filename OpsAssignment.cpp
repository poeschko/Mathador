/*
**
** OpsAssignment.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsAssignment.h"
#include "Exceptions.h"
#include "Definition.h"

void Set::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Set expects 2 operands.");
	// If expression is a symbol, set the corresponding value.
	// If it is a function, set the corresponding down value.
	Expression *var = expression->Leaf(0);
	if(var->SymbolHead())
	{
		Definition *def = var->SymbolDefinition(calculator);
		if(!def)
			throw DefinitionException("Set::Apply: Cannot access definition for symbol.");
		if(def->Attributes().Contains(atProtected))
			throw EvaluateException("Symbol is protected.");
		def->Value(expression->Leaf(1));
	}
	else if(var->IsFunctionCall())
	{
		Definition *def = var->FrontFunctionDefinition(calculator);
		if(def->Attributes().Contains(atProtected))
			throw EvaluateException("Function is protected.");
		def->SetDownValue(var, expression->Leaf(1));
	}
	else
		throw EvaluateException("Cannot set an object that is not a symbol or a function.");
	// The resulting expression is the expression that the variable was set to
	expression->AssignLeaf(1);
}

void SetDelayed::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Set expects 2 operands.");
	// If expression is a symbol, set the corresponding value.
	// If it is a function, set the corresponding down value.
	Expression *var = expression->Leaf(0);
	if(var->SymbolHead())
	{
		Definition *def = var->SymbolDefinition(calculator);
		if(!def)
			throw DefinitionException("Set::Apply: Cannot access definition for symbol.");
		if(def->Attributes().Contains(atProtected))
			throw EvaluateException("Symbol is protected.");
		def->Value(expression->Leaf(1));
	}
	else if(var->IsFunctionCall())
	{
		Definition *def = var->FrontFunctionDefinition(calculator);
		if(def->Attributes().Contains(atProtected))
			throw EvaluateException("Function is protected.");
		def->SetDownValue(var, expression->Leaf(1));
	}
	else
		throw EvaluateException("Cannot set an object that is not a symbol or a function.");
	// SetDelayed returns Null
	expression->MoveNotCloned(ExprPtr(new Expression("Null")));
}

void UpSet::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("UpSet expects 2 operands.");
	// If expression is a symbol, set the corresponding value.
	// If it is a function, set the corresponding down value.
	Expression *var = expression->Leaf(0);
	if(var->IsAtom())
	{
		throw EvaluateException("UpSet expects a non-atomic expression.");
	}
	else
	{
		for(ExprVector::const_iterator leaf = var->Leaves().begin(); leaf != var->Leaves().end(); ++leaf)
		{
			Definition *def(0);
			if((*leaf)->SymbolHead())
				def = (*leaf)->SymbolDefinition(calculator);
			else if((*leaf)->IsFunctionCall())
				def = (*leaf)->FrontFunctionDefinition(calculator);
			else
				throw EvaluateException("Cannot set an object that is not a symbol or a function.");
			def->SetUpValue(var, expression->Leaf(1));
		}
	}
	// The resulting expression is the expression that the variable was set to
	expression->AssignLeaf(1);
}

void UpSetDelayed::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("UpSetDelayed expects 2 operands.");
	// If expression is a symbol, set the corresponding value.
	// If it is a function, set the corresponding down value.
	Expression *var = expression->Leaf(0);
	if(var->IsAtom())
	{
		throw EvaluateException("UpSet expects a non-atomic expression.");
	}
	else
	{
		for(ExprVector::const_iterator leaf = var->Leaves().begin(); leaf != var->Leaves().end(); ++leaf)
		{
			Definition *def(0);
			if((*leaf)->SymbolHead())
				def = (*leaf)->SymbolDefinition(calculator);
			else if((*leaf)->IsFunctionCall())
				def = (*leaf)->FrontFunctionDefinition(calculator);
			else
				throw EvaluateException("Cannot set an object that is not a symbol or a function.");
			def->SetUpValue(var, expression->Leaf(1));
		}
	}
	// UpSetDelayed returns Null
	expression->MoveNotCloned(ExprPtr(new Expression("Null")));
}

void Increment::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	// x++ corresponds to temp = x; x = x + 1 and returns temp
	if(expression->LeafCount() != 1)
		throw ArgumentException("Increment expects one argument.");
	Expression *operand = expression->Leaf(0);
	ExprPtr evaluated(operand->Clone());
	evaluated->Evaluate(calculator, recursions);
	if(evaluated->SameExpression(operand))
		throw EvaluateException("Increment expects its argument to have a value.");
	ExprPtr set(new Expression("Set", 2));
	set->MoveLeaves(expression);
	ExprPtr plus(new Expression("Plus", 2));
	plus->AppendLeaf(evaluated->Clone());
	plus->AppendLeaf(new Expression(1));
	set->AppendLeaf(plus);
	set->Evaluate(calculator, recursions);
	expression->MoveNotCloned(evaluated);
}

void PreIncrement::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	// ++x corresponds to x = x + 1 and returns x
	if(expression->LeafCount() != 1)
		throw ArgumentException("PreIncrement expects one argument.");
	Expression *operand = expression->Leaf(0);
	ExprPtr evaluated(operand->Clone());
	evaluated->Evaluate(calculator, recursions);
	if(evaluated->SameExpression(operand))
		throw EvaluateException("PreIncrement expects its argument to have a value.");
	ExprPtr set(new Expression("Set", 2));
	set->MoveLeaves(expression);
	ExprPtr plus(new Expression("Plus", 2));
	plus->AppendLeaf(evaluated);
	plus->AppendLeaf(new Expression(1));
	set->AppendLeaf(plus);
	set->Evaluate(calculator, recursions);
	expression->MoveNotCloned(set);
}
