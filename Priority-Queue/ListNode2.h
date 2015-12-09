/*
* Authors: Philip Clark and Susannah Meyer
* The ListNode2.h file is the same as ListNode.h but modified to have a prev pointer so that a doubly
* linked list can be created.
 */

#ifndef _listnode2_h
#define _listnode2_h

#include <iostream>
#include <string>
#include "PQEntry.h"
using namespace std;

struct ListNode2 {
public:
    string value;
    int priority;
    ListNode2* next;
    ListNode2* prev; //points to previous element in linkedList

    ListNode2(string value = "", int priority = 0, ListNode2* next = NULL, ListNode2* back = NULL);
};

bool operator <(const ListNode2& node1, const ListNode2& node2);
bool operator <=(const ListNode2& node1, const ListNode2& node2);
bool operator ==(const ListNode2& node1, const ListNode2& node2);
bool operator !=(const ListNode2& node1, const ListNode2& node2);
bool operator >(const ListNode2& node1, const ListNode2& node2);
bool operator >=(const ListNode2& node1, const ListNode2& node2);
ostream& operator <<(ostream& out, const ListNode2& node);

#endif
