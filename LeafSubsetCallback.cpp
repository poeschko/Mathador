#include "StdAfx.h"
#include "LeafSubsetCallback.h"
#include "MatchCallbacks.h"
#include <memory>

using std::auto_ptr;

void LeafSubsetCallback::Execute(ExprVectorPositions selection, bool &continueGenerating)
{
	MatchLeafCallback leafCallback;
	leafCallback.Initialize(expression, pattern, calculator, parentCallback);
	leafCallback.InitializeLeaf(subsetGenerator, curPatternLeaf);
	if(selection.size() != 1)
	{
		Definition *functionDef = pattern->FunctionDefinition(calculator);
		AttributeSet attributes;
		if(functionDef)
			attributes = functionDef->Attributes();
		//bool continueGenerating(true);
		auto_ptr<Expression> sequence(new Expression);
		sequence->Leaves().reserve(selection.size());
		for(ExprVectorPositions::const_iterator leaf = selection.begin(); leaf != selection.end(); ++leaf)
			sequence->AppendLeaf(**leaf);
		if(attributes.Contains(atFlat) && selection.size() > 1)
		{
			sequence->Head(pattern->Head()->Clone());
			continueGenerating = sequence->Matches(*curPatternLeaf, calculator, subs, true, leafCallback);
			delete sequence->Head();
			sequence->Head(0);
			if(!continueGenerating)
			{
				sequence->Leaves().clear();
				return;
			}
			//auto_ptr<Expression> flat(new Expression(pattern->FunctionName(), selection.size()));
		}
		sequence->Head(new Expression("Sequence"));
		//auto_ptr<Expression> sequence(new Expression("Sequence", selection.size()));
		continueGenerating = sequence->Matches(*curPatternLeaf, calculator, subs, true, leafCallback);
		sequence->Leaves().clear();
	}
	else
	{
		Expression *leaf = **selection.begin();
		continueGenerating = leaf->Matches(*curPatternLeaf, calculator, subs, true, leafCallback);
	}
}
