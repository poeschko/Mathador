#pragma once

#include "Expression.h"
#include <vector>

class HistoryItem;

typedef std::vector<HistoryItem*> HistoryItemVector;

class HistoryItem
{
private:
	ExprPtr in;
	ExprPtr out;
public:
	HistoryItem() : in(0), out(0)
	{
	}
	HistoryItem(const HistoryItem &instance) : in(instance.in->Clone()), out(instance.out->Clone())
	{
	}
	HistoryItem(ExprPtr itsIn, ExprPtr itsOut) : in(itsIn), out(itsOut)
	{
	}
	~HistoryItem()
	{
	}
	void In(ExprPtr theIn)
	{
		in = theIn;
	}
	void Out(ExprPtr theOut)
	{
		out = theOut;
	}
};

class History
{
private:
	HistoryItemVector items;
public:
	History();
	virtual ~History();
	virtual void Push(ExprPtr inValue, ExprPtr outValue);
};
