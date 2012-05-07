/*
**
** Calculator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Parser.h"
//#include "Stack.h"
#include "History.h"
#include "Help.h"
#include "Definitions.h"

class Calculator
{
private:
	//InputMode mode;
	Parser parser;
	//Stack stack;
	History history;
	Help help;
	Definitions defs;
	/*template <class PredefinedClass, Predefined *predef> void RegisterPredefined()
	{
		defs.Predefine<PredefinedClass>();
	}
	template <class PredefinedClass, Operator *op> void RegisterPredefined()
	{
	}
	template <class OperatorClass, Operator *op> void RegisterOperator()
	{
		parser.RegisterOperator<OperatorClass>();
	}
	template <class OperatorClass, Predefined *predef> void RegisterOperator()
	{
	}*/
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
	/*virtual InputMode Mode()
	{
		return mode;
	}*/
	virtual Definitions &Defs()
	{
		return defs;
	}
};
