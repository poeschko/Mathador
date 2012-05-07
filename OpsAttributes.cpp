/*
**
** OpsAttributes.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsAttributes.h"
#include "Exceptions.h"
#include "Definition.h"
#include <set>
#include <string>
using std::set;
using std::string;

void OpAttributes::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Attributes expects 1 argument.");
	Definition *def = expression->Leaf(0)->SymbolDefinition(calculator);
	if(def)
	{
		delete expression->Head();
		expression->Head(new Expression("List"));
		set<string> attributes = def->Attributes().AsStringSet();
		expression->DeleteLeaves();
		for(set<string>::const_iterator item = attributes.begin(); item != attributes.end(); ++item)
			expression->AppendLeaf(new Expression(*item));
	}
	else
		throw EvaluateException("Attributes expects a symbol as its argument.");
}

void OpSetAttributes::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("SetAttributes expects 2 arguments.");
	Definition *def = expression->Leaf(0)->SymbolDefinition(calculator);
	if(def)
	{
		Expression *attributes = expression->Leaf(1);
		if(attributes->FunctionName() == "List")
		{
			for(ExprVector::const_iterator item = attributes->Leaves().begin(); item != attributes->Leaves().end(); ++item)
				if((*item)->SymbolHead())
					def->Attributes().Include((*item)->SymbolName());
				else
					throw EvaluateException("SetAttributes expects the list to only contain symbols.");
		}
		else if(attributes->SymbolHead())
		{
			def->Attributes().Include(attributes->SymbolName());
		}
		else
			throw EvaluateException("SetAttributes expects a symbol or a list of symbols as attributes.");
		//def->Attributes().Include(
		/*delete expression->Head();
		expression->Head(new Expression("List"));
		set<string> attributes = def->Attributes().AsStringSet();
		expression->DeleteLeaves();
		for(set<string>::const_iterator item = attributes.begin(); item != attributes.end(); ++item)
			expression->AppendLeaf(new Expression(*item));*/
	}
	else
		throw EvaluateException("SetAttributes expects a symbol as its first argument.");
	// SetAttributes returns Null
	expression->MoveNotCloned(ExprPtr(new Expression("Null")));
}

void OpClearAttributes::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("ClearAttributes expects 2 arguments.");
	Definition *def = expression->Leaf(0)->SymbolDefinition(calculator);
	if(def)
	{
		Expression *attributes = expression->Leaf(1);
		if(attributes->FunctionName() == "List")
		{
			for(ExprVector::const_iterator item = attributes->Leaves().begin(); item != attributes->Leaves().end(); ++item)
				if((*item)->SymbolHead())
					def->Attributes().Exclude((*item)->SymbolName());
				else
					throw EvaluateException("ClearAttributes expects the list to only contain symbols.");
		}
		else if(attributes->SymbolHead())
		{
			def->Attributes().Exclude(attributes->SymbolName());
		}
		else
			throw EvaluateException("ClearAttributes expects a symbol or a list of symbols as attributes.");
	}
	else
		throw EvaluateException("ClearAttributes expects a symbol as its first argument.");
	// ClearAttributes returns Null
	expression->MoveNotCloned(ExprPtr(new Expression("Null")));
}
