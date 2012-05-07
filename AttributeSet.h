/*
**
** AttributeSet.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <set>
#include <string>
using std::set;
using std::string;

enum Attribute {
	atProtected, atLocked, atReadProtected,
	atFlat, atOneIdentity, atOrderless,
	atListable,
	atNumericFunction,
	atHoldFirst, atHoldRest, atHoldAll, atHoldAllComplete,
	atSequenceHold,
	atTemporary
};

class AttributeSet
{
private:
	set<Attribute> attributes;
public:
	AttributeSet();
	AttributeSet(Attribute single)
	{
		attributes.insert(single);
	}
	AttributeSet(Attribute first, Attribute second)
	{
		attributes.insert(first);
		attributes.insert(second);
	}
	AttributeSet(Attribute first, Attribute second, Attribute third)
	{
		attributes.insert(first);
		attributes.insert(second);
		attributes.insert(third);
	}
	AttributeSet(Attribute first, Attribute second, Attribute third, Attribute fourth)
	{
		attributes.insert(first);
		attributes.insert(second);
		attributes.insert(third);
		attributes.insert(fourth);
	}
	AttributeSet(Attribute first, Attribute second, Attribute third, Attribute fourth,
		Attribute fifth)
	{
		attributes.insert(first);
		attributes.insert(second);
		attributes.insert(third);
		attributes.insert(fourth);
		attributes.insert(fifth);
	}
	AttributeSet(Attribute first, Attribute second, Attribute third, Attribute fourth,
		Attribute fifth, Attribute sixth)
	{
		attributes.insert(first);
		attributes.insert(second);
		attributes.insert(third);
		attributes.insert(fourth);
		attributes.insert(fifth);
		attributes.insert(sixth);
	}
	virtual ~AttributeSet();
	virtual void Include(Attribute attribute)
	{
		attributes.insert(attribute);
	}
	virtual void Include(string attributeName);
	virtual void Exclude(Attribute attribute)
	{
		attributes.erase(attribute);
	}
	virtual void Exclude(string attributeName);
	virtual bool Contains(Attribute attribute)
	{
		return attributes.find(attribute) != attributes.end();
	}
	virtual set<string> AsStringSet();
};
