// string class definition.

#ifndef STRING_H
#define STRING_H

#include <iostream>
using std::ostream;

class string
{
   friend ostream &operator<<( ostream &output, string &str );
   friend bool operator==( const string &lhs, const string &rhs );
   friend bool operator!=( const string &lhs, const string &rhs );
public:
   // the greatest possible value for an element of type size_t ( i.e., 4294967295 )
   static const size_t npos = -1; 

   string(); // Constructs an empty string, with a length of zero characters.

   string( const string &str ); // Constructs a copy of str.

   string( const char *s, size_t n );

   ~string(); // Destroys the string object.

   // Assigns a new value to the string, replacing its current contents.
   string& operator=( const string &str );

   // If the string is empty, the returned pointer shall not be dereferenced.
   char* begin(); // Returns an pointer pointing to the first character in the string.

   // The past-the-end character is a theoretical character that
   // would follow the last character in the string. It shall not be dereferenced.
   char* end(); // Returns an pointer pointing to the past-the-end character of the string.

   size_t size() const; // Returns the number of characters in the string.

   // Resizes the string to a length of n characters.
   // If n is smaller than the current string length,
   // the current value is shortened to its first n character, removing the characters beyond the nth.
   // If n is greater than the current string length,
   // the current content is extended by inserting at the end as many characters as needed to reach a size of n.
   // The new elements are initialized as copies of null characters.
   void resize( size_t n );

   // Returns a reference to the character at position pos in the string.
   char& operator[]( size_t pos );

   // Returns a reference to the character at position pos in the string.
   const char& operator[]( size_t pos ) const;

   string& assign( const char *s, size_t n );

   // Erases the portion of the string value that
   // begins at the character position pos and spans len characters
   // (or until the end of the string, if either the content is too short or if len is string::npos).
   // Returns *this
   string& erase( size_t pos = 0, size_t len = npos );

   // Searches the string for the first occurrence of the character specified by its arguments.
   // When pos is specified, the search only includes characters at or after position pos.
   // Returns the position of the first match. If no matches were found, the function returns string::npos.
   size_t find( char c, size_t pos = 0 ) const;

   // Returns a newly constructed string object with its value initialized to a copy of a substring of this object.
   // The substring is the portion of the object that starts at character position pos and spans len characters
   // (or until the end of the string, whichever comes first).
   string substr( size_t pos = 0, size_t len = npos ) const;

   // Compares the value of the string object to the string object specified by its arguments.
   int compare( const string& str ) const;

private:
   union Bxty
   {	// storage for small buffer or pointer to larger one
      char buf[ 16 ];
      char *ptr;
   } bx;

   size_t mySize = 0; // current length of string

   size_t myRes = 15; // current storage reserved for string
}; // end class string

#endif