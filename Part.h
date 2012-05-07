#pragma once

//#include "Expression.h"
#include "ExtendedIterator.h"
#include "Typedefs.h"
#include <string>
#include <vector>
#include <list>
#include <memory>

// Forward declarations
class Expression;
class Part;
class StandalonePart;
class CompoundPart;
class SequencePart;
class Calculator;
class Operator;

// Container definitions
typedef std::vector<Part*> PartVector;
typedef std::list<Part*> PartList;
typedef std::auto_ptr<Part> PartPtr;
typedef std::auto_ptr<CompoundPart> CompoundPartPtr;
typedef std::auto_ptr<SequencePart> SequencePartPtr;
typedef std::list<Expression> ExpressionList;
typedef std::auto_ptr<Expression> ExprPtr;
typedef std::vector<StandalonePart*> StandalonePartVector;

// Typedefs
typedef double OperatorPrecedence;

typedef ExtendedIterator<PartList> PartsIterator;

class Part
{
private:
	static int32 instanceCount;
public:
	Part();
	virtual ~Part();
	static int32 InstanceCount()
	{
		return instanceCount;
	}
	virtual Expression *Group(PartsIterator part) = 0;	// change list entry and return "itself" as Expression
	virtual OperatorPrecedence Precedence() = 0;
	virtual bool ReverseGroup()
	{
		return false;
	}
	virtual Operator *AsOperator()
	{
		return 0;
	}
};

class StandalonePart : public Part
{
public:
	virtual ExprPtr Structure() = 0;	// return structured part, which should then be independent from this!
};

class CompoundPart : public StandalonePart
{
private:
	PartList parts;
public:
	CompoundPart();
	CompoundPart(Part *head, SequencePart *leaves);
	virtual ~CompoundPart();
	virtual ExprPtr Structure();
	virtual Expression *Group(PartsIterator part);
	virtual bool HasSubParts()
	{
		return !parts.empty();
	}
	virtual Part *LastSubPart()
	{
		return parts.back();
	}
	virtual void AppendSubPart(Part *subPart)
	{
		parts.push_back(subPart);
	}
	virtual void PrependSubPart(Part *subPart)
	{
		parts.push_front(subPart);
	}
	virtual void PopLastSubPart()
	{
		parts.pop_back();
	}
	virtual OperatorPrecedence Precedence()
	{
		return 0;
	}
};

class SequencePart : public StandalonePart
{
private:
	StandalonePartVector leaves;
public:
	SequencePart();
	virtual ~SequencePart();
	virtual ExprPtr Structure();
	virtual Expression *Group(PartsIterator part);
	virtual void AppendLeaf(StandalonePart *leaf);
	virtual OperatorPrecedence Precedence()
	{
		return 0;
	}
};
