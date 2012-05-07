/*
**
** Parser.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Parser.h"
#include "Calculator.h"
#include "Operator.h"
#include "Number.h"
#include "Symbol.h"
#include "List.h"
#include "FunctionOperator.h"
#include "OpsArithmetic.h"
#include "Exceptions.h"

Parser::Parser(): calculator(0)
{
}

Parser::Parser(Calculator *theCalculator): calculator(theCalculator)
{
}

Parser::~Parser()
{
}

// Creates an operator instance, depending on the symbol id and
// the previously read-in element.
OperatorPtr Parser::CreateOperator(string id, Part *prev)
{
	vector<Operator*> ops = factory.CreateInstances(id);
	if(ops.empty())
		return OperatorPtr(0);
	if(ops.size() == 1)
	{
		/*while(ops.size() > 1)
		{
			delete ops.back();
			ops.pop_back();
		}*/
		return OperatorPtr(ops.front());
	}
	Operator *prevOp = dynamic_cast<Operator*>(prev);
	OperatorPtr result;
	while(!ops.empty())
	{
		if(result.get())
		{
			delete ops.back();
			ops.pop_back();
			continue;
		}
		Operator *op = ops.back();
		switch(op->Type())
		{
		case otPrefix:
			if(!prev || (prevOp && (prevOp->Type() != otPostfix)))
				result.reset(op);
			else
				delete op;
			break;
		case otFlatInfix:
		case otLeftInfix:
		case otRightInfix:
		case otPostfix:
			if(prev && (!prevOp || (prevOp && prevOp->Type() == otPostfix)))
				result.reset(op);
			else
				delete op;
			break;
		default:
			delete op;
		}
		ops.pop_back();
	}
	return result;
}

