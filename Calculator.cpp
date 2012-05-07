/*
**
** Calculator.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Calculator.h"
#include "Exceptions.h"
#include "Expression.h"

// Predefined operators
#include "Complex.h"
#include "EvaluationControl.h"
#include "FlowControl.h"
#include "List.h"
#include "NumericEvaluation.h"
#include "OpsArithmetic.h"
#include "OpsAttributes.h"
#include "OpsAssignment.h"
#include "OpsElementary.h"
#include "OpsExpressionStructure.h"
#include "OpsFunctional.h"
#include "OpsLogical.h"
#include "OpsTests.h"
#include "PatternMatching.h"
#include "Rational.h"
#include "Rule.h"
#include "Scoping.h"

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::cin;
using std::endl;

//#define REGISTER_OPERATOR(op) defs.DefineOperator<op>(); parser.RegisterOperator<op>();

#define REGISTER_OPERATOR(op) Register<op>();

Calculator::Calculator()
{
	// Initialize member objects
	parser.SetCalculator(this);
	//stack.SetCalculator(this);
	defs.SetCalculator(this);
	
	// Register pre-defined operators

	// Complex
	//REGISTER_OPERATOR(Complex)
	REGISTER_OPERATOR(OpAbs)

	// Evaluation control
	REGISTER_OPERATOR(Unevaluated)

	// FlowControl
	REGISTER_OPERATOR(CompoundExpression)
	REGISTER_OPERATOR(OpIf)
	REGISTER_OPERATOR(OpFor)

	// List
	REGISTER_OPERATOR(List)
	REGISTER_OPERATOR(Length)

	// Numeric evaluation
	REGISTER_OPERATOR(OpN)

	// Pattern matching
	REGISTER_OPERATOR(Pattern)
	REGISTER_OPERATOR(Blank)
	REGISTER_OPERATOR(BlankSequence)
	REGISTER_OPERATOR(BlankNullSequence)
	REGISTER_OPERATOR(MatchQ)

	// Rational
	//REGISTER_OPERATOR(Rational)

	// Rule
	REGISTER_OPERATOR(OpRule)
	REGISTER_OPERATOR(OpRuleDelayed)
	REGISTER_OPERATOR(OpReplace)
	REGISTER_OPERATOR(OpReplaceAll)
	REGISTER_OPERATOR(OpReplaceRepeated)

	// Arithmetic
	REGISTER_OPERATOR(Plus)
	REGISTER_OPERATOR(Times)
	REGISTER_OPERATOR(Power)
	REGISTER_OPERATOR(Minus)
	REGISTER_OPERATOR(Subtract)
	REGISTER_OPERATOR(Divide)
	REGISTER_OPERATOR(NonCommutativeMultiply)

	// Elementary
	REGISTER_OPERATOR(Sin)
	REGISTER_OPERATOR(Cos)

	// Logical
	REGISTER_OPERATOR(Not)
	REGISTER_OPERATOR(OpAnd)
	REGISTER_OPERATOR(OpOr)

	// Functional
	REGISTER_OPERATOR(Function)
	REGISTER_OPERATOR(Slot)
	REGISTER_OPERATOR(SlotSequence)
	REGISTER_OPERATOR(OpApply)
	REGISTER_OPERATOR(OpMap)

	// Assignment
	REGISTER_OPERATOR(Set)
	REGISTER_OPERATOR(SetDelayed)
	REGISTER_OPERATOR(UpSet)
	REGISTER_OPERATOR(UpSetDelayed)
	REGISTER_OPERATOR(Increment)
	REGISTER_OPERATOR(PreIncrement)

	// Attributes
	REGISTER_OPERATOR(OpAttributes)
	REGISTER_OPERATOR(OpSetAttributes)
	REGISTER_OPERATOR(OpClearAttributes)

	// Expression Structure
	REGISTER_OPERATOR(FullForm)
	REGISTER_OPERATOR(OpHead)

	// Tests
	REGISTER_OPERATOR(OpEqual)
	REGISTER_OPERATOR(OpUnequal)
	REGISTER_OPERATOR(OpLess)
	REGISTER_OPERATOR(OpGreater)
	REGISTER_OPERATOR(OpLessEqual)
	REGISTER_OPERATOR(OpGreaterEqual)

	// Scoping
	REGISTER_OPERATOR(Module)


	/*
	// Complex
	RegisterOperator<Complex>();
	RegisterOperator<OpAbs>();

	// Evaluation control
	RegisterOperator<Unevaluated>();

	// FlowControl
	RegisterOperator<CompoundExpression>();
	RegisterOperator<OpIf>();
	RegisterOperator<OpFor>();

	// List
	RegisterOperator<List>();
	RegisterOperator<Length>();

	// Numeric evaluation
	RegisterOperator<OpN>();

	// Pattern matching
	RegisterOperator<Pattern>();
	RegisterOperator<Blank>();
	RegisterOperator<BlankSequence>();
	RegisterOperator<BlankNullSequence>();
	RegisterOperator<MatchQ>();

	// Rational
	RegisterOperator<Rational>();

	// Rule
	RegisterOperator<OpRule>();
	RegisterOperator<OpRuleDelayed>();
	RegisterOperator<OpReplace>();
	RegisterOperator<OpReplaceAll>();
	RegisterOperator<OpReplaceRepeated>();

	// Arithmetic
	RegisterOperator<Plus>();
	RegisterOperator<Times>();
	RegisterOperator<Power>();
	RegisterOperator<Minus>();
	RegisterOperator<Subtract>();
	RegisterOperator<Divide>();
	RegisterOperator<NonCommutativeMultiply>();

	// Elementary
	RegisterOperator<Sin>();
	RegisterOperator<Cos>();

	// Logical
	RegisterOperator<Not>();
	RegisterOperator<OpAnd>();
	RegisterOperator<OpOr>();

	// Functional
	RegisterOperator<Function>();
	RegisterOperator<Slot>();
	RegisterOperator<SlotSequence>();
	RegisterOperator<OpApply>();
	RegisterOperator<OpMap>();

	// Assignment
	RegisterOperator<Set>();
	RegisterOperator<SetDelayed>();
	RegisterOperator<UpSet>();
	RegisterOperator<UpSetDelayed>();
	RegisterOperator<Increment>();
	RegisterOperator<PreIncrement>();

	// Attributes
	RegisterOperator<OpAttributes>();
	RegisterOperator<OpSetAttributes>();
	RegisterOperator<OpClearAttributes>();

	// Expression Structure
	RegisterOperator<FullForm>();
	RegisterOperator<OpHead>();

	// Tests
	RegisterOperator<OpEqual>();
	RegisterOperator<OpUnequal>();
	RegisterOperator<OpLess>();
	RegisterOperator<OpGreater>();
	RegisterOperator<OpLessEqual>();
	RegisterOperator<OpGreaterEqual>();

	// Scoping
	RegisterOperator<Module>();
	*/
}

