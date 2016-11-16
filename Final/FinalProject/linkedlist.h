/*LinkedList and ListNode classes for a Doubly Linked List.
 * Author: Will Kastner
 * Date: 9/25/2016
 */

#ifndef LINKED_LIST
#define LINKED_LIST

#include <iostream>

using namespace std;

template <class T>
class ListNode {
    template<class U> friend class LinkedList;
public:

    ListNode<T>();
    ListNode<T>(T);

private:
    ListNode<T>*next = nullptr;
    ListNode<T>*prev = nullptr;
    T data;
};


template<class T>
class LinkedList {

public:

    LinkedList();
    LinkedList(T);
    LinkedList(const LinkedList<T>&);

    void add(T);
    void addToFront(T);
    T& get(int);
    int size();
    T remove(int);
    bool isEmpty();

    ~LinkedList();

    T& operator[] (int);
    LinkedList<T>& operator=(const LinkedList<T>&);

private:

    ListNode<T>*front = nullptr;
    int num_elements = 0;
    ListNode<T>*tail = nullptr;
    ListNode<T>*curr = nullptr;

};

//default list node constructor
template<typename T>
ListNode<T>::ListNode()
{
    prev = next = nullptr;
}

//List node constructor adds  the data
template<typename T>
ListNode<T>::ListNode(T val): next(nullptr), prev(nullptr)
{
    data = val;
}

//default constructor, no elements added
template <typename T>
LinkedList<T>::LinkedList()
{
    num_elements = 0;
    front = tail = curr = nullptr;
}

//Constructor that adds first node
template <typename T>
LinkedList<T>::LinkedList(T val)
{
    front = new ListNode<T>(val);
    tail = front;
    num_elements = 1;
}

//copy constructor
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T> & rhs)
{
    if (rhs.front == nullptr)
    {
        curr = tail = front = nullptr;
        num_elements = rhs.num_elements;
    }
    else
    {
        //create temporary pointer to traverse passed List
        ListNode<T> *temp = rhs.front;
        //copy size
        num_elements = rhs.num_elements;

        if (rhs.front != nullptr){
            //copy first element of list
            front = new ListNode<T>(rhs.front->data);
            curr = front;
        }

        while(temp->next != nullptr)
        {
            //create node of next element and set same data
            curr->next = new ListNode<T>(temp->next->data);

            //next node is tail
            tail = curr->next;

            //assign current node as previous node for next element
            curr->next->prev = curr;

            //advance curr and temporary pointer
            curr = curr->next;
            temp = temp->next;
        }
    }

}

//function that adds a node to the end of the list
template <typename T>
void LinkedList<T>::add(T val)
{
    //if no elements yet add new element
    if(front == nullptr)
    {
        front = new ListNode<T>(val);
        num_elements++;
        tail = front;
    }
    //else loop through the list until the tail is reached
    else
    {
        curr = front;
        while(curr->next != nullptr)
            curr = curr->next;

        //create a new Node at the end of the list and assign the data to it
        curr->next = new ListNode<T>(val);
        curr->next->prev = curr;
        tail = curr->next;
        num_elements++;
    }
}

//function that adds a node to the front of the list
template <typename T>
void LinkedList<T>::addToFront(T val)
{
    //if first element
    if(front == nullptr)
    {
        //create new element at front
        front = new ListNode<T>(val);
        num_elements++;
        tail = front;
    }
    //else add a new element to the end and shift all elements to the back
    //(setting the new value to front node)
    else
    {
        curr = tail;

        //add a new Listnode to the tail
        curr->next = new ListNode<T>(curr->data);
        num_elements++;

        //assign the new ListNode's previous pointer
        curr->next->prev = curr;

        //reassign tail pointer
        tail = curr->next;
        curr = curr->next;

        //move all Nodes back one position
        while(curr->prev != nullptr)
        {
            curr->data = curr->prev->data;
            curr = curr->prev;
        }

        //assign value to first nodes data
        curr->data = val;
    }
}

//loop through list and return data at given position
template <typename T>
T& LinkedList<T>::get(int pos)
{
    curr = front;
    for(int i = 0; i < pos; i++)
    {
        curr = curr->next;
    }
    return curr->data;
}

//return the number of elements in the list
template <typename T>
int LinkedList<T>::size()
{
    return num_elements;
}

//function removes a list node at a specific position
template <typename T>
T LinkedList<T>::remove(int pos)
{
    curr = front;
    //go to node at given position
    for(int i = 0; i < pos; i++)
    {
        curr = curr->next;
    }

    //assign previous and next pointers to nodes on either side of node to be removed
    if (curr != tail)
    {
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
    }

    else if (curr != front)
    {
        curr->prev->next = curr->next;
        tail = curr->prev;
    }

    T ret = curr->data;
    //delete node and return the data inside it.
    delete curr;
    curr = front;
    num_elements--;
    if(num_elements == 0)
        front = nullptr;
    return ret;
}

//function checks whether or not there are nodes in the list
template <typename T>
bool LinkedList<T>::isEmpty()
{
    if(front == nullptr)
        return true;
    else if (num_elements > 0)
        return false;
    else
        return true;
}

//go through list deleting each node
template <typename T>
LinkedList<T>::~LinkedList()
{
    curr = front;

    while(curr != nullptr)
    {
        //set temp to next listnode and delete the current node.
        ListNode<T>* temp = curr->next;
        delete curr;
        num_elements--;
        //set curr pointer to next listnode
        curr = temp;

    }
}

//Subscript operator: run through list x times to return value.
template <typename T>
T& LinkedList<T>::operator [](int pos)
{
    curr = front;
    for(int i = 0; i < pos; i++)
    {
        curr = curr->next;
    }
    return curr->data;
}

//assignment operator
template <typename T>
LinkedList<T>& LinkedList<T>::operator =(const LinkedList<T>& rhs)
{
    if(this != &rhs)
    {
        //if copying an empty list
        if(rhs.front == nullptr)
        {
            //delete the current contents of the list
            curr = front;
            while(curr != nullptr)
            {
                //set temp to next listnode and delete the current node.
                ListNode<T>* temp = curr->next;
                delete curr;
                //set curr pointer to next listnode
                curr = temp;
            }
            //assign nullptr to nodes
            curr = front = tail = nullptr;
            num_elements = 0;
        }

        else
        {
            ListNode<T> *temp = rhs.front;
            num_elements = rhs.num_elements;

            //copy first element of list
            if (rhs.front != nullptr){
                front = new ListNode<T>(rhs.front->data);
                curr = front;
                tail = front;
            }

            while(temp->next != nullptr)
            {
                //create node of next element and set same data
                curr->next = new ListNode<T>(temp->next->data);
                tail = curr->next;

                //assign current node as previous node for next element
                curr->next->prev = curr;

                //advance curr and temporary pointer
                curr = curr->next;
                temp = temp->next;
            }
        }
    }
    return *this;
}

#endif

