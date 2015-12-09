/*
 * Authors: Philip Clark and Susannah Meyer
 * This is the HeapPriorityQueue.h file which acts as a header file for the heap priority queue class implementation.
 */

#ifndef _heappriorityqueue_h
#define _heappriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class HeapPriorityQueue {
public:
    HeapPriorityQueue();
    ~HeapPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const HeapPriorityQueue& queue);
    void merge(HeapPriorityQueue& queue); //bonus function that merges two heap priority queues

private:
    void checkResize(); //resizes array of elements in heap form if it gets too big
    void bubbleUp(int index); //resorts by going from child to parent in checking urgency
    void bubbleDown(int index); //resorts heap by going from parent to child in checking urgency
    PQEntry* myElements; //heap array
    int mySize; //number of elements in the array
    int myCapacity; //actual size of the array
    bool needSwap(int indexOne, int indexTwo);//checks if two elements need to be swaped (ie one's priority is more urgent than the other)
};

#endif
