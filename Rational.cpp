/*
**
** Rational.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Rational.h"
#include "Exceptions.h"
#include "Integer.h"
#include "Real.h"
#include <iostream>

using std::cout;

Rational::Rational() : numerator(0), denominator(1)
{
}

Rational::Rational(const Rational &instance) :
	numerator(instance.numerator), denominator(instance.denominator)
{
}

Rational::Rational(const Integer &itsNumerator, const Integer &itsDenominator) :
	numerator(itsNumerator), denominator(itsDenominator)
{
}

Rational::Rational(IntegerType itsNumerator, IntegerType itsDenominator) :
	numerator(itsNumerator), denominator(itsDenominator)
{
}

Rational::Rational(auto_ptr<Integer> itsNumerator, auto_ptr<Integer> itsDenominator) :
	numerator(*itsNumerator.get()), denominator(*itsDenominator.get())
{
}

Rational::~Rational()
{
}

void Rational::Print(Calculator *calculator, PrintMode mode)
{
	numerator.Print(calculator, mode);
	cout << "/";
	denominator.Print(calculator, mode);
}

void Rational::Negate()
{
	numerator.Negate();
}

NumberPtr Rational::Abs()
{
	return NumberPtr(new Rational(*dynamic_cast<Integer*>(numerator.Abs().get()),
		*dynamic_cast<Integer*>(numerator.Abs().get())));
}

NumberPtr Rational::ApplyN()
{
	MachineReal real;
	return NumberPtr(real.Convert(this));
}

auto_ptr<Rational> Rational::DoConvert(Number *number)
{
	Integer *integer;
	Rational *rational;
	if(integer = dynamic_cast<Integer*>(number))
		return auto_ptr<Rational>(new Rational(*integer, Integer(1)));
	else if(rational = dynamic_cast<Rational*>(number))
		return auto_ptr<Rational>(new Rational(*rational));
	else
		throw NumberException("Unsupported conversion to Rational.");
}

auto_ptr<Number> Rational::Plus(Rational *number)
{
	auto_ptr<Integer> num(numerator.Times(&number->denominator)->Plus(number->numerator.Times(&denominator).get()));
	auto_ptr<Integer> denom(denominator.Times(&number->denominator));
	Rational result(num, denom);
	return result.Reduce();
}

auto_ptr<Number> Rational::Times(Rational *number)
{
	auto_ptr<Integer> num(numerator.Times(&number->numerator));
	auto_ptr<Integer> denom(denominator.Times(&number->denominator));
	//return auto_ptr<Rational>(new Rational(num, denom))->Reduce();
	Rational result(num, denom);
	return result.Reduce();
}

auto_ptr<Number> Rational::Power(Rational *number)
{
	// TODO: implement
	throw NumberException("Not implemented yet.");
	//return 0;
}

CompareResult Rational::Compare(Rational *number)
{
	auto_ptr<Integer> left(numerator.Times(&number->denominator));
	auto_ptr<Integer> right(denominator.Times(&number->numerator));
	return left->Compare(right.get());
}

/*
void Rational::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Rational expects 2 arguments.");
	// If both numerator and denominator are Integers, try to reduce the fraction.
	Integer *intHead1, *intHead2;
	if((intHead1 = expression->Leaf(0)->IntegerHead()) &&
		(intHead2 = expression->Leaf(1)->IntegerHead()))
	{
		// Reduce fraction
		intHead1->ReduceFraction(intHead2);
		// If denominator = 1: not really a fraction
		// If denominator = 0: throw exception
		if(intHead2->Value() == 1)
			expression->AssignLeaf(0);
		else if(intHead2->Value() == 0)
			throw MathException("Integer division by 0.");
		return;
	}
	// If one operand is Real, numerically evaluate both.
	if(expression->Leaf(0)->RealHead() || expression->Leaf(1)->RealHead())
	{
		expression->Leaf(0)->ApplyN(calculator);
		expression->Leaf(1)->ApplyN(calculator);
	}
	// If both numerator and denominator are Reals, simply carry out the division and
	// return the Real result
	Real *realHead1, *realHead2;
	if((realHead1 = expression->Leaf(0)->RealHead()) &&
		(realHead2 = expression->Leaf(1)->RealHead()))
	{
        Real minusOne(-1);
		realHead2->Power(&minusOne);
		realHead1->Times(realHead2);
		expression->AssignLeaf(0);
	}
}

void Rational::PrintExpression(Expression *expression, Calculator *calculator, PrintMode mode)
{
	if(expression->LeafCount() != 2)
		throw PrintException("Rational expects 2 arguments.");
	if(expression->Leaf(0)->NumberHead() && expression->Leaf(1)->NumberHead())
	{
		expression->Leaf(0)->Print(calculator, mode);
		cout << "/";
		expression->Leaf(1)->Print(calculator, mode);
	}
	else
		Predefined::PrintExpression(expression, calculator, mode);
}
*/

NumberPtr Rational::Reduce()
{
	auto_ptr<Integer> value1(new Integer(numerator));
	auto_ptr<Integer> value2(new Integer(denominator));
	auto_ptr<Integer> gcD(new Integer(0));
	Integer one(1);
	while(gcD->IsZero())
	{
		if(value1->IsZero())
		{
			gcD = value2;
			break;
		}
		if(value2->IsZero())
		{
			gcD = value1;
			break;
		}
		if(value1->Compare(&one) == crEqual)
		{
			gcD = auto_ptr<Integer>(new Integer(1));
			break;
		}
		if(value2->Compare(&one) == crEqual)
		{
			gcD = auto_ptr<Integer>(new Integer(1));
			break;
		}
		switch(value1->Compare(value2.get()))
		{
		case crGreater:
			value1 = value1->Mod(value2.get());
			break;
		case crLess:
			value2 = value2->Mod(value1.get());
			break;
		case crEqual:
			gcD = value1;
			break;
		}
	}
	if(gcD->Compare(&denominator) == crEqual)
		return NumberPtr(numerator.Divide(&denominator));
	else if(gcD->Compare(&one) == crEqual)
		return NumberPtr(new Rational(*this));
	else
	{
		return NumberPtr(new Rational(
			numerator.Divide(gcD.get()),
			denominator.Divide(gcD.get())));
	}
	//value /= gcD;
	//denominator->value /= gcD;
}
