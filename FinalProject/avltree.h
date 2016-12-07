/*Purpose: Class that implements the functionality of an AVL Tree.
 * Author: Will Kastner
 * Date: 11/2/2016
 *
 * Implementation based on code from http://users.cis.fiu.edu/~weiss/dsaa_c++3/code/
 */

#ifndef AVLTREE
#define AVLTREE

#include <iostream>

using namespace std;

template< class T >
class AvlNode
{
    template < class U > friend class AvlTree;
    T data;
    AvlNode *left;
    AvlNode *right;
    int height;

    AvlNode( const T & element, AvlNode *lt, AvlNode *rt, int h = 0 )
        : data( element ), left( lt ), right( rt ), height( h ) { }

    bool operator>(AvlNode* rhs)
    {
        if(data > rhs->data)
            return true;
        else
            return false;
    }

    bool operator ==(AvlNode* rhs)
    {
        if(data == rhs->data)
            return true;
        else
            return false;
    }

};


template < class T >
class AvlTree
{
private:

    AvlNode<T> *root;
    int NumberofNodes = 0;
    int height( AvlNode<T>* );
    int max( int, int );
    void makeEmpty( AvlNode<T>*& );
    void insert( const T&, AvlNode<T>*& );
    void rotateL( AvlNode<T>*& );
    void rotateR( AvlNode<T>*& );
    void doubleL( AvlNode<T>*& );
    void doubleR( AvlNode<T>*& );
    bool contains( const T &, AvlNode<T>*& );
    void printNode( AvlNode<T>* );
    T& getNode(const T &, AvlNode<T>*& );
    AvlNode<T>* clone( AvlNode<T>* ) const;



public:
    AvlTree( );
    AvlTree( AvlTree& );
    ~AvlTree();
    void makeEmpty( );
    bool contains( const T& );
    bool isEmpty( ) const;
    void insert( const T& );
    AvlTree<T>& operator=(AvlTree<T>& );
    void printTree();
    T& getNode(const T&);
    int getNumNodes();


};

#endif // AVLTREE

template <class T>
AvlTree<T>::AvlTree(): root(nullptr) { }

template <class T>
AvlTree<T>::AvlTree(AvlTree& rhs)
{
    root = nullptr;
    *this = rhs;
}

template <class T>
AvlTree<T>::~AvlTree()
{
    makeEmpty();
}


//template<class T>
//AvlNode<T>* AvlTree<T>::findMin(AvlNode<T>* node) const
//{
//    if (node != nullptr)
//    {
//        while (node->left != nullptr)
//            node = node->left;
//    }
//    return node;
//}

//template <class T>
//const T& AvlTree<T>::findMin() const
//{
//    return findMin(root)->data;
//}

//template<class T>
//AvlNode<T>* AvlTree<T>::findMax(AvlNode<T>* node) const
//{
//    if (node != nullptr)
//    {
//        while (node->right != nullptr)
//            node = node->right;
//    }
//    return node;
//}


//template <class T>
//const T& AvlTree<T>::findMax() const
//{
//    if (isEmpty())
//        throw Underflow
//    return findMax(root)->data;
//}

//function that traverses the tree to test if it contains an element
template <class T>
bool AvlTree<T>::contains(const T & data)
{
    return contains( data, root );
}

//function that checks if the tree is empty
template <class T>
bool AvlTree<T>::isEmpty() const
{
    if (root == nullptr)
        return true;
    else
        return false;
}


//public function that calls the private function to empty the tree
template <class T>
void AvlTree<T>::makeEmpty()
{
    makeEmpty( root );
}


//private function that goes through the AVL Tree in post order and delete each node
template <class T>
void AvlTree<T>::makeEmpty(AvlNode<T> *& node)
{
    //call the function for left and right node
    //untile both are nullptr, then delete the node
    if (node != nullptr)
    {
        makeEmpty(node->left);
        makeEmpty(node->right);
        delete node;
    }
    node = nullptr;
}



//external insert function that the user calls to insert data to the tree
template <class T>
void AvlTree<T>::insert( const T & data)
{
    //calls the internal insert function and passes the root node
    insert( data, root );
    NumberofNodes++;

}



//function that returns the height of the subnode, or -1 if nullptr
template <class T>
int AvlTree<T>::height( AvlNode<T>* node)
{
    return node == nullptr ? -1 : node->height;
}



//function takes two integers and returns the greater of the two
template <class T>
int AvlTree<T>::max( int lhs, int rhs)
{
    return ( lhs > rhs ? lhs : rhs );
}



/* Private function to insert an element into the AVL Tree.
 * Function is recursive and runs from the bottom element in the path.
 * Function also calls rotation functions to set new root of the subtree.
 */
