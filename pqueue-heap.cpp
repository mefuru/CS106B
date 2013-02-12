/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

HeapPriorityQueue::HeapPriorityQueue() {
    capacity=INITIAL_CAPACITY;
    array = new string[capacity];
    head=0;
    tail = 0;
}

HeapPriorityQueue::~HeapPriorityQueue() {
  delete [] array;
}

int HeapPriorityQueue::size() {
	return (tail+capacity-head) % capacity;
}

bool HeapPriorityQueue::isEmpty() {
	return (head==tail);
}

void HeapPriorityQueue::enqueue(string value) {
    if (head==tail){
        array[head]=value;
        tail++;
    } else {
    if (size()==capacity-1) expandCapacity();
    array[tail]=value;
    bubbleUp(tail);
    tail++;
    }
}

string HeapPriorityQueue::peek() {
    if (isEmpty()) error("Attempting to peek an empty queue");
    return array[head];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Attempting to dequeue an empty queue");
    string result = array[head];
    swap(array[head], array[tail-1]);
    deleteTail();
    bubbleDown(head);
    return result;
}

void HeapPriorityQueue::expandCapacity() {
    int count = size();
    string *oldArray = array;
    array = new string[2*capacity];
    for (int i = 0; i < count; i++){
        array[i] = oldArray[i];
    }
    capacity *=2;
    head=0;
    tail=count;
    delete  [] oldArray;
}

void HeapPriorityQueue::deleteTail(){
    string *oldArray = array;
    array = new string[capacity];
    for (int i = 0; i < tail; i++){
        array[i] = oldArray[i];
    }
    tail--;
    delete  [] oldArray;
}

void HeapPriorityQueue::bubbleUp(int child){
    if (child!=0){
        int parent;
            if (child % 2==0){
                parent = (child-2)/2;
            }   else {
                parent = (child-1)/2;
            }
            if (array[child]<array[parent]){
                swap(array[parent],array[child]);
                bubbleUp(parent);
            }
    }
}

void HeapPriorityQueue::bubbleDown(int parent){
    int child1 = 2*parent+1;
    int child2 = 2*parent+2;
    // CASE 1: CHILD1 < TAIL AND CHILD2 < TAIL
    if(child1<tail&&child2<tail){
        if (array[child1]<array[child2]){
            if (array[parent]>array[child1]){
                swap(array[parent], array[child1]);
                bubbleDown(child1);
            }
        } else if (array[parent]>array[child2]){
            swap(array[parent], array[child2]);
            bubbleDown(child2);
        }
    }
    // CASE 2: CHILD1 < TAIL BUT CHILD2>= TAIL
    if(child1<tail&&child2>=tail){
            if (array[parent]>array[child1]){
                swap(array[parent], array[child1]);
                bubbleDown(child1);
            }
    }
    // CASE 3: CHILD2< TAIL BUT CHILD1>= TAIL
    if(child2<tail&&child1>=tail){
        if (array[parent]>array[child2]){
            swap(array[parent], array[child2]);
            bubbleDown(child2);
        }
    }
    // CASE 4: CHILD1>= TAIL AND CHILD2>= TAIL
}