Calculator::~Calculator()
{
}

void Calculator::Run()
{
	cout << "Mathador by Jan Poeschko" << endl;
	cout << "Version 0.0.1" << endl;
	cout << "Type 'exit' to exit, '?' for help." << endl;
	cout << "Options start with '$'." << endl << endl;
	while(true)
	{
		Input input;
		string helpTopic, option, stackOp;
		cout << "> ";
		input.ReadInput();
		if(input.IsString("exit"))
			break;
		if(input.StartsWith('?', helpTopic))
		{
			help.Print(helpTopic);
		}
		else if(input.StartsWith('$', option))
		{
			if(option == "defs")
				defs.Print(false);
			else if(option == "userdefs")
				defs.Print(true);
			else
				cout << "Unknown option." << endl;
		}
		else
		{
			try
			{
				CompoundPartPtr part = parser.Parse(input);
				input.SkipBlanks();
				if(!input.AtEnd())
					throw ParseException("Unrecognized command(s) at end of input.");
				ExprPtr expression(part->Structure());
				expression->Evaluate(this, 0);
				if(expression->SymbolName() != "Null")
				{
					expression->Print(this, pmStandard);
					cout << endl;
				}
				history.Push(ExprPtr(0), expression);
			}
			catch(Exception &exception)
			{
				exception.Print();
			}
		}
		cout << endl;
	}
}
