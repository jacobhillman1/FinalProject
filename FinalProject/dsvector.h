#ifndef VECTOR
#define VECTOR

#include<iostream>
#include<math.h>
#include<typeinfo>
#include<ostream>
#include"dsstring.h"
#include <stdexcept>

using namespace std;

template<class T>
class DSVector{

  public:

      DSVector();
      DSVector(int);
      DSVector(const DSVector<T>&);

      void add(T);
      void add(T, int);
      void remove();
      void remove(int);
      T get(int);

      ~DSVector();

      int size() const;
      int capacity();
      void sort();
      bool isEmpty();
      void clear();

      T& operator[](int) const;
      DSVector<T>& operator=(const DSVector<T>&) ;
      DSVector<T>& operator+=(DSVector<T>&);
      bool operator==(DSVector<T>& );

      DSVector<T> intersect(DSVector<T>& ) const;
      DSVector<T> unite(DSVector<T>& ) const;
      DSVector<T> subtract(DSVector<T> & ) const;

  private:

      bool checkVctr(T);
      void resize();
      T* data = nullptr;
      int vsize;
      int cap;

};


template<typename T>
DSVector<T>::DSVector(): vsize(0), cap(10)
{
    data = new T[10];
}

template<typename T>
DSVector<T>::DSVector(int c)
{
    cap = c;
    vsize = 0;
    data = new T[c];
}

template<typename T>
//copy constructor
DSVector<T>::DSVector(const DSVector<T>& rhs)

{
    cap = rhs.cap;
    vsize = rhs.vsize;
    data = new T[cap];
    for (int i = 0; i < cap; i++)
    {
        data[i] = rhs.data[i];
    }
}

template<typename T>
void DSVector<T>::add(T val)
{
    //add passed value to the first available element in the vector
    if(this->size() < cap)
    {
        data[this->size()] = val;
        vsize++;
    }
    else
    {
        resize();
        data[vsize] = val;
        vsize++;
    }

}

//add passed value to the first available element in the vector
template<typename T>
void DSVector<T>::add(T val , int pos)
{
    //if the vector is not full add at the first available element
    if(this->size() < cap)
    {
        for (int i = vsize; i > pos; i--)
        {
            data[i] = data[i-1];
        }
        data[pos] = val;
    }
    //if the vector is full, resize, then add
    else {
        resize();
        for (int i = vsize; i > pos; i--)
        {
            data[i] = data[i-1];
        }
        data[pos] = val;
    }
    vsize++;
}

//clear the last available element on the vector.
template<typename T>
void DSVector<T>::remove()
{
    T* temp = nullptr;
    temp = new T[cap];
    for(int i = 0; i < (vsize - 1); i++)
    {
        temp[i] = data[i];
    }
    delete[] data;
    data = temp;
    vsize -= 1;
}

//removes a element at a specific point in vector.
template<typename T>
void DSVector<T>::remove(int pos)
{
    //delete info and create a new array of size cap
    T* temp = nullptr;
    temp = new T[cap];
    int count = 0;
    for(int i = 0; i < vsize; i++)
    {
        //skip over the element that we want to remove
        if (count == pos)
        {
            count++;
        }
         //add data[i] to temp[i] until element to remove
        temp[i] = data[count];
        count++;
    }
    //free memory and assign data to new array.
    delete[] data;
    data = temp;

    vsize--;
    cap = vsize + 1;
}

//returns element in vector
template<typename T>
T DSVector<T>::get(int pos)
{
    return data[pos];
}

//returns number of elements that hold data in vector
template<typename T>
int DSVector<T>::size() const
{
    return vsize;
}

//returns capacity of vector
template<typename T>
int DSVector<T>::capacity()
{
    return cap;
}


//loops find the smallest element in the vector and set that to the first element. Then ignore that smallest
//element and loop through again setting the second smallest element. Done until entire vector is sorted.

//function to sort vector from smallest to largest
template<typename T>
void DSVector<T>::sort()
{
    //if the vector size is one element or less, no need to sort
    if (vsize < 2)
        return;
    //as i increases, the elements being considered in comparisons shrink
    for (int i = 0; i < vsize; i++)
    {
        //if element at smallest/next-smallest is greater than element later in loop
        for(int j =0; j < (vsize - i); j++)
        {
            if (data[i] > data[j+i])
            {
                //replace element value with smaller value.
                T temp = data[j+i];
                data[j+i] = data[i];
                data[i] = temp;
            }
        }
    }
}

