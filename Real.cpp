/*
**
** Real.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Real.h"
#include "Integer.h"
#include "Rational.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::ostringstream;

MachineReal::MachineReal() : value(0)
{
}

MachineReal::MachineReal(RealType itsValue) : value(itsValue)
{
}

MachineReal::MachineReal(const Integer &intValue) : value(0.0)
{
	static const RealType digitSize(Integer::DIGIT_RANGE);
	for(Integer::IntVector::const_reverse_iterator digit = intValue.digits.rbegin(); digit != intValue.digits.rend(); ++digit)
		value = value * digitSize + *digit;
	if(intValue.sign)
		value = -value;
}

MachineReal::MachineReal(const auto_ptr<Integer> &intValuePtr) : value(0.0)
{
	const Integer &intValue(*intValuePtr);
	static const RealType digitSize(Integer::DIGIT_RANGE);
	/*for(Integer::IntVector::const_reverse_iterator digit = intValue->digits.rbegin(); digit != intValue->digits.rend(); ++digit)
		value = value * digitSize + *digit;
	if(intValue->sign)
		value = -value;*/
	for(Integer::IntVector::const_reverse_iterator digit = intValue.digits.rbegin(); digit != intValue.digits.rend(); ++digit)
		value = value * digitSize + *digit;
	if(intValue.sign)
		value = -value;
}

MachineReal::~MachineReal()
{
}

void MachineReal::Print(Calculator *calculator, PrintMode mode)
{
	ostringstream stream;
	stream << value;
	string display = stream.str();
	cout << display;
	if(display.find('.') == string::npos)
		cout << ".";
}

auto_ptr<MachineReal> MachineReal::DoConvert(Number *number)
{
	Integer *integer;
	Rational *rational;
	if(integer = dynamic_cast<Integer*>(number))
	{
		//RealType value(0);
		//RealType digitSize(pow(static_cast<RealType>(256), static_cast<RealType>(sizeof(IntegerType))));
		//for(Integer::IntVector::const_iterator digit = integer->digits.begin(); digit != integer->digits.end(); ++digit)
		//	value = value * digitSize + *digit;
		//return auto_ptr<MachineReal>(new MachineReal(static_cast<RealType>(value)));
		return auto_ptr<MachineReal>(new MachineReal(*integer));
	}
	else if(rational = dynamic_cast<Rational*>(number))
	{
		MachineReal minusOne(-1);
		auto_ptr<MachineReal> numerator(new MachineReal(rational->Numerator()));
		auto_ptr<MachineReal> denominator(new MachineReal(rational->Denominator()));
		return numerator->Times(dynamic_cast<MachineReal*>(denominator->Power(&minusOne).get()));
		/*if((numerator = dynamic_cast<Integer*>(rational->Numerator())) &&
			(denominator = dynamic_cast<Integer*>(rational->Denominator())))
		{
			return auto_ptr<MachineReal>(new MachineReal(static_cast<RealType>(numerator->Value() / denominator->Value())));
		}*/

	}
	throw NumberException("Unsupported conversion to MachineReal.");
	//ArbitraryReal arbitrary;
	//return arbitrary.Convert(number);
}

auto_ptr<MachineReal> MachineReal::Plus(MachineReal *number)
{
	//MachineReal *real(dynamic_cast<MachineReal*>(number));
	//if(!real)
	//	throw NumberException("MachineReal expected.");
	return auto_ptr<MachineReal>(new MachineReal(value + number->value));
}

auto_ptr<MachineReal> MachineReal::Times(MachineReal *number)
{
	//MachineReal *real(dynamic_cast<MachineReal*>(number));
	//if(!real)
	//	throw NumberException("MachineReal expected.");
	return auto_ptr<MachineReal>(new MachineReal(value * number->value));
}

auto_ptr<Number> MachineReal::Power(MachineReal *number)
{
	//MachineReal *real(dynamic_cast<MachineReal*>(number));
	//if(!real)
	//	throw NumberException("MachineReal expected.");
	return NumberPtr(new MachineReal(pow(value, number->value)));
}

CompareResult MachineReal::Compare(MachineReal *number)
{
	//MachineReal *real(dynamic_cast<MachineReal*>(number));
	//if(!real)
	//	throw NumberException("MachineReal expected.");
	RealType value2 = number->value;
	if(value < value2)
		return crLess;
	else if(value > value2)
		return crGreater;
	else
		return crEqual;
}

void MachineReal::Negate()
{
	value = -value;
}

NumberPtr MachineReal::Abs()
{
	if(value < 0)
		return NumberPtr(new MachineReal(-value));
	else
		return NumberPtr(new MachineReal(value));
}

NumberPtr MachineReal::Sin()
{
	return NumberPtr(new MachineReal(sin(value)));
}

NumberPtr MachineReal::Cos()
{
	return NumberPtr(new MachineReal(cos(value)));
}

ArbitraryReal::ArbitraryReal() : sign(false), mantissa(0), exponent(0)
{
}

ArbitraryReal::ArbitraryReal(RealType itsValue) : sign(itsValue >= 0)
{
	// TODO
}

ArbitraryReal::ArbitraryReal(bool itsSign, const Integer &itsMantissa,
	const Integer &itsExponent) : sign(itsSign), mantissa(itsMantissa), exponent(itsExponent)
{
}

ArbitraryReal::~ArbitraryReal()
{
}

auto_ptr<ArbitraryReal> ArbitraryReal::DoConvert(Number *number)
{
	// TODO
	throw NumberException("Not implemented yet.");
	//return 0;
}

void ArbitraryReal::Print(Calculator *calculator, PrintMode mode)
{
	if(sign)
		cout << "-";
	// TODO
	mantissa.Print(calculator, mode);
	cout << "E";
	exponent.Print(calculator, mode);
}

void ArbitraryReal::Negate()
{
	sign = !sign;
}

NumberPtr ArbitraryReal::Abs()
{
	return NumberPtr(new ArbitraryReal(false, mantissa, exponent));
}

auto_ptr<ArbitraryReal> ArbitraryReal::Plus(ArbitraryReal *number)
{
	//ArbitraryReal *real(dynamic_cast<ArbitraryReal*>(number));
	//if(!real)
	//	throw NumberException("ArbitraryReal expected.");
	// TODO
	throw NumberException("Not implemented yet.");
	//return 0;
}

auto_ptr<ArbitraryReal> ArbitraryReal::Times(ArbitraryReal *number)
{
	//ArbitraryReal *real(dynamic_cast<ArbitraryReal*>(number));
	//if(!real)
	//	throw NumberException("ArbitraryReal expected.");
	// TODO
	throw NumberException("Not implemented yet.");
	//return 0;
}

auto_ptr<Number> ArbitraryReal::Power(ArbitraryReal *number)
{
	//ArbitraryReal *real(dynamic_cast<ArbitraryReal*>(number));
	//if(!real)
	//	throw NumberException("ArbitraryReal expected.");
	// TODO
	throw NumberException("Not implemented yet.");
	//return 0;
}

CompareResult ArbitraryReal::Compare(ArbitraryReal *number)
{
	//ArbitraryReal *real(dynamic_cast<ArbitraryReal*>(number));
	//if(!real)
	//	throw NumberException("ArbitraryReal expected.");
	// TODO
	return crEqual;
}
