/*
**
** Mathador.cpp
** Copyright by Jan Poeschko
**
** Description:
** Contains the main program.
**
*/

#include "StdAfx.h"
#include "Calculator.h"
#include "Test.h"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

//int _tmain(int argc, _TCHAR* argv[])
int main()
{
	{
		Test test;
		//test.Run();
	}

	// Enclose calculator run in brackets to delete variables before
	// debug output.
	{
		Calculator calculator;
		calculator.Run();
	}

	//Calculator *calculator = new Calculator;
	//calculator->Run();
	//delete calculator;

	/*{
	Parser parser;
	Input input;
	input.ReadInput();
	//CompoundPartPtr part(parser.Parse(input));
	CompoundPartPtr part(new CompoundPart);
	Expression *expr = new Expression;
	part->AppendSubPart(expr);
	//delete expr;
	//part->AppendSubPart(new Expression);
	//delete part.release();
	}*/

	// Debug output
	if(Part::InstanceCount() > 0 || Predefined::InstanceCount() > 0)
	{
		cout << "Part instance count: " << Part::InstanceCount() << endl;
		cout << "Predefined instance count: " << Predefined::InstanceCount() << endl;
		char c;
		cin >> c;
	}

	if(Expression::instanceCount > 0 || Definition::instanceCount > 0 || Definitions::instanceCount > 0)
	{
		cout << "Expression instance count: " << Expression::instanceCount << endl;
		/*for(std::map<int, Expression*>::iterator instance = Expression::instances.begin(); instance != Expression::instances.end(); ++instance)
			cout << instance->first << ": Symbol '" << instance->second->SymbolName() <<
				"', Function '" << instance->second->FunctionName() <<
				"', Leave count " << instance->second->LeafCount() <<
				", " << (dynamic_cast<Operator*>(instance->second) != 0 ? "Is Operator" : "No Operator") <<
				endl;*/
		/*for(std::map<int, Expression*>::iterator instance = Expression::instances.begin(); instance != Expression::instances.end(); ++instance)
			cout << instance->first << " (" << instance->second << "): "<<
				instance->second->LeafCount() << endl;*/
		cout << "Definition instance count: " << Definition::instanceCount << endl;
		//for(std::map<int, Definition*>::iterator instance = Definition::instances.begin(); instance != Definition::instances.end(); ++instance)
		//	cout << instance->first << endl;
		cout << "Definitions instance count: " << Definitions::instanceCount << endl;
		char c;
		cin >> c;
	}

	return 0;
}
