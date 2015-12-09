// This is an optional .h file that you might edit and turn in.
// We have provided a skeleton for you,
// and you may finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _extrapriorityqueue2_h
#define _extrapriorityqueue2_h

#include <iostream>
#include <string>
#include "PQEntry.h"
#include "map.h"
#include "set.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class ExtraPriorityQueue2 {
public:
    ExtraPriorityQueue2();
    ~ExtraPriorityQueue2();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ExtraPriorityQueue2& queue);

private:
    Map<int, Set<string>> queue;
    int mySize;
    Set<int> priorities;
};

#endif
