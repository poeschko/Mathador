#pragma once

#include "Typedefs.h"
#include "AttributeSet.h"
#include <string>

// Forward declarations
class Calculator;
class Expression;
class Definition;
class Operator;

enum PrintMode {pmStandard, pmFullForm};

class Predefined
{
private:
	static int32 instanceCount;
public:
	Predefined();
	virtual ~Predefined();
	static int32 InstanceCount()
	{
		return instanceCount;
	}
	virtual std::string Name() = 0;
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions)
	{
	}
	virtual void PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode);
	virtual void Define(Definition *definition);
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atProtected);
	}
	/*virtual std::string OperatorSymbol()
	{
		return "";
	}*/
	virtual Operator *AsOperator()
	{
		return 0;
	}
};

class PredefConst : public Predefined
{
};
