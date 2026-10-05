#include <vector>
#include <iostream>
using namespace std;



// void linked_list()


// Linked list

struct Node {
    int value; 
    Node* next;
    Node* prev;
};


int main () {

    // Method A
    Node* first = new Node{100, nullptr};
    Node* second = new Node{200, nullptr};
    Node* third = new Node{300, nullptr};


    first->next = second;
    second->next = third;


    // prev

    first->prev = third;
    second->prev = first;
    third->prev = second;



// Method B
    Node* chain = new Node{100, new Node{200, new Node{300, nullptr}}};


    cout << first << endl;

    cout << chain->value;

};




// Proper linked list



// Using method, using .insert better way of building a linked list, make change from insert to append 
// we can also prepend apart from appending, where it will append at the start of the list.