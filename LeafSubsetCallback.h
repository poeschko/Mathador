#pragma once

#include "SubsetGenerator.h"
#include "Expression.h"

class LeafSubsetCallback : public SubsetCallback<Expression*>
{
private:
	typedef std::set<ExprVector::iterator> ExprVectorPositions;
	Expression *expression;
	Expression *pattern;
	Calculator *calculator;
	Substitutions *subs;
	MatchCallback *parentCallback;
	SubsetGenerator<Expression*> *subsetGenerator;
	ExprVector::iterator curPatternLeaf;
public:
	LeafSubsetCallback(Expression *itsExpression, Expression *itsPattern,
		Calculator *itsCalculator, Substitutions *itsSubs,
		MatchCallback *itsParentCallback,
		SubsetGenerator<Expression*> *itsSubsetGenerator,
		ExprVector::iterator itsCurPatternLeaf) :
			expression(itsExpression), pattern(itsPattern),
			calculator(itsCalculator), subs(itsSubs),
			parentCallback(itsParentCallback),
			subsetGenerator(itsSubsetGenerator),
			curPatternLeaf(itsCurPatternLeaf)
	{
	}
	virtual ~LeafSubsetCallback()
	{
	}
	virtual void Execute(ExprVectorPositions selection, bool &continueGenerating);
};
