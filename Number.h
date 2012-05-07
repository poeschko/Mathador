/*
**
** Number.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Atom.h"
#include "Input.h"
#include <memory>

// Forward declarations
class Number;

// Typedefs
//typedef std::auto_ptr<ConvertingNumber> ConvertingNumberPtr;
//typedef int NumberConvertLevel;

// Number hierarchy (states who can rely on who;
// e.g. MachineInteger need not implement addition with an
// ArbitraryInteger, only of two MachineInteger's):
// 11 MachineInteger
// 12 ArbitraryInteger
// 20 Rational
// 31 MachineReal
// 32 ArbitraryReal
// 40 Complex

class Number : public Atom
{
public:
	// Constructors, destructor
	Number();
	virtual ~Number();

	// Read functions
	static IntegerType ReadInteger(Input &input);
	static NumberPtr Read(Input &input);

	// abstract Number functions
	virtual NumberPtr Plus(Number *number) = 0;
	virtual NumberPtr Times(Number *number) = 0;
	virtual NumberPtr Power(Number *number) = 0;
	virtual CompareResult Compare(Number *number) = 0;
	virtual void Negate() = 0;
	virtual NumberPtr Abs() = 0;
	virtual NumberPtr ApplyN() = 0;
	virtual bool IsNumeric() = 0;

	virtual NumberPtr Sin();
	virtual NumberPtr Cos();

	virtual NumberPtr CloneNumber()
	{
		return auto_ptr_cast<Number>(Clone());
	}
};
