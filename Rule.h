/*
**
** Rule.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"

class OpRule : public RightInfixOperator
{
public:
	virtual string Name() { return "Rule"; }
	virtual string OperatorSymbol() { return "->"; }
	virtual OperatorPrecedence Precedence() { return 120; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpRuleDelayed : public RightInfixOperator
{
public:
	virtual string Name() { return "RuleDelayed"; }
	virtual string OperatorSymbol() { return ":>"; }
	virtual OperatorPrecedence Precedence() { return 120; }
	//virtual OperatorGrouping Grouping() { return ogRight; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldRest, atProtected, atSequenceHold);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpReplace : public PredefinedFunc
{
public:
	virtual string Name() { return "Replace"; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpReplaceAll : public LeftInfixOperator
{
public:
	virtual string Name() { return "ReplaceAll"; }
	virtual string OperatorSymbol() { return "/."; }
	virtual OperatorPrecedence Precedence() { return 110; }
	//virtual OperatorGrouping Grouping() { return ogLeft; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class OpReplaceRepeated : public RightInfixOperator
{
public:
	virtual string Name() { return "ReplaceRepeated"; }
	virtual string OperatorSymbol() { return "//."; }
	virtual OperatorPrecedence Precedence() { return 110; }
	//virtual OperatorGrouping Grouping() { return ogLeft; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
