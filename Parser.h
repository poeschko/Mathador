/*
**
** Parser.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Input.h"
#include "Expression.h"
#include "OperatorFactory.h"
#include "Part.h"

class Parser
{
private:
	Calculator *calculator;
	OperatorFactory factory;
	OperatorPtr CreateOperator(string id, Part *prev);
public:
	Parser();
	Parser(Calculator *theCalculator);
	virtual ~Parser();
	virtual CompoundPartPtr Parse(Input &input);
	virtual void SetCalculator(Calculator *theCalculator)
	{
		calculator = theCalculator;
	}
	template <class OperatorClass> void RegisterOperator()
	{
		factory.RegisterOperator<OperatorClass>();
	}
};
