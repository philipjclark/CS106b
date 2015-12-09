/*
 * Authors: Philip Clark and Susannah Meyer
 * The ListNode2 class is the implementation of a class for a listnode which has both a front and back pointer,
 * as used to create a doubly linked list in ExtraPriorityQueue3.cpp
 */
#include "ListNode2.h"

ListNode2::ListNode2(string value, int priority, ListNode2* next, ListNode2* prev) { //only modified function is the constructor which now continues a prev back pointer
    this->value = value;
    this->priority = priority;
    this->next = next;
    this->prev = prev;
}

bool operator <(const ListNode2& node1, const ListNode2& node2) {
    return node1.priority < node2.priority ||
            (node1.priority == node2.priority && node1.value < node2.value);
}

bool operator <=(const ListNode2& node1, const ListNode2& node2) {
    return node1 < node2 || node1 == node2;
}

bool operator ==(const ListNode2& node1, const ListNode2& node2) {
    return node1.priority == node2.priority && node1.value == node2.value;
}

bool operator !=(const ListNode2& node1, const ListNode2& node2) {
    return !(node1 == node2);
}

bool operator >(const ListNode2& node1, const ListNode2& node2) {
    return !(node1 <= node2);
}

bool operator >=(const ListNode2& node1, const ListNode2& node2) {
    return !(node1 < node2);
}

ostream& operator <<(ostream& out, const ListNode2& node) {
    out << "\"" << node.value << "\":" << node.priority;
    return out;
}
