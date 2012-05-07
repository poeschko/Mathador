#pragma once

#include "Number.h"
#include "Exceptions.h"
#include <memory>

using std::auto_ptr;

/*class ConvertableNumber : public Number
{
protected:
	virtual NumberConvertLevel ConvertLevel() = 0;
	virtual NumberPtr Convert(Number *number) = 0;
};*/

//class ConvertingNumber;

typedef int NumberConvertLevel;

class AbstractConvertingNumber : public Number
{
public:
	virtual NumberConvertLevel ConvertLevel() = 0;
	virtual auto_ptr<AbstractConvertingNumber> Convert(Number *number) = 0;
//protected:
	//virtual NumberConvertLevel ConvertLevel() = 0;
	//template <class OperationResult, class Result>//,
		//class ImplementedNumber, class Level, class PlusResult, class TimesResult,
		//class PowerResult, class BaseClass>
	//OperationResult Operate(Number *number, Result (ConvertingNumber<ImplementedNumber, Level, PlusResult, TimesResult, PowerResult, BaseClass>::*operation) (ImplementedNumber *n)) = 0;
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
	/*template <class OperationResult, class Result>
	OperationResult Operate(Number *number, Result (ConvertingNumber<ImplementedNumber, Level, PlusResult, TimesResult, PowerResult, BaseClass>::*operation) (ImplementedNumber *n))
    {
		ConvertingNumber *convertingNumber(dynamic_cast<ConvertingNumber*>(number));
		//if(!convertingNumber)
		//	throw NumberException("Inconvertible number.");
		// TODO ??
	    NumberConvertLevel level1(ConvertLevel());
	    NumberConvertLevel level2(convertingNumber->ConvertLevel());
	    if(level1 < level2)
	    {
		    // convert this to type of number
		    //auto_ptr< converted(number->Convert(this));
		    return convertingNumber->Convert(this)->Operate<OperationResult>(convertingNumber, operation);
	    }
	    else if(level1 > level2)
	    {
		    // convert number to type of this
		    auto_ptr<ConvertingNumber> converted(Convert(convertingNumber));
		    return Operate<OperationResult>(converted.get(), operation);
	    }
	    else
	    {
		    // no conversion necessary
		    return OperationResult((this->*operation)(dynamic_cast<ImplementedNumber*>(convertingNumber)));
	    }
	}*/
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
		//return Operate<NumberPtr>(number, &ConvertingNumber<ImplementedNumber, Level, PlusResult, TimesResult, PowerResult, BaseClass>::Times);
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
		//return Operate<NumberPtr>(number, &ConvertingNumber<ImplementedNumber, Level, PlusResult, TimesResult, PowerResult, BaseClass>::Power);
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
		//return Operate<CompareResult>(number, &ConvertingNumber<ImplementedNumber, Level, PlusResult, TimesResult, PowerResult, BaseClass>::Compare);
	}

	/*static auto_ptr<ImplementedNumber> StaticConvert(Number *number)
	{
		ImplementedNumber instance;
		return instance.Convert(number);
	}*/
	virtual auto_ptr<AbstractConvertingNumber> Convert(Number *number)
	{
		return auto_ptr<AbstractConvertingNumber>(DoConvert(number));
	}
	
	/*virtual string AtomName() = 0;
	virtual bool SameExpression(Expression *expression) = 0;
	virtual NumberPtr ApplyN() = 0;*/
};
