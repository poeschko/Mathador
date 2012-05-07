/*
**
** OpsExpressionStructure.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"

class OpHead : public PredefinedFunc
{
public:
	virtual std::string Name() { return "Head"; }
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};

class FullForm : public PredefinedFunc
{
public:
	virtual std::string Name() { return "FullForm"; }
	virtual void PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode);
};
