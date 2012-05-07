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
	Rational result(num, denom);
	return result.Reduce();
}

auto_ptr<Number> Rational::Power(Rational *number)
{
	// TODO: implement
	throw NumberException("Not implemented yet.");
}

CompareResult Rational::Compare(Rational *number)
{
	auto_ptr<Integer> left(numerator.Times(&number->denominator));
	auto_ptr<Integer> right(denominator.Times(&number->numerator));
	return left->Compare(right.get());
}

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
}
