#pragma once

#include "Expression.h"
#include "SubsetGenerator.h"
#include "Substitutions.h"
#include "Calculator.h"
#include <string>

// Pattern matching callbacks (functors)
class MatchExpressionCallback : public MatchCallback
{
protected:
	Expression *expression;
	Expression *pattern;
	Calculator *calculator;
	MatchCallback *parentCallback;
	//virtual void Execute(Substitutions *subs, bool &continueMatching) = 0;
public:
	MatchExpressionCallback();
	virtual ~MatchExpressionCallback();
	//void operator ()(Substitutions *subs, bool &continueMatching);
	virtual void Initialize(Expression *theExpression, Expression *thePattern,
		Calculator *theCalculator, MatchCallback *callback);
};

class MatchFullExpressionCallback : public MatchExpressionCallback
{
public:
	virtual void ExecutePartial(Substitutions *subs, bool &continueMatching,
		std::set<ExprVector::iterator> selection)
	{
	}
};

class MatchPatternCallback : public MatchFullExpressionCallback
{
private:
	std::string patternName;
	Expression *patternValue;
protected:
	virtual void Execute(Substitutions *subs, bool &continueMatching);
public:
	virtual void InitializePattern(const std::string &name, Expression *value);
};

class MatchHeadCallback : public MatchFullExpressionCallback
{
protected:
	virtual void Execute(Substitutions *subs, bool &continueMatching);
};

class MatchLeafCallback : public MatchFullExpressionCallback
{
private:
	SubsetGenerator<Expression*> *subsetGenerator;
	ExprVector::iterator curPatternLeaf;
protected:
	virtual void Execute(Substitutions *subs, bool &continueMatching);
public:
	virtual void InitializeLeaf(SubsetGenerator<Expression*> *itsSubsetGenerator,
		ExprVector::iterator leaf);
};

class MatchSimpleCallback : public MatchFullExpressionCallback
{
private:
	bool *matches;
protected:
	virtual void Execute(Substitutions *subs, bool &continueMatching);
public:
	virtual void SetMatch(bool *match);
};

class MatchReplaceCallback : public MatchExpressionCallback
{
private:
	typedef ExprSubsetGenerator::PositionSet PositionSet;
	Expression *replace;
	bool *changed;
	bool *matches;
protected:
	virtual void Execute(Substitutions *subs, bool &continueMatching);
	virtual void ExecutePartial(Substitutions *subs, bool &continueMatching,
		ExprSubsetGenerator::PositionSet selection);
public:
	virtual void InitializeReplace(Expression *theReplace, bool *theChanged,
		bool *theMatches);
};
