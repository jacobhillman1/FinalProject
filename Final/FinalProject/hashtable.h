#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "dsvector.h"
#include "linkedlist.h"
#include <utility>

template<class Key, class Value>
class HashTable
{
public:
    HashTable();
    void insertValue( Key& , Value& );
    Value& getValue( Key& ) const;

private:

    DSVector< LinkedList< pair< Key, Value > >* > hashtable;
    int size = hashtable.size();
    int filledKeys = 0;

    void rehash();
    void hashKey( Key& );

};

#endif // HASHTABLE_H
