/*
**
** Exceptions.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <exception>
#include <string>
using std::string;

// Abstract base class for all exceptions
class Exception : public std::exception
{
private:
	string message;
public:
	Exception();
	Exception(string msg);
	virtual ~Exception() throw();
	virtual const string &Message() const;
	virtual void Print() const;
	virtual string Name() const = 0;
};

// Thrown when error occurs during parsing
class ParseException : public Exception
{
public:
	ParseException() { }
	ParseException(string msg) : Exception(msg) { }
	virtual void Print() const;
	virtual string Name() const
	{
		return "Parse Error";
	}
};

// Thrown when too little values on stack for operation in postfix mode
class StackException : public Exception
{
public:
	StackException() { }
	StackException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Stack Error";
	}
};

// Thrown when expression can't be evaluated due to mathematical reasons
// (e.g. division by zero)
class MathException : public Exception
{
public:
	MathException() { }
	MathException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Math Error";
	}
};

// Thrown when general error during evaluation process
class EvaluateException : public Exception
{
public:
	EvaluateException() { }
	EvaluateException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Evaluate Error";
	}
};

// Thrown when error during printing
class PrintException : public Exception
{
public:
	PrintException() { }
	PrintException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Print Error";
	}
};

// Thrown when too little arguments to function call
class ArgumentException : public Exception
{
public:
	ArgumentException() { }
	ArgumentException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Argument Error";
	}
};

// Thrown when certain limitation is reached
// (e.g. recursion limit, iteration limit)
class LimitationException : public Exception
{
public:
	LimitationException() { }
	LimitationException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Limitation Error";
	}
};

// Base class for all internal exceptions
// (that should not occur at all)
class InternalException : public Exception
{
public:
	InternalException() { }
	InternalException(string msg) : Exception(msg) { }
	virtual string Name() const
	{
		return "Internal Error";
	}
};

// Unexpected structure of expression (e.g. no head)
class ExpressionException : public InternalException
{
public:
	ExpressionException() { }
	ExpressionException(string msg) : InternalException(msg) { }
	virtual string Name() const
	{
		return "Expression Error";
	}
};

// Definition for symbol not found (expected to have one!) or inconsistent
class DefinitionException : public InternalException
{
public:
	DefinitionException() { }
	DefinitionException(string msg) : InternalException(msg) { }
	virtual string Name() const
	{
		return "Definition Error";
	}
};

// Certain class not not/expected (e.g. unknown number or attribute type)
class UnknownClassException : public InternalException
{
public:
	UnknownClassException() { }
	UnknownClassException(string msg) : InternalException(msg) { }
	virtual string Name() const
	{
		return "Unknown Class Error";
	}
};

// Error with Number types etc.
class NumberException : public InternalException
{
public:
	NumberException() { }
	NumberException(string msg) : InternalException(msg) { }
	virtual string Name() const
	{
		return "Number Error";
	}
};
