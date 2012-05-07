/*
**
** Help.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Help.h"
#include <iostream>
using std::cout;
using std::endl;

Help::Help()
{
}

Help::~Help()
{
}

void Help::Print(string topic)
{
	cout << endl;
	cout << "Developed by Jan Poeschko." << endl;
	cout << "Contact and Feedback: poeschko@student.tugraz.at" << endl;
	cout << endl;
	cout << "Numbers and Variables are entered in common way," << endl;
	cout << "with the dot (.) as decimal separator." << endl;
	cout << "Lists are entered in { }, e.g. {1,2,3}." << endl;
	cout << "Parameters to functions are entered in [ ], e.g. Sin[x]." << endl;
	cout << "All operators can also be entered in function form," << endl;
	cout << "e.g. Plus[a,b] is the same as a+b." << endl;
	cout << endl;
	cout << "Operators (standard operator precedence is applied):" << endl;
	cout << "Plus (+), Times (*), Power (^), Minus (-), Subtract (-), Divide (/)" << endl;
	cout << "Sin (sin), Cos (cos)" << endl;
	cout << "Not (!)" << endl;
	cout << "Apply (@@)" << endl;
	cout << "Set (=), SetDelayed (:=)" << endl;
	cout << "Rule (->), RuleDelayed (:>), ReplaceAll (/.), ReplaceRepeated (//.)" << endl;
	cout << endl;
	cout << "For a complete list of pre-defined operators and functions," << endl;
	cout << "type '$defs' to view all definitions." << endl;
	cout << endl;
	cout << "Options:" << endl;
	cout << "$infix:     Switch to infix input mode." << endl;
	cout << "$postfix:   Switch to postfix input mode." << endl;
	cout << "$defs:      View all current defitions." << endl;
	cout << "$userdefs:  View user definitions." << endl;
	cout << "$printall:  Print the whole stack after an operation." << endl;
	cout << "$printtwo:  Print last two elements on the stack after an operation." << endl;
	cout << "$printlast: Only print the last element on the stack after an operation." << endl;
	cout << endl;
	cout << "Stack operations:" << endl;
	cout << "\\print:   Print the stack." << endl;
	cout << "\\pop:     Pop last element of the stack." << endl;
	cout << "\\clear:   Clear the stack." << endl;
	cout << "\\reverse: Reverse the elements of the stack." << endl;
	cout << "\\swap:    Swap the last two elements of the stack." << endl;
	cout << endl;
	cout << "exit: Quit the program." << endl;
}
