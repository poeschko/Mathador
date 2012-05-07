/*
**
** NumericEvaluation.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "PredefinedFunc.h"

class OpN : public PredefinedFunc
{
public:	
	virtual string Name()
	{
		return "N";
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
