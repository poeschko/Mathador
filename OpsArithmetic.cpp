/*
**
** OpsArithmetic.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "OpsArithmetic.h"
#include "Exceptions.h"
#include "Integer.h"
#include "Real.h"

void Plus::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->EmptyLeaves())
	{
		delete expression->Head();
		expression->Head(new Integer(0));
		return;
	}
	ExprVector &leaves(expression->Leaves());
	// Numeric values are on the beginning of leaves, because operation is orderless
	// and thus leaves are sorted.
	expression->ApplyNIfContainsReal(calculator);
	// Now, if the first leaf is an Integer, all numbers will be Integers.
	// If the first leaf is an Real, all numbers will be Reals.
	Number *firstNumber = leaves.front()->NumberHead();
	if(firstNumber)
	{
		NumberPtr result(firstNumber->CloneNumber());
		ExprVector::iterator firstNonNumber(leaves.end());
		for(ExprVector::iterator leaf = leaves.begin() + 1; leaf != leaves.end(); ++leaf)
		{
			Number *leafNumber = (*leaf)->NumberHead();
			if(leafNumber)
				result = result->Plus(leafNumber);
			else
			{
				firstNonNumber = leaf;
				break;
			}
		}
		for(ExprVector::iterator leaf = leaves.begin() + 1; leaf != firstNonNumber; ++leaf)
			delete *leaf;
		leaves.erase(leaves.begin() + 1, firstNonNumber);
		delete firstNumber;
		leaves.front()->Head(result.release());
	}
	// If just one operand left, this is the result.
	expression->ApplyOneIdentity();
}

void Times::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->EmptyLeaves())
	{
		delete expression->Head();
		expression->Head(new Integer(0));
		return;
	}
	ExprVector &leaves(expression->Leaves());
	// Numeric values are on the beginning of leaves, because operation is orderless
	// and thus leaves are sorted.
	expression->ApplyNIfContainsReal(calculator);
	// Now, if the first leaf is a Number, all numbers will be Numbers.
	Number *firstNumber = leaves.front()->NumberHead();
	if(firstNumber)
	{
		NumberPtr result(firstNumber->CloneNumber());
		ExprVector::iterator firstNonNumber(leaves.end());
		for(ExprVector::iterator leaf = leaves.begin() + 1; leaf != leaves.end(); ++leaf)
		{
			Number *leafNumber = (*leaf)->NumberHead();
			if(leafNumber)
				result = result->Times(leafNumber);
			else
			{
				firstNonNumber = leaf;
				break;
			}
		}
		for(ExprVector::iterator leaf = leaves.begin() + 1; leaf != firstNonNumber; ++leaf)
			delete *leaf;
		leaves.erase(leaves.begin() + 1, firstNonNumber);
		delete firstNumber;
		leaves.front()->Head(result.release());
	}
	// If just one operand left, this is the result.
	expression->ApplyOneIdentity();
}

void Power::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->EmptyLeaves())
	{
		delete expression->Head();
		expression->Head(new Integer(1));
	}
	ExprVector &leaves(expression->Leaves());
	expression->ApplyNIfContainsReal(calculator);
	// If more than 2 operands: First split up leaves (right-associative!)
	if(leaves.size() > 2)
	{
		for(ExprVector::reverse_iterator leaf = leaves.rbegin()+1; leaf != leaves.rend()-1; ++leaf)
		{
			Expression *power = new Expression("Power", 2);
			power->AppendLeaf(*leaf);
			power->AppendLeaf(*(leaf-1));
			*leaf = power;
		}
		leaves.erase(leaves.begin()+2, leaves.end());
		expression->Evaluate(calculator, recursions);
	}
	else if(leaves.size() == 2)
	{
		// TODO: implement using Number methods
		Number *mantissa(0);
		Number *exponent(0);
		if((mantissa = leaves[0]->NumberHead()) && (exponent = leaves[1]->NumberHead()))
		{
			NumberPtr result(mantissa->Power(exponent));
			expression->MoveNotCloned(ExprPtr(new Expression(result)));
		}

		/*if(leaves[0]->RealHead() || leaves[1]->RealHead())
		{
			leaves[0]->ApplyN(calculator);
			leaves[1]->ApplyN(calculator);
		}
		if(leaves[0]->NumberHead() && leaves[1]->NumberHead())
		{
			// If numbers are integers and second value is negative, create Rational
			if(leaves[0]->IntegerHead() && leaves[1]->IntegerHead() && leaves[1]->IntegerHead()->Value() < 0)
			{
				// n ^ (-m) = 1 / n ^ m
				Expression *base = leaves[0];
				Expression *exponent = leaves[1];
				exponent->IntegerHead()->Negate();
				delete expression->Head();
				expression->Head(new Expression("Rational"));
				leaves[0] = new Expression(1);
				leaves[1] = new Expression("Power", 2);
				leaves[1]->AppendLeaf(base);
				leaves[1]->AppendLeaf(exponent);
				expression->Evaluate(calculator, recursions);
			}
			else
			{		
				// Otherwise simply let Number instance calculate power
				Number *number = leaves[0]->NumberHead();
				number->Power(leaves[1]->NumberHead());
				delete leaves[1];
				leaves.erase(leaves.begin()+1);
				expression->ApplyOneIdentity();
			}
		}*/
	}
}

void Minus::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Minus expects one operand.");
	// -a = a * (-1)
	delete expression->Head();
	Expression *times = new Expression("Times");
	expression->Head(times);
	Expression *minusOne = new Expression(-1);
	expression->AppendLeaf(minusOne);
	expression->Evaluate(calculator, recursions);
}

void Subtract::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Subtract expects 2 operands.");
	// a - b = a + (-b)
	delete expression->Head();
	Expression *plus = new Expression("Plus");
	expression->Head(plus);
	Expression *minus = new Expression("Minus", 1);
	minus->AppendLeaf(expression->Leaves().at(1));
	expression->Leaves().at(1) = minus;
	expression->Evaluate(calculator, recursions);
}

void Divide::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Divide expects 2 operands.");
	// a / b = a * b ^ (-1)
	delete expression->Head();
	Expression *times = new Expression("Times");
	expression->Head(times);
	Expression *power = new Expression("Power", 2);
	power->AppendLeaf(expression->Leaves().at(1));
	Expression *minusOne = new Expression(-1);
	power->AppendLeaf(minusOne);
	expression->Leaves().at(1) = power;
	expression->Evaluate(calculator, recursions);
}
