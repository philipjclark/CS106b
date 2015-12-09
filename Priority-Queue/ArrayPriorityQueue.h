/*
 * Authors: Philip Clark and Susannah Meyer
 */

#ifndef _arraypriorityqueue_h
#define _arraypriorityqueue_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

class ArrayPriorityQueue {
public:
    ArrayPriorityQueue();
    ~ArrayPriorityQueue();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ArrayPriorityQueue& queue);
    void merge(ArrayPriorityQueue& queue); //made public?

private:
    PQEntry* myElements; //array
    void remove(int index);
    int findValue(string value);
    void checkResize();
    int findNext() const;
    int mySize;
    int myCapacity;
};

#endif
