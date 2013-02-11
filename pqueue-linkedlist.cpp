/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
    head = tail = NULL;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
    while (head != NULL) {
        cellT *next = head->next;
        delete head;
        head = next;
    }
}

int LinkedListPriorityQueue::size() {
    int size = 0;
    for (cursorA = head; cursorA != NULL; cursorA = cursorA->next)
        size++;
    return size;
}

bool LinkedListPriorityQueue::isEmpty() {
  return(head==NULL);
}

void LinkedListPriorityQueue::enqueue(string value) {
	cellT *newName = new cellT;
    newName -> name = value;
    if (isEmpty()){
    // CASE 1: LINKED LIST IS EMPTY
        newName -> next = NULL;
        head = tail = newName;
    } else if (value <= head -> name) {
     // CASE 2: VALUE <= HEAD
        newName -> next = head;
        head = newName;
    } else if (value >= tail -> name){
        // CASE 3: IF THE VALUE > TAIL->NAME, SPLICE ONTO THE END
        tail -> next = newName;
        newName -> next = NULL;
        tail = newName;
    } else {
        // CASE 4
        for (cursorA = head; cursorA != NULL; cursorA = cursorA->next){
            if (cursorA -> name == value){
                newName->next=cursorA->next;
                cursorA->next = newName;
                break;
            }
            if (cursorA -> name > value){
                // Let cursor B equal the element before cursor A
                cellT *cursorB = head;
                while (cursorB->next != cursorA){
                    cursorB = cursorB-> next;
                }
                cursorB -> next = newName;
                newName -> next = cursorA;
                break;
            }
        }
    }
}


string LinkedListPriorityQueue::peek(){
    if (isEmpty()) error("Can't peek empty Priority Queue");
    string front = head -> name;
    return front;
}

string LinkedListPriorityQueue::dequeueMin() {
    if (isEmpty()) error("Can't dequeue empty Priority Queue");
    string front = head -> name;
    cellT *old = head;
    head = head -> next;
    delete old;
    return front;
}


