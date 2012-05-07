/*
**
** OperatorFactory.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Operator.h"
#include "Factory.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class OperatorFactory : public Factory<Operator, string>
{
public:
	template <class OperatorClass> void RegisterOperator()
	{
		OperatorClass instance;
		Operator *op = instance.AsOperator();
		if(op)
		{
			string symbol = op->OperatorSymbol();
			if(symbol != "")
				RegisterType<OperatorClass>(symbol);
		}
	}
};
