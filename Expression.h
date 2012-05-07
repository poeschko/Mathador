/*
**
** Expression.h
** Copyright by Jan Poeschko
**
*/

#pragma once

#include "Part.h"
#include "Typedefs.h"
#include "Input.h"
#include "SubsetGenerator.h"
#include "Predefined.h"
#include <map>
#include <vector>
#include <set>

using std::vector;
using std::map;

// Forward declarations
class Expression;
class Parser;
class Stack;
class Calculator;
class Definition;
class Definitions;
class Substitutions;
class Operator;
class Atom;
class Number;
class Integer;
class MachineInteger;
class Real;
class Symbol;

// Container definitions
typedef vector<Expression*> ExprVector;
typedef vector<Definitions*> DefsVector;
typedef map<string, string> StringMap;
typedef std::auto_ptr<Number> NumberPtr;

// Typedefs
typedef int64 IntegerType;
typedef long double RealType;

typedef SubsetGenerator<Expression*> ExprSubsetGenerator;

// Enum
enum CompareResult {crLess = -1, crEqual = 0, crGreater = 1};

class MatchCallback
{
public:
	virtual void Execute(Substitutions *subs, bool &continueMatching) = 0;
	virtual void ExecutePartial(Substitutions *subs, bool &continueMatching,
		ExprSubsetGenerator::PositionSet selection) = 0;
};

class Expression : public StandalonePart
{
private:
	// Member variables
	Expression *head;
	ExprVector leaves;
protected:
	virtual Expression *DoClone();
public:
	// Debug information
	int32 id;
	static int32 instanceCount;
	static int32 currentId;
	static std::map<int32, Expression*> instances;
	virtual void CreateDebugInfo();

	// Constructors, destructor
	Expression();
	Expression(string symbolName);
	Expression(string functionName, ExprVector::size_type desiredLeafCount);
	Expression(IntegerType integerValue);
	Expression(NumberPtr number);
	virtual ~Expression();

	// Copying
	virtual ExprPtr Clone();	// not virtual to prohibit overriding. DoClone() must be overridden
	virtual void AssignCloned(Expression *expression);
	//virtual void MoveNotCloned(Expression &expression);
	virtual void MoveNotCloned(ExprPtr source);
	virtual void MoveLeaves(Expression *source);
	virtual void AssignLeaf(ExprVector::size_type position);

	// Structure, evaluation
	virtual void Evaluate(Calculator *calculator, int32 recursions);
	virtual void ApplyN(Calculator *calculator);
	virtual void ApplyNIfContainsReal(Calculator *calculator);
	virtual void Print(Calculator *calculator, PrintMode mode);
	virtual OperatorPrecedence Precedence();
	virtual ExprPtr Structure();
	virtual Expression *Group(PartsIterator part);

	// Function & symbol definition
	virtual Definition *FunctionDefinition(Calculator *calculator);
	virtual Definition *FrontFunctionDefinition(Calculator *calculator);
	virtual string FunctionName();
	virtual bool IsFunctionCall();
	virtual Definition *SymbolDefinition(Calculator *calculator);
	virtual string SymbolName();
	virtual Predefined *PredefFunc(Calculator *calculator);
	virtual Operator *PredefOperator(Calculator *calculator);

	// Leaf operations
	virtual void AppendLeaf(ExprPtr leaf);
	virtual void AppendLeaf(Expression *leaf);
	virtual void PopLastLeaf();
	virtual void DeleteFirstLeaf();
	virtual void ReserveLeaves(ExprVector::size_type size);
	virtual bool HasLeaves();
	virtual Expression *LastLeaf();
	virtual Expression *Leaf(ExprVector::size_type position);
	virtual ExprVector &Leaves();
	virtual bool EmptyLeaves();
	virtual ExprVector::size_type LeafCount();
	virtual void DeleteLeaves();
	virtual void DeleteLeaf(ExprVector::iterator &leaf);
	virtual void InsertLeaf(ExprVector::iterator position, Expression *leaf);

	// Head operations
	virtual Expression *Head();
	virtual void Head(Expression *newHead);
	virtual void Head(ExprPtr newHead);
	virtual bool HasHead(Expression *theHead);
	virtual void ReplaceHead(ExprPtr newHead);

	// Atom conversion
	virtual bool IsAtom();
	virtual Atom *AtomHead();
	virtual Number *NumberHead();
	//virtual Integer *IntegerHead();
	//virtual MachineInteger *MachineIntegerHead();
	//virtual Real *RealHead();
	virtual Symbol *SymbolHead();

	// Attribute application
	virtual bool ApplyOneIdentity();
	virtual void Flatten(const string head);

	// Sorting
	virtual string OrderKey();
	static int32 CompareEqual(Expression *expression1, Expression *expression2);
	static bool Compare(Expression *expression1, Expression *expression2);

	// Pattern matching
	virtual bool SameExpression(Expression *expression);
	virtual bool SamePattern(Expression *expression, StringMap &aliases);
	virtual bool SamePattern(Expression *expression);

	virtual void Substitute(Substitutions *subs);
	virtual bool Matches(Expression *pattern, Calculator *calculator);
	virtual bool Matches(Expression *pattern, Calculator *calculator,
		Substitutions *subs, bool fullMatch,
		MatchCallback &callback);
	virtual bool Replace(Expression *rule, Calculator *calculator, bool *changed = 0);
	virtual bool ReplaceAll(const ExprVector &rules, Calculator *calculator, bool *changed = 0);

	// Other
	virtual void SubstituteNames(const StringMap &substitutions);
	virtual void SubstituteSlots(const ExprVector &slots);
};
