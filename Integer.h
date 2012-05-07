/*
**
** Integer.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "ConvertingNumber.h"
#include "Exceptions.h"
#include <cmath>
#include <vector>
#include <memory>

class MachineReal;
class Rational;

class Integer : public ConvertingNumber<Integer, 12, Integer, Integer, Number>
{
	friend class MachineReal;
public:
	typedef unsigned int Digit;
	typedef unsigned long long int Extended;
	typedef signed long long int SignedExtended;
	typedef std::vector<Digit> IntVector;
private:
	static const Extended DIGIT_RANGE = 0x100000000ll;
	static const Extended MAX_DIGIT = DIGIT_RANGE - 1;
	static const uint32 DIGIT_BITS = 32;
private:
	bool sign;
	IntVector digits;	// least significant digit first ("Little Endian")
	IntVector::size_type GetBitCount()
	{
		for(IntVector::size_type bit = digits.size() * DIGIT_BITS - 1; bit > 0; --bit)
			if(GetBit(bit) != 0)
				return bit + 1;
		return 1;
	}
	bool GetBit(IntVector::size_type index)
	{
		Digit digit = digits.at(index / DIGIT_BITS);
		return ((digit >> (index % DIGIT_BITS)) & 1) == 1;
	}
protected:
	std::auto_ptr<Integer> DoConvert(Number *number);
public:
	// Constructors, destructor
	Integer();
	Integer(IntegerType value);
	Integer(bool itsSign, const IntVector &itsDigits);
	Integer(const Integer &instance);
	virtual ~Integer();

	// Atom methods
	virtual ExprPtr Clone();
	virtual void Print(Calculator *calculator, PrintMode mode);
	virtual string AtomName()
	{
		return "Integer";
	}
	virtual bool SameExpression(Expression *expression)
	{
		Integer *integer(dynamic_cast<Integer*>(expression));
		return integer && (Compare(integer) == crEqual);
	}
	virtual NumberPtr ApplyN();

	// Number methods
	virtual void Negate();
	virtual NumberPtr Abs();
	virtual bool IsNumeric()
	{
		return false;
	}
	
	virtual auto_ptr<Integer> Plus(Integer *number);
	virtual auto_ptr<Integer> Times(Integer *number);
	virtual auto_ptr<Number> Power(Integer *number);
	virtual CompareResult Compare(Integer *number);
	
	bool IsZero() const;
	virtual void DivMod(Integer *number, auto_ptr<Integer> &result,
		auto_ptr<Integer> &mod);
	virtual auto_ptr<Integer> Divide(Integer *number);
	virtual auto_ptr<Integer> Mod(Integer *number);

	virtual IntegerType IntValue()
	{
		// TODO: check if not too large
		return digits.at(0) * (sign ? -1 : 1);
	}
};
