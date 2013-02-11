/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"
#include "vector.h"
#include <string>

using namespace std;

VectorPriorityQueue::VectorPriorityQueue() {
    Vector<string> priorityQueue;
    
}

VectorPriorityQueue::~VectorPriorityQueue() {
    // empty
}

int VectorPriorityQueue::size() {
    int size = priorityQueue.size();
    return size;

}

bool VectorPriorityQueue::isEmpty() {
    int size = priorityQueue.size();
    if (size==0){
        return true;
    } else {
        return false;
    }
}

void VectorPriorityQueue::enqueue(string value) {
    priorityQueue.add(value);
}

string VectorPriorityQueue::peek() {
    if (priorityQueue.size()==0){
        error("Priority queue is empty");
    }
    string s = priorityQueue[0];
    for (int i =0; i< priorityQueue.size(); i++){
        string t = priorityQueue[i];
        if (s>t){
            s = t;
        }
    }
    return s;
}

string VectorPriorityQueue::dequeueMin() {
    if (priorityQueue.size()==0){
        error("Priority queue is empty");
    }
    string s = priorityQueue[0];
    int location = 0;
    for (int i =0; i< priorityQueue.size(); i++){
        string t = priorityQueue[i];
        if (s>t){
            s = t;
            location=i;
        }
    }
    priorityQueue.removeAt(location);
    return s;
}
