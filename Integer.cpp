/*
**
** Integer.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Integer.h"
#include "Rational.h"
#include "Real.h"
#include <iostream>
#include <iomanip>

using std::cout;
using std::hex;
using std::setw;

Integer::Integer() : sign(false), digits(1, 0)
{
}

Integer::Integer(IntegerType itsValue) : sign(itsValue < 0)
{
	if(itsValue == -9223372036854775808ll)
	{
		digits.push_back(0);
		digits.push_back(0);
		digits.push_back(1);
	}
	else
	{
		itsValue = abs(itsValue);
		digits.push_back(static_cast<Digit>(itsValue % DIGIT_RANGE));
		if(itsValue > MAX_DIGIT)
			digits.push_back(static_cast<Digit>(itsValue >> DIGIT_BITS));
	}
}

Integer::Integer(bool itsSign, const IntVector &itsDigits) : sign(itsSign), digits(itsDigits)
{
}

Integer::Integer(const Integer &instance) : sign(instance.sign), digits(instance.digits)
{
}

Integer::~Integer()
{
}

ExprPtr Integer::Clone()
{
	return ExprPtr(new Integer(*this));
}

NumberPtr Integer::ApplyN()
{
	MachineReal real;
	return NumberPtr(real.Convert(this));
}

bool Integer::IsZero() const
{
	if(digits.empty())
		return true;
	for(IntVector::const_iterator digit = digits.begin(); digit != digits.end(); ++digit)
		if(*digit != 0)
			return false;
	return true;
}

void Integer::Print(Calculator *calculator, PrintMode mode)
{
	if(IsZero())
	{
		cout << "0";
		return;
	}
	if(sign)
		cout << "-";
	string number("");
	Integer ten(10);
	auto_ptr<Integer> quotient(new Integer(false, digits));
	auto_ptr<Integer> oldQuotient(new Integer(false, digits));
	while(!quotient->IsZero())
	{
		auto_ptr<Integer> mod;
		auto_ptr<Integer> newQuotient(oldQuotient);
		oldQuotient = quotient;
		oldQuotient->DivMod(&ten, newQuotient, mod);
		char digit = mod->digits.at(0) + '0';
		number.insert(number.begin(), digit);
		quotient = newQuotient;
	}
	cout << number;
}

auto_ptr<Integer> Integer::DoConvert(Number *number)
{
	Integer *integer(dynamic_cast<Integer*>(number));
	if(!integer)
		throw NumberException("Unsupported conversion to Integer.");
	return auto_ptr<Integer>(integer);
}

auto_ptr<Integer> Integer::Plus(Integer *number)
{
	auto_ptr<Integer> result(new Integer);
	result->digits.clear();
	IntVector::size_type sumsize(max(digits.size(), number->digits.size()));
	if(sign == number->sign)
	{
		// perform addition
		result->sign = sign;
		Extended overflow(0);
		for(IntVector::size_type digit = 0; digit < sumsize; ++digit)
		{
			Extended digit1(digit < digits.size() ? digits.at(digit) : 0);
			Extended digit2(digit < number->digits.size() ? number->digits.at(digit) : 0);
			Extended sum(overflow + digit1 + digit2);
			if(sum > MAX_DIGIT)
			{
				sum -= DIGIT_RANGE;
				overflow = 1;
			}
			else
				overflow = 0;
			result->digits.push_back(static_cast<Digit>(sum));
		}
		if(overflow > 0)
			result->digits.push_back(static_cast<Digit>(overflow));
	}
	else
	{
		// perform subtraction
		SignedExtended overflow(0);
		for(IntVector::size_type digit = 0; digit < sumsize; ++digit)
		{
			Extended digit1(digit < digits.size() ? digits.at(digit) : 0);
			Extended digit2(digit < number->digits.size() ? number->digits.at(digit) : 0);
			SignedExtended difference(overflow + digit1 - digit2);
			if(difference < 0)
			{
				difference += DIGIT_RANGE;
				overflow = -1;
			}
			else
				overflow = 0;
			result->digits.push_back(static_cast<Digit>(difference));
		}
		if(overflow < 0)
		{
			// need to negate result, i.e. calculate 2-complement
			// invert digits
			for(IntVector::iterator digit = result->digits.begin(); digit != result->digits.end(); ++digit)
				*digit = ~ *digit;
			// add 1
			IntVector::iterator digit = result->digits.begin();
			while((*digit += 1) < 1)
				++digit;
			result->sign = true;
		}
		else
			result->sign = false;
	}
	return result;
}

auto_ptr<Integer> Integer::Times(Integer *number)
{
	auto_ptr<Integer> result(new Integer);
	result->digits.resize(digits.size() + number->digits.size(), 0);
	result->sign = sign ^ number->sign;

	for(IntVector::size_type i = 0; i < digits.size(); ++i)
		for(IntVector::size_type j = 0; j < number->digits.size(); ++j)
		{
			Extended digit1(digits.at(i));
			Extended digit2(number->digits.at(j));
			Extended product(digit1 * digit2);
			Digit low(static_cast<Digit>(product % DIGIT_RANGE));
			Digit high(static_cast<Digit>(product >> DIGIT_BITS));
			IntVector::size_type index(i + j);
			result->digits.at(index) += low;
			Digit carry((result->digits.at(index) < low) ? 1 : 0);
			result->digits.at(index + 1) += carry;
			carry = (result->digits.at(index + 1) < carry) ? 1 : 0;
			result->digits.at(index + 1) += high;
			carry += (result->digits.at(index + 1) < high) ? 1 : 0;
			for (IntVector::size_type k = index + 2; carry > 0; ++k)
			{
				result->digits.at(k) += carry;
				carry = (result->digits.at(k) < carry) ? 1 : 0;
			}
		}

	// remove leading 0's
	while(result->digits.size() > 1 && result->digits.back() == 0)
		result->digits.pop_back();

	return result;
}

auto_ptr<Number> Integer::Power(Integer *number)
{
	// Square and multiply
	auto_ptr<Integer> power(new Integer(*this));
	auto_ptr<Integer> result(new Integer(1));
	IntVector::size_type bitCount(number->GetBitCount());
	for(IntVector::size_type bit = 0; bit < bitCount; ++bit)
	{
		if(number->GetBit(bit))
			result = result->Times(power.get());
		power = power->Times(power.get());
	}
	if(number->sign)
		return auto_ptr<Number>(new Rational(auto_ptr<Integer>(new Integer(1)), result));
	else
		return auto_ptr<Number>(result);
}

void Integer::DivMod(Integer *number, auto_ptr<Integer> &result,
	auto_ptr<Integer> &mod)
{
	IntVector::const_iterator leading(number->digits.end());
	--leading;
	while(*leading == 0)
		if(leading == number->digits.begin())
			throw MathException("Integer division by 0.");
		else
			--leading;
	mod = auto_ptr<Integer>(new Integer(*this));

	IntVector::difference_type numberLength(leading - number->digits.begin());
	result->digits.resize(mod->digits.size() - numberLength);
	for(IntVector::difference_type digit = mod->digits.size() - 1; digit >= numberLength; --digit)
	{
		Extended curDigit(mod->digits[digit]);
		if(digit < static_cast<Extended>(mod->digits.size() - 1))
			curDigit |= static_cast<Extended>(mod->digits[digit + 1]) << DIGIT_BITS;
		Digit quotient(static_cast<Digit>(curDigit / *leading));
		result->digits[digit - numberLength] = quotient;
		Integer quotientInt(-static_cast<SignedExtended>(quotient));
		auto_ptr<Integer> product(number->Times(&quotientInt));
		product->digits.insert(product->digits.begin(), digit, 0);
		mod = mod->Plus(product.get());
	}
}

auto_ptr<Integer> Integer::Divide(Integer *number)
{
	auto_ptr<Integer> result(new Integer);
	auto_ptr<Integer> mod;
	DivMod(number, result, mod);
	return result;
}

auto_ptr<Integer> Integer::Mod(Integer *number)
{
	auto_ptr<Integer> result(new Integer);
	auto_ptr<Integer> mod;
	DivMod(number, result, mod);
	return mod;
}

CompareResult Integer::Compare(Integer *number)
{
	IntVector::difference_type size1(digits.size());
	IntVector::difference_type size2(number->digits.size());
	IntVector::difference_type digit(max(digits.size(), number->digits.size()) - 1);
	while(digit >= 0)
	{
		Digit digit1 = (size1 > digit) ? digits.at(digit) : 0;
		Digit digit2 = (size2 > digit) ? number->digits.at(digit) : 0;
		if(digit1 > digit2)
			return crGreater;
		else if(digit1 < digit2)
			return crLess;
		--digit;
	}
	return crEqual;
}

void Integer::Negate()
{
	sign = !sign;
}

NumberPtr Integer::Abs()
{
	return NumberPtr(new Integer(false, digits));
}
