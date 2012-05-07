#pragma once

#include "Number.h"
#include "Exceptions.h"
#include <memory>

using std::auto_ptr;

typedef int NumberConvertLevel;

class AbstractConvertingNumber : public Number
{
public:
	virtual NumberConvertLevel ConvertLevel() = 0;
	virtual auto_ptr<AbstractConvertingNumber> Convert(Number *number) = 0;
};

template <class ImplementedNumber, NumberConvertLevel Level,
    class PlusResult = Number, class TimesResult = Number, class PowerResult = Number,
	class BaseClass = AbstractConvertingNumber>
class ConvertingNumber : public BaseClass
{
protected:
    virtual auto_ptr<ImplementedNumber> DoConvert(Number *number) = 0;
    virtual auto_ptr<PlusResult> Plus(ImplementedNumber *number) = 0;
    virtual auto_ptr<TimesResult> Times(ImplementedNumber *number) = 0;
    virtual auto_ptr<PowerResult> Power(ImplementedNumber *number) = 0;
    virtual CompareResult Compare(ImplementedNumber *number) = 0;
	virtual NumberConvertLevel ConvertLevel()
	{
		return Level;
	}
public:
	virtual NumberPtr Plus(Number *number)
	{
		AbstractConvertingNumber *convertingNumber(dynamic_cast<AbstractConvertingNumber*>(number));
		if(!convertingNumber)
			throw NumberException("Inconvertible number.");
	    NumberConvertLevel level1(ConvertLevel());
	    NumberConvertLevel level2(convertingNumber->ConvertLevel());
		if(level1 < level2)
		    // convert this to type of number
			return convertingNumber->Convert(this)->Plus(convertingNumber);
	    else if(level1 > level2)
		    // convert number to type of this
		    return Plus(Convert(convertingNumber).get());
	    else
		    // no conversion necessary
		    return NumberPtr(Plus(dynamic_cast<ImplementedNumber*>(convertingNumber)));
	}
	virtual NumberPtr Times(Number *number)
	{
		AbstractConvertingNumber *convertingNumber(dynamic_cast<AbstractConvertingNumber*>(number));
		if(!convertingNumber)
			throw NumberException("Inconvertible number.");
	    NumberConvertLevel level1(ConvertLevel());
	    NumberConvertLevel level2(convertingNumber->ConvertLevel());
		if(level1 < level2)
		    // convert this to type of number
			return convertingNumber->Convert(this)->Times(convertingNumber);
	    else if(level1 > level2)
		    // convert number to type of this
		    return Times(Convert(convertingNumber).get());
	    else
		    // no conversion necessary
		    return NumberPtr(Times(dynamic_cast<ImplementedNumber*>(convertingNumber)));
	}
	virtual NumberPtr Power(Number *number)
	{
		AbstractConvertingNumber *convertingNumber(dynamic_cast<AbstractConvertingNumber*>(number));
		if(!convertingNumber)
			throw NumberException("Inconvertible number.");
	    NumberConvertLevel level1(ConvertLevel());
	    NumberConvertLevel level2(convertingNumber->ConvertLevel());
		if(level1 < level2)
		    // convert this to type of number
			return convertingNumber->Convert(this)->Power(convertingNumber);
	    else if(level1 > level2)
		    // convert number to type of this
		    return Power(Convert(convertingNumber).get());
	    else
		    // no conversion necessary
		    return NumberPtr(Power(dynamic_cast<ImplementedNumber*>(convertingNumber)));
	}
	virtual CompareResult Compare(Number *number)
	{
		AbstractConvertingNumber *convertingNumber(dynamic_cast<AbstractConvertingNumber*>(number));
		if(!convertingNumber)
			throw NumberException("Inconvertible number.");
	    NumberConvertLevel level1(ConvertLevel());
	    NumberConvertLevel level2(convertingNumber->ConvertLevel());
		if(level1 < level2)
		    // convert this to type of number
			return convertingNumber->Convert(this)->Compare(convertingNumber);
	    else if(level1 > level2)
		    // convert number to type of this
		    return Compare(Convert(convertingNumber).get());
	    else
		    // no conversion necessary
		    return Compare(dynamic_cast<ImplementedNumber*>(convertingNumber));
	}

	virtual auto_ptr<AbstractConvertingNumber> Convert(Number *number)
	{
		return auto_ptr<AbstractConvertingNumber>(DoConvert(number));
	}
};
