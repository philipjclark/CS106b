/*
 * Authors: Philip Clark and Susannah Meyer
 */

#ifndef _extrapriorityqueue3_h
#define _extrapriorityqueue3_h

#include <cstddef>    // for NULL
#include <iostream>
#include <string>
#include "ListNode2.h"
#include "PQEntry.h"
using namespace std;

/*
 * For documentation of each member, see VectorPriorityQueue.h.
 */
class ExtraPriorityExtra3 {
public:
    ExtraPriorityExtra3();
    ~ExtraPriorityExtra3();
    void changePriority(string value, int newPriority);
    void clear();
    string dequeue();
    void enqueue(string value, int priority);
    bool isEmpty() const;
    string peek() const;
    int peekPriority() const;
    int size() const;
    friend ostream& operator <<(ostream& out, const ExtraPriorityExtra3& queue);
    void merge(ExtraPriorityExtra3& queue); //make public?

private:
   ListNode2* front;
   ListNode2* back;
};

#endif
