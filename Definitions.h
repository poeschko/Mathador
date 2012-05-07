/*
**
** Definitions.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include "Definition.h"
#include <map>
using std::map;

typedef map<string, Definition*> DefMap;

class Definitions
{
private:
	DefMap defs;
	Calculator *calculator;
public:
	static int32 instanceCount;
	Definitions();
	Definitions(Definitions &definitions);
	virtual ~Definitions();
	virtual void SetCalculator(Calculator *theCalculator)
	{
		calculator = theCalculator;
	}
	virtual bool HasDefinition(string id) const;
	virtual Definition *GetDefinition(string id);
	virtual void InsertDefinitions(Definitions *definitions);
	virtual void Print(bool userOnly);
	template <class PredefinedClass> void Predefine()
	{
		PredefinedClass *op = new PredefinedClass;
		Definition *def = GetDefinition(op->Name());
		op->Define(def);
	}
};
