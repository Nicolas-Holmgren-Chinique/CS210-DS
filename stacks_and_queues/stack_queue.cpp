
#include <iostream>

using namespace std;

// Nicolas Holmgren
// 10/07/2026
// I used github copilot and gmeini to finish off the test cases 

// 
// What its a stack?
// A stack is a data structure that uses either an Array, or a linked list

// We place something in the stack and the last thing placed in the stack is removed


// Linked list, head [1] -> [4] -> [8] -> [13] tail 
// Items are pushed to the head, and then once we begin removing the last thing pushed is removed from the head

// What we need, linked list, and then we need to be able to perform different actions


// Pop(remove from the head, last in first out)
// push (add items to the linked list)
// is empty (checks if the linked list is empty)
// peek Looks inside the linked list 

// Linked list class
struct Node {
    // value of the node
    int value;
    // pointer of the node which points to the next node
    Node* next;
};



// Stack

// Linked list implementation of a stack
class StackLinkedList {

public:
    // Pop function
    // pop from the head
    int pop(Node*& head) {

        if (head != nullptr) {

            // temp 
            Node* deleteThisValue = head;

            // save the value so that we can return the value being removed from the stack
            int saveValue = deleteThisValue->value;

            head = head->next;
            delete deleteThisValue;

            return saveValue;
        }

        return 0;

    }

    // Push function
    // push to the head
    void push(Node*& head, int pushValue) {

        Node* newNode = new Node;
        newNode->value = pushValue;
        newNode->next = head;

        // set the newNode to the top of the stack by assigning it to the head
        head = newNode;

    }

    // Checks if the list is empty
    bool isEmpty(Node* head) {

        // check if the head is nullptr if so then return true, it is empty
        if (head == nullptr) {
            return true;
            // else it has itmes and therefore not empty
        } else {
            return false;
        }
    }

// checks the first itme in the list
    int peek(Node*&head) {

        if (isEmpty(head)) {
            return 0;
        }
// return the value of head, so at the top of the stack
        return head->value;
    }

};


// Array implementation of a stack
class StackArray {

    private:
        int* arr;
        int capacity;
        int topIndex;


    public:
    // constructor
        StackArray(int size) {
            capacity = size;
            arr = new int[capacity];
            topIndex = -1;
        }


        ~StackArray() {
            delete[] arr;
        }


        bool isEmpty() {
            return topIndex == -1;
        }

        bool isFull() {
            return topIndex == capacity - 1;
        }

        void push(int value) {
            if (isFull()) {
                return;
            }

            topIndex++;

            arr[topIndex] = value;
        }

        int pop() {
            if (isEmpty()) {
                return 0;
            }

            int savedValue = arr[topIndex];
            topIndex--;
            return savedValue;
        }

        int peek() {
            if (isEmpty()) {
                return 0;
            }

            return arr[topIndex];
        }
};




// Queue 

// In a queue the first item pused is the first item removed 

class QueueLinkedList {
    
    struct Node {
        int value;
        Node* next;
    };

    Node* front;
    Node* rear;
    
    public:

        QueueLinkedList() {
            front = nullptr;
            rear = nullptr;
        }

        bool isEmpty() {
            return front == nullptr;
        }

        void enqueue(int value) {
            Node* newNode = new Node;
            newNode->value = value;
            newNode->next = nullptr;

            if (isEmpty()) {
                front = newNode;
                rear = newNode;
            } else {
                rear->next = newNode;
                rear = newNode;
            }
        }

        int dequeue() {
            if (isEmpty()) {

                return 0;
            }

            Node* temp = front;
            int savedValue = temp->value;

            front = front->next;


            if (front == nullptr) {
                rear = nullptr;
            }

            delete temp;
            return savedValue;
        }

        int peek() {
            if (isEmpty()) {
                return 0;
            }

            return front->value;
        }



    // arry implementation of a queue

    class QueueArray {
        private:
            int* arr;
            int capacity;
            int frontIndex;
            int backIndex;
            int count;


        public:

            QueueArray(int size) {
                capacity = size;
                arr = new int[capacity];
                frontIndex = 0;
                backIndex = -1;
                count = 0;
            }

