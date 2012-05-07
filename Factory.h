/*
**
** Factory.h
** Copyright by Jan Poeschko
**
*/

#pragma once
#include <map>
#include <vector>

template <class BaseType, typename KeyType>
class Factory
{
private:
	typedef BaseType* (*CreatorFunc)();
	typedef std::multimap<KeyType, CreatorFunc> CreatorMap;
	typedef typename CreatorMap::iterator CreatorMapItr;
	typedef typename CreatorMap::const_iterator CreatorMapConstItr;
	template <class DerivedType> static BaseType* CreateInstance()
	{
		return dynamic_cast<BaseType*>(new DerivedType);
	}
	CreatorMap creatorMap;
protected:
	template <class DerivedType> void RegisterType(KeyType const& key)
	{
		creatorMap.insert(std::make_pair<KeyType, CreatorFunc>(key, &CreateInstance<DerivedType>));
	}
	void UnregisterType(KeyType const& key) 
	{
		creatorMap.erase(key);
	}
	bool IsTypeRegistered(KeyType const& key) const
	{
		return creatorMap.find(key) != creatorMap.end();
	}
	std::vector<KeyType> GetKeys() const
	{
		std::vector<KeyType> keys;
		keys.reserve(creatorMap.size());
		
		for(CreatorMapConstItr it = creatorMap.begin(); it != creatorMap.end(); ++it)
		{
			keys.push_back(it->first);  
		}
		return keys;
	}
public:
	virtual std::vector<BaseType*> CreateInstances(KeyType const &key) const
	{
		std::pair<CreatorMapConstItr, CreatorMapConstItr> found;
		found = creatorMap.equal_range(key);
		std::vector<BaseType*> result;
		for(CreatorMapConstItr it = found.first; it != found.second; ++it)
			result.push_back((it->second)());
		return result;
	}
};
