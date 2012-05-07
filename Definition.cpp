/*
**
** Definition.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Definition.h"
#include "Exceptions.h"
#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

// Static member initialization
int32 Definition::instanceCount(0);
int32 Definition::currentId(0);
map<int32, Definition*> Definition::instances;

Definition::Definition() : value(0), predef(0)
{
	id = currentId++;
	++instanceCount;
	instances.insert(std::make_pair<int32, Definition*>(id, this));
}

Definition::Definition(const Definition &instance)
{
	if(instance.value.get())
		value = instance.value->Clone();
	downValues.reserve(instance.downValues.size());
	for(ExprVector::const_iterator down = instance.downValues.begin(); down != instance.downValues.end(); ++down)
		downValues.push_back((*down)->Clone().release());
	upValues.reserve(instance.upValues.size());
	for(ExprVector::const_iterator up = instance.upValues.begin(); up != instance.upValues.end(); ++up)
		upValues.push_back((*up)->Clone().release());
	predef = instance.predef;
	attributes = instance.attributes;
}

Definition::~Definition()
{
	for(ExprVector::const_iterator down = downValues.begin(); down != downValues.end(); ++down)
		delete (*down);
	for(ExprVector::const_iterator up = upValues.begin(); up != upValues.end(); ++up)
		delete (*up);
	delete predef;
	instances.erase(id);
	--instanceCount;
}

DefinitionPtr Definition::Clone() const
{
	return DefinitionPtr(new Definition(*this));
}

void Definition::Value(Expression *theValue)
{
	value = theValue->Clone();
}

Predefined *Definition::Predef() const
{
	return predef;
}

void Definition::Predef(Predefined *thePredef)
{
	if(predef)
		delete predef;
	predef = thePredef;
}

void Definition::Attributes(const AttributeSet &theAttributes)
{
	attributes = theAttributes;
}

Expression *Definition::Value() const
{
	return value.get();
}

AttributeSet &Definition::Attributes()
{
	return attributes;
}

void Definition::SetDownValue(Expression *pattern, Expression *value)
{
	bool inserted(false);
	Expression *rule = new Expression("Rule", 2);
	for(ExprVector::iterator down = downValues.begin(); down != downValues.end(); ++down)
	{
		if((*down)->LeafCount() != 2)
			throw ExpressionException("Definition::SetDownValue: Down value should have 2 arguments.");
		if((*down)->Leaf(0)->SamePattern(pattern))
		{
			delete *down;
			*down = rule;
			inserted = true;
			break;
		}
	}
	if(!inserted)
		downValues.push_back(rule);
	rule->AppendLeaf(pattern->Clone());
	rule->AppendLeaf(value->Clone());
	std::sort(downValues.begin(), downValues.end(), &CompareDownValues);
}

void Definition::SetUpValue(Expression *pattern, Expression *value)
{
	bool inserted(false);
	Expression *rule = new Expression("Rule", 2);
	for(ExprVector::iterator up = upValues.begin(); up != upValues.end(); ++up)
	{
		if((*up)->LeafCount() != 2)
			throw ExpressionException("Definition::SetUpValue: Up value should have 2 arguments.");
		if((*up)->Leaf(0)->SamePattern(pattern))
		{
			delete *up;
			*up = rule;
			inserted = true;
			break;
		}
	}
	if(!inserted)
		upValues.push_back(rule);
	rule->AppendLeaf(pattern->Clone());
	rule->AppendLeaf(value->Clone());
	std::sort(upValues.begin(), upValues.end(), &CompareDownValues);
}

bool Definition::CompareDownValues(Expression *value1, Expression *value2)
{
	if(value1->LeafCount() == 2 && value2->LeafCount() == 2)
		return Expression::Compare(value1->Leaf(0), value2->Leaf(0));
	else
		return true;
}

ExprVector &Definition::DownValues()
{
	return downValues;
}

bool Definition::ApplyDownValues(Expression *expression, Calculator *calculator, bool *changed)
{
	for(ExprVector::const_iterator down = downValues.begin(); down != downValues.end(); ++down)
		if(expression->Replace(*down, calculator, changed))
			return true;
	if(changed)
		*changed = false;
	return false;
}

bool Definition::ApplyUpValues(Expression *expression, Calculator *calculator, bool *changed)
{
	for(ExprVector::const_iterator up = upValues.begin(); up != upValues.end(); ++up)
		if(expression->Replace(*up, calculator, changed))
			return true;
	if(changed)
		*changed = false;
	return false;
}

void Definition::Print(const string &name, Calculator *calculator)
{
	cout << name << ": ";
	if(value.get())
		value->Print(calculator, pmStandard);
	else
		cout << "no value";
	if(predef)
		cout << " (predefined)";
	cout << endl;
	if(downValues.size() > 0)
	{
		cout << "Down Values:" << endl;
		for(ExprVector::const_iterator down = downValues.begin(); down != downValues.end(); ++down)
		{			
			cout << "  ";
			(*down)->Print(calculator, pmStandard);
			cout << endl;
		}
	}
	if(upValues.size() > 0)
	{
		cout << "Up Values:" << endl;
		for(ExprVector::const_iterator up = upValues.begin(); up != upValues.end(); ++up)
		{			
			cout << "  ";
			(*up)->Print(calculator, pmStandard);
			cout << endl;
		}
	}
}
