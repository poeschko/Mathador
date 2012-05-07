/*
**
** PatternMatching.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "PatternMatching.h"
#include "Exceptions.h"
#include "Symbol.h"

void MatchQ::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("MatchQ expects 2 arguments.");
	/*DefsVector possibleDefs;
	bool matches = expression->Leaf(0)->Matches(expression->Leaf(1), possibleDefs, calculator);
	for(DefsVector::const_iterator defs = possibleDefs.begin(); defs != possibleDefs.end(); ++defs)
		delete *defs;
	delete expression->Head();
	expression->DeleteLeaves();*/
	bool matches = expression->Leaf(0)->Matches(expression->Leaf(1), calculator);
	delete expression->Head();
	expression->DeleteLeaves();
	if(matches)
		expression->Head(new Symbol("True"));
	else
		expression->Head(new Symbol("False"));
}
