/*
**
** AttributeSet.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "AttributeSet.h"
#include "Exceptions.h"

AttributeSet::AttributeSet()
{
}

AttributeSet::~AttributeSet()
{
}

set<string> AttributeSet::AsStringSet()
{
	set<string> result;
	for(set<Attribute>::const_iterator item = attributes.begin(); item != attributes.end(); ++item)
		switch(*item)
		{
		case atProtected:		result.insert("Protected"); break;
		case atLocked:			result.insert("Locked"); break;
		case atReadProtected:	result.insert("ReadProtected"); break;
		case atFlat:			result.insert("Flat"); break;
		case atOneIdentity:		result.insert("OneIdentity"); break;
		case atOrderless:		result.insert("Orderless"); break;
		case atListable:		result.insert("Listable"); break;
		case atNumericFunction:	result.insert("NumericFunction"); break;
		case atHoldFirst:		result.insert("HoldFirst"); break;
		case atHoldRest:		result.insert("HoldRest"); break;
		case atHoldAll:			result.insert("HoldAll"); break;
		case atHoldAllComplete:	result.insert("HoldAllComplete"); break;
		case atSequenceHold:	result.insert("SequenceHold"); break;
		case atTemporary:		result.insert("Temporary"); break;
		default:
			throw UnknownClassException("AttributeSet::AsStringSet: Unknown Attribute."); break;
		}
	return result;
}

void AttributeSet::Include(string attributeName)
{
	Attribute attribute;
	if(attributeName == "Protected")
		attribute = atProtected;
	else if(attributeName == "Locked")
		attribute = atLocked;
	else if(attributeName == "ReadProtected")
		attribute = atReadProtected;
	else if(attributeName == "Flat")
		attribute = atFlat;
	else if(attributeName == "OneIdentity")
		attribute = atOneIdentity;
	else if(attributeName == "Orderless")
		attribute = atOrderless;
	else if(attributeName == "Listable")
		attribute = atListable;
	else if(attributeName == "NumericFunction")
		attribute = atNumericFunction;
	else if(attributeName == "HoldFirst")
		attribute = atHoldFirst;
	else if(attributeName == "HoldRest")
		attribute = atHoldRest;
	else if(attributeName == "HoldAll")
		attribute = atHoldAll;
	else if(attributeName == "HoldAllComplete")
		attribute = atHoldAllComplete;
	else if(attributeName == "SequenceHold")
		attribute = atSequenceHold;
	else if(attributeName == "Temporary")
		attribute = atTemporary;
	else
		throw EvaluateException("Unknown Attribute.");
	Include(attribute);
}

void AttributeSet::Exclude(string attributeName)
{
	Attribute attribute;
	if(attributeName == "Protected")
		attribute = atProtected;
	else if(attributeName == "Locked")
		attribute = atLocked;
	else if(attributeName == "ReadProtected")
		attribute = atReadProtected;
	else if(attributeName == "Flat")
		attribute = atFlat;
	else if(attributeName == "OneIdentity")
		attribute = atOneIdentity;
	else if(attributeName == "Orderless")
		attribute = atOrderless;
	else if(attributeName == "Listable")
		attribute = atListable;
	else if(attributeName == "NumericFunction")
		attribute = atNumericFunction;
	else if(attributeName == "HoldFirst")
		attribute = atHoldFirst;
	else if(attributeName == "HoldRest")
		attribute = atHoldRest;
	else if(attributeName == "HoldAll")
		attribute = atHoldAll;
	else if(attributeName == "HoldAllComplete")
		attribute = atHoldAllComplete;
	else if(attributeName == "SequenceHold")
		attribute = atSequenceHold;
	else if(attributeName == "Temporary")
		attribute = atTemporary;
	else
		throw EvaluateException("Unknown Attribute.");
	Exclude(attribute);
}
