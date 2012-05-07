/*
**
** Rational.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Integer.h"

class Rational : public ConvertingNumber<Rational, 20, Number, Number, Number>
{
private:
	Integer numerator;
	Integer denominator;
protected:
	// ConvertingNumber methods
	/*virtual NumberConvertLevel ConvertLevel()
	{
		return 20;
	}*/
	virtual auto_ptr<Rational> DoConvert(Number *number);
	virtual auto_ptr<Number>  Plus(Rational *number);
	virtual auto_ptr<Number>  Times(Rational *number);
	virtual auto_ptr<Number>  Power(Rational *number);
	//virtual NumberPtr Divide(Number *number);
	//virtual NumberPtr Mod(Number *number);
	virtual CompareResult Compare(Rational *number);
public:
	// Constructors, destructor
	Rational();
	Rational(const Rational &instance);
	Rational(const Integer &itsNumerator, const Integer &itsDenominator);
	Rational(IntegerType itsNumerator, IntegerType itsDenominator);
	Rational(auto_ptr<Integer> itsNumerator, auto_ptr<Integer> itsDenominator);
	virtual ~Rational();

	// Atom methods
	virtual ExprPtr Clone()
	{
		return ExprPtr(new Rational(*this));
	}
	virtual void Print(Calculator *calculator, PrintMode mode);
	virtual string AtomName()
	{
		return "Rational";
	}
	virtual bool SameExpression(Expression *expression)
	{
		Rational *rational(dynamic_cast<Rational*>(expression));
		return rational && (Compare(rational) == crEqual);
	}

	// Number methods
	virtual NumberPtr ApplyN();
	virtual void Negate();
	virtual NumberPtr Abs();

	// Rational methods
	virtual Integer Numerator()
	{
		return numerator;
	}
	virtual Integer Denominator()
	{
		return denominator;
	}
	virtual NumberPtr Reduce();

	virtual bool IsNumeric()
	{
		return numerator.IsNumeric() || denominator.IsNumeric();
	}
};