//function checks if the vector is empty
template<typename T>
bool DSVector<T>::isEmpty()
{
    if (vsize > 0)
        return false;
    else
        return true;
}

//function clears the vector
template<typename T>
void DSVector<T>::clear()
{
    T* temp = nullptr;
    temp = new T[cap];
    delete[] data;
    vsize = 0;
    data = temp;
}

//function returns the data of specified element in vector
template<typename T>
T& DSVector<T>::operator[](int pos) const
{
    if (pos < cap)
        return data[pos];
    else
        throw out_of_range ("Element out of range");
}

//assignment operator
template<typename T>
DSVector<T>& DSVector<T>::operator=(const DSVector<T>& rhs)
{
    try{
         //assign cap & size of rhs
        cap = rhs.cap;
        vsize = rhs.vsize;
        //make sure data points to nullptr
        if (data != nullptr)
            delete[] data;
        data = nullptr;
        //assign data with array of new capacity
        data = new T[cap];
        //assign data with elements of rhs data
        if (rhs.data != nullptr){
            for (int i = 0; i < cap; i++){
                data[i] = rhs.data[i];
            }
            return *this;
        }
        else
            throw " Assigning vector of data nullptr";
    }
    //if rhs data was nullptr print exception
    catch(DSString e){
        cout << "Exception: " << e << endl;
    }

}

//function adds one vector to the end of the other and adjusts to size
template<typename T>
DSVector<T>& DSVector<T>::operator+=(DSVector<T>& rhs)
{
    //new vectors cap & size is the combination of both vectors'
    int newcap = rhs.cap + cap;
    int newsize = rhs.vsize + vsize; \

    T* temp = nullptr;
    temp = new T[newcap];

    int count = 0;

    //add elements from first array to combined array
    for (int i = 0; i < vsize; i++)
    {
        temp[i] = data[i];
        count++;
    }

    //add elements of second array to combined array
    for (int i = 0; i < rhs.vsize; i++)
    {
        temp[count] = rhs.data[i];
        count++;
    }

    delete[] data;
    //assign attributes of new vector to this object.
    cap = newcap;
    vsize = newsize;
    data = temp;
    return *this;
}

template<class T>
bool DSVector<T>::operator ==(DSVector<T>& rhs)
{

    if (this->vsize != rhs.vsize)
        return false;

    for(int i = 0; i < vsize; i++)
    {
        if (data[i] == rhs.data[i])
            continue;
        else
            return false;
    }

    return true;
}

//function that is called to resize the vector when it reaches capacity
template<typename T>
void DSVector<T>::resize()
{
    //1.5x multiplier to increase capcity by 1.5x
    double mltp = 1.5;

    //set new capacity to 1.5 * current capacity (rounding up)
    int newcap = ceil(mltp * cap);

    //create a new array and fill it with data elements
    T* ptr = new T[newcap];
    for (int i = 0; i < cap; i++)
        ptr[i] = data[i];

    delete[] data;
    data = ptr;
    cap = newcap;
}

//destructor
template<typename T>
DSVector<T>::~DSVector()
{
    delete[] data;
}

template <class T>
DSVector<T> DSVector<T>::intersect(DSVector<T>& vctr) const
{
    DSVector<T> temp;
    T content;

    for( int i = 0; i < vsize; i++)
    {
        for( int j = 0; j < vctr.vsize; j++)
        {
            if(data[i] == vctr.data[j])
            {
                content = data[i];
                temp.add(data[i]);
                break;
            }
        }
    }
    return temp;
}

template<class T>
DSVector<T> DSVector<T>::unite(DSVector<T>& vctr) const
{
    DSVector<T> temp;
    T content;
    int i = 0;
    for( i = 0; i < vsize; i++)
    {
        temp.add(data[i]);
    }
    for( i = 0; i < vctr.vsize; i++)
    {
        if (!temp.checkVctr(vctr.data[i]))
            temp.add(vctr.data[i]);
    }

    return temp;

}

template <class T>
bool DSVector<T>::checkVctr(T val)
{
    for (int i = 0; i < vsize; i++)
    {
        if(data[i] == val)
            return true;
    }

    return false;
}

template <class T>
DSVector<T> DSVector<T>::subtract(DSVector<T> & rhs) const
{
    DSVector<T> temp;

    for ( int i = 0; i < vsize; i++)
    {
        if (!rhs.checkVctr(data[i]))
            temp.add(data[i]);
    }

    return temp;

}
#endif
