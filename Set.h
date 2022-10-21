#ifndef SET_H
#define SET_H

enum Color { Red, Black };

// TreeNode class template definition
template< typename Kty >
class TreeNode
{
	template< typename K > friend class set;
private:
	TreeNode *left;   // left subtree, or smallest element if head
	TreeNode *parent; // parent, or root of tree if head
	TreeNode *right;  // right subtree, or largest element if head
	Color color;      // red or black, black if head
	bool isNil;       // true only if head (also nil) node
	Kty myval;
}; // end class template TreeNode

// set class template definition
template< typename Kty >
class set
{
public:
	set(); // Constructs an empty set object, with head node.

	~set(); // Destroys the set object.

	// Extends the set container by inserting a new element,
	// effectively increasing the container size by one.
	void insert(const Kty &val);

	// Removes from the set container a single element whose value is val
	// This effectively reduces the container size by one, which are destroyed.
	// Returns the number of elements erased.
	unsigned int erase(const Kty &val);

private:
	TreeNode< Kty > *myHead; // pointer to head node
	unsigned int mySize;     // number of elements ( not include head node )

	// Removes all elements from the set object (which are destroyed)
	void clear(TreeNode< Kty > *node);

	// rebalance for insertion
	void reBalance(TreeNode< Kty > *node);

	// rotate right at g, where p = g->left and node = p->left
	void LLRotation(TreeNode< Kty > *p);

	// LR rotation; p = g->left and node = p->right
	void LRRotation(TreeNode< Kty > *node);

	// RL rotation; p = g->right and node = p->left
	void RLRotation(TreeNode< Kty > *node);

	// rotate left at g, where p = g->right and node = p->right
	void RRRotation(TreeNode< Kty > *p);

	// erase node provided that the degree of node is at most one
	void eraseDegreeOne(TreeNode< Kty > *node);

	// rebalance for deletion
	void fixUp(TreeNode< Kty > *N, TreeNode< Kty > *P);

	void twoTraversals(); // preorder traversal and inorder traversal

	void preorder(TreeNode< Kty > *node); // preorder traversal

	void inorder(TreeNode< Kty > *node); // inorder traversal
}; // end class template set

// Constructs an empty set object, with head node.
template< typename Kty >
set< Kty >::set()
	: myHead(new TreeNode< Kty >),
	mySize(0)
{
	myHead->left = myHead;
	myHead->parent = myHead;
	myHead->right = myHead;
	myHead->color = Black;
	myHead->isNil = true;
}

// Destroys the set object.
template< typename Kty >
set< Kty >::~set()
{
	clear(myHead->parent);
	delete myHead;
}

// Extends the container by inserting a new element,
// effectively increasing the container size by one.
template< typename Kty >
void set< Kty >::insert(const Kty& val)
{
	TreeNode <Kty> *newNode = myHead->parent, *p = myHead;
	while (!newNode->isNil)
	{
		p = newNode;
		if (val < p->myval)
			newNode = p->left;
		else if (val > p->myval)
			newNode = p->right;
		else
			return;
	}
	newNode = new TreeNode<Kty>();
	newNode->color = Red;
	newNode->isNil = false;
	newNode->parent = p;
	newNode->left = newNode->right = myHead;
	newNode->myval = val;
	if (val < p->myval)
		p->left = newNode;
	else
		p->right = newNode;
	if (p == myHead)
		p->left = p->right = newNode;
	else if (p == myHead->left&& p->left==newNode)
		myHead->left = newNode;
	else if (p == myHead->right&&p->right==newNode)
		myHead->right = newNode;
	if (mySize == 0)
		myHead->parent = newNode;
	reBalance(newNode);
	mySize++;
}

// Removes from the set container a single element whose value is val
// This effectively reduces the container size by one, which are destroyed.
// Returns the number of elements erased.
template< typename Kty >
unsigned int set< Kty >::erase(const Kty &val)
{
	TreeNode <Kty> *find = myHead->parent;
	while (!find->isNil&&find->myval != val)
	{
		if (val < find->myval)
			find = find->left;
		else
			find = find->right;
	}
	if (find->isNil)
		return -1;
	TreeNode <Kty> *exch = find;
	if (!find->left->isNil && !find->right->isNil)
	{
		exch = find->right;
		while (!exch->left->isNil)
			exch = exch->left;
		find->myval = exch->myval;
	}
	if (myHead->left == exch)
		myHead->left = exch->parent;
	else if (myHead->right == exch)
		myHead->right = exch->parent;
	eraseDegreeOne(exch);
	mySize--;
	return val;
}

