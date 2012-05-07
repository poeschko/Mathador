#pragma once
#include "Expression.h"
#include <string>
#include <map>
using std::string;
using std::map;

typedef map<string, Expression*> SubsMap;

class Substitutions
{
private:
	SubsMap subs;
public:
	Substitutions();
	virtual ~Substitutions();
	virtual Expression *GetSubstitution(const string &name) const;
	virtual void SetSubstitution(const string &name, Expression *value);
	virtual void Insert(Substitutions *insert);
};
