/*
**
** Calculator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Parser.h"
#include "History.h"
#include "Help.h"
#include "Definitions.h"

class Calculator
{
private:
	Parser parser;
	History history;
	Help help;
	Definitions defs;

	template <class Class> void Register()
	{
		defs.Predefine<Class>();
		parser.RegisterOperator<Class>();
		/*Class *instance = new Class;
		RegisterPredefined<Class, instance>();
		RegisterOperator<Class, instance>();
		delete instance*/
	}
public:
	Calculator();
	virtual ~Calculator();
	virtual void Run();
	virtual Definitions &Defs()
	{
		return defs;
	}
};
