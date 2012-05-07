#include "StdAfx.h"
#include "Substitutions.h"

Substitutions::Substitutions()
{
}

Substitutions::~Substitutions()
{
	for(SubsMap::const_iterator sub = subs.begin(); sub != subs.end(); ++sub)
		delete sub->second;
}


Expression *Substitutions::GetSubstitution(const string &name) const
{
	SubsMap::const_iterator found = subs.find(name);
	if(found == subs.end())
		return 0;
	else
		return found->second;
}

void Substitutions::SetSubstitution(const string &name, Expression *value)
{
	subs.insert(std::make_pair(name, value->Clone().release()));
}

void Substitutions::Insert(Substitutions *insert)
{
	if(insert)
		for(SubsMap::const_iterator sub = insert->subs.begin(); sub != insert->subs.end(); ++sub)
			subs.insert(std::make_pair(sub->first, sub->second->Clone().release()));
}
