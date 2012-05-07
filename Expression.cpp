/*
**
** Expression.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Expression.h"
#include "Exceptions.h"
#include "Atom.h"
#include "Symbol.h"
#include "Number.h"
#include "Integer.h"
#include "Real.h"
#include "Definition.h"
#include "Substitutions.h"
#include "MatchCallbacks.h"
#include <algorithm>
#include <memory>

using std::auto_ptr;

int32 Expression::instanceCount(0);
int32 Expression::currentId(0);
std::map<int32, Expression*> Expression::instances;

// Default constructor: create expression without head and any leaves.
Expression::Expression(): head(0)
{
	CreateDebugInfo();
}

// Create a symbol expression (i.e. an expression with a Symbol instance in its head).
Expression::Expression(string symbolName) : head(0)
{
	CreateDebugInfo();
	head = new Symbol(symbolName);
}

// Create a function expression (i.e. an expression with a symbol expression in its head).
// The number of desired leaves can be specified and their space will be reserved,
// though the leaves themselves will not be created.
Expression::Expression(string functionName, ExprVector::size_type desiredLeafCount) : head(0)
{
	CreateDebugInfo();
	head = new Expression(functionName);
	leaves.reserve(desiredLeafCount);
}

// Create an integer expression (i.e. an expression with an Integer instance in its head).
// (This is similar to a symbol expression.)
Expression::Expression(IntegerType integerValue) : head(0)
{
	CreateDebugInfo();
	head = new Integer(integerValue);
}

Expression::Expression(NumberPtr number) : head(0)
{
	CreateDebugInfo();
	head = number.release();
}

// Destructor: clean up head and leaves.
Expression::~Expression()
{
	delete head;
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		delete *leaf;
	instances.erase(id);
	--instanceCount;
}

void Expression::CreateDebugInfo()
{
	id = currentId++;
	++instanceCount;
	instances.insert(std::make_pair<int32, Expression*>(id, this));
}

void Expression::AppendLeaf(ExprPtr leaf)
{
	leaves.push_back(leaf.release());
}

void Expression::AppendLeaf(Expression *leaf)
{
	leaves.push_back(leaf);
}

void Expression::PopLastLeaf()
{
	leaves.pop_back();
}

void Expression::DeleteFirstLeaf()
{
	delete leaves.front();
	leaves.erase(leaves.begin());
}

void Expression::ReserveLeaves(ExprVector::size_type size)
{
	leaves.reserve(size);
}

bool Expression::HasLeaves()
{
	return !leaves.empty();
}

Expression *Expression::LastLeaf()
{
	return leaves.back();
}

Expression *Expression::Leaf(ExprVector::size_type position)
{
	if(position >= 0 && position < leaves.size())
		return leaves.at(position);
	else
		return 0;
}

Expression *Expression::Head()
{
	return head;
}

void Expression::Head(Expression *newHead)
{
	head = newHead;
}

void Expression::Head(ExprPtr newHead)
{
	head = newHead.release();
}

void Expression::ReplaceHead(ExprPtr newHead)
{
	delete head;
	head = newHead.release();
}

ExprVector &Expression::Leaves()
{
	return leaves;
}

bool Expression::EmptyLeaves()
{
	return leaves.empty();
}

ExprVector::size_type Expression::LeafCount()
{
	return leaves.size();
}

void Expression::DeleteLeaves()
{
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		delete *leaf;
	leaves.clear();
}

void Expression::DeleteLeaf(ExprVector::iterator &leaf)
{
	delete *leaf;
	ExprVector::size_type position = leaf - leaves.begin();
	leaves.erase(leaf);
	leaf = leaves.begin() + position;
}

void Expression::InsertLeaf(ExprVector::iterator position, Expression *leaf)
{
	leaves.insert(position, leaf);
}

Expression *Expression::DoClone()
{
	Expression *result(new Expression());
	result->Head(head->Clone());
	result->ReserveLeaves(leaves.size());
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		result->AppendLeaf((*leaf)->Clone());
	return result;
}

ExprPtr Expression::Clone()
{
	ExprPtr result(new Expression());
	result->Head(head->Clone());
	result->ReserveLeaves(leaves.size());
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		result->AppendLeaf((*leaf)->Clone());
	return result;
}

void Expression::AssignCloned(Expression *expression)
{
	delete head;
	DeleteLeaves();
	head = expression->head->Clone().release();
	leaves.reserve(expression->leaves.size());
	for(ExprVector::const_iterator leaf = expression->leaves.begin(); leaf != expression->leaves.end(); ++leaf)
		leaves.push_back((*leaf)->Clone().release());
}

// Move head and leaves of an expression to this expression.
// The items are not cloned, and the source will be deleted.
void Expression::MoveNotCloned(ExprPtr source)
{
	delete head;
	DeleteLeaves();
	head = source->head;
	leaves = source->leaves;
	source->head = 0;
	source->leaves.clear();
}

void Expression::MoveLeaves(Expression *source)
{
	leaves = source->leaves;
	source->leaves.clear();
}

void Expression::AssignLeaf(ExprVector::size_type position)
{
	delete head;
	ExprVector::const_iterator posIterator = leaves.begin() + position;
	Expression *leaf = *posIterator;
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != posIterator; ++leaf)
		delete *leaf;
	if(posIterator < leaves.end())
		for(ExprVector::const_iterator leaf = posIterator + 1; leaf != leaves.end(); ++leaf)
			delete *leaf;
	head = leaf->head;
	leaves = leaf->leaves;
	leaf->head = 0;
	leaf->leaves.clear();
	delete leaf;
}

Predefined *Expression::PredefFunc(Calculator *calculator)
{
	if(head)
	{
		Definition *def = FunctionDefinition(calculator);
		if(def)
			return def->Predef();
	}
	return 0;
}

Operator *Expression::PredefOperator(Calculator *calculator)
{
	return dynamic_cast<Operator*>(PredefFunc(calculator));
}

// For sorting, every expression is assigned a string, depending on that it is ordered.
// Order: Integers, Real, Rationals, Symbols (sorted by name), Functions (also sorted by name).
string Expression::OrderKey()
{
	if(!head)
		return "";
	string function = FunctionName();
	if(dynamic_cast<Integer*>(head))
		return "11";
	else if(dynamic_cast<Real*>(head))
		return "12";
	else if(function == "Rational")
		return "13";
	else if(function == "Complex")
		return "14";
	else if(dynamic_cast<Symbol*>(head))
		return "2" + SymbolName();
	else if(function == "Pattern")
		return "90";
	else if(function == "Blank")
		return "91";
	else if(function == "BlankSequence")
		return "92";
	else if(function == "BlankNullSequence")
		return "93";
	else
		return "5" + FunctionName();
}

// Returns
// -1: expression1 < expression2 (i.e. expression1 comes before expression2 when sorted)
//  0: expression1 is equal to expression2, considering sorting
//  1: expression1 > expression2
// TODO: There are still some not covered cases, e.g. concerning Blanks and Patterns,
// Power and Product sorting, Number comparison
int32 Expression::CompareEqual(Expression *expression1, Expression *expression2)
{
	string order1 = expression1->OrderKey();
	string order2 = expression2->OrderKey();
	if(order1 == order2)
	{
		string function1 = expression1->FunctionName();
		if(expression1->LeafCount() == expression2->LeafCount())
		{
			// The "most significant" leaf of a Pattern is its second leaf.
			if(function1 == "Pattern" && expression1->LeafCount() == 2)
			{
				int32 cmp = CompareEqual(expression1->leaves.at(1), expression2->leaves.at(1));
				if(cmp != 0)
					return cmp;
			}
			// In general, loop through leaves while equal order.
			for(ExprVector::const_iterator leaf = expression1->leaves.begin(); leaf != expression1->leaves.end(); ++leaf)
			{
				Expression *leaf2 = expression2->leaves.at(leaf - expression1->leaves.begin());
				int32 cmp = CompareEqual(*leaf, leaf2);
				if(cmp != 0)
					return cmp;
			}
			return 0;
		}
		else
		{
			// For the following special cases, the two expressions are the 
			// same function for sure, because they have the same order.
			// Blanks are ordered backwards if they are more general, i.e.
			// they have less leaves.
			if(function1 == "Blank" || function1 == "BlankSequence" || function1 == "BlankNullSequence")
			{
				if(expression1->LeafCount() > expression2->LeafCount())
					return -1;
				else
					return 1;
			}
			else
			{
				if(expression1->LeafCount() < expression2->LeafCount())
					return -1;
				else
					return 1;
			}
		}
	}
	else if(order1 < order2)
		return -1;
	else
		return 1;
}

bool Expression::Compare(Expression *expression1, Expression *expression2)
{
	return CompareEqual(expression1, expression2) < 0;
}

// If the expression has one had, this will become the whole expression.
// Otherwise the expression will not be changed.
bool Expression::ApplyOneIdentity()
{
	if(leaves.size() == 1)
	{
		AssignLeaf(0);
		return true;
	}
	else
		return false;
}

// Flatten out all expressions with a given head.
void Expression::Flatten(const string head)
{
	ExprVector newLeaves;
	newLeaves.reserve(leaves.size());
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		if((*leaf)->FunctionName() == head)
		{
			for(ExprVector::const_iterator subLeaf = (*leaf)->Leaves().begin(); subLeaf != (*leaf)->Leaves().end(); ++subLeaf)
				newLeaves.push_back(*subLeaf);
			(*leaf)->Leaves().clear();
			delete *leaf;
		}
		else
			newLeaves.push_back(*leaf);
	leaves = newLeaves;
}

// Evaluates an expression. Considers all rules etc. In some cases there are simply
// no changes possible and the expression remains unchanged (e.g. a+b).
void Expression::Evaluate(Calculator *calculator, int32 recursions)
{
	// Since the expression is expected to be already structured, it must have a head.
	if(!head)
		throw DefinitionException("Expression::Evaluate: Expression has no head.");
	if(recursions > Max_Evaluate_Recursions)
		throw LimitationException("Maximum number of recursive definitions reached.");
	head->Evaluate(calculator, recursions);
	// If Expression is a symbol
	Definition *symbolDef = SymbolDefinition(calculator);
	if(symbolDef)
	{
		// Look if value is defined
		if(symbolDef->Value())
		{
			bool changed = !SameExpression(symbolDef->Value());
			AssignCloned(symbolDef->Value());
			if(changed)
				Evaluate(calculator, recursions + 1);
		}
	}
	// If head is a pure function
	if(head->FunctionName() == "Function" && head->LeafCount() == 1)
	{
		ExprPtr body(head->leaves.at(0));
		body->SubstituteSlots(leaves);
		body->Evaluate(calculator, recursions);
		head->leaves.clear();
		MoveNotCloned(body);
	}
	// If Expression is Function call
	Definition *def = FunctionDefinition(calculator);
	if(def)
	{
		AttributeSet attributes = def->Attributes();
		// Evaluate child leaves if no Hold attribute
		if(!attributes.Contains(atHoldAll) && !attributes.Contains(atHoldAllComplete))
		{
			if(!attributes.Contains(atHoldFirst))
				if(!leaves.empty())
					leaves.at(0)->Evaluate(calculator, recursions);
			if(!attributes.Contains(atHoldRest))
				if(leaves.size() > 1)
					for(ExprVector::const_iterator leaf = leaves.begin()+1; leaf != leaves.end(); ++leaf)
						(*leaf)->Evaluate(calculator, recursions);
		}
		// Flatten out sequences
		if(!attributes.Contains(atSequenceHold))
		{
			Flatten("Sequence");
		}
		// Apply down values
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		{
			Definition *def(0);
			if((*leaf)->SymbolHead())
				def = (*leaf)->SymbolDefinition(calculator);
			else if((*leaf)->IsFunctionCall())
				def = (*leaf)->FrontFunctionDefinition(calculator);
			if(def)
			{
				bool changed(false);
				if(def->ApplyUpValues(this, calculator, &changed) && changed)
				{
					Evaluate(calculator, recursions + 1);
					return;
				}
			}
		}
		// Apply Flat attribute
		if(attributes.Contains(atFlat))
		{
			Flatten(FunctionName());
		}
		// Apply OneIdentity attribute
		if(attributes.Contains(atOneIdentity))
		{
			if(ApplyOneIdentity())
				return;
		}
		// Apply Listable attribute
		if(attributes.Contains(atListable))
		{
			// Determine resulting list length, check if there are lists with different lengths
			bool listFound(false);
			ExprVector::size_type listSize(0);
			for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
				if((*leaf)->FunctionName() == "List")
					if(!listFound)
					{
						listSize = (*leaf)->LeafCount();
						listFound = true;
					}
					else
						if(listSize != (*leaf)->LeafCount())
							throw EvaluateException("Listable operation requires lists of the same length.");
			// Construct resulting list
			if(listFound)
			{
				ExprVector items;
				// Number of resulting list items = items in operand list
				items.reserve(listSize);
				// Loop through list items
				for(ExprVector::size_type position = 0; position < listSize; ++position)
				{
					// List item = function call, number of operands = number of original operands
					Expression *item = new Expression(FunctionName(), leaves.size());
					for(ExprVector::iterator itemLeaf = leaves.begin(); itemLeaf != leaves.end(); ++itemLeaf)
					{
						if((*itemLeaf)->FunctionName() == "List")
						{
							// Add corresponding list item
							item->AppendLeaf(*((*itemLeaf)->leaves.begin() + position));
						}
						else
						{
							// Add "whole" item (clone if necessary)
							if(position == 0)
								item->AppendLeaf(*itemLeaf);
							else
								item->AppendLeaf((*itemLeaf)->Clone());
						}
					}
					items.push_back(item);
				}
				// Delete lists (elements are now in resulting list)
				for(ExprVector::iterator itemLeaf = leaves.begin(); itemLeaf != leaves.end(); ++itemLeaf)
					if((*itemLeaf)->FunctionName() == "List")
					{
						(*itemLeaf)->leaves.clear();
						delete *itemLeaf;
					}
				delete head;
				head = new Expression("List");
				leaves = items;
				// Evaluate the list for itself
				Evaluate(calculator, recursions);
				return;
			}
		}
		// Apply Orderless attribute
		if(attributes.Contains(atOrderless))
		{
			std::sort(leaves.begin(), leaves.end(), &Compare);
		}
		// Apply down value rules
		bool changed(false);
		if(def->ApplyDownValues(this, calculator, &changed))
		{
			if(changed)
				Evaluate(calculator, recursions + 1);
			return;
		}
		// Execute linked operator, if specified
		if(def->Predef())
			def->Predef()->Apply(this, calculator, recursions);
	}
	else
	{
		// Otherwise simply evaluate the leaves
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
			(*leaf)->Evaluate(calculator, recursions);
	}
}

bool Expression::IsFunctionCall()
{
	if(head)
		return head->SymbolHead() != 0;
	else
		return false;
}

// Suppposes that expression is a function call, i.e. the head is a symbol expression,
// thus the head of the head is a symbol. Returns the name of this symbol.
string Expression::FunctionName()
{
	if(head)
	{
		Symbol *symbolHead = dynamic_cast<Symbol*>(head->Head());
		if(symbolHead)
			return symbolHead->Name();
	}
	return "";
}

// Suppposes that expression is a function call, i.e. the head is a symbol expression,
// thus the head of the head is a symbol. Returns the definition of this symbol.
Definition *Expression::FunctionDefinition(Calculator *calculator)
{
	if(head)
	{
		Symbol *symbolHead = dynamic_cast<Symbol*>(head->Head());
		if(symbolHead)
			return symbolHead->GetDefinition(calculator);
	}
	return 0;
}

Definition *Expression::FrontFunctionDefinition(Calculator *calculator)
{
	Definition *funcDef = FunctionDefinition(calculator);
	if(funcDef)
		return funcDef;
	else if(head)
		return head->FrontFunctionDefinition(calculator);
	else
		return 0;
}

// Supposes that expression is a symbol expression, i.e. the head is a symbol.
// Returns the definition of this symbol. (Overridden by Symbol.)
Definition *Expression::SymbolDefinition(Calculator *calculator)
{
	if(head)
	{
		Symbol *symbolHead = dynamic_cast<Symbol*>(head);
		if(symbolHead)
			return symbolHead->GetDefinition(calculator);
	}
	return 0;
}

string Expression::SymbolName()
{
	if(head)
	{
		Symbol *symbolHead = dynamic_cast<Symbol*>(head);
		if(symbolHead)
			return symbolHead->Name();
	}
	return "";
}

bool Expression::IsAtom()
{
	return dynamic_cast<Atom*>(head) != 0;
}

Atom *Expression::AtomHead()
{
	return dynamic_cast<Atom*>(head);
}

Number *Expression::NumberHead()
{
	return dynamic_cast<Number*>(head);
}

Symbol *Expression::SymbolHead()
{
	return dynamic_cast<Symbol*>(head);
}

// Evaluates an expression numerically.
// This is done by numerically evaluating its leaves and then evaluating the whole expression.
void Expression::ApplyN(Calculator *calculator)
{
	Number *numberHead = NumberHead();
	if(numberHead)
	{
		NumberPtr numericHead(numberHead->ApplyN());
		delete head;
		head = numericHead.release();
	}
	// Numerically evaluate leaves
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		(*leaf)->ApplyN(calculator);
	// Evaluate expression (with new numeric leaves)
	Evaluate(calculator, 0);
}

void Expression::ApplyNIfContainsReal(Calculator *calculator)
{
	// If one leaf is Real, all leaves will be converted to Real (N applied).
	bool containsReal(false);
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		Number *number((*leaf)->NumberHead());
		if(number && number->IsNumeric())
		{
			containsReal = true;
			break;
		}
	}
	if(containsReal)
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
			(*leaf)->ApplyN(calculator);
}

// Prints out an expression.
void Expression::Print(Calculator *calculator, PrintMode mode)
{
	if(!head)
		throw PrintException("Expression has no head.");
	// In standard print mode, let operator decide how expression should be printed.
	// (E.g. print Plus[a,b] as a+b.)
	if(mode == pmStandard)
	{
		Definition *def = FunctionDefinition(calculator);
		if(def && def->Predef())
		{
			def->Predef()->PrintExpression(this, calculator, mode);
			return;
		}
	}
	// If expression has no linked operator or print mode is FullForm, simply print
	// head and leaves. If head is no Atom, we have to put it in parentheses.
	if(!head->IsAtom() && !dynamic_cast<Atom*>(head))
		cout << "(";
	head->Print(calculator, mode);
	if(!head->IsAtom() && !dynamic_cast<Atom*>(head))
		cout << ")";
	if(!dynamic_cast<Atom*>(head))
	{
		cout << "[";
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		{
			if(leaf != leaves.begin())
				cout << ", ";
			(*leaf)->Print(calculator, mode);
		}
		cout << "]";
	}
}

OperatorPrecedence Expression::Precedence()
{
	return 0;
}

bool Expression::HasHead(Expression *theHead)
{
	if(!head)
		throw ExpressionException("Expression has no head.");
	// If expression is an atom expression (i.e. it has an Atom head),
	// its "head" is a symbol like Integer, Real, or Symbol.
	// Otherwise simply compare if the heads have the same structure.
	Atom *atomHead = dynamic_cast<Atom*>(head);
	if(atomHead)
		return atomHead->AtomName() == theHead->SymbolName();
	else
		return head->SameExpression(theHead);
}

bool Expression::SameExpression(Expression *expression)
{
	if(!expression->head)
		return false;
	if(!head->SameExpression(expression->head))
		return false;
	if(leaves.size() != expression->leaves.size())
		return false;
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		Expression *exprLeaf = *(expression->leaves.begin() + (leaf - leaves.begin()));
		if(!(*leaf)->SameExpression(exprLeaf))
			return false;
	}
	return true;
}

bool Expression::SamePattern(Expression *expression, StringMap &aliases)
{
	if(!expression->head)
		return false;
	if(FunctionName() == "Pattern" && expression->FunctionName() == "Pattern" &&
		leaves.size() == 2 && expression->leaves.size() == 2 &&
		leaves.at(0)->SymbolHead() && expression->leaves.at(0)->SymbolHead())
	{
		string name1 = leaves.at(0)->SymbolName();
		string name2 = expression->leaves.at(0)->SymbolName();
		map<string, string>::const_iterator found = aliases.find(name1);
		if(found == aliases.end())
			aliases.insert(std::make_pair<string, string>(name1, name2));
		else
			if(found->first != name1 || found->second != name2)
				return false;
		return Leaf(1)->SamePattern(expression->Leaf(1), aliases);
	}
	if(!head->SamePattern(expression->head))
		return false;
	if(leaves.size() != expression->leaves.size())
		return false;
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
	{
		Expression *exprLeaf = *(expression->leaves.begin() + (leaf - leaves.begin()));
		if(!(*leaf)->SamePattern(exprLeaf))
			return false;
	}
	return true;
}

bool Expression::SamePattern(Expression *expression)
{
     StringMap aliases;
     return SamePattern(expression, aliases);
}

// Overloaded version of Matches that discards the possible definitions and just
// checks whether the expression matches against the pattern.
bool Expression::Matches(Expression *pattern, Calculator *calculator)
{
	MatchSimpleCallback callback;
	callback.Initialize(this, pattern, calculator, 0);
	bool matches(false);
	callback.SetMatch(&matches);
	Matches(pattern, calculator, 0, true, callback);
	return matches;
}

// returns whether matching should continue
bool Expression::Matches(Expression *pattern, Calculator *calculator,
	Substitutions *subs, bool fullMatch,
	MatchCallback &callback)
{
	bool continueMatching(true);
	string patternFunction = pattern->FunctionName();
	// If Atom: must be completely identical
	if(pattern->AtomHead())
	{
		if(SameExpression(pattern))
			callback.Execute(subs, continueMatching);
	}
	else if(patternFunction == "Blank")
	{
		if(FunctionName() != "Sequence")
			if(pattern->EmptyLeaves() || HasHead(pattern->Leaf(0)))
				callback.Execute(subs, continueMatching);
	}
	else if(patternFunction == "BlankSequence")
	{
		if((FunctionName() != "Sequence") || (leaves.size() > 0))
		{
			bool hasNeededHead(true);
			if(!pattern->EmptyLeaves())
			{
				if(FunctionName() == "Sequence")
				{
					for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
						if(!(*leaf)->HasHead(pattern->Leaf(0)))
						{
							hasNeededHead = false;
							break;
						}
				}
				else
					hasNeededHead = HasHead(pattern->Leaf(0));
			}
			if(hasNeededHead)
				callback.Execute(subs, continueMatching);
		}
	}
	else if(patternFunction == "BlankNullSequence")
	{
		bool hasNeededHead(true);
		if(!pattern->EmptyLeaves())
		{
			if(FunctionName() == "Sequence")
			{
				for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
					if(!(*leaf)->HasHead(pattern->Leaf(0)))
					{
						hasNeededHead = false;
						break;
					}
			}
			else
				hasNeededHead = HasHead(pattern->Leaf(0));
		}
		if(hasNeededHead)
			callback.Execute(subs, continueMatching);
	}
	else if(patternFunction == "Pattern")
	{
		if(pattern->LeafCount() < 2)
			throw ArgumentException("Pattern expects 2 arguments.");
		Expression *name = pattern->Leaf(0);
		Expression *actualPattern = pattern->Leaf(1);
		if(!name->SymbolHead())
			throw ArgumentException("Pattern expects its first argument to be a symbol name.");
		// If current name is already defined, check if same value
		Expression *substitution;
		if(subs && (substitution = subs->GetSubstitution(name->SymbolName())) != 0)
		{
			if(SameExpression(substitution))
				callback.Execute(subs, continueMatching);
		}
		else
		{
			// Match expression against actual pattern
			MatchPatternCallback patternCallback;
			patternCallback.Initialize(this, pattern, calculator, &callback);
			patternCallback.InitializePattern(name->SymbolName(), this);
			return Matches(actualPattern, calculator, subs, fullMatch, patternCallback);
		}
	}
	else
	{
		MatchHeadCallback headCallback;
		headCallback.Initialize(this, pattern, calculator, &callback);
		return head->Matches(pattern->head, calculator, subs, true, headCallback);
	}
	return continueMatching;
}

void Expression::Substitute(Substitutions *subs)
{
	if(!subs)
		return;
	Expression *substitute(subs->GetSubstitution(SymbolName()));
	if(substitute)
		AssignCloned(substitute);
	else
	{
		if(head)
			head->Substitute(subs);
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
			(*leaf)->Substitute(subs);
	}
}

void Expression::SubstituteNames(StringMap const &substitutions)
{
	Symbol *symbolHead = SymbolHead();
	if(symbolHead)
	{
		StringMap::const_iterator found = substitutions.find(symbolHead->Name());
		if(found != substitutions.end())
			symbolHead->Name(found->second);
	}
	else
	{
		if(head)
			head->SubstituteNames(substitutions);
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
			(*leaf)->SubstituteNames(substitutions);
	}
}

void Expression::SubstituteSlots(const ExprVector &slots)
{
	string functionName = FunctionName();
	if(functionName == "Slot")
	{
		IntegerType index;
		if(leaves.empty())
			index = 1;
		else
		{
			//MachineInteger *indexInt = leaves.at(0)->MachineIntegerHead();
			Integer *indexInt(dynamic_cast<Integer*>(leaves.at(0)->NumberHead()));
			if(indexInt)
				index = indexInt->IntValue();
			else
				throw EvaluateException("Slot expected to have an Integer argument.");
		}
		Expression *slot;
		if(index > 0 && index-1 < slots.size())
			slot = slots.at(static_cast<ExprVector::size_type>(index-1));
		else
			throw EvaluateException("Slot not given.");
		AssignCloned(slot);
	}
	else if(functionName == "SlotSequence")
	{
		delete head;
		head = new Expression("Sequence");
		DeleteLeaves();
		leaves.reserve(slots.size());
		for(ExprVector::const_iterator leaf = slots.begin(); leaf != slots.end(); ++leaf)
			AppendLeaf((*leaf)->Clone());
	}
	else if(functionName != "Function")
	{
		if(head)
			head->SubstituteSlots(slots);
		for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
			(*leaf)->SubstituteSlots(slots);
	}
}

bool Expression::Replace(Expression *rule, Calculator *calculator, bool *changed)
{
	if(rule->leaves.size() != 2)
		throw ArgumentException("Replace expects a rule with 2 parts.");
	bool matches(false);
	MatchReplaceCallback replaceCallback;
	replaceCallback.Initialize(this, rule->leaves.at(0), calculator, 0);
	replaceCallback.InitializeReplace(rule->leaves.at(1), changed, &matches);
	if(changed)
		*changed = false;
	Matches(rule->leaves.at(0), calculator, 0, true, replaceCallback);
	return matches;
}

bool Expression::ReplaceAll(const ExprVector &rules, Calculator *calculator, bool *changed)
{
	for(ExprVector::const_iterator rule = rules.begin(); rule != rules.end(); ++rule)
		if(Replace(*rule, calculator, changed))
			return true;
	for(ExprVector::const_iterator leaf = leaves.begin(); leaf != leaves.end(); ++leaf)
		if((*leaf)->ReplaceAll(rules, calculator, changed))
			return true;
	return false;
}

Expression *Expression::Group(PartsIterator part)
{
	return this;
}

ExprPtr Expression::Structure()
{
	return Clone();
}
