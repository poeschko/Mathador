/*
**
** SubsetGenerator.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <vector>
#include <set>

template <class Type>
class SubsetCallback
{
public:
	virtual void Execute(std::set<typename std::vector<Type>::iterator> selection,
		bool &continueGenerating) = 0;
};

template <class Type>
class SubsetGenerator
{
public:
	typedef typename std::vector<Type>::iterator Position;
	typedef std::set<Position> PositionSet;
	virtual bool GenerateSubsets(SubsetCallback<Type> *callback) = 0;
	virtual bool AllSelected() = 0;
	virtual PositionSet Selection() = 0;
};

template <class Type>
class OrderlessSubsetGenerator : public SubsetGenerator<Type>
{
private:
	typename SubsetGenerator<Type>::PositionSet alreadySelected;
	std::vector<Type> &list;
	bool GenerateOrderlessSubsets(typename SubsetGenerator<Type>::PositionSet &selection,
        typename SubsetGenerator<Type>::Position current,
		SubsetCallback<Type> *callback)
	{
		if(current == list.end())
		{
			bool continueGenerating(true);
			callback->Execute(selection, continueGenerating);
			if(!continueGenerating)
				return false;
		}
		else
		{
			if(!GenerateOrderlessSubsets(selection, current + 1, callback))
				return false;
			if(alreadySelected.find(current) == alreadySelected.end())
			{
				alreadySelected.insert(current);
				selection.insert(current);
				bool continueGenerating = GenerateOrderlessSubsets(selection, current + 1, callback);
				if(!continueGenerating)
				{
					return false;
				}
				selection.erase(current);
				alreadySelected.erase(current);
			}
		}
		return true;
	}
public:
	OrderlessSubsetGenerator(std::vector<Type> &itsList) :
		list(itsList)
	{
	}
	virtual ~OrderlessSubsetGenerator()
	{
		alreadySelected.clear();
	}
	virtual bool GenerateSubsets(SubsetCallback<Type> *callback)
	{
		typename SubsetGenerator<Type>::PositionSet positions;
		return GenerateOrderlessSubsets(positions, list.begin(), callback);
	}
	virtual bool AllSelected()
	{
		return alreadySelected.size() == list.size();
	}
	virtual typename SubsetGenerator<Type>::PositionSet Selection()
	{
		return alreadySelected;
	}
};

template <class Type>
class OrderedSubsetGenerator : public SubsetGenerator<Type>
{
private:	
	typename std::vector<Type>::size_type notSelectedStart;	// cannot use iterator - error when deleting corresponding list
	std::vector<Type> *list;
	typename SubsetGenerator<Type>::PositionSet positions;	// not local in GenerateSubsets(), so it can be accessed by Selection()
public:
	OrderedSubsetGenerator(std::vector<Type> *itsList) :
		list(itsList), notSelectedStart(0)
	{
	}
	virtual ~OrderedSubsetGenerator()
	{
	}
	virtual bool GenerateSubsets(SubsetCallback<Type> *callback)
	{	
		positions.clear();
		bool continueGenerating(true);
		callback->Execute(positions, continueGenerating);
		if(!continueGenerating)
			return false;
		for(typename SubsetGenerator<Type>::Position itr = list->begin() + notSelectedStart; itr != list->end(); ++itr)
		{
			positions.insert(itr);
			notSelectedStart = itr - list->begin() + 1;
			callback->Execute(positions, continueGenerating);
			if(!continueGenerating)
				return false;
		}
		return true;
	}
	virtual bool AllSelected()
	{
		return notSelectedStart == list->size();
	}
	virtual typename SubsetGenerator<Type>::PositionSet Selection()
	{
		return positions;
	}
};
