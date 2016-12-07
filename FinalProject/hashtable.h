/* This class is a templated hash table. It performs all of the basic functions of a hashtable and
 * allows users to use any type for key and value.
 *
 * Author: Will Kastner & Jacob Hillman
 *
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "dsvector.h"
#include "dsstring.h"
#include "linkedlist.h"
#include <utility>
#include <functional>
#include <stdlib.h>

using namespace std;

namespace std{
    template<> struct hash<DSString>
    {
        typedef DSString argument_type;
        typedef size_t result_type;
        result_type operator() (argument_type & str) const
        {
            result_type const h1 = std::hash<string> {} (string(str.c_str()));
            return h1;
        }
    };
}

template<class Key, class Value>
class HashTable
{
public:
    HashTable();
    void insertValue(Key , Value );
    Value& getValue(Key );
    size_t getKeyHash(Key& );
    bool isEmpty();
    void clearHash();
    int getCap();
    int getNumEntries();
    ~HashTable();

private:

    LinkedList< pair< Key, Value > >** hashtable = nullptr;
    int capacity = 10;
    int filledKeys = 0;
    int numEntries = 0;
    double loadFactor = 0.6;

    void rehash();
    int hashKey(Key& , int cap);

};

#endif // HASHTABLE_H

//default constructor sets the capacity of the hashtable and a loadfactor of 0.75;
template<class Key, class Value>
HashTable<Key , Value>::HashTable()
{
    hashtable = new LinkedList< pair < Key, Value > > *[capacity];
    for (int i = 0; i < capacity; i++)
    {
        hashtable[i] = nullptr;
    }
}

//function to inserting key/value pairs into the hashtable
template<class Key, class Value>
void HashTable<Key, Value>::insertValue(Key key , Value value)
{
    //make a pair for key and value to input in linked list
    pair<Key, Value> newPair = make_pair(key, value);

    //hash the key and mod it by the capacity to get ther index #
    int index = hashKey(key, capacity);

    //if the linked list pointer for that index number is nullptr
    //create a new dynamically allocated linked list;
    if ( hashtable[index] == nullptr )
    {
        hashtable[index] = new LinkedList< pair< Key, Value > >;
        filledKeys++;
    }

    //input the new pair into the linked list
    hashtable[index]->add(newPair);
    numEntries++;

    //determine the number of filled keys in the hashtable
    double rehashFactor = static_cast<double>(filledKeys)/static_cast<double>(capacity) + 0.001;

    //test to see if table needs to be rehashed
    if ( rehashFactor > loadFactor)
    {
        rehash();
    }
}

//function that returns the value paired to a given key
template<typename Key, class Value>
Value& HashTable<Key, Value>::getValue(Key key)
{
    //find the index number for the key
    int index = hashKey(key, capacity);

    //loop through the linked list of that index until the key is found
    for ( int i = 0 ; i < hashtable[index]->size(); i++)
    {

        //return the value when the key is found
        if (hashtable[index]->get(i).first == key)
            return hashtable[index]->get(i).second;

    }

}

//function that returns the index after the key is hashed
template<class Key, class Value>
size_t HashTable<Key, Value>::getKeyHash(Key& key)
{
    return hashKey(key);
}

//function that returns the capacity ( # of slots ) in the hashtable
template<class Key, class Value>
int HashTable<Key, Value>::getCap()
{
    return capacity;
}

//function that returns the number of entries in the hashtable
template<class Key, class Value>
int HashTable<Key, Value>::getNumEntries()
{
    return numEntries;
}


//function checks whether the hashtable is empty or not
template<class Key, class Value>
bool HashTable<Key, Value>::isEmpty()
{
    for (int i = 0; i < capacity; i++)
    {
        if (hashtable[i] != nullptr)
            return false;
    }
    return true;
}

//function that clears the hashtable
template<class Key, class Value>
void HashTable<Key, Value>::clearHash()
{
    //loop that clears each linked list in the hashtable
    for (int i = 0; i < capacity; i++)
    {
        if (hashtable[i] != nullptr)
        {
            hashtable[i]->clear();
            hashtable[i] = nullptr;
            filledKeys--;
        }
    }
    numEntries = 0;
}

//rehash function that rehashes the table after it reaches the load factor
template<class Key, class Value>
void HashTable<Key, Value>::rehash()
{
    //set a new capacity of 1.5 times the previous
    //and create a new hashtable of that size;
    int newcap = static_cast<double>(capacity) * 1.5;
    int newFK = 0;
    int newNum = 0;

    //create a new hashtable with the new capacity
    LinkedList< pair< Key, Value > >** temp = new LinkedList < pair < Key, Value > > *[newcap];
    for (int i = 0; i < newcap; i++)
    {
        temp[i] = nullptr;
    }


    //loop through the hashtable and rehash each element with the new capacity
    //and assign it to a linked list of the corresponding index number
    //delete the linked list and reassign the hashtable to the temp;
    for (int i = 0; i < capacity; i++)
    {
        if (hashtable[i] != nullptr)
        {
            for (int j = 0; j < hashtable[i]->size(); j++)
            {

                int newIndex = hashKey(hashtable[i]->get(j).first, newcap);

                if (temp[newIndex] == nullptr)
                {
                    temp[newIndex] = new LinkedList< pair < Key, Value > >;
                    newFK++;
                }

                temp[newIndex]->add(hashtable[i]->get(j));
                newNum++;

            }
            delete hashtable[i];
            hashtable[i] = nullptr;
        }
    }

    //delete the old hashtable and reassign all variables
    hashtable = temp;
    capacity = newcap;
    filledKeys = newFK;
    numEntries = newNum;

    temp = nullptr;
    delete[] temp;

}

//function that hashes the key and returns the index of the hashed key
//modded by the capacity that is chosen
template<class Key, class Value>
int HashTable<Key, Value>::hashKey(Key & key, int cap)
{
    size_t keyHash = std::hash<Key>{}(key);
    return keyHash % cap;
}


//destructor for the hashtable
template<class Key, class Value>
HashTable<Key, Value>::~HashTable()
{
    //loop through the hashtable deleting the linked lists
    for (int i = 0; i < capacity; i++)
    {
        if (hashtable[i] != nullptr)
            delete hashtable[i];
    }

    //delete the hashtable
    delete[] hashtable;
}