template <class T>
void AvlTree<T>::insert(const T& x, AvlNode<T>*& node)
{
    //if the node is empty, add a new node to the tree
    if (node == nullptr)
        node = new AvlNode<T>(x, nullptr, nullptr);

    //else if the element is smaller than the current data, move to left node
    else if (node->data > x)
    {
        insert(x, node->left);
        //if the tree is not balanced
        if( height(node->left) - height(node->right) == 2 )
        {

            if ( node->left->data > x)
                //case 1 rotation
                rotateL(node);
            else
                //case 2 rotation
                doubleL(node);

        }
    }
    // else if the element is greater than the current dat, move to right node
    else if ( x > node->data)
    {
        insert(x, node->right);
        //if the tree is not balanced
        if ( height(node->right) - height(node->left) == 2 )
        {
            //case 4 rotation
            if ( x > node->right->data)
                rotateR(node);
            //case 3 rotation
            else
                doubleR(node);
        }
    }

    //set the height to the greater of the two children and add 1 for the parent node
    node->height = max(height(node->left), height(node->right)) + 1;
}

//Rotate binary tree with the left child. Case 1.
template <class T>
void AvlTree<T>::rotateL( AvlNode<T>*& k2)
{
    //set k1 to the right node of k2
    AvlNode<T>* k1 = k2->left;

    //rotate so k1 is the new root for that subtree
    k2->left = k1->right;
    k1->right = k2;

    //adjust each subtree for height and reassign k2 to root of subtree
    k2->height = max( height(k2->right) , height(k2->left) ) + 1;
    k1->height = max( height(k1->right) , height(k1->left) ) + 1;
    k2 = k1;
}

//Rotate binary tree with the right child. Case 4.
template <class T>
void AvlTree<T>::rotateR( AvlNode<T>*& k2)
{
    //set k1 to the right node of k2
    AvlNode<T>* k1 = k2->right;

    //rotate so k1 is the new root for that subtree
    k2->right = k1->left;
    k1->left = k2;

    //adjust each subtree for height and reassign k2 to root of subtree
    k2->height = max( height(k2->right) , height(k2->left) ) + 1;
    k1->height = max( height(k1->right) , height(k1->left) ) + 1;
    k2 = k1;
}

//Rotate binary tree with right child of first left child. Case 2.
template <class T>
void AvlTree<T>::doubleL( AvlNode<T>*& k1)
{
    rotateR(k1->left);
    rotateL(k1);
}

//Rotate binary tree with left child of first right child. Case 3.
template <class T>
void AvlTree<T>::doubleR( AvlNode<T>*& k1)
{
    rotateL(k1->right);
    rotateR(k1);
}


//recursive function that takes the root node of one tree and copies its nodes
template <class T>
AvlNode<T>* AvlTree<T>::clone(AvlNode<T> *rhs) const
{
    if (rhs == nullptr)
        return nullptr;
    else
        return new AvlNode<T>(rhs->data, clone( rhs->left ), clone( rhs->right ), rhs->height);
}

//assignment operator
template <class T>
AvlTree<T>& AvlTree<T>::operator=(AvlTree<T>& rhs)
{
    //function clears the nodes in the tree already
    //it then calls the clone function to copy the other tree
    if (this != &rhs)
    {
        makeEmpty();
        root = clone( rhs.root );
    }

    return *this;
}

//internal function that is recursively called to find the
template<class T>
bool AvlTree<T>::contains( const T& x, AvlNode<T>*& node)
{
    //if the node is nullptr, no data found
    if(node == nullptr)
        return false;

    //if the data in the node is greater than the target
    //call contains for the left node (smaller values)
    else if( node->data > x)
        return contains( x, node->left );

    //opposite of above
    else if(x > node->data)
        return contains( x, node->right);

    //if all of those are untrue, the value is within the node
    else
        return true;
}

//External function that calls the recursive printNode function
template<class T>
void AvlTree<T>::printTree()
{
    printNode( root );
}

//function that prints the data at a specific node
//recursive so that the entire tree is printed when printTree is called
//this is an InOrder print
template<class T>
void AvlTree<T>::printNode(AvlNode<T>* node)
{
    //if the node is nullptr it prints nothing
    if ( node != nullptr)
    {
        //print in Order
        printNode(node->left);
        cout << node->data << " ";
        printNode(node->right);
    }
}

//function returns the object given another object that is used to search for it
//paired with contains method to ensure that the value is in the tree
template<class T>
T& AvlTree<T>::getNode(const T &find)
{
    getNode(find, root);
}

template<class T>
T& AvlTree<T>::getNode(const T &x, AvlNode<T> *& node)
{
    if (node != nullptr)
    {
        if (x > node->data)
            return getNode(x, node->right);
        else if (node->data > x)
            return getNode(x, node->left);
        else
            return node->data;
    }
}

template<class T>
int AvlTree<T>::getNumNodes()
{
    return NumberofNodes;
}

