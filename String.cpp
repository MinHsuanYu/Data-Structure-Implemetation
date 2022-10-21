// string class member-function definitions.

#include <iostream>
using std::cerr;

#include "String.h" // string class definition

string::string()
{
   bx.buf[ 0 ] = '\0';
}

string::string( const string &str )
   : mySize( str.mySize ),
     myRes( str.myRes )
{
   if( myRes == 15 )
      for( size_t i = 0; i <= mySize; i++ )
         bx.buf[ i ] = str.bx.buf[ i ];
   else
   {
      bx.ptr = new char[ myRes + 1 ];
      for( size_t i = 0; i <= mySize; i++ )
         bx.ptr[ i ] = str.bx.ptr[ i ];
   }
}

string::string(const char *s, size_t n)//初始化,依據容量選擇
	:mySize(n),
	myRes(n)
{
	if (myRes == 15)
		for (size_t i = 0; i <= mySize; i++)
			bx.buf[i] = s[i];
	else
	{
		bx.ptr = new char[myRes + 1];
		for (size_t i = 0; i <= mySize; i++)
			bx.ptr[i] = s[i];
	}
}

// destructor
string::~string()
{
   if( myRes > 15 )
      delete[] bx.ptr;
} // end destructor

string& string::operator=(const string &str)//初始化,依據容量選擇
{
	if (myRes > 15)
		delete[]bx.ptr;
	mySize = str.mySize;
	myRes = str.myRes;
	if (myRes == 15)
		for (size_t i = 0; i <= mySize; i++)
			bx.buf[i] = str[i];
	else
	{
		bx.ptr = new char[myRes + 1];
		for (size_t i = 0; i <= mySize; i++)
			bx.ptr[i] = str[i];
	}
	return *this;
}

char* string::begin()
{
   if( myRes == 15 )
      return bx.buf;
   else
      return bx.ptr;
}

char* string::end()
{
   if( myRes == 15 )
      return bx.buf + mySize;
   else
      return bx.ptr + mySize;
}

size_t string::size() const
{
   return mySize;
}

void string::resize(size_t n) 
{
	int oldRes = myRes;
	if (n <= myRes)//如果容量夠用,直接放入就好
	{
		if (myRes == 15)//buf夠存的話用buf,否則用ptr存放
		{
			for (int i = mySize; i < 16; i++)
				bx.buf[i] = '\0';
			mySize = n;
		}
		else
		{
			for (int i = mySize; i <= myRes; i++)
				bx.ptr[i] = '\0';
			mySize = n;
		}
		return;
	}
	else//依據公式選擇大的為新的size
		if ((n / 16 + 1) * 16 - 1 > myRes + myRes / 2)
			myRes = (n / 16 + 1) * 16 - 1;
		else
			myRes += myRes / 2;
	if (oldRes == 15)//複製buf到ptr
	{
		char* buffer = new char[16];
		for (int i = 0; i < 16; i++)
			buffer[i] = bx.buf[i];
		bx.ptr = buffer;
		mySize = n;
	}
	char* buffer = new char[myRes + 1];//複製內容並將新空間初始化
	for (int i = 0; i < oldRes; i++)
		buffer[i] = bx.ptr[i];
	for (int i = mySize; i <= myRes; i++)
		buffer[i] = '\0';
	delete[]bx.ptr;
	bx.ptr = buffer;
	mySize = n;
}

char& string::operator[](size_t pos)//依據容量選擇
{
	if (myRes == 15)
		return this->bx.buf[pos];
	else
		return this->bx.ptr[pos];
}

const char& string::operator[](size_t pos) const//依據容量選擇
{
	if (myRes == 15)
		return this->bx.buf[pos];
	else
		return this->bx.ptr[pos];
}

string& string::assign(const char *s, size_t n)//依據容量進行調整
{
	resize(n);
	char* i = begin();
	for (int j = 0; i != end(); i++, j++)
		*i = s[j];
	return *this;
}

string& string::erase(size_t pos , size_t len )
{
	if (bx.ptr[pos] == '\0'&&pos + len > myRes)//刪除到超出空間,重整一次size
		resize(pos);
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (mySize == pos)//到尾的時候停止
				break;
			else//一次刪去一個字元
			{
				if (myRes == 15)
					for (int j = pos; j <= mySize; j++)
						bx.buf[j] = bx.buf[j + 1];
				else
				{
					for (int j = pos; j <= mySize; j++)
						bx.ptr[j] = bx.ptr[j + 1];
				}
				mySize--;
			}
		}
	}
	return *this;
}

size_t string::find(char c, size_t pos) const
{
	for (size_t i = pos; i < mySize; i++)
	{
		if (myRes == 15)
			if (bx.buf[i] == c)
				return i;
			else
				continue;
		else if (bx.ptr[i] == c)
			return i;
	}
	return npos;
}

string string::substr(size_t pos, size_t len) const
{
	string subString;
	if (len == npos || len < 0)
		return subString;
	else if (mySize - pos < len)//找尋適當空間
		subString.resize(mySize - pos);
	else
		subString.resize(len);

	if (subString.mySize == 15)
		for (size_t i = pos, j = 0; i < len; i++, j++)
			if (pos + j == mySize)
				break;
			else
				subString[j] = bx.buf[i];
	else
		for (size_t i = pos, j = 0; i < len; i++, j++)
			if (pos + j == mySize)
				break;
			else
				subString[j] = bx.ptr[i];
	return subString;
}

std::ostream &operator<<( std::ostream &output, string &str )
{
   if( str.myRes == 15 )
      for( size_t i = 0; i < str.mySize; i++ )
         output << str.bx.buf[ i ];
   else
      for( size_t i = 0; i < str.mySize; i++ )
         output << str.bx.ptr[ i ];

   return output; // enables cout << x << y;
}

bool operator==( const string &lhs, const string &rhs )
{
   return ( lhs.compare( rhs ) == 0 );
}

bool operator!=( const string &lhs, const string &rhs )
{
   return ( lhs.compare( rhs ) != 0 );
}

int string::compare(const string& str) const
{
	if (size() > str.size())
		return 1;
	else if (size() < str.size())
		return -1;
	else
	{
		for (int i = 0; i < size(); i++)
			if ((*this)[i] > str[i])
				return 1;
			else if ((*this)[i] < str[i])
				return -1;
			else if ((*this)[i] == str[i])
				continue;		
	}
	return 0;
}