/*
**
** Complex.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Real.h"
#include "PredefinedFunc.h"

class Complex : public ConvertingNumber<Complex, 40, Complex, Complex, Complex>
{
private:
	NumberPtr real;
	NumberPtr imaginary;
protected:
	// ConvertingNumber methods
	/*virtual NumberConvertLevel ConvertLevel()
	{
		return 40;
	}*/
	virtual auto_ptr<Complex> DoConvert(Number *number);
	virtual auto_ptr<Complex> Plus(Complex *number);
	virtual auto_ptr<Complex> Times(Complex *number);
	virtual auto_ptr<Complex> Power(Complex *number);
	//virtual NumberPtr DoDivide(Number *number);
	//virtual NumberPtr DoMod(Number *number);
	virtual CompareResult Compare(Complex *number);
public:
	// Constructors, destructor
	Complex();
	Complex(Number *itsReal, Number *itsImaginary);
	Complex(NumberPtr itsReal, NumberPtr itsImaginary);
	Complex(const Complex &instance);
	virtual ~Complex();

	// Atom methods
	virtual string AtomName()
	{
		return "Complex";
	}
	virtual bool SameExpression(Expression *expression)
	{
		Complex *complex(dynamic_cast<Complex*>(expression));
		return complex && (Compare(complex) == crEqual);
	}
	virtual ExprPtr Clone();
	virtual void Print(Calculator *calculator, PrintMode mode);

	// Number methods
	virtual NumberPtr ApplyN();
	virtual void Negate();
	virtual NumberPtr Abs();

	virtual bool IsNumeric()
	{
		return real->IsNumeric() || imaginary->IsNumeric();
	}
};

class OpAbs : public PredefinedFunc
{
public:
	virtual string Name()
	{
		return "Abs";
	}
	virtual AttributeSet Attributes()
	{
		return AttributeSet(atListable, atNumericFunction, atProtected);
	}
	virtual void Apply(Expression *expression, Calculator *calculator, int32 recursions);
};
