#pragma once

template <class Container>
class ExtendedIterator : private Container::iterator
{
private:
	Container &container;
	bool beforeFirst;
public:
	ExtendedIterator(typename Container::iterator value, Container &itsContainer) :
	  Container::iterator(value), container(itsContainer), beforeFirst(false)
	{
	}

	~ExtendedIterator()
	{
	}	

	bool IsFirst()
	{
		return *this == container.begin();
	}

	bool IsLast()
	{
		typename Container::iterator itr(*this);
		return (++itr) == container.end();
	}

	bool IsEnd()
	{
		return *this == container.end();
	}

	bool IsBegin()
	{
		return beforeFirst;
	}

	ExtendedIterator Prev()
	{
		typename Container::iterator itr(*this);
		return ExtendedIterator(--itr, container);
	}

	ExtendedIterator Next()
	{
		typename Container::iterator itr(*this);
		return ExtendedIterator(++itr, container);
	}

	void Erase()
	{
		container.erase(*this);
	}

	typename Container::iterator::value_type &operator *()
	{
		return Container::iterator::operator *();
	}

	ExtendedIterator<Container> operator ++()		// Prefix increment
	{
		Container::iterator::operator++();
		return *this;
	}

	ExtendedIterator<Container> operator ++(int n)	// Postfix increment
	{
		ExtendedIterator<Container> temp(*this);
		operator ++();
		return temp;
	}

	ExtendedIterator<Container> operator --()		// Prefix decrement
	{
		if(*this == container.begin())
			beforeFirst = true;
		else
			Container::iterator::operator --();
		return *this;
	}

	ExtendedIterator<Container> operator --(int n)	// Postfix decrement
	{
		ExtendedIterator<Container> temp(*this);
		operator --();
		return temp;
	}
};
