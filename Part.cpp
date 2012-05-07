#include "StdAfx.h"
#include "Part.h"
#include "Expression.h"
#include "Exceptions.h"
#include "FunctionOperator.h"

int32 Part::instanceCount(0);

Part::Part()
{
	++instanceCount;
}

Part::~Part()
{
	--instanceCount;
}

CompoundPart::CompoundPart()
{
}

CompoundPart::CompoundPart(Part *head, SequencePart *leaves)
{
	parts.push_back(head);
	parts.push_back(new FunctionOperator);
	parts.push_back(leaves);
}

CompoundPart::~CompoundPart()
{
	for(PartList::const_iterator subPart = parts.begin(); subPart != parts.end(); ++subPart)
		delete *subPart;
}

Expression *CompoundPart::Group(PartsIterator part)
{
	Expression *result(Structure().release());
	delete this;
	*part = result;
	return result;
}

// Structure an expression according to infix input mode.
// This means that in an expression without a head, operands are "clued" to
// operators in order of operator precedence.
ExprPtr CompoundPart::Structure()
{
	if(parts.empty())
		return ExprPtr(0);

	Expression *result(0);

	if(parts.size() == 1)
		result = parts.front()->Group(PartsIterator(parts.begin(), parts));

	while(parts.size() > 1)
	{
		// Find operator with highest precedence
		OperatorPrecedence maxPrecedence(0);
		PartList::const_iterator maxPart(parts.end());
		for(PartList::const_iterator subPart = parts.begin(); subPart != parts.end(); ++subPart)
		{
			OperatorPrecedence precedence((*subPart)->Precedence());
			if(precedence > maxPrecedence)
			{
				maxPrecedence = precedence;
				maxPart = subPart;
			}
		}
		if(maxPart == parts.end())
			throw ParseException("Unexpected lack of operator.");
		// Group leaves around operators
		if((*maxPart)->ReverseGroup())
		{
			PartList::iterator part = parts.end();
			do
			{
				--part;
				if((*part)->Precedence() == maxPrecedence)
					result = (*part)->Group(PartsIterator(part, parts));
			}
			while(part != parts.begin());
		}
		else
		{
			for(PartList::iterator part = parts.begin(); part != parts.end(); ++part)
			{
				if((*part)->Precedence() == maxPrecedence)
					result = (*part)->Group(PartsIterator(part, parts));
			}
		} 		
	}

	// Structured part is now owned by caller.
	parts.clear();

	return ExprPtr(result);
}

SequencePart::SequencePart()
{
}

SequencePart::~SequencePart()
{
	for(StandalonePartVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		delete *leaf;
}

Expression *SequencePart::Group(PartsIterator part)
{
	Expression *result(Structure().release());
	delete this;
	*part = result;
	return result;
}

ExprPtr SequencePart::Structure()
{
	ExprPtr result(new Expression);
	result->ReserveLeaves(leaves.size());
	for(StandalonePartVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		result->AppendLeaf((*leaf)->Structure().release());
		delete *leaf;
	}
	leaves.clear();
	return result;
}

void SequencePart::AppendLeaf(StandalonePart *leaf)
{
	leaves.push_back(leaf);
}
