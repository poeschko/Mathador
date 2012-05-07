/*
**
** Operator.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Part.h"
#include "PredefinedFunc.h"
#include "Input.h"
#include <iostream>
#include <string>
#include <memory>

enum OperatorType {otFlatInfix, otLeftInfix, otRightInfix, otPrefix, otPostfix};

// Forward declaration
class Definition;

class Operator : public Part, public Predefined
{
public:
	virtual OperatorType Type() = 0;
	virtual std::string Name() = 0;
	virtual std::string OperatorSymbol() = 0;
	virtual Operator *AsOperator()
	{
		return this;
	}
};

typedef std::auto_ptr<Operator> OperatorPtr;
