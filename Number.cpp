/*
**
** Number.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Number.h"
#include "Integer.h"
#include "Real.h"

Number::Number()
{
}

Number::~Number()
{
}

IntegerType Number::ReadInteger(Input &input)
{
	IntegerType intValue(0);
	// Process part before decimal separator
	while(input.Current() >= '0' && input.Current() <= '9')
	{
		intValue = intValue * 10 + (input.Current() - '0');
		input.Next();
	}
	return intValue;
}

NumberPtr Number::Read(Input &input)
{
	Integer ten(10);
	auto_ptr<Integer> intValue(new Integer);
	while(input.Current() >= '0' && input.Current() <= '9')
	{
		Integer digit(input.Current() - '0');
		intValue = intValue->Times(&ten)->Plus(&digit);
		input.Next();
	}

	// Process part after decimal separator
	if(input.Current() == '.')
	{
		// Cast the current value to float (just to prevent compiler warning)
		auto_ptr<MachineReal> realValue(new MachineReal(intValue));
		input.Next();
		auto_ptr<MachineReal> factor(new MachineReal(1.0));
		MachineReal tenth(0.1);
		while(input.Current() >= '0' && input.Current() <= '9')
		{
			MachineReal digit(input.Current() - '0');
			factor = factor->Times(&tenth);
			realValue = realValue->Plus(factor->Times(&digit).get());
			input.Next();
		}
		return NumberPtr(realValue);
	}
	else
		return NumberPtr(intValue);
}

/*NumberPtr Number::Divide(Number *number)
{

}

NumberPtr Number::Mod(Number *number)
{
	MachineInteger minusOne(-1);
	NumberPtr div(Times(number->Power(&minusOne).get()));
}*/

NumberPtr Number::Sin()
{
    throw NumberException("Not implemented yet.");
	//return 0;
}

NumberPtr Number::Cos()
{
    throw NumberException("Not implemented yet.");
	//return 0;
}
