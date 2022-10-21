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

string::string(const char *s, size_t n)//��l��,�̾ڮe�q���
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

string& string::operator=(const string &str)//��l��,�̾ڮe�q���
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
	if (n <= myRes)//�p�G�e�q����,������J�N�n
	{
		if (myRes == 15)//buf���s���ܥ�buf,�_�h��ptr�s��
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
	else//�̾ڤ�����ܤj�����s��size
		if ((n / 16 + 1) * 16 - 1 > myRes + myRes / 2)
			myRes = (n / 16 + 1) * 16 - 1;
		else
			myRes += myRes / 2;
	if (oldRes == 15)//�ƻsbuf��ptr
	{
		char* buffer = new char[16];
		for (int i = 0; i < 16; i++)
			buffer[i] = bx.buf[i];
		bx.ptr = buffer;
		mySize = n;
	}
	char* buffer = new char[myRes + 1];//�ƻs���e�ñN�s�Ŷ���l��
	for (int i = 0; i < oldRes; i++)
		buffer[i] = bx.ptr[i];
	for (int i = mySize; i <= myRes; i++)
		buffer[i] = '\0';
	delete[]bx.ptr;
	bx.ptr = buffer;
	mySize = n;
}

char& string::operator[](size_t pos)//�̾ڮe�q���
{
	if (myRes == 15)
		return this->bx.buf[pos];
	else
		return this->bx.ptr[pos];
}

const char& string::operator[](size_t pos) const//�̾ڮe�q���
{
	if (myRes == 15)
		return this->bx.buf[pos];
	else
		return this->bx.ptr[pos];
}

string& string::assign(const char *s, size_t n)//�̾ڮe�q�i��վ�
{
	resize(n);
	char* i = begin();
	for (int j = 0; i != end(); i++, j++)
		*i = s[j];
	return *this;
}

string& string::erase(size_t pos , size_t len )
{
	if (bx.ptr[pos] == '\0'&&pos + len > myRes)//�R����W�X�Ŷ�,����@��size
		resize(pos);
	else
	{
		for (int i = 0; i < len; i++)
		{
			if (mySize == pos)//������ɭ԰���
				break;
			else//�@���R�h�@�Ӧr��
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
	else if (mySize - pos < len)//��M�A��Ŷ�
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