/*
**
** Definition.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Predefined.h"
#include "Expression.h"
#include "AttributeSet.h"
#include "Rule.h"
#include <string>
#include <map>
#include <memory>

using std::string;
using std::map;

// Forward declaration
class Calculator;
class Definition;

typedef std::auto_ptr<Definition> DefinitionPtr;

class Definition
{
private:
	//Expression *value;
	ExprPtr value;
	ExprVector downValues;
	ExprVector upValues;
	Predefined *predef;
	AttributeSet attributes;
public:
	int32 id;
	static int32 instanceCount;
	static int32 currentId;
	static map<int32, Definition*> instances;

	Definition();
	Definition(const Definition &instance);
	virtual ~Definition();
	virtual DefinitionPtr Clone() const;
	virtual void Value(Expression *theValue);
	virtual ExprVector &DownValues();
	virtual Predefined *Predef() const;
	virtual void Predef(Predefined *thePredef);
	virtual void Attributes(const AttributeSet &theAttributes);
	virtual Expression *Value() const;
	virtual AttributeSet &Attributes();
	virtual void SetDownValue(Expression *pattern, Expression *value);
	virtual void SetUpValue(Expression *pattern, Expression *value);
	static bool CompareDownValues(Expression *value1, Expression *value2);
	virtual bool ApplyDownValues(Expression *expression, Calculator *calculator, bool *changed);
	virtual bool ApplyUpValues(Expression *expression, Calculator *calculator, bool *changed);
	virtual void Print(const string &name, Calculator *calculator);
};
