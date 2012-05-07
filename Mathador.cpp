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

int main()
{
	{
		Test test;
	}

	// Enclose calculator run in brackets to delete variables before
	// debug output.
	{
		Calculator calculator;
		calculator.Run();
	}

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
		cout << "Definition instance count: " << Definition::instanceCount << endl;
		cout << "Definitions instance count: " << Definitions::instanceCount << endl;
		char c;
		cin >> c;
	}

	return 0;
}
