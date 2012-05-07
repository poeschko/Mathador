/*
**
** Operator.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Part.h"
#include "PredefinedFunc.h"
//#include "Expression.h"
#include "Input.h"
//#include "AttributeSet.h"
#include <iostream>
#include <string>
#include <memory>

//using std::cout;
//using std::string;

//enum OperatorPosition {opNone, opInfix, opPrefix, opPostfix};
//enum OperatorGrouping {ogNone, ogFlat, ogLeft, ogRight};

enum OperatorType {otFlatInfix, otLeftInfix, otRightInfix, otPrefix, otPostfix};

// Forward declaration
class Definition;

class Operator : public Part, public Predefined
{
public:
	//virtual OperatorPosition Position() = 0;
	//virtual OperatorGrouping Grouping() = 0;
	virtual OperatorType Type() = 0;
	//virtual OperatorPrecedence Precedence() = 0;
	/*virtual bool ReverseGroup()
	{
		return Grouping() == ogRight;
	}*/
	virtual std::string Name() = 0;
	virtual std::string OperatorSymbol() = 0;
	//virtual void Group() = 0;
	//virtual ExpressionPtr Structure();
	virtual Operator *AsOperator()
	{
		return this;
	}
};

typedef std::auto_ptr<Operator> OperatorPtr;
