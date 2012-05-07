#include "StdAfx.h"
#include "Scoping.h"
#include "Exceptions.h"
#include "Symbol.h"
#include <map>
#include <string>
#include <sstream>

using std::map;
using std::string;
using std::ostringstream;

int32 Module::moduleNumber(0);

void Module::Apply(Expression *expression, Calculator *calculator, int32 recursions)
{
	if(expression->LeafCount() != 2)
		throw ArgumentException("Module expects 2 arguments.");
	++moduleNumber;
	Expression *vars = expression->Leaf(0);
	map<string, string> substitutions;
	if(vars->FunctionName() == "List")
	{
		for(ExprVector::const_iterator item = vars->Leaves().begin(); item != vars->Leaves().end(); ++item)
		{
			Symbol *symbol = (*item)->SymbolHead();
			Expression *value(0);
			if(!symbol)
			{
				if((*item)->FunctionName() == "Set")
				{
					if((*item)->LeafCount() != 2)
						throw ArgumentException("Definition list expects set operation with 2 arguments.");
					symbol = (*item)->Leaf(0)->SymbolHead();
					value = (*item)->Leaf(1);
					value->Evaluate(calculator, recursions);
				}
				else
					throw ArgumentException("Definition list expects symbols or set operations.");
			}
			string name = symbol->Name();
			string newName;
			ostringstream newNameStream;
			newNameStream << name << "$" << moduleNumber;
			newName = newNameStream.str();
			Definition *def = calculator->Defs().GetDefinition(newName);
			def->Attributes(AttributeSet(atTemporary));
			if(value)
				def->Value(value);
			substitutions.insert(std::make_pair<string, string>(name, newName));
		}
		expression->AssignLeaf(1);
		expression->SubstituteNames(substitutions);
		expression->Evaluate(calculator, recursions);
	}
	else
		throw ArgumentException("Module expects its first argument to be a list of symbols.");
}
