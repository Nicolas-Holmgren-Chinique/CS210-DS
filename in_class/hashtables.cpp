


// A hashtable is a data structure which stores a key and a value


// we get the key and then we pass it through a hashfunction,

// We use modulo (%) to divide by the array size and then we get the remainder which comes from
// the % and with that remainder it determines the index that the key value pair will be stored

// because their can be collisions we will use a linked list in each of the buckets (indices)
// If an already occupied bucket(index) requires another key value pair to be placed then we send that
// key value pair to the linked list that is pointing to at the that array index




#include <iostream>
#include <string>
using namespace std;

// HashNode class
struct HashNode {

    int key;
    string value;

    HashNode* next; //pointer to next node


// constructor of the HashNode class, this will allow us to create new nodes later on
    HashNode(int k, string v) {

        key = k;
        value = v;

        next = nullptr;
    }
};




