#ifndef UNORDERED_SET
#define UNORDERED_SET

unsigned int maxValue = 99;

// unordered_set class template definition
template< typename T >
class unordered_set
{
public:
	using iterator = typename list< T >::iterator;

	unordered_set();

	~unordered_set();

	// Returns the number of elements in the unordered_set container.
	unsigned int size() const;

	// Searches the container for an element with k as value and
	// returns an iterator to it if found,
	// otherwise it returns an iterator to unordered_set::end
	// (the element past the end of the container).
	iterator find(const T &k);

	// Inserts a new element in the unordered_set.
	// The element is inserted only if it is not equivalent to any other element
	// already in the container ( elements in an unordered_set have unique values ).
	// This effectively increases the container size by one.
	void insert(const T &val);

	// Removes from the unordered_set container a single element.
	// This effectively reduces the container size by one.
	void erase(const T &k);

	// Returns the number of buckets in the unordered_set container.
	unsigned int bucket_count() const;

	// Returns the number of elements in bucket n.
	unsigned int bucket_size(unsigned int n) const;

	// Returns the bucket number where the element with value k is located.
	unsigned int bucket(const T &k) const;

	// Returns true iff the current object is equal to data
	bool operator==(std::unordered_set< T > &data);

	//   void verify( std::unordered_set< T > &data );

private:
	list< T > myList;    // list of elements, must initialize before myVec

	vector< iterator > myVec;  // vector of list iterators, begin() then end() - 1

	unsigned int maxidx = 8; // current maximum key value

	// put a new element in the unordered_set when myVec is large enough
	void putIn(const T& val);

	// Computes hash value which will be used to compute bucket number
	unsigned int hashSeq(const unsigned int &key, unsigned int count) const;
}; // end class template unordered_set


template< typename T >
unordered_set< T >::unordered_set()
{
	myVec.reserve(16);
	myVec.assign(16, myList.end());
}

template< typename T >
unordered_set< T >::~unordered_set()
{
	myList.clear();
	myVec.clear();
}

template< typename T >
unsigned int unordered_set< T >::size() const
{
	return myList.size();
}

template< typename T >
typename unordered_set< T >::iterator unordered_set< T >::find(const T &k)
{
	unsigned int position = 2 * bucket(k);
	iterator it = myVec[position], it2 = myVec[position + 1];
	if (it == myList.end() || it2 == myList.end())
		return myList.end();
	it2++;
	for (; it != it2; it++)
		if (*it == k)
			return it;
	return myList.end();
}

template< typename T >
void unordered_set< T >::insert(const T& val)
{
	if (find(val) == myList.end())
	{
		if (myList.size() == maxidx)
		{
			if (maxidx >= 512)
				maxidx *= 2;
			else
				maxidx *= 8;
			list< T > newList(myList);
			newList.insert(newList.begin(), val);
			myList.clear();
			myVec.reserve(2 * maxidx);
			myVec.assign(2 * maxidx, myList.end());
			iterator it1 = newList.begin();
			for (; it1 != newList.end(); it1++)
				putIn(*it1);
		}
		else
			putIn(val);
	}
	else
		return;
}

template< typename T >
void unordered_set< T >::erase(const T &k)
{
	unsigned int position = 2 * bucket(k);
	if (myVec[position] == myList.end())
		return;
	else
	{
		iterator it1 = myVec[position], it2 = myVec[position + 1];
		it2++;
		while (it1 != it2)
		{
			if (*it1 == k)
				if (myVec[position] == myVec[position + 1])
				{
					myList.erase(it1);
					myVec[position] = myList.end();
					myVec[position + 1] = myVec[position];
					return;
				}
				else
				{
					iterator itBuf = it1;
					if (it1 == myVec[position + 1])
					{
						itBuf--;
						myVec[position + 1] = itBuf;
						myList.erase(it1);
					}
					else if (it1 == myVec[position])
					{
						itBuf++;
						myVec[position] = itBuf;
						myList.erase(it1);
					}
					else
						myList.erase(it1);
					return;
				}
			it1++;
		}
	}
}

template< typename T >
unsigned int unordered_set< T >::bucket_count() const
{
	return maxidx;
}

template< typename T >
unsigned int unordered_set< T >::bucket_size(unsigned int n) const
{ // return size of bucket n
	if (myVec[2 * n] == myList.end())
		return 0;
	int number = 1;
	for (iterator it = myVec[2 * n]; it != myVec[2 * n + 1]; it++, number++)
		;
	return number;
}

template< typename T >
unsigned int unordered_set< T >::bucket(const T &k) const
{
	unsigned int hashValue = hashSeq(k, sizeof(T));
	return hashValue % maxidx; // bucket number
}

template< typename T >
void unordered_set< T >::putIn(const T& val)
{
	unsigned int position = 2 * bucket(val);
	if (myVec[position] == myList.end())
	{
		myList.push_back(val);
		iterator it = myList.end();
		it--;
		myVec[position] = it;
		myVec[position + 1] = it;
	}
	else
	{
		myList.insert(myVec[position], val);
		iterator it = myVec[position];
		it--;
		myVec[position] = it;
	}
}

template< typename T >
unsigned int unordered_set< T >::hashSeq(const unsigned int &key, unsigned int count) const
{
	const unsigned int _FNV_offset_basis = 2166136261U; // 10000001 00011100 10011101 11000101
	const unsigned int _FNV_prime = 16777619U;          // 00000001 00000000 00000001 10010011
	const unsigned char *first = reinterpret_cast<const unsigned char *>(&key);
	unsigned int val = _FNV_offset_basis;
	for (unsigned int next = 0; next < count; ++next)
	{ // fold in another byte
		val ^= static_cast<unsigned int>(first[next]);
		val *= _FNV_prime;
	}

	return val;
}

template< typename T >
bool unordered_set< T >::operator==(std::unordered_set< T > &data)
{
	if (myList.size() != data.size())
		return false;

	unsigned int *firstVec = *(reinterpret_cast<unsigned int **>(&data) + 5);
	vector< list< T >::iterator >::iterator it = myVec.begin();
	for (unsigned int bucketNo = 0; it != myVec.end(); ++it, bucketNo++)
	{
		if (data.bucket_size(bucketNo) != bucket_size(bucketNo))
			return false;

		unsigned int *stlBucketFirst = *(reinterpret_cast<unsigned int **>(&firstVec[2 * bucketNo]));
		unsigned int *stlBucketLast = *(reinterpret_cast<unsigned int **>(&firstVec[2 * bucketNo + 1]));

		list< T >::iterator myBucketFirst = *it;
		++it;
		list< T >::iterator myBucketLast = *it;
		if (myBucketFirst != myList.end())
		{
			if (myBucketFirst == myBucketLast)
			{
				if (*myBucketFirst != *(stlBucketFirst + 2))
					return false;
			}
			else
			{
				unsigned int *stlPtr = stlBucketFirst;
				list< T >::iterator myIt = myBucketFirst;
				while (myIt != myBucketLast)
				{
					if (*myIt != *(stlPtr + 2))
						return false;
					stlPtr = *(reinterpret_cast<unsigned int **>(stlPtr));
					++myIt;
				}

				if (*myBucketLast != *(stlBucketLast + 2))
					return false;
			}
		}
	}

	for (unsigned int key = 1; key <= maxValue; key++)
		if (data.bucket(key) != bucket(key))
			return false;

	return true;
}

#endif