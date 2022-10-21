#ifndef DEQUE_H
#define DEQUE_H

// DequeIterator class template definition
template< typename T >
class DequeIterator
{
	template< typename T > friend class deque;
public:
	DequeIterator() // default constructor
	{
	}

	DequeIterator(deque< T > *container, unsigned int offset)
		: myCont(container),
		myOff(offset)
	{
	}

	DequeIterator(const DequeIterator &iteratorToCopy) // copy constructor
		: myCont(iteratorToCopy.myCont),
		myOff(iteratorToCopy.myOff)
	{
	}

	~DequeIterator() // destructor
	{
	}

	// const return avoids: ( a1 = a2 ) = a3
	const DequeIterator &operator=(const DequeIterator &right)
	{
		if (&right != this) // avoid self-assignment
		{
			myCont = right.myCont;
			myOff = right.myOff;
		}
		return *this; // enables x = y = z, for example
	}

	T& operator*() const // dereferencing operator
	{
		unsigned int block = (myOff % (4 * myCont->mapSize)) / 4;
		unsigned int off = myOff % 4;
		return (myCont->map)[block][off];
	}

	DequeIterator& operator++() // prefix increment operator
	{
		++myOff;
		return *this;
	}

	bool operator==(const DequeIterator &right) const // equal to
	{
		return (*myCont == *(right.myCont)) && (myOff == right.myOff);
	}

	bool operator!=(const DequeIterator &right) const // not equal to
	{
		return (*myCont != *(right.myCont)) || (myOff != right.myOff);
	}

	bool operator<(const DequeIterator &right) const // less than equal to
	{
		return (*myCont == *(right.myCont)) && (myOff < right.myOff);
	}

	bool operator<=(const DequeIterator &right) const // less than equal to
	{
		return (*myCont == *(right.myCont)) && (myOff <= right.myOff);
	}

	DequeIterator operator+(unsigned int i)
	{
		return DequeIterator(myCont, myOff + i);
	}

	DequeIterator operator-(unsigned int i)
	{
		return DequeIterator(myCont, myOff - i);
	}

private:
	deque< T > *myCont = nullptr; // keep a pointer to deque
	unsigned int myOff = 0;       // offset of element in deque
}; // end class template DequeIterator


// deque class template definition
template< typename T >
class deque
{
	template< typename T > friend class DequeIterator;

	template< typename T >
	friend bool operator==(const deque< T > &lhs, const deque< T > &rhs);

	template< typename T >
	friend bool operator!=(const deque< T > &lhs, const deque< T > &rhs);

public:
	using iterator = DequeIterator< T >;

	deque(); // Constructs an empty deque container, with no elements.

	// Constructs a deque container with n elements. Each element is a copy of val.
	deque(unsigned int n, const T &val);

	~deque(); // Destroys the deque container.

	// Returns an iterator pointing to the first element in the deque container.
	// If the deque is empty, the returned iterator shall not be dereferenced.
	iterator begin();

	// Returns an iterator referring to the past-the-end element in the deque container.
	// The past-the-end element is the theoretical element
	// that would follow the last element in the deque container.
	// It does not point to any element, and thus shall not be dereferenced.
	iterator end();

	// Returns the number of elements in the deque container.
	unsigned int size() const;

	// The deque container is extended by inserting a new element
	// before the element at the specified position.
	// This effectively increases the container size by one.
	void insert(const iterator position, const T &val);

	// Removes from the deque container a single element at the specified position.
	// This effectively reduces the container size by one.
	void erase(const iterator position);

private:
	T **map = nullptr;         // pointer to array of pointers to blocks
	unsigned int mapSize = 0;  // size of map array, zero or 2^N
	unsigned int myOff = 0;    // offset of initial element
	unsigned int mySize = 0;   // current length of sequence, in [ 0 .. 4 * mapSize )

	void doubleMapSize();      // double the size of map array without changing myOff and mySize
}; // end class template deque


// Constructs an empty deque container, with no elements.
template< typename T >
deque< T >::deque()
	: map(nullptr),
	mapSize(0),
	myOff(0),
	mySize(0)
{
} // end deque default constructor

// Constructs a deque container with n elements. Each element is a copy of val.
template< typename T >
deque< T >::deque(unsigned int n, const T &val)
	: myOff(0),
	mySize(n)
{
	if (n == 0)
	{
		map = nullptr;
		mapSize = 0;
	}
	else
	{
		mapSize = 8;
		while (n > 4 * (mapSize - 1))
			mapSize *= 2;

		map = new T*[mapSize]();
		for (unsigned int i = 0; i <= (n - 1) / 4; i++)
			map[i] = new T[4];

		for (unsigned int i = 0; i < n; i++)
			map[i / 4][i % 4] = val;
	}
} // end deque constructor

// destroys the deque container
template< typename T >
deque< T >::~deque()
{
	if (mapSize > 0)
	{
		for (unsigned int i = 0; i < mapSize; i++)
			if (map[i] != nullptr)
				delete[] map[i];
		delete[] map;
	}
} // end destructor

