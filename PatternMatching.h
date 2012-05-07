/*
**
** PatternMatching.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"
#include "BinaryOperator.h"

class Pattern : public RightInfixOperator
{
public:
	virtual string Name() { return "Pattern"; }
	virtual string OperatorSymbol() { return ":"; }
	virtual OperatorPrecedence Precedence() { return 150; }
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atHoldFirst, atProtected);
	}
};

class Blank : public PredefinedFunc
{
public:
	virtual string Name() { return "Blank"; }
};

class BlankSequence : public PredefinedFunc
{
public:
	virtual string Name() { return "BlankSequence"; }
};

class BlankNullSequence : public PredefinedFunc
{
public:
	virtual string Name() { return "BlankNullSequence"; }
};

class MatchQ : public PredefinedFunc
{
public:
	virtual string Name() { return "MatchQ"; }
	virtual OperatorPrecedence Precedence() { return 670; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