// Parse a given input into an expression.
// (The resulting expression is unstructured, i.e. it has no head and operators are
// represented by instances of Operator, not their corresponding function calls.
// The structure process follows later and depends on the input mode infix <-> postfix.)
CompoundPartPtr Parser::Parse(Input &input)
{
	//Expression *result = new Expression;
	//Expression *object(0);
	CompoundPartPtr result(new CompoundPart);
	//result.reset(new CompoundPart);
	//try
	//{
	while(!input.AtEnd())
	{
		PartPtr part;
		input.SkipBlanks();
		//object = 0;
		Part *prev;
		if(result->HasSubParts())
			prev = result->LastSubPart();
		else
			prev = 0;
		char c = input.Current();
		// Letter or blank symbol (_)
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
		{
			string id = input.GetIdentifier();
			// Try to create a corresponding operator. (It's not very likely that this happens
			// because most operators are given by non-alphanumeric characters.)
			part = CreateOperator(id, prev);
			// Otherwise create a symbol.
			if(!part.get())
			{
				//if(id != "")
				//	part.reset(new Expression(id));
				// Create Blank / BlankSequence / BlankNullSequence
				Expression *blank(0);
				if(input.Current() == '_')
				{
					input.Next();
					if(input.Current() == '_')
					{
						input.Next();
						if(input.Current() == '_')
						{
							input.Next();
							blank = new Expression("BlankNullSequence", 0);
						}
						else
							blank = new Expression("BlankSequence", 0);
					}
					else
						blank = new Expression("Blank", 0);
				}
				if(blank)
				{
					// Look for following head ("type") specifier
					string typeId = input.GetIdentifier();
					if(typeId != "")
					{
						Expression *headSpecifier = new Expression(typeId);
						blank->AppendLeaf(headSpecifier);
					}
					// If a leading symbol was specified, construct a corresponding pattern.
					// Otherwise return the blank.
					if(id != "")
					{
						/*Expression *pattern = new Expression("Pattern", 2);
						pattern->AppendLeaf(part->Structure(deleteIt));
						pattern->AppendLeaf(blank);
						object = pattern;*/
						/*FunctionPartPtr pattern(new FunctionPart("Pattern"));
						pattern->AppendLeaf(part.release());
						pattern->AppendLeaf(blank);
						part = pattern;*/
						SequencePartPtr leaves(new SequencePart);
						leaves->AppendLeaf(new Expression(id));
						leaves->AppendLeaf(blank);
						part = PartPtr(new CompoundPart(new Expression("Pattern"),
							leaves.release()));
					}
					else
						part = PartPtr(blank);
						//part.reset(blank);
				}
				else
					part = PartPtr(new Expression(id));
			}
		}
		// Number
		else if(c >= '0' && c <= '9' || (c == '.' && input.NextChar() >= '0' && input.NextChar() <= '9'))
		{
			//object = new Expression;
			//object->Head(Number::Read(input));
			//Expression *number = new Expression;
			//number->Head(Number::Read(input));
			//part.reset(new Expression);
			//part.reset(number);
			ExprPtr number(new Expression);
			//input.Next();
			number->Head(Number::Read(input).release());
			part = number;
			//part.reset(new Expression(14));
			//part.reset(new Expression);
		}
		// List
		else if(c == '{')
		{
			//object = new Expression("List", 0);
			//FunctionPartPtr list(new FunctionPart("List"));
			SequencePartPtr items(new SequencePart);
			//list->Head(new Expression("List"));
			while(true)
			{
				input.Next();
				if(input.Current() == '}')
					break;
				//Expression *leaf = Parse(input);
				CompoundPartPtr leaf = Parse(input);
				items->AppendLeaf(leaf.release());
				//object->AppendLeaf(leaf);
				input.SkipBlanks();
				if(input.Current() == '}')
					break;
				else if(input.Current() != ',')
					throw ParseException("List item delimiter expected.");
			}
			part = PartPtr(new CompoundPart(new Expression("List"), items.release()));
			input.Next();
		}
		// Parantheses
		else if(c == '(')
		{
			input.Next();
			part = Parse(input);
			//object = Parse(input);
			if(input.Current() != ')')
				throw ParseException("Closing bracket expected.");
			input.Next();
		}
		// Argument list
		else if(c == '[')
		{
			if(!prev)
				throw ParseException("Unexpected argument list.");
			if(prev->AsOperator())
				throw ParseException("Operator must not be followed by argument list.");
			//object = new Expression;
			//FunctionPartPtr function(new FunctionPart);
			// Take previous object in expression as function head
			//object->Head(prev);
			//result->PopLastLeaf();
			result->AppendSubPart(new FunctionOperator);
			SequencePartPtr arguments(new SequencePart);
			//function->Head(prev);
			//result->PopLastSubPart();
			// Read arguments
			while(true)
			{
				input.Next();
				if(input.Current() == ']')
					break;
				CompoundPartPtr leaf = Parse(input);
				//object->AppendLeaf(leaf);
				//function->AppendLeaf(leaf.release());
				arguments->AppendLeaf(leaf.release());
				input.SkipBlanks();
				if(input.Current() == ']')
					break;
				else if(input.Current() != ',')
					throw ParseException("List item delimiter expected.");
			}
			//part = function;
			//part = PartPtr(new CompoundPart(
			part = arguments;
			input.Next();
		}
		// Other character (probably operator symbol or slot)
		else
		{
			string id = input.GetOperator();
			// Subsequently delete chars on the right when no corresponding operator found
			while(id != "")
			{
				// Check for slot specification.
				// Otherwise try to create operator
				if(id == "##")
				{
					//object = new Expression("SlotSequence", 0);
					//part.reset(new FunctionPart("SlotSequence"));
					part.reset(new CompoundPart(new Expression("SlotSequence"), new SequencePart));
					//slot->Head(new Expression("SlotSequence"));
					break;
				}
				else if(id == "#")
				{
					//FunctionPartPtr slot(new FunctionPart("Slot"));
					//object = new Expression("Slot", 0);
					SequencePartPtr slotArg(new SequencePart);
					if(input.Current() >= '0' && input.Current() <= '9')
					{
						IntegerType index = Number::ReadInteger(input);
						Expression *indexExpr = new Expression(index);
						//object->AppendLeaf(indexExpr);
						slotArg->AppendLeaf(indexExpr);
					}
					part = PartPtr(new CompoundPart(new Expression("Slot"), slotArg.release()));
					break;
				}
				else
				{
					part = CreateOperator(id, prev);
					if(part.get())
						break;
					id.erase(id.size()-1);
					input.Back();
				}
			}
		}
		if(!part.get())
			break;
		// Insert multiplication between unlinked operands
		//if(calculator && calculator->Mode() == imInfix)
		//{
			if(result->HasSubParts())
				prev = result->LastSubPart();
			else
				prev = 0;
			//Operator *prevOp = dynamic_cast<Operator*>(prev);
			//Operator *curOp = dynamic_cast<Operator*>(part.get())
			Operator *prevOp = prev ? prev->AsOperator() : 0;
			Operator *curOp = part->AsOperator();
			// Insert Times if...
			// * both operands are no operators
			// * both operands are operators, and previous is postfix and current is prefix
			// * previous operator is postfix and current is no operator
			// * previous is no operator and current operator is prefix
			if((prev && !prevOp && !curOp) ||
				(prevOp && curOp && prevOp->Type() == otPostfix && curOp->Type() == otPrefix) ||
				(prevOp && prevOp->Type() == otPostfix && !curOp) ||
				(prev && !prevOp && curOp && curOp->Type() == otPrefix))
			{
				Operator *times = new Times;
				result->AppendSubPart(times);
			}
		//}
		// Append the object to resulting expression
		result->AppendSubPart(part.release());
	}
	/*}
	catch(...)
	{
		//delete object;
		//delete result;
		throw;
	}*/
	return result;
}
