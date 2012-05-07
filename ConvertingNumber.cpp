#include "StdAfx.h"
#include "ConvertingNumber.h"

/*NumberPtr ConvertingNumber::Operate(Number *number, OperationFunction operation)
{
	ConvertingNumber *converting(dynamic_cast<ConvertingNumber*>(number));
	if(!converting)
		throw NumberException("Inconvertible number.");
	NumberConvertLevel level1(ConvertLevel());
	NumberConvertLevel level2(converting->ConvertLevel());
	if(level1 < level2)
	{
		// convert this to type of number
		ConvertingNumberPtr converted(converting->Convert(this));
		return converted->Operate(converting, operation);
	}
	else if(level1 > level2)
	{
		// convert number to type of this
		ConvertingNumberPtr converted(Convert(converting));
		return Operate(converted.get(), operation);
	}
	else
	{
		// no conversion necessary
		return (this->*operation)(converting);
	}
}*/
