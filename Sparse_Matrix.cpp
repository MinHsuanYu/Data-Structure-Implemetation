#include "Matrix.h"

MatrixNode::MatrixNode( bool b, Triple &t )  // constructor
{
   head = b;
   if( b )
      right = down = next = this; // row/column header node
   else
      triple = t; // element node or header node for list of header nodes
}

Matrix::Matrix()
   : headNode( nullptr )
{
}

Matrix::Matrix( TwoDimArray &twoDimArray )
{
   Triple s;
   s.row = twoDimArray.numRows;
   s.col = twoDimArray.numCols;
   s.value = twoDimArray.numValues;

   if( s.col <= 0 || s.row <= 0 )
      return;

   // set up header node for the list of header nodes.
   headNode = new MatrixNode( false, s );

   Triple *triples = new Triple[ s.value ];
   int count = 0;
   for( int i = 0; i < s.row; i++ )
      for( int j = 0; j < s.col; j++ )
         if( twoDimArray.ptr[ i ][ j ] != 0 )
         {
            triples[ count ].row = i;
            triples[ count ].col = j;
            triples[ count ].value = twoDimArray.ptr[ i ][ j ];
            count++;
         }

   int p = ( s.col > s.row ) ? s.col : s.row;

   // at least one row and column
   MatrixNode **head = new MatrixNode *[ p ];
   Triple zeroTriple = {};
   for( int i = 0; i < p; i++ )
      head[ i ] = new MatrixNode( true, zeroTriple ); // create header nodes

   int currentRow = 0;
   MatrixNode *last = head[ 0 ]; // last node in current row
   for( int i = 0; i < s.value; i++ ) // input triples
   {
      if( triples[ i ].row > currentRow ) // close current row
      {
         last->right = head[ currentRow ];
         currentRow = triples[ i ].row;
         last = head[ currentRow ];
      }

      // link new node into row list
      last = last->right = new MatrixNode( false, triples[ i ] );

      // link into column list
      head[ triples[ i ].col ]->next = head[ triples[ i ].col ]->next->down = last;
   }

   last->right = head[ currentRow ];// close last row

   // close all column lists
   for( int i = 0; i < s.col; i++ )
      head[ i ]->next->down = head[ i ];

   // link the header nodes together
   for( int i = 0; i < p - 1; i++ )
      head[ i ]->next = head[ i + 1 ];

   head[ p - 1 ]->next = headNode;
   headNode->right = head[ 0 ];
   delete[] head;
}

Matrix::Matrix( const Matrix &matrixToCopy )
{
	*this = matrixToCopy;
}

Matrix::~Matrix()
{
   erase();
}

const Matrix &Matrix::operator=( const Matrix &op2 )
{
	this->erase();
	if (op2.headNode == nullptr)
		return *this;
	else
	{
		headNode = new MatrixNode(false, op2.headNode->triple);
		int number = (headNode->triple.col > headNode->triple.row) ? headNode->triple.col : headNode->triple.row;
		MatrixNode **heads = new MatrixNode *[number];
		Triple zeroTriple = {};
		for (int i = 0; i < number; i++)
			heads[i] = new MatrixNode(true, zeroTriple);
		MatrixNode *rHeads = op2.headNode->right;
		for (int i = 0; i < number; i++)
		{
			MatrixNode *find = rHeads->down, *add = heads[i];
			while (find != rHeads)
			{
				MatrixNode *buffer = new MatrixNode(false, find->triple);
				add->down = buffer;
				find = find->down;
				add = add->down;
			}
			rHeads = rHeads->next;
			add->down = heads[i];
		}
		MatrixNode **find = new MatrixNode*[number];
		for (int i = 0; i < number; i++)
			find[i] = heads[i];
		for (int i = 0; i < number; i++)
		{
			MatrixNode *now = heads[i];
			if (now->down != now)
			{
				now = now->down;
				while (now!=heads[i])
				{
					int row = now->triple.row;
					find[row]->right = now;
					find[row] = find[row]->right;
					now = now->down;
				}
			}
		}
		number--;
		heads[number]->next = headNode;
		for (int i = 0; i < number; i++)
		{
			heads[i]->next = heads[i + 1];
			find[i]->right = heads[i];
		}
		headNode->right = heads[0];
		find[number]->right = heads[number];
	}
}

