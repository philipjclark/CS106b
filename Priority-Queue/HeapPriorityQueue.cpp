/*
 * Authors: Philip Clark and Susannah Meyer
 * This is the heap priority queue class which implements a priority queue using a heap structure.
 * The heap is stored in an array (which can resize itself if it exceeds its capacity), reordering itself
 * to match the heap specifications after every operation that modifies the priority queue (dequeue, enqueue, change priority).
 * Date: 11/9/15
 * Sources: ArrayList from CS106B lecture
 */

#include "HeapPriorityQueue.h"

/*
 * The HeapPriorityQueue constructor intializes the needed values for mySize and myCapacity and creates the array itself.
 * Big-Oh: O(1)
 */
HeapPriorityQueue::HeapPriorityQueue() {
    mySize = 0;
    myCapacity = 10;
    myElements = new PQEntry[myCapacity]; //create array
}

/*
 * The deconstructor simply deletes the array in heap formation as soon as the object falls out of scope.
 * Big-Oh: O(1)
 */
HeapPriorityQueue::~HeapPriorityQueue() {
    delete[] myElements;
}

/*
 * The heap implementation of changePriority loops through the heap array to see if the value can be found.
 * If this is the case, it sets a new priority for the element in question and calls the bubbleUp function to reorder
 * the heap accordingly.
 * Big-Oh: O(NlogN)
 * @value: the value of the element being changed
 * @newPriority: the priority being changed to
 */
void HeapPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    for (int i = 1; i <= mySize; i++) { //incrementation includes i = mySize because the first index is 0
        if (myElements[i].value == value) {
            if (newPriority > myElements[i].priority) {
                throw("Error: new priority cannot be greater than old priority"); //throw error if input is invalid
            } else {
                myElements[i].priority = newPriority;
                bubbleUp(i);
                found = true;
            }
        }
    }
    if (!found) {
        throw("Error: The given value is not found in this priority queue"); //throw error if the value cannot be found in the heap
    }
}

/*
 * Just like the ArrayListQueue, checkResize uses much of the code we developed in class
 * to "resize" an array if it exceeds its capacity by making a new array of capacity*2
 * and copying the old array over and trashing the old array to prevent a memory leak.
 * Big-Oh: O(N)
 */
void HeapPriorityQueue::checkResize() {
    if (mySize+1 >= myCapacity) {
        PQEntry* bigger = new PQEntry[myCapacity*2];
        for (int i = 1; i <= mySize; i++) { //incrementation includes i = mySize because the first index is 0
            bigger[i] = myElements[i];
        }
        myCapacity *= 2;
        delete[] myElements;
        myElements = bigger;
    }
}

/*
 * Clear just sets the array size to 1 -- all data that currently exists
 * will be overwritten and the array otherwise behaves as though it were empty.
 * Big-Oh: O(1)
 */
void HeapPriorityQueue::clear() {
    mySize = 0;
}

/*
 * The dequeue method takes advantage of the heap structure to dequeue, taking the value of the first element in the array,
 * moving the last element up to the first, decreases the size (effectively deleting the last element that is now useless)
 * and calling the bubbleDown function to reorder.
 * Big-Oh: O(logN)
 */
string HeapPriorityQueue::dequeue() {
    if (mySize == 0) {
        throw("Error: priority queue is empty");
    }

    string dequeueString = myElements[1].value;
    myElements[1] = myElements[mySize];
    mySize--;

    bubbleDown(1);
    return dequeueString;
}

/*
 * The enqueue method enqueues to the priority queue by adding a value to the end of the array (resizing if necessary)
 * modifying the necessary values and then calling bubbleUp to reorder.
 * @value: the value of the new entry
 * @priority: the priority of the new entry
 * Big-Oh: O(logN)
 */
void HeapPriorityQueue::enqueue(string value, int priority) {
    PQEntry item; //add item
    item.priority = priority;
    item.value = value;
    checkResize();
    myElements[mySize+1] = item; //throw new item at end
    mySize++; //increase size to reflect enqueue
    bubbleUp(mySize); //reorder
}

/*
 * The bubbleDown method plays an essential role in the dequeuing process by reordering the heap
 * starting from the first index. It recursively checks and swaps parent and child nodes until the entire
 * heap is properly sorted.
 * @index: the index where the bubble down begins (in this heap implementation always 1)
 * Big-Oh: O(logN)
 */
void HeapPriorityQueue::bubbleDown(int index) {
    if (index*2 <= mySize) {
        int childIndexOne = index*2; //index of child node 1
        int childIndexTwo = index*2 + 1; //index of child node 2
        int moreUrgent = 0; //will be used for index of more urgent child node

        if (childIndexTwo >= mySize+1) { //makes sure that the second child node is in the bounds of the array
            moreUrgent = childIndexOne;
        } else {
            if (needSwap(childIndexTwo, childIndexOne)) { //if the second child node is more urgent
                moreUrgent = childIndexTwo;
            } else { //if the first is more urgent
                moreUrgent = childIndexOne;
            }
        }

        if (needSwap(moreUrgent, index)) { //if the parent node needs to be swapped with the more urgent child node
            PQEntry temp = myElements[index]; //create temp entry for swap
            myElements[index] = myElements[moreUrgent];
            myElements[moreUrgent] = temp;
            bubbleDown(moreUrgent); //recursively call the rest of the function where the parent node has been swapped to
        }
        //    return false;
    }
}

/*
 * This method serves to reduce code blocks which just check which of two elements in the array are more urgent
 * @indexOne: the index of one entry
 * @indexTwo: the index of the other entry
 * Big-Oh: O(1)
 */
bool HeapPriorityQueue::needSwap(int indexOne, int indexTwo) {
    return myElements[indexTwo].priority > myElements[indexOne].priority || myElements[indexTwo].priority == myElements[indexOne].priority
    && myElements[indexTwo].value > myElements[indexOne].value;
}

/*
 * The bubbleUp method is one of the other essential methods of the class and is used both in enqueue and change priority.
 * Like bubbleDown, the function is recursive and sorts the array, sending the more urgent element in each child pair up
 * until the index is 1.
 * @index: the index where the bubbleUp starts (mySize for enqueue and the old priority of the element whose priority
 * is being changed in changePriority)
 * Big-Oh: O(logN)
 */
void HeapPriorityQueue::bubbleUp(int index) {
    if (index != 1) { //if at the first position, the rest of the array has been sorted
        int parentIndex = index/2; //integer division yields index of parent element
        if (needSwap(index, parentIndex)) { //if they need to be swapped, do so
            PQEntry temp = myElements[parentIndex];
            myElements[parentIndex] = myElements[index];
            myElements[index] = temp;
            bubbleUp(parentIndex); //continue recursive process
        }
    }
}

/*
 * Returns whether or not the priority queue is empty.
 * Big-Oh: O(1)
 */
bool HeapPriorityQueue::isEmpty() const {
    return mySize == 0;
}

/*
 * Returns the value of the most urgent element of the queue.
 * Big-Oh: O(1)
 */
string HeapPriorityQueue::peek() const {
    if (mySize == 0) {
        throw("Error: priority queue is empty"); //if the queue is empty throw an error
    }
    return myElements[1].value;
}

/*
 * Returns the priority of the most urgent value in the queue.
 * Big-Oh: O(1)
 */
int HeapPriorityQueue::peekPriority() const {
    if (mySize == 0) {
        throw("Error: priority queue is empty"); //if the queue is empty throw an error
    }
    return myElements[1].priority;
}

/*
 * Returns the size of the queue by taking the array's size and subtracting 1 (since the array size was
 * set to 1 at the beginning to deal with index issues).
 * Big-Oh: O(1)
 */
int HeapPriorityQueue::size() const {
    return mySize;
}

/*
 * The merge bonus function takes another queue and simply enqueues its elements (which are naturally
 * sorted by the heap class).
 * Big-Oh: O(1)
 * @queue: the heap queue being merged
 */
void HeapPriorityQueue::merge(HeapPriorityQueue& queue) {
    while (!queue.isEmpty()) {
        enqueue(queue.peek(), queue.peekPriority()); //enqueue top element into heap priority queue
        queue.dequeue(); //get rid of merging heap queue's top element
    }
}

/*
 * This operator gives functionality to the << operator, allowing the heap queue to be printed
 * Big-Oh: O(N)
 * @out: the ostream being used to print
 * @queue: the heap queue being printed
 */
ostream& operator<<(ostream& out, const HeapPriorityQueue& queue) {
    out << "{";
    if (!queue.isEmpty()) {
        out << "\"" << queue.myElements[1].value << "\":" << queue.myElements[1].priority; //First element without comma
        for (int i = 2; i < queue.mySize+1; i++) {
            out << ", \"" << queue.myElements[i].value << "\":" << queue.myElements[i].priority; //Rest of elements with comma
        }
    }
    out << "}";
    return out;
}
