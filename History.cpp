#include "StdAfx.h"
#include "History.h"

History::History()
{
}

History::~History()
{
	for(HistoryItemVector::const_iterator item = items.begin(); item != items.end(); ++item)
		delete *item;
}

void History::Push(ExprPtr inValue, ExprPtr outValue)
{
	//in.push_back(inValue);
	//out.push_back(outValue);
	//items.push_back(HistoryItem(inValue, outValue));
	items.push_back(new HistoryItem(inValue, outValue));
}
