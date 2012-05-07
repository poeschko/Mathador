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
		Operator *op = instance.AsOperator(); //dynamic_cast<Operator*>(&instance);
		if(op)
		{
			string symbol = op->OperatorSymbol(); //OperatorClass().OperatorSymbol();
			if(symbol != "")
				RegisterType<OperatorClass>(symbol);
		}
	}
};