Matrix Matrix::operator+( Matrix &op2 )
{
	int max = headNode->triple.row > headNode->triple.col ? headNode->triple.row : headNode->triple.col;
	Matrix buffer;
	if (max == 0)
		return buffer;
	buffer.headNode = new MatrixNode(false,headNode->triple);
	buffer.headNode->triple.value = 0;
	MatrixNode *lHead = headNode->right, *rHead = op2.headNode->right, **heads = new MatrixNode*[max];
	Triple tri{ 0,0,0 };
	for (int i = 0; i < max; i++)
		heads[i] = new MatrixNode(true, tri);
	for (int i = 0; i < max; i++)
	{
		MatrixNode *l = lHead->down, *r = rHead->down,*now=heads[i];
		while (!l->head|| !r->head)
		{
			tri.col = i;
			if (!l->head && !r->head)
			{
				if (l->triple.row == r->triple.row)
				{
					tri.value = l->triple.value + r->triple.value;
					tri.row = l->triple.row;
					l = l->down;
					r = r->down;
					if (tri.value == 0)
						continue;
				}
				else if (l->triple.row < r->triple.row)
				{
					tri.value = l->triple.value;
					tri.row = l->triple.row;
					l = l->down;
				}
				else
				{
					tri.value = r->triple.value;
					tri.row = r->triple.row;
					r = r->down;
				}
			}
			else if (!l->head)
			{
				tri.value = l->triple.value;
				tri.row = l->triple.row;
				l = l->down;
			}
			else
			{
				tri.value = r->triple.value;
				tri.row = r->triple.row;
				r = r->down;
			}
			now->down = new MatrixNode(false, tri);
			buffer.headNode->triple.value++;
			now = now->down;
		}
		lHead = lHead->next;
		rHead = rHead->next;
		now->down = heads[i];
	}
	buffer.headNode->right = heads[0];
	MatrixNode **find = new MatrixNode*[max];
	for (int i = 0; i < max; i++)
		find[i] = heads[i];
	for (int i = 0; i < max; i++)
	{
		MatrixNode *nowNode = heads[i];
		if (nowNode->down != nowNode)
		{
			nowNode = nowNode->down;
			while (!nowNode->head)
			{
				int row = nowNode->triple.row;
				find[row]->right = nowNode;
				find[row] = find[row]->right;
				nowNode = nowNode->down;
			}
		}
	}
	max--;
	heads[max]->next = buffer.headNode;
	for (int i = 0; i < max; i++)
	{
		heads[i]->next = heads[i + 1];
		find[i]->right = heads[i];
	}
	find[max]->right = heads[max];
	return buffer;
}

Matrix Matrix::operator*( Matrix &op2 )
{
	Matrix buffer;
	Triple tri = { headNode->triple.row ,op2.headNode->triple.col,0 };
	int max = tri.row > tri.col ? tri.row : tri.col;
	buffer.headNode = new MatrixNode(false, tri);
	MatrixNode **heads = new MatrixNode*[max];
	for (int i = 0; i < max; i++)
		heads[i] = new MatrixNode(true, tri);
	MatrixNode *Rheads = op2.headNode->right;
	for (int i = 0; i < buffer.headNode->triple.col; i++,Rheads=Rheads->next)
	{
		MatrixNode *now = heads[i], *Lheads = headNode->right;
		for (int j = 0; j < buffer.headNode->triple.row; j++, Lheads = Lheads->next)
		{
			MatrixNode *l = Lheads->right, *r = Rheads->down;
			int mult = 0;
			while (!l->head && !r->head)
			{
				if (l->triple.col == r->triple.row)
				{
					mult += l->triple.value*r->triple.value;
					l = l->right;
					r = r->down;

				}
				else if (l->triple.col < r->triple.row)
					l = l->right;
				else
					r = r->down;
			}
			if (mult != 0)
			{
				Triple nowtri = { j, i, mult };
				now->down = new MatrixNode(false, nowtri);
				buffer.headNode->triple.value++;
				now = now->down;
			}
		}
		now->down = heads[i];
	}
	buffer.headNode->right = heads[0];
	MatrixNode **find = new MatrixNode*[max];
	for (int i = 0; i < max; i++)
		find[i] = heads[i];
	for (int i = 0; i < max; i++)
	{
		MatrixNode *nowNode = heads[i];
		if (nowNode->down != nowNode)
		{
			nowNode = nowNode->down;
			while (!nowNode->head)
			{
				int row = nowNode->triple.row;
				find[row]->right = nowNode;
				find[row] = find[row]->right;
				nowNode = nowNode->down;
			}
		}
	}
	max--;
	heads[max]->next = buffer.headNode;
	for (int i = 0; i < max; i++)
	{
		heads[i]->next = heads[i + 1];
		find[i]->right = heads[i];
	}
	find[max]->right = heads[max];
	return buffer;
}

