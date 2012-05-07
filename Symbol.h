/*
**
** Symbol.h
** Copyright by Jan Poeschko
**
*/

#include "Atom.h"
#include "Input.h"
#include "Definition.h"
#include "Calculator.h"
#include <string>
using std::cout;

class Symbol : public Atom
{
private:
	string name;
	Calculator *calculator;
public:
	Symbol();
	Symbol(string itsName);
	virtual ~Symbol();
	virtual string Name() const { return name; }
	virtual void Name(string theName)
	{
		name = theName;
	}
	virtual ExprPtr Clone()
	{
		return ExprPtr(new Symbol(name));
	}
	virtual string AtomName()
	{
		return "Symbol";
	}
	virtual Definition *GetDefinition(Calculator *calculator)
	{
		return calculator->Defs().GetDefinition(name);
	}
	virtual void Print(Calculator *calculator, PrintMode mode);
	virtual bool SameExpression(Expression *expression)
	{
		Symbol *exprSymbol = dynamic_cast<Symbol*>(expression);
		if(exprSymbol)
			return name == exprSymbol->name;
		else
			return false;
	}
};