// Removes all elements from the set object (which are destroyed)
template< typename Kty >
void set< Kty >::clear(TreeNode< Kty > *node)
{
	if (!node->isNil) // node is not an external node
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

// rebalance for insertion
template< typename Kty >
void set< Kty >::reBalance(TreeNode< Kty > *node)
{
	TreeNode <Kty> *pa = node->parent;
	if (pa == myHead)
		node->color = Black;
	else if (pa->color == Red)
	{
		TreeNode <Kty> *gr = pa->parent;
		if (gr->left == pa)//L-
		{
			if (gr->right->color == Red)
			{
				pa->color = gr->right->color = Black;
				gr->color = Red;
				reBalance(gr);
			}
			else if (pa->left == node)
				LLRotation(pa);
			else
				LRRotation(node);

		}
		else//R-
		{
			if (gr->left->color == Red)
			{
				pa->color = gr->left->color = Black;
				gr->color = Red;
				reBalance(gr);
			}
			else if (pa->right == node)
				RRRotation(pa);
			else
				RLRotation(node);
		}
	}
}

// rotate right at g, where p = g->left and node = p->left
template< typename Kty >
void set< Kty >::LLRotation(TreeNode< Kty > *p)
{
	TreeNode <Kty> *g = p->parent;
	Color buffer = p->color;
	p->color = g->color;
	g->color = buffer;
	if (g == myHead->parent)
		myHead->parent = p;
	else if (g->parent->left == g)
		g->parent->left = p;
	else if (g->parent->right == g)
		g->parent->right = p;
	if (!p->right->isNil)
		p->right->parent = g;
	g->left = p->right;
	p->parent = g->parent;
	g->parent = p;
	p->right = g;
}

// LR rotation; p = g->left and node = p->right
template< typename Kty >
void set< Kty >::LRRotation(TreeNode< Kty > *node)
{
	RRRotation(node);
	/*	TreeNode <Kty> *p = node->parent;
		p->right = node->left;
		node->parent = p->parent;
		node->left = p;
		p->parent = node;*/
	LLRotation(node);
}

// RL rotation; p = g->right and node = p->left
template< typename Kty >
void set< Kty >::RLRotation(TreeNode< Kty > *node)
{
	LLRotation(node);
	/*TreeNode <Kty> *p = node->parent;
	p->left = node->right;
	node->parent = p->parent;
	node->right = p;
	p->parent = node;*/
	RRRotation(node);
}

// rotate left at g, where p = g->right and node = p->right
template< typename Kty >
void set< Kty >::RRRotation(TreeNode< Kty > *p)
{
	TreeNode <Kty> *g = p->parent;
	Color buffer = p->color;
	p->color = g->color;
	g->color = buffer;
	if (g == myHead->parent)
		myHead->parent = p;
	else if (g->parent->right == g)
		g->parent->right = p;
	else if (g->parent->left == g)
		g->parent->left = p;
	if (!p->left->isNil)
		p->left->parent = g;
	g->right = p->left;
	p->parent = g->parent;
	g->parent = p;
	p->left = g;
}

// erase node provided that the degree of node is at most one
template< typename Kty >
void set< Kty >::eraseDegreeOne(TreeNode< Kty > *node)
{
	TreeNode <Kty> *child = node->left, *p = node->parent;
	if (child->isNil)
		child = node->right;
	if (p->isNil)
	{
		myHead->parent = child;
		child->color = Black;
	}
	else
	{
		if (p->left == node)
			p->left = child;
		else
			p->right = child;
		child->parent = node->parent;

		if (node->color != child->color)//one-red + one-black
			child->color = Black;
		else
			fixUp(child, p);
	}
	delete node;
}

// rebalance for deletion
template< typename Kty >
void set< Kty >::fixUp(TreeNode< Kty > *N, TreeNode< Kty > *P)
{
	TreeNode <Kty> *S = P->left;
	if (S == N)
		S == P->right;
	if (S->color == Red)
	{
		if (P->left == N)
		{
			RRRotation(S);//case1.1
			S = P->right;
		}
		else
		{
			LLRotation(S);//case1.2
			S = P->left;
		}
	}
	if (S->left->color == Black && S->right->color == Black)
	{
		if (P->color == Red)//case4
			P->color = Black, S->color = Red;
		else
		{
			S->color = Red;//case5
			fixUp(P, P->parent);
		}
	}
	if (P->left == N)//case3.1&&2.1
	{
		if (S->left->color == Red)//case3.1
		{
			S->left->color == Black;
			S->color = Red;
			LLRotation(S->left);
			S = P->right;
		}
		if (S->left->color == Black)//case2.1
		{
			S->color = P->color;
			P->color = Black;
			S->right->color = Black;
			RRRotation(S);
		}
	}
	else//case3.2&&2.2
	{
		if (S->right->color == Red)//case3.2
		{
			S->right->color = Black;
			S->color = Red;
			RRRotation(S->right);
			S = P->left;
		}
		if (S->right->color == Black)//case2.2
		{
			S->color = P->color;
			P->color = Black;
			S->right->color = Black;
			LLRotation(S);
		}
	}
}

// preorder traversal and inorder traversal
template< typename Kty >
void set< Kty >::twoTraversals()
{
	cout << "Preorder sequence:\n";
	preorder(myHead->parent);

	cout << "\nInorder sequence:\n";
	inorder(myHead->parent);
	cout << endl;
}

// preorder traversal
template< typename Kty >
void set< Kty >::preorder(TreeNode< Kty > *node)
{
	if (node != myHead)
	{
		cout << setw(5) << node->myval << (node->color == Red ? "R" : "B");
		preorder(node->left);
		preorder(node->right);
	}
}

// inorder traversal
template< typename Kty >
void set< Kty >::inorder(TreeNode< Kty > *node)
{
	if (node != myHead)
	{
		inorder(node->left);
		cout << setw(5) << node->myval << (node->color == Red ? "R" : "B");
		inorder(node->right);
	}
}

#endif