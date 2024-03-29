#ifndef VECTOR_H
#define VECTOR_H

// VecIterator class template definition
template< typename T >
class VecIterator
{
public:
   VecIterator( T *p = nullptr ) // default constructor
      : ptr( p )
   {
   }

   VecIterator( const VecIterator &iteratorToCopy ) // copy constructor
      : ptr( iteratorToCopy.ptr )
   {
   }

   ~VecIterator() // destructor
   {
   }

   // const return avoids: ( a1 = a2 ) = a3
   const VecIterator &operator=( const VecIterator &right )
   {
      if( &right != this ) // avoid self-assignment
         ptr = right.ptr;
      return *this; // enables x = y = z, for example
   }

   bool operator==( const VecIterator &right ) const // equal to
   {
      return ptr == right.ptr;
   }

   bool operator!=( const VecIterator &right ) const // not equal to
   {
      return ptr != right.ptr;
   }

   T& operator*() const // dereferencing operator
   {
      return *ptr;
   }

   VecIterator& operator++() // prefix increment operator
   {
      ++ptr;
      return *this;
   }

   VecIterator& operator--() // prefix decrement an iterator
   {
      --ptr;
      return *this;
   }

private:
   T *ptr; // keep a pointer to vector
}; // end class template VecIterator


// ReverseVecIterator class template definition
template< typename T >
class ReverseVecIterator
{
public:
   ReverseVecIterator( T *p = nullptr ) // default constructor
      : ptr( p )
   {
   }

   ReverseVecIterator( const ReverseVecIterator &iteratorToCopy ) // copy constructor
      : ptr( iteratorToCopy.ptr )
   {
   }

   ~ReverseVecIterator() // destructor
   {
   }

   // const return avoids: ( a1 = a2 ) = a3
   const ReverseVecIterator &operator=( const ReverseVecIterator &right )
   {
      if( &right != this ) // avoid self-assignment
         ptr = right.ptr;
      return *this; // enables x = y = z, for example
   }

   bool operator==( const ReverseVecIterator &right ) const // equal to
   {
      return ptr == right.ptr;
   }

   bool operator!=( const ReverseVecIterator &right ) const // not equal to
   {
      return ptr != right.ptr;
   }

   T& operator*() const // dereferencing operator
   {
      return *ptr;
   }

   ReverseVecIterator& operator++() // prefix increment operator
   {
      --ptr;
      return *this;
   }

   ReverseVecIterator& operator--() // prefix decrement an iterator
   {
      ++ptr;
      return *this;
   }
   
private:
   T *ptr; // keep a pointer to vector
}; // end class template ReverseVecIterator


// vector class template definition
template< typename T >
class vector
{
   template< typename T >
   friend bool operator==( const vector< T > &lhs, const vector< T > &rhs );

   template< typename T >
   friend bool operator!=( const vector< T > &lhs, const vector< T > &rhs );

public:
   using iterator = VecIterator< T >;
   using reverse_iterator = ReverseVecIterator< T >;

   vector( unsigned int n = 0 ); // Constructor; Constructs a zero vector with size n

   // Copy constructor; Constructs a vector with a copy of each of the elements in x.
   vector( const vector &x );

   ~vector(); // Destroys the vector.

   const vector& operator=( const vector &x ); // assignment operator

   // Returns a pointer pointing to the first element in the vector.
   // If the vector is empty, the returned pointer shall not be dereferenced.
   iterator begin() const;

   // Returns an pointer pointing to the past-the-end element in the vector.
   // The past-the-end element is the theoretical element
   // that would follow the last element in the vector.
   // It does not point to any element, and thus shall not be dereferenced.
   iterator end() const;

   // Returns a iterator pointing to the last element in the vector.
   reverse_iterator rbegin() const;

   // Returns a iterator pointing to the theoretical element
   // preceding the first element in the vector.
   reverse_iterator rend() const;

   // Returns the number of elements in the vector.
   // This is the number of actual objects held in the vector,
   // which is not necessarily equal to its storage capacity.
   unsigned int size() const;

   // Returns the size of the storage space currently allocated for the vector,
   // expressed in terms of elements.
   unsigned int capacity() const;

   // Resizes the vector so that it contains n elements.
   // If n is smaller than the current vector size,
   // the content is reduced to its first n elements, removing those beyond.
   // If n is greater than the current vector size,
   // the content is expanded by inserting at the end as many elements as needed to reach a size of n.
   // The new elements are initialized as 0.
   // If n is also greater than the current vector capacity,
   // an automatic reallocation of the allocated storage space takes place.
   void resize( unsigned int n );
   
   // Adds a new element at the end of the vector,
   // after its current last element.
   // The content of val is copied to the new element.
   // This effectively increases the vector size by one,
   // which causes an automatic reallocation of the allocated storage space
   // if and only if the new vector size surpasses the current vector capacity.
   void push_back( const T val );

   // Removes the last element in the vector,
   // effectively reducing the container size by one.
   void pop_back();

private:
   T *myFirst = nullptr;
   T *myLast = nullptr;
   T *myEnd = nullptr;
}; // end class template vector


// constructor; constructs a zero vector with size n
template< typename T >
vector< T >::vector( unsigned int n )
   : myFirst( n == 0 ? nullptr : new T[ n ]() ),
      myLast( n == 0 ? nullptr : myFirst + n ),
       myEnd( n == 0 ? nullptr : myLast )
{
} // end vector default constructor

// copy constructor; constructs a vector with a copy of each of the elements in x
template< typename T >
vector< T >::vector( const vector< T > &x )
{
	*this = x;//直接利用operator=
} // end vector copy constructor

// destructor; destroys the vector
template< typename T >
vector< T >::~vector()
{
   if( myFirst != nullptr )
      delete[] myFirst;
} // end destructor

// overloaded assignment operator;
// const return avoids: ( a1 = a2 ) = a3
template< typename T >
const vector< T >& vector< T >::operator=( const vector< T > &x )
{
	if (myFirst != nullptr)
		delete[]myFirst;//刪除舊的空間
	myFirst = new T[x.capacity()];
	myEnd = myFirst + x.capacity();
	myLast = myFirst + x.size();
	iterator i = begin();
	for (iterator j = x.begin(); j != x.end(); ++i, ++j)
		*i = *j;
	return *this;
} // end function operator=

template< typename T >
typename vector< T >::iterator vector< T >::begin() const
{
   return iterator( myFirst );
}

template< typename T >
typename vector< T >::iterator vector< T >::end() const
{
   return iterator( myLast );
}

template< typename T >
typename vector< T >::reverse_iterator vector< T >::rbegin() const
{
   return reverse_iterator( myLast - 1 );
}

template< typename T >
typename vector< T >::reverse_iterator vector< T >::rend() const
{
   return reverse_iterator( myFirst - 1 );
}

template< typename T >
unsigned int vector< T >::size() const
{
   return ( myLast - myFirst );
}

template< typename T >
unsigned int vector< T >::capacity() const
{
   return ( myEnd - myFirst );
}

template< typename T >
void vector< T >::resize( unsigned int n )
{
	if (myFirst == nullptr)
	{
		myFirst = new T[n]();
		myLast = myEnd = myFirst + n;
	}
	else if (n > capacity())//需要擴大
	{
		unsigned int NewSize = 0;
 		n > size() + size() / 2 ? NewSize = n : NewSize = size() + size() / 2;//依據規則擴大
		vector< T > NewVec(NewSize);
		NewVec.myLast = NewVec.myFirst + n;
		VecIterator< T > Old = begin();
		for (VecIterator< T > New = NewVec.begin(); Old != end(); ++Old, ++New)
			*New = *Old;
		*this = NewVec;
	}
	else
		myLast = myFirst + n;
}

template< typename T >
void vector< T >::push_back(const T val)
{
	if (myLast == myEnd)
	{
		resize(size() + 1);
		*(myLast - 1) = val;
	}
	else 
	{
		*myLast = val;
		myLast++;
	}
}

// Removes the last element in the vector,
// effectively reducing the container size by one.
template< typename T >
void vector< T >::pop_back()
{
	if (size() > 0)
	{
		--myLast;
		*myLast = T();
	}
}

// determine if two Arrays are equal and
// return true, otherwise return false
template< typename T >
bool operator==( const vector< T > &lhs, const vector< T > &rhs )
{
	if (lhs.size() != rhs.size())//先檢查Size
		return false;
	else//依次檢查元素
	{
		VecIterator< T > it1 = lhs.begin();
		for (VecIterator< T > it2 = rhs.begin(); it1 != lhs.end(); ++it1, ++it2)
			if (*it1 != *it2)
				return false;
	}
	return true;
} // end function operator==

// inequality operator; returns opposite of == operator
template< typename T >
bool operator!=( const vector< T > &lhs, const vector< T > &rhs )
{
   return !( lhs == rhs ); // invokes vector::operator==
}

#endif