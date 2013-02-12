/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
  head = tail = NULL;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
    while (head != NULL) {
        cellT *next = head->next;
        delete head;
        head = next;
    }
}

int DoublyLinkedListPriorityQueue::size() {
    int size = 0;
    for (cursorA = head; cursorA != NULL; cursorA = cursorA->next)
        size++;
    return size;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	return(head==NULL);
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	cellT *newName = new cellT;
    newName -> name = value;
    newName -> previous = NULL;
    if (isEmpty()){
        // CASE 1: LINKED LIST IS EMPTY
        newName -> next = NULL;
        head = tail = newName;
    } else {
        newName -> next = head;
        head->previous=newName;
        head = newName;
    }
}

string DoublyLinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Can't peek empty Priority Queue");
    string min = head ->name;
    if (size()==1){
        return min;
    }
    // determine minimum element
    for (cursorA = head; cursorA != NULL; cursorA = cursorA->next){
        if (cursorA->name < min) {
            min = cursorA->name;
        }
    }
    return min;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
    if (isEmpty()) error("Can't dequeue empty Priority Queue");
    // initialise string min
    string min = head ->name;
    
    // special case: queue has one element
    if (size()==1){
        while (head != NULL) {
            cellT *next = head->next;
            delete head;
            head = next;
        }
        return min;
    }
    
    // determine minimum element
    for (cursorA = head; cursorA != NULL; cursorA = cursorA->next){
        if (cursorA->name < min) {
            min = cursorA->name;
        }
    }
    // special case: queue has two elements
    if (size()==2){
        if (head->name== min){
            delete head;
            tail -> previous = NULL;
            head = tail;
        } else {
            delete tail;
            head -> next = NULL;
            tail=head;
        }
        return min;
    }
    
    // remove element and update next and previous for elements around it
    for (cursorA = head; cursorA != NULL; cursorA = cursorA->next){
        if (cursorA->name == min){
            // special case if min elem is head
            if (cursorA==head){
                cellT*oldHead=head;
                head=head->next;
                head->previous=NULL;
                delete oldHead;
                return min;
            }
            // special case if min elem is tail
            if(cursorA==tail){
                cellT*oldTail=tail;
                tail=tail->previous;
                tail -> next = NULL;
                delete oldTail;
                return min;
            }
            // for cases where min elem is not head or tail
            cellT *previousCell = cursorA->previous;
            cellT *nextCell = cursorA->next;
            previousCell -> next = cursorA->next;
            nextCell -> previous = cursorA->previous;
            delete cursorA;
            return min;
        }
    }
}