void Matrix::erase()
{  // erase the matrix, return the nodes to the heap
   MatrixNode *x, *y;
   if( headNode != nullptr )
   {
      MatrixNode *head = headNode->right;

      while( head != headNode ) // free the triple and header nodes by row
      {
         y = head->right;
         while( y != head )
         {
            x = y;
            y = y->right;
            delete x;
         }

         x = head;
         head = head->next;
         delete x;
      }

      delete headNode;
      headNode = nullptr;
   }
}

bool Matrix::operator==( TwoDimArray &twoDimArray )
{
   int numRows = headNode->triple.row;
   int numCols = headNode->triple.col;

   if( numRows != twoDimArray.numRows )
      return false;

   if( numCols != twoDimArray.numCols )
      return false;

   if( headNode->triple.value != twoDimArray.numValues )
      return false;

   int col;
   MatrixNode *head = headNode->right;
   // scan header nodes one by one
   for( int row = 0; row < numRows; row++, head = head->next )
   {
      col = 0;
      // scan elements in the current row
      for( MatrixNode *temp = head->right; temp != head; temp = temp->right )
      {
         if( row != temp->triple.row )
            return false;

         // find next nonzero element in the current row
         while( twoDimArray.ptr[ row ][ col ] == 0 )
            col++;

         if( col != temp->triple.col )
            return false;

         if( twoDimArray.ptr[ row ][ col ] != temp->triple.value )
            return false;

         col++;
      }
   }
   
   int row;
   head = headNode->right;
   // scan header nodes one by one
   for( int col = 0; col < numRows; col++, head = head->next )
   {
      row = 0;
      // scan elements in the current column
      for( MatrixNode *temp = head->down; temp != head; temp = temp->down )
      {
         if( col != temp->triple.col )
            return false;

         // find next nonzero element in the current column
         while( twoDimArray.ptr[ row ][ col ] == 0 )
            row++;

         if( row != temp->triple.row )
            return false;

         if( twoDimArray.ptr[ row ][ col ] != temp->triple.value )
            return false;

         row++;
      }
   }

   return true;
}

ostream &operator<<( ostream &os, Matrix &matrix )
{  // print out the matrix in row major form
   int preCol;
   int numRows = matrix.headNode->triple.row;
   MatrixNode *head = matrix.headNode->right;
   // scan header nodes one by one
   for( int row = 0; row < numRows; row++, head = head->next )
   {
      preCol = -1;
      // scan elements in the current row
      for( MatrixNode *temp = head->right; temp != head; temp = temp->right )
      {
         // display zero elements
         for( int i = 1; i < temp->triple.col - preCol; i++ )
            os << setw( 4 ) << 0;
         os << setw( 4 ) << temp->triple.value; // display nonzero element
         preCol = temp->triple.col;
      }

      for( int i = 1; i < matrix.headNode->triple.col - preCol; i++ )
         os << setw( 4 ) << 0;
      os << endl;
   }

   return os; // enables os << x << y;
}