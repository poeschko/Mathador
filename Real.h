/*
**
** Real.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "ConvertingNumber.h"
#include "Integer.h"

class Complex;

class Real : public AbstractConvertingNumber
{
public:
	virtual string AtomName()
	{
		return "Real";
	}
	virtual bool SameExpression(Expression *expression)
	{
		Real *real(dynamic_cast<Real*>(expression));
		return real && (Compare(real) == crEqual);
	}
	virtual NumberPtr ApplyN()
	{
		return NumberPtr(CloneNumber());
	}
};

class MachineReal : public ConvertingNumber<MachineReal, 31,
	MachineReal, MachineReal, Number, Real>
{
private:
	RealType value;
protected:
	// ConvertingNumber methods
	virtual auto_ptr<MachineReal> DoConvert(Number *number);
public:
	// Constructors, destructor
	MachineReal();
	MachineReal(RealType itsValue);
	MachineReal(const Integer &intValue);
	MachineReal(const auto_ptr<Integer> &intValue);
	virtual ~MachineReal();

	// Atom methods
	virtual ExprPtr Clone()
	{
		return ExprPtr(new MachineReal(value));
	}
	virtual void Print(Calculator *calculator, PrintMode mode);

	// Number methods
	virtual void Negate();
	virtual NumberPtr Abs();
	virtual bool IsNumeric()
	{
		return true;
	}
	
	virtual auto_ptr<MachineReal> Plus(MachineReal *number);
	virtual auto_ptr<MachineReal> Times(MachineReal *number);
	virtual auto_ptr<Number> Power(MachineReal *number);
	virtual CompareResult Compare(MachineReal *number);

	virtual NumberPtr Sin();
	virtual NumberPtr Cos();
};

class ArbitraryReal : public ConvertingNumber<ArbitraryReal, 32,
	ArbitraryReal, ArbitraryReal, Number, Real>
{
private:
	bool sign;
	Integer mantissa;
	Integer exponent;
protected:
	virtual auto_ptr<ArbitraryReal> DoConvert(Number *number);
public:
	// Constructors, destructor
	ArbitraryReal();
	ArbitraryReal(RealType itsValue);
	ArbitraryReal(bool itsSign, const Integer &itsMantissa, const Integer &itsExponent);
	virtual ~ArbitraryReal();

	// Atom methods
	virtual ExprPtr Clone()
	{
		return ExprPtr(new ArbitraryReal(*this));
	}
	virtual void Print(Calculator *calculator, PrintMode mode);

	// Number methods
	virtual void Negate();
	virtual NumberPtr Abs();
	virtual bool IsNumeric()
	{
		return true;
	}
	
	virtual auto_ptr<ArbitraryReal> Plus(ArbitraryReal *number);
	virtual auto_ptr<ArbitraryReal> Times(ArbitraryReal *number);
	virtual auto_ptr<Number> Power(ArbitraryReal *number);
	virtual CompareResult Compare(ArbitraryReal *number);
};
