#include "StdAfx.h"
#include "MatchCallbacks.h"
#include "LeafSubsetCallback.h"
#include <memory>

using std::auto_ptr;

MatchExpressionCallback::MatchExpressionCallback() : expression(0), pattern(0), calculator(0), parentCallback(0)
{
}

MatchExpressionCallback::~MatchExpressionCallback()
{
}

void MatchExpressionCallback::Initialize(Expression *theExpression, Expression *thePattern,
	Calculator *theCalculator, MatchCallback *callback)
{
	expression = theExpression;
	pattern = thePattern;
	calculator = theCalculator;
	parentCallback = callback;
}

/*void MatchExpressionCallback::operator ()(Substitutions *subs, bool &continueMatching)
{
	Execute(subs, continueMatching);
}*/

void MatchPatternCallback::InitializePattern(const string &name, Expression *value)
{
	patternName = name;
	patternValue = value;
}

void MatchPatternCallback::Execute(Substitutions *subs, bool &continueMatching)
{
	//Definition *functionDef = expression->FunctionDefinition(calculator);
	Substitutions newSubs;
	newSubs.Insert(subs);
	newSubs.SetSubstitution(patternName, patternValue);
	//(*parentCallback)(&newSubs, continueMatching);
	parentCallback->Execute(&newSubs, continueMatching);
}

void MatchHeadCallback::Execute(Substitutions *subs, bool &continueMatching)
{
	// Cannot match if pattern has no leaves and expression has leaves
	// (possible vice-versa: BlankNullSequence!)
	if(pattern->EmptyLeaves() && expression->EmptyLeaves())
		//(*parentCallback)(subs, continueMatching);
		parentCallback->Execute(subs, continueMatching);
	else if(!pattern->EmptyLeaves())
	{
		Definition *functionDef = pattern->FunctionDefinition(calculator);
		AttributeSet attributes;
		if(functionDef)
			attributes = functionDef->Attributes();
		auto_ptr< SubsetGenerator<Expression*> > subsetGenerator(0);
		//SubsetGenerator<Expression*> *subsetGenerator(0);
		if(attributes.Contains(atOrderless))
			subsetGenerator.reset(new OrderlessSubsetGenerator<Expression*>(expression->Leaves()));
		else
			subsetGenerator.reset(new OrderedSubsetGenerator<Expression*>(&expression->Leaves()));
		LeafSubsetCallback subsetCallback(expression, pattern, calculator, subs,
			parentCallback, subsetGenerator.get(), pattern->Leaves().begin());
		continueMatching = subsetGenerator->GenerateSubsets(&subsetCallback);
		//delete subsetGenerator;
	}
}

void MatchLeafCallback::InitializeLeaf(SubsetGenerator<Expression*> *itsSubsetGenerator,
	ExprVector::iterator leaf)
{
	subsetGenerator = itsSubsetGenerator;
	curPatternLeaf = leaf;
}

void MatchLeafCallback::Execute(Substitutions *subs, bool &continueMatching)
{
	ExprVector::iterator nextPatternLeaf(curPatternLeaf + 1);
	if(nextPatternLeaf == pattern->Leaves().end())
	{
		if(subsetGenerator->AllSelected())
			parentCallback->Execute(subs, continueMatching);
		else
			parentCallback->ExecutePartial(subs, continueMatching, subsetGenerator->Selection());
	}
	else
	{
		LeafSubsetCallback subsetCallback(expression, pattern, calculator, subs,
			parentCallback, subsetGenerator, nextPatternLeaf);
		continueMatching = subsetGenerator->GenerateSubsets(&subsetCallback);
	}
}

void MatchSimpleCallback::Execute(Substitutions *subs, bool &continueMatching)
{
	continueMatching = false;
	if(matches)
		*matches = true;
}

void MatchSimpleCallback::SetMatch(bool *match)
{
	matches = match;
}

void MatchReplaceCallback::Execute(Substitutions *subs, bool &continueMatching)
{
	continueMatching = false;
	ExprPtr result(replace->Clone());
	result->Substitute(subs);
	if(changed)
		*changed = !expression->SameExpression(result.get());
	expression->MoveNotCloned(result);
	if(matches)
		*matches = true;
}

void MatchReplaceCallback::ExecutePartial(Substitutions *subs, bool &continueMatching,
	PositionSet selection)
{
	// If no leaf mathes, don't replace anything!
	// (Could be the case when matching against x___Type.)
	if(selection.empty())
		return;

	continueMatching = false;
	ExprPtr old(0);
	if(changed)
		old = expression->Clone();
	// Delete first expression, but keep leave. (Will be replaced by 
	// replacing expression.) Delete all other matching leaves.
	//delete **selection.begin();
	//PositionSet::const_iterator selected = selection.begin();
	ExprVector::iterator firstMatchingLeaf;
	for(PositionSet::const_reverse_iterator selected = selection.rbegin(); selected != selection.rend(); ++selected)
	{
		//ExprVector::iterator leaf = *selected;
		firstMatchingLeaf = *selected;
		expression->DeleteLeaf(firstMatchingLeaf);
	}
	/*for(PositionSet::const_iterator selected = selection.begin(); selected != selection.end(); ++selected)
	{
		ExprVector::iterator leaf = *selected;
		expression->DeleteLeaf(leaf);
	}*/
	ExprPtr partialResult(replace->Clone());
	partialResult->Substitute(subs);
	//*selection. = partialResult;
	expression->InsertLeaf(firstMatchingLeaf, partialResult.release());
	if(changed)
	{
		*changed = !old->SameExpression(expression);
		//delete old;
	}
	if(matches)
		*matches = true;
}

void MatchReplaceCallback::InitializeReplace(Expression *theReplace, bool *theChanged,
	bool *theMatches)
{
	replace = theReplace;
	changed = theChanged;
	matches = theMatches;
}