template< typename T >
typename deque< T >::iterator deque< T >::begin()
{
	return iterator(this, myOff);
}

template< typename T >
typename deque< T >::iterator deque< T >::end()
{
	return iterator(this, myOff + mySize);
}

template< typename T >
unsigned int deque< T >::size() const
{
	return mySize;
}

template< typename T >
void deque< T >::insert(const iterator position, const T &val)
{
	if (map == nullptr)
	{
		mapSize = 8;
		map = new T*[mapSize]();
	}
	if (position.myOff <= myOff + mySize / 2)
	{
		if (begin().myOff % 4 == 0 && mapSize <= mySize / 4 + 1)
			doubleMapSize();
		unsigned int number = (position.myOff - myOff + 4 * mapSize) % (4 * mapSize);
		myOff = (myOff + 4 * mapSize - 1) % (4 * mapSize);
		if (map[myOff / 4] == nullptr)
			map[myOff / 4] = new T[4]();
		iterator it1 = begin(), it2 = it1 + 1;
		for (unsigned int i=0;i<number;i++,++it1,++it2)
			*it1 = *it2;
		*it1 = val;
		mySize++;
	}
	else
	{
		if (end().myOff % 4 == 0 && mapSize <= mySize / 4 + 1)
			doubleMapSize();
		unsigned int number = (end().myOff + 4 * mapSize - position.myOff) % (4 * mapSize);
		if (map[((end().myOff + 1) % (4 * mapSize)) / 4] == nullptr)
			map[((end().myOff + 1) % (4 * mapSize)) / 4] = new T[4]();
		iterator it1 = end(), it2 = it1;
		if (it2.myOff == 0)
			it2.myOff += 4 * mapSize;
		it2 = it2 - 1;
		for (unsigned int i = 0; i < number; i++, it1 = it1 - 1, it2 = it2 - 1)
		{
			if (it1.myOff == 0)
				it1.myOff += 4 * mapSize;
			if (it2.myOff == 0)
				it2.myOff += 4 * mapSize;
			*it1 = *it2;
		}
		*it1 = val;
		mySize++;
	}
	myOff %= 4 * mapSize;
}

template< typename T >
void deque< T >::erase(const iterator position)
{
	if (position.myOff < myOff + mySize / 2)
	{
		unsigned int number = (4 * mapSize + position.myOff - begin().myOff) % (4 * mapSize);
		iterator it1 = position, it2 = it1;
		if (it2.myOff == 0)
			it2.myOff += 4 * mapSize;
		it2 = it2 - 1;
		for (unsigned int i = 0; i < number; i++, it1 = it1 - 1, it2 = it2 - 1)
		{
			if (it1.myOff == 0)
				it1.myOff += 4 * mapSize;
			if (it2.myOff == 0)
				it2.myOff += 4 * mapSize;
			*it1 = *it2;
		}
		myOff++;
		//if(myOff>4*mapSize)
		//myOff -= (4 * mapSize);
		mySize--;
	}
	else
	{
		unsigned int number = (end().myOff - position.myOff + 4 * mapSize - 1) % (4 * mapSize);
		iterator it1 = position, it2 = it1 + 1;
		for (unsigned int i = 0; i < number; i++, ++it1, ++it2)
			*it1 = *it2;
		mySize--;
	}
	if (mySize == 0)
		myOff = 0;
}

// double the size of map array without changing myOff and mySize
template< typename T >
void deque< T >::doubleMapSize()
{
	mapSize *= 2;
	T** newMap = new T*[mapSize]();
	int oldPosit = myOff, newPosit = myOff;
	for (int i = 0; i < mySize; i += 4, oldPosit += 4, newPosit += 4)
	{
		if (oldPosit >= (mapSize / 2) * 4)
			oldPosit -= (mapSize / 2) * 4;
		if (newPosit >= mapSize * 4)
			newPosit -= mapSize * 4;
		newMap[newPosit / 4] = map[oldPosit / 4];
	}
	delete[] map;
	map = newMap;
}

// determine if two deques are equal and return true, otherwise return false
template< typename T >
bool operator==(const deque< T > &lhs, const deque< T > &rhs)
{

	if (lhs.size() != rhs.size()||lhs.mapSize!=rhs.mapSize)
		return false;
	else
	{
		unsigned int bufferL = lhs.myOff, bufferR = rhs.myOff;
		for (unsigned int i = 0; i < lhs.size(); i++)
		{
			if (bufferL >= 4 * lhs.mapSize)
				bufferL %= (4 * lhs.mapSize), bufferR %= (4 * rhs.mapSize);
			if (lhs.map[bufferL / 4][bufferL % 4] != rhs.map[bufferR / 4][bufferR % 4])
				return false;
		}
	}
	return true;
} // end function operator==

// inequality operator; returns opposite of == operator
template< typename T >
bool operator!=(const deque< T > &lhs, const deque< T > &rhs)
{
	return !(lhs == rhs); // invokes operator==
}

#endif

