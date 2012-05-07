/*
**
** Atom.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Expression.h"
#include "Definitions.h"

class Atom : public Expression
{
public:
	// Constructor, destructor
	Atom();
	virtual ~Atom();

	// Abstract methods
	virtual string AtomName() = 0;
	virtual ExprPtr Clone() = 0;
	virtual void Print(Calculator *calculator, PrintMode mode) = 0;
	virtual bool SameExpression(Expression *expression) = 0;

	// Implemented methods
	virtual bool CompletelyParsed() const
	{
		return true;
	}
	virtual OperatorPrecedence Precedence()
	{
		return 0;
	}
	virtual void Evaluate(Calculator *calculator, int32 recursions)
	{
	}
	virtual bool SamePattern(Expression *expression, StringMap &aliases)
	{
		return SameExpression(expression);
	}
	virtual bool Matches(Expression *pattern, DefsVector &possibleNewDefs,
		Calculator *calculator, Definitions *definitions = 0)
	{
		if(SameExpression(pattern))
		{
			possibleNewDefs.push_back(new Definitions);
			return true;
		}
		else
			return false;
	}
	virtual void Substitute(Definitions *definitions)
	{
	}
};
