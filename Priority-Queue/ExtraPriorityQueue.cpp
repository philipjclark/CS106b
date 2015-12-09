/*
 * Authors: Philip Clark and Sussanah
 * Description: The ExtraPriorityQueue class is simply a sorted implementation of the ArrayPriorityQueue class.
 * The key difference is that while dequeuing now only involves looking at the front of the queue, not unlike the sorted
 * LinkedList.
 * Sources: Insert method based on CS106B lecture slides as well as those used in ArrayPriorityQueue
 */

#include "ExtraPriorityQueue.h"
#include "PQEntry.h"

ExtraPriorityQueue::ExtraPriorityQueue() {
    mySize = 0;
    myCapacity = 10;
    myElements = new PQEntry[myCapacity];
}

ExtraPriorityQueue::~ExtraPriorityQueue() {
    delete[] myElements;
}

void ExtraPriorityQueue::changePriority(string value, int newPriority) { 
    PQEntry item;
    item.value = value;
    item.priority = newPriority;
    remove(item);
    enqueue(value, newPriority);
}

void ExtraPriorityQueue::clear() {
    mySize = 0;
}

void ExtraPriorityQueue::remove(PQEntry item) {
    int index = 0;
    bool found = false;
    for (int i = 0; i < mySize; ++i) {
        if(myElements[i].value == item.value) { //finds place where it should be sorted into
            if(myElements[i].priority < item.priority) throw("Error: new priority cannot be greater than old priority");
            index = i;
            found = true;
            break;
        }
    }
    if(found) {
        for (int i = index; i < mySize; ++i) {
            myElements[i] = myElements[i+1];
        }
        mySize--;
    }
    else throw("Error: The given value is not found in this priority queue");


}

string ExtraPriorityQueue::dequeue() {
    if(mySize == 0) throw("Error: priority queue is empty");
    PQEntry dequeueItem = myElements[0]; //now needs to only look at first element
    remove(dequeueItem); //removes this element
    return dequeueItem.value;
}

void ExtraPriorityQueue::enqueue(string value, int priority) {
    PQEntry item;
    item.priority = priority;
    item.value = value;
    insert(item); //needs to go through to find sorted place
}

void ExtraPriorityQueue::insert(PQEntry item) { //collectively O(N)
    int index = 0;
    checkResize();
    for (int i = 0; i < mySize; ++i) { //find where the sorted index is for the new item
        if(myElements[i].priority > item.priority || myElements[i].priority == item.priority && myElements[i].value > item.value) break;
        index++;
    }
    for (int i = mySize; i > index; i--) { //move the rest of the items over
        myElements[i] = myElements[i-1];
    }
    myElements[index] = item; //set new element value
    mySize++;
}

void ExtraPriorityQueue::checkResize() {
    if(mySize >= myCapacity) {
        PQEntry* bigger = new PQEntry[myCapacity*2];

        for (int i = 0; i < mySize; ++i) {
            bigger[i] = myElements[i];
        }
        myCapacity*=2;
        delete[] myElements;
        myElements = bigger;
    }
}

bool ExtraPriorityQueue::isEmpty() const {
    return mySize==0;
}

string ExtraPriorityQueue::peek() const {
    if(mySize == 0) throw("Error: priority queue is empty");
    return myElements[0].value;
}

int ExtraPriorityQueue::peekPriority() const {
    if(mySize == 0) throw("Error: priority queue is empty");
    return myElements[0].priority;
}

int ExtraPriorityQueue::size() const {
    return mySize;
}

ostream& operator<<(ostream& out, const ExtraPriorityQueue& queue) {
    out << "{";
    if(!queue.isEmpty()) {
        out << "\"" << queue.myElements[0].value << "\":" << queue.myElements[0].priority;
        for (int i = 1; i < queue.mySize; ++i) {
            out << ", \"" << queue.myElements[i].value << "\":" << queue.myElements[i].priority;
        }
    }
    out << "}";
    return out;
}
