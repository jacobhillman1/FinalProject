
//#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "linkedlist.h"
#include "dsstring.h"
#include "hashtable.h"

TEST_CASE("HashTable class", ""){
    DSString keys[10];
    keys[0] = DSString("key") + "0";
    keys[1] = DSString("key") + "1";
    keys[2] = DSString("key") + "2";
    keys[3] = DSString("key") + "3";
    keys[4] = DSString("key") + "4";
    keys[5] = DSString("key") + "5";
    keys[6] = DSString("key") + "6";
    keys[7] = DSString("key") + "7";
    keys[8] = DSString("key") + "8";
    keys[9] = DSString("key") + "9";

    DSString values[10];
    values[0] = DSString("value") + "0";
    values[1] = DSString("value") + "1";
    values[2] = DSString("value") + "2";
    values[3] = DSString("value") + "3";
    values[4] = DSString("value") + "4";
    values[5] = DSString("value") + "5";
    values[6] = DSString("value") + "6";
    values[7] = DSString("value") + "7";
    values[8] = DSString("value") + "8";
    values[9] = DSString("value") + "9";

    HashTable<DSString, DSString> table;
    HashTable<int, double> nums;

    for (int i = 0; i < 20; i++)
    {
        nums.insertValue( i , (static_cast<double>(i) * 1.01));
    }

    for (int i = 0 ; i < 4; i++)
    {
        table.insertValue(keys[i], values[i]);
    }

    HashTable<DSString, DSString> empty;


    SECTION("constructor & insertValue, isEmpty, getCap, getNumEntries")
    {
        REQUIRE(empty.isEmpty() == true);
        REQUIRE(table.isEmpty() == false);
        REQUIRE(nums.isEmpty() == false);
        REQUIRE(empty.getCap() == 10);
        REQUIRE(table.getCap() == 10);
        REQUIRE(nums.getCap() == 49);
        REQUIRE(empty.getNumEntries() == 0);
        REQUIRE(table.getNumEntries() == 4);
        REQUIRE(nums.getNumEntries() == 20);
    }

    SECTION("getValue")
    {
        DSString str("key0");
        REQUIRE(table.getValue(str) == "value0");
        str = "key1";
        REQUIRE(table.getValue(str) == "value1");
        str = "key2";
        REQUIRE(table.getValue(str) == "value2");
        str = "key3";
        REQUIRE(table.getValue(str) == "value3");
        REQUIRE(nums.getValue(1) == 1.01);
        REQUIRE(nums.getValue(19) == 19.19);
        REQUIRE(nums.getValue(13) == 13.13);

    }

//    SECTION("removeValue")
//    {
//        DSString str("key0");
//        REQUIRE(table.removeValue(str) == "value0");
//        str = "key1";
//        REQUIRE(table.getValue(str) == "value1");
//        REQUIRE(table.getCap() == 10);
//        REQUIRE(table.getNumEntries() == 3);
//        REQUIRE(table.isEmpty() == false);
//    }

    table.clearHash();

    SECTION("clearHash")
    {
        REQUIRE(table.getCap() == 10);
        REQUIRE(table.isEmpty() == true);
        REQUIRE(table.getNumEntries() == 0);
    }



    //only works for load factor set <= 0.6
    SECTION("test rehash function")
    {
        for (int i = 0 ; i < 10; i++)
        {
            table.insertValue(keys[i], values[i]);
        }

        DSString str("key0");
        REQUIRE(table.getCap() == 15);
        REQUIRE(table.isEmpty() == false);
        REQUIRE(table.getNumEntries() == 10);
        REQUIRE(table.getValue(str) == "value0");
        str = "key2";
        REQUIRE(table.getValue(str) == ("value2"));
        str = "key3";
        REQUIRE(table.getValue(str) == "value3");
        str = "key7";
        REQUIRE(table.getValue(str) == "value7");
        str = "key8";
        REQUIRE(table.getValue(str) == "value8");
        str = "key9";
        REQUIRE(table.getValue(str) == "value9");
    }

}




