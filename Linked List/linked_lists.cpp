// Part 1: Build Your Own Linked List
// Implement and test a linked list that supports adding, searching, removing, and printing items. Do not use std::list.

// Part 2: Monopoly Game
// Use your linked list to build a circular board with at least 10 properties.

// Each property stores its name, cost, owner, and next pointer.

// Players move around the board, wrapping back to the beginning.

// Players can buy unowned properties, but cannot buy owned properties.

// Simulate at least 10 turns and print the results 

// BONUS POINTS IF YOU CREATE A GUI




// Nicolas Holmgren
// CS 210-11



#include <vector>
#include <iostream>
using namespace std;



// void linked_list()

// -----------PART 1 ------------------------------------------------
// Linked list

// first we build the struct called Node, the node is what stores the data
// we give it an int value which is the value being stored in the node.
// and then the node points to the next node in the list
struct Node {
    int value; 
    Node* next;
};



// We need to add adding to the list, deleting from list, searching the list, and printing items


void append(Node*& head, int newValue) {
    // when we append something we are adding it in to the list
    // we will append items at the end, so at the tail

    Node* appendValue = new Node{newValue, nullptr};


    if (head == nullptr) {
        head = appendValue;
        return;
    }

    Node* current = head;

    while (current->next != nullptr) {
        current = current->next;
    }

    current->next = appendValue;

}

// Search function
bool search(Node*& head, int searchValue) {
    // we will have to loop through the list to search for an item

    Node* current = head;

    while (current != nullptr) {
        if (current->value == searchValue) {
            return true;
        }

        current = current->next;
    }

    return false;
}

// deleteItem
void deleteItem(Node*& head, int deleteValue) {
    // in this case we will need to search and then if item found we remove item

    // we will need two pointers current and prev

    if (head == nullptr) return;


    // if the vlaue we want to delete is in the head then we go ahead and delete it, no more operations required
    if (head->value == deleteValue) {
        Node* deleteThisValue = head;

        head = head->next;
        delete deleteThisValue;
    }

// we create prev and current, prev is set to nullptr and current to head
    Node* prev = nullptr;
    Node* current = head;

// search through the list to find the value we want to delete
    while (current != nullptr && current->value != deleteValue) {

        prev = current;
        current = current->next;
    }


    if (current == nullptr) return;


    // more to work through tmrw

    prev->next = current->next;


    delete current;
}

// Print, we loop through the linked list and print the contents in each node
void printList (Node*& head) {

// set current node to the head to start at the begining of list
    Node* current = head;

    // loop through as long as current not nullptr
    while (current != nullptr) {
        // print value of current node
        cout << current->value << " ->";

        // keep moving to next node 
        current = current->next;
    }

    // once we break out of loop we print that we have reached nullptr
    cout << "nullptr" << endl;
}



int main () {

    // Method A
//     Node* first = new Node{100, nullptr};
//     Node* second = new Node{200, nullptr};
//     Node* third = new Node{300, nullptr};


//     first->next = second;
//     second->next = third;


// // Method B
//     Node* chain = new Node{100, new Node{200, new Node{300, nullptr}}};


//     cout << first << endl;

//     cout << chain->value;

    Node* head = nullptr;

    cout << "------ Append test case -------" << endl; 
    append(head, 200);
    append(head, 320);
    append(head, 354);
    append(head, 19283);



    cout << "List should be 100 -> 200 -> 300 -> nullptr" << endl;
    cout << "Actual list: ";


    printList(head);

    cout << endl;


    cout << "------------ Testing Search function ------------" << endl;

    cout << "expected 1: " << search(head, 320) << endl;
    cout << "expected 0: " << search(head, 2) << endl;



    cout << "--------------- Testing delete item--------------" << endl;

    deleteItem(head, 19283);
    printList(head);

    cout << "removing the head " << endl;

    deleteItem(head, 200);
    printList(head);

    return 0;

};