/*
**
** Complex.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Complex.h"
#include "Exceptions.h"
#include "Number.h"
#include "Rational.h"
#include <iostream>

using std::cout;

Complex::Complex() : real(new Integer(0)), imaginary(new Integer(0))
{
}

Complex::Complex(Number *itsReal, Number *itsImaginary) : real(itsReal->CloneNumber()),
	imaginary(itsImaginary->CloneNumber())
{
}

Complex::Complex(NumberPtr itsReal, NumberPtr itsImaginary) : real(itsReal),
	imaginary(itsImaginary)
{
}

Complex::Complex(const Complex &instance) : real(instance.real->CloneNumber()),
	imaginary(instance.imaginary->CloneNumber())
{
}

Complex::~Complex()
{
}

ExprPtr Complex::Clone()
{
	return ExprPtr(new Complex(*this));
}

void Complex::Print(Calculator *calculator, PrintMode mode)
{
	cout << "(";
	real->Print(calculator, mode);
	cout << " + I * ";
	imaginary->Print(calculator, mode);
	cout << ")";
}

void Complex::Negate()
{
	real->Negate();
	imaginary->Negate();
}

NumberPtr Complex::Abs()
{
	Rational half(1, 2);
	NumberPtr sum(real->Times(real.get())->Plus(imaginary->Times(imaginary.get()).get()));
	return sum->Power(&half);
}

NumberPtr Complex::ApplyN()
{
	return NumberPtr(new Complex(real->ApplyN(), imaginary->ApplyN()));
}

auto_ptr<Complex> Complex::DoConvert(Number *number)
{
	Complex *complex(dynamic_cast<Complex*>(number));
	if(complex)
		return auto_ptr_cast<Complex>(complex->Clone());
	else
	{
		NumberPtr imag(new Integer(0));
		return auto_ptr<Complex>(new Complex(number->CloneNumber(), imag));
	}
}

auto_ptr<Complex> Complex::Plus(Complex *number)
{
	return auto_ptr<Complex>(new Complex(real->Plus(number->real.get()), imaginary->Plus(number->imaginary.get())));
}

auto_ptr<Complex> Complex::Times(Complex *number)
{	
    throw NumberException("Not implemented yet.");
	//return 0;	// TODO: implement
}

auto_ptr<Complex> Complex::Power(Complex *number)
{	
    throw NumberException("Not implemented yet.");
	//return 0;	// TODO: implement
}

CompareResult Complex::Compare(Complex *number)
{	
	throw NumberException("Complex not compareable.");
	/*Complex *complex(dynamic_cast<Complex*>(number));
	if(!complex)
		throw NumberException("Complex expected.");
	return 0;*/
	//return new Complex(real->Plus(complex->real), imaginary->Plus(complex->imaginary));
}

void OpAbs::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 1)
		throw ArgumentException("Abs expects 1 argument.");
	Expression *leaf = expression->Leaf(0);
	Number *number = leaf->NumberHead();
	if(number)
	{
		number->Abs();
		expression->AssignLeaf(0);
	}
}
