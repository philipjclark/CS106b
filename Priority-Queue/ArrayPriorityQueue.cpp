/*
 * Authors: Philip Clark and Susannah Meyer
 */

#include "ArrayPriorityQueue.h"
#include "PQEntry.h"

ArrayPriorityQueue::ArrayPriorityQueue() {
    mySize = 0;
    myCapacity = 10;
    myElements = new PQEntry[myCapacity];

}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete[] myElements;
}

int ArrayPriorityQueue::findValue(string value) {
    for (int i = 0; i < mySize; ++i) {
        if (myElements[i].value == value) {
            return i;
        }
    }
    return -1;
}

void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    int index = findValue(value);
    if (index == -1) {
        throw("Error: The given value is not found in this priority queue");
    }
    if (myElements[index].priority < newPriority) {
        throw("Error: new priority cannot be greater than old priority");
    }
    myElements[index].priority = newPriority;
}

void ArrayPriorityQueue::clear() {
    mySize = 0;
}

string ArrayPriorityQueue::dequeue() {
    if (mySize == 0) {
        throw("Error: priority queue is empty");
    }
    int dequeueIndex = findNext();
    string dequeueString = myElements[dequeueIndex].value;
    remove(dequeueIndex);
    return dequeueString;
}

void ArrayPriorityQueue::enqueue(string value, int priority) {
    PQEntry item(value, priority);
    checkResize();
    myElements[mySize] = item;
    mySize ++;
}

int ArrayPriorityQueue::findNext() const{
    PQEntry min("", 0);
    int index = 0;

    for (int i = 0; i < mySize; ++i) {
        if (myElements[i].priority < min.priority || (myElements[i].priority == min.priority && myElements[i].value < min.value) || min.priority == 0) {
            index = i;
            min.priority = myElements[i].priority;
            min.value = myElements[i].value;
        }
    }
    return index;
}

void ArrayPriorityQueue::remove(int index) {
    for (int i = index; i < mySize; i++) {
        myElements[i] = myElements[i+1];
    }
    mySize--;
}

void ArrayPriorityQueue::checkResize() {
    if (mySize >= myCapacity) {
        PQEntry* bigger = new PQEntry[myCapacity*2];
        for (int i = 0; i < mySize; ++i) {
            bigger[i] = myElements[i];
        }
        myCapacity *= 2;
        delete[] myElements;
        myElements = bigger;
    }
}

bool ArrayPriorityQueue::isEmpty() const {
    return mySize == 0;
}

string ArrayPriorityQueue::peek() const {
    if (mySize == 0) {
        throw("Error: priority queue is empty");
    }
    return myElements[findNext()].value;
}

int ArrayPriorityQueue::peekPriority() const {
    if (mySize == 0) {
        throw("Error: priority queue is empty");
    }
    return myElements[findNext()].priority;
}

int ArrayPriorityQueue::size() const {
    return mySize;
}

void ArrayPriorityQueue::merge(ArrayPriorityQueue& queue) {
    for (int i = 0; i < queue.mySize; i++) {
        checkResize();
        myElements[i+mySize] = queue.myElements[i];
    }
    mySize += queue.mySize;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << "\"" << queue.myElements[0].value << "\":" << queue.myElements[0].priority;
        for (int i = 1; i < queue.mySize; ++i) {
            out << ", \"" << queue.myElements[i].value << "\":" << queue.myElements[i].priority;
        }
    }
    out << "}";
    return out;
}
