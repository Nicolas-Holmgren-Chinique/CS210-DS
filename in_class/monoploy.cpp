#include <vector>
#include <iostream>
using namespace std;



// void linked_list()


// Linked list

struct Node {
    int value; 
    Node* next;
};


int main () {

    // Method A
    Node* first = new Node{100, nullptr};
    Node* second = new Node{200, nullptr};
    Node* third = new Node{300, nullptr};


    first->next = second;
    second->next = third;
    third->next = first;


// Method B
    Node* chain = new Node{100, new Node{200, new Node{300, nullptr}}};


    cout << first << endl;

    cout << chain->value;

};