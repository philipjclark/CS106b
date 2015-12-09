/*
 * Authors: Philip Clark and Susannah Meyer
 * Description: The ExtraPriorityQueue2 class is a map implementation of the priority queue class which uses a map from int to Set of string
 * (chose this data type instead of the queue recommended in the spec for easier sorting). Instead of having PQ entries in an array,
 * this implementation has the priority as the key and the set of all elements with the priority in alphebetically sorted order (as
 * a set naturally does). Dequeuing simply involves going to the minimum key value (which is stored in another int set of priorities)
 * and pulling the first set element. Enqueuing simply involves adding a new value at the right key in the map. Changing prioirty is just a mixture
 * of dequeuing and enqueuing.
 * Date: 11/9/15
 */

#include "ExtraPriorityQueue2.h"
#include "map.h"
#include "set.h"

ExtraPriorityQueue2::ExtraPriorityQueue2() {
    mySize = 0;
}

ExtraPriorityQueue2::~ExtraPriorityQueue2() {
    clear();
}

void ExtraPriorityQueue2::changePriority(string value, int newPriority) { //O(NlogN)
    bool found = false; //to see if the new value is ever found
    for(int i: queue) { //go through ever priority
        if(queue[i].contains(value)) { //if the mapped to set has the value
            if(newPriority > i) throw("Error: new priority cannot be greater than old priority"); //throw error if wrong input
            queue[i].remove(value); //remove
            found = true;
            break;
        }
    }
    if(!found) throw("Error: The given value is not found in this priority queue");
    queue[newPriority]+= value; //add new value with correct priority
    priorities.add(newPriority); //add new priority to priority set
}

void ExtraPriorityQueue2::clear() {
    queue.clear();
    mySize = 0;
    priorities.clear();
}

string ExtraPriorityQueue2::dequeue() {//O(logN)
    if(mySize == 0) throw("Error: priority queue is empty");
    string dequeueString = queue[priorities.first()].first(); //grab first string from the most urgent priority key
    queue[priorities.first()].remove(dequeueString);
    if(queue[priorities.first()].isEmpty()) {
        priorities.remove(priorities.first());
    }
    mySize --;
    if(mySize == 0) {
        queue.clear();
        priorities.clear();
    }
    return dequeueString;   // remove this
}

void ExtraPriorityQueue2::enqueue(string value, int priority) { //O(logN)
    queue[priority] += value; //add value at correct key
    priorities.add(priority); //update priorities list
    mySize++; //update size
}

bool ExtraPriorityQueue2::isEmpty() const { //O(1)
    cout << queue.size();
    return mySize == 0;   // remove this
}

string ExtraPriorityQueue2::peek() const { //O(logN)
    if(mySize == 0) throw("Error: priority queue is empty");
    return queue[priorities.first()].first();
}

int ExtraPriorityQueue2::peekPriority() const {//O(1)
    if(mySize == 0) throw("Error: priority queue is empty");
    return priorities.first();   // remove this
}

int ExtraPriorityQueue2::size() const { //O(1)
    return mySize;
}

ostream& operator<<(ostream& out, const ExtraPriorityQueue2& queue) {
    ExtraPriorityQueue2 print = queue;
    bool first = true;
    out << "{";
    for(int i: print.queue) {
        while(!print.queue[i].isEmpty()) {
            if(first) {
                out << "\"" << print.queue[i].first() << "\":" << i;
                first = false;
            }
            else out << ", \"" << print.queue[i].first() << "\":" << i;
            print.queue[i].remove(print.queue[i].first());
        }
    }

out << "}";
return out;
}