            ~QueueArray() {
                delete[] arr;
            }


            bool isEmpty() {
                return count == 0;
        
            }

            bool isFull() {
                return count == capacity;
            }

            void enqueue(int value) {
                if (isFull()) {
                    return;
                }

                backIndex = (backIndex + 1) % capacity;
                arr[backIndex] = value;
                count++;
            }


            int dequeue() {
                if (isEmpty()) {
                    return 0;
                }

                int savedValue = arr[frontIndex];

                frontIndex = (frontIndex + 1) % capacity;
                count--;

                return savedValue;
            }


            int peek() {
                if (isEmpty()) {
                    return 0;
                }

                return arr[frontIndex];
            }
    };
};


int main() {
    cout << "------- Testing StackArray class ------" << endl;
    StackArray myStack(5); // stack with 5 elements

    cout << "Pushing 2, 19, 34, 8 " << endl;

    myStack.push(2);
    myStack.push(19);
    myStack.push(34);
    myStack.push(8);
    myStack.push(12);
    myStack.push(32);
    myStack.push(9);
    myStack.push(3);

    cout << "Top of stack is: " << myStack.peek() << endl;

    cout << "Popping: " << myStack.pop() << endl;
    cout << "Popping: " << myStack.pop() << endl;
    cout << "Popping: " << myStack.pop() << endl;
    cout << "Popping: " << myStack.pop() << endl;
    cout << "Popping: " << myStack.pop() << endl;


    // Linked list test

    cout << "\n ----Testing StackLinkedList ---- \n";
    
    StackLinkedList linkedStack;
    Node* stackHead = nullptr;


    linkedStack.push(stackHead, 100);
    linkedStack.push(stackHead, 200);
    linkedStack.push(stackHead, 300);
    linkedStack.push(stackHead, 400);
    linkedStack.push(stackHead, 500);
    linkedStack.push(stackHead, 600);


    cout << "Popping: " << linkedStack.pop(stackHead) << "\n"; 
    cout << "Popping: " << linkedStack.pop(stackHead) << "\n"; 
    cout << "Popping: " << linkedStack.pop(stackHead) << "\n"; 
    cout << "Popping: " << linkedStack.pop(stackHead) << "\n"; 
    cout << "Popping: " << linkedStack.pop(stackHead) << "\n"; 
    cout << "Popping: " << linkedStack.pop(stackHead) << "\n";


    // Queue linked list testing


    cout << "--------- Testing queue linked list ------ \n";

    QueueLinkedList linkedQueue;


    cout << "Handling Empty: " << linkedQueue.dequeue() << "\n";


    linkedQueue.enqueue(10);
    linkedQueue.enqueue(20);
    linkedQueue.enqueue(30);
    linkedQueue.enqueue(40);
    linkedQueue.enqueue(50);
    linkedQueue.enqueue(60);


    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing: " << linkedQueue.dequeue() << "\n"; 
    cout << "Dequeuing (Empty): " << linkedQueue.dequeue() << "\n";






    cout << "\n--- Testing Queue Array ---\n";
    QueueLinkedList::QueueArray queueArr(4); // Max capacity of 4 slots
    
    // Op 1: Test empty handling
    cout << "Handling Empty: " << queueArr.dequeue() << "\n"; 
    
    // Ops 2-6: Enqueue items to hit the capacity limit
    queueArr.enqueue(11);
    queueArr.enqueue(22);
    queueArr.enqueue(33);
    queueArr.enqueue(44);
    queueArr.enqueue(55); // Will trigger "Queue is full" warning
    
    // Ops 7-8: Dequeue two items to free up space at the front
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 11
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 22
    
    // Ops 9-10: Enqueue more to demonstrate the Circular wrap-around
    cout << "Enqueuing 66 and 77 (Wrapping around)...\n";
    queueArr.enqueue(66);
    queueArr.enqueue(77);
    
    // Ops 11-15: Empty the rest of the queue
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 33
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 44
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 66
    cout << "Dequeuing: " << queueArr.dequeue() << "\n"; // Removes 77
    cout << "Dequeuing (Empty): " << queueArr.dequeue() << "\n";
}
