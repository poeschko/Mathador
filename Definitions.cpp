/*
**
** Definitions.cpp
** Copyright by Jan Poeschko
**
*/

#include "StdAfx.h"
#include "Definitions.h"
#include "Exceptions.h"
#include <iostream>

using std::cout;
using std::endl;

// Static member initialization
int32 Definitions::instanceCount(0);

Definitions::Definitions(): calculator(0)
{
	++instanceCount;
}

Definitions::Definitions(Definitions &definitions)
{
	++instanceCount;
	calculator = definitions.calculator;
	// Clone definitions
	InsertDefinitions(&definitions);
}

Definitions::~Definitions()
{
	for(DefMap::const_iterator def = defs.begin(); def != defs.end(); ++def)
		delete def->second;
	--instanceCount;
}

bool Definitions::HasDefinition(string id) const
{
	DefMap::const_iterator found = defs.find(id);
	return found != defs.end();
}

Definition *Definitions::GetDefinition(string id)
{
	DefMap::const_iterator found = defs.find(id);
	if(found == defs.end())
	{
		Definition *def = new Definition;
		defs.insert(std::make_pair<string, Definition*>(id, def));
		return def;
	}
	else
		return found->second;
}

void Definitions::InsertDefinitions(Definitions *definitions)
{
	if(definitions)
		for(DefMap::const_iterator def = definitions->defs.begin(); def != definitions->defs.end(); ++def)
		{
			string key = def->first;
			// Check if definition already exists. If so, it has to have the same value!
			// (Unequal definitions should be cared about in the Matches method.)
			DefMap::const_iterator found = defs.find(key);
			if(found == defs.end())
			{
				Definition *definition = (*def).second->Clone().release();
				defs.insert(std::make_pair<string, Definition*>(key, definition));
			}
			else
				if(!found->second->Value()->SameExpression(def->second->Value()))
					throw DefinitionException("Unconsistend definition.");
		}
}

void Definitions::Print(bool userOnly)
{
	bool printed(false);
	for(DefMap::const_iterator def = defs.begin(); def != defs.end(); ++def)
		if(!userOnly || !def->second->Predef())
		{
			def->second->Print(def->first, calculator);
			printed = true;
		}
	if(!printed)
		cout << "No definitions." << endl;
}
