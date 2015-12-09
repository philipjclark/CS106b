/*
 * Authors: Philip Clark and Susannah Meyer
 */

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    front = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    bool found = false;
    ListNode* trash;
    if (front->value == value) {
        if (front->priority < newPriority) {
            throw("Error: new priority cannot be greater than old priority");
        }
        front->priority = newPriority;
        found = true;
    } else {
        ListNode* current = front;
        while (current->next != NULL) {
            if (current->next->value == value) {
                if (newPriority > current->next->priority) {
                    throw("Error: new priority cannot be greater than old priority");
                } else {
                    trash = current->next;
                    current->next = current->next->next;
                    enqueue(value, newPriority);
                    found = true;
                    delete trash;
                    break;
                }
            }
            current = current->next;
        }
    }
    if (!found) {
        throw("Error: The given value is not found in this priority queue");
    }
}

void LinkedPriorityQueue::clear() {
    ListNode* trash;
    ListNode* current = front;
    while (current != NULL) {
        trash = current;
        current = current -> next;
        delete trash;
    }
    front = current;
}

string LinkedPriorityQueue::dequeue() {
    if (front == NULL) {
        throw("Error: priority queue is empty");
    }
    ListNode* trash = front;
    string dequeueString = front->value;
    front = front->next;
    delete trash;
    return dequeueString;
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    if (front == NULL || front->priority > priority || (front->priority == priority && front->value > value)) {
        front = new ListNode(value, priority, front);
    } else {
        ListNode* current = front;
        while (current->next!=NULL) {
            if (current->next->priority > priority || (current->next->priority == priority && current->next->value > value)) {
                current->next = new ListNode(value, priority, current->next);
                return;
            }
            current = current->next;
        }
       current->next = new ListNode(value, priority);
    }
}

bool LinkedPriorityQueue::isEmpty() const {
    return front == NULL;
}

string LinkedPriorityQueue::peek() const {
    if (front == NULL) {
        throw("Error: priority queue is empty");
    }
    return front->value;
}

int LinkedPriorityQueue::peekPriority() const {
    if (front == NULL) {
        throw("Error: priority queue is empty");
    }
    return front->priority;
}

int LinkedPriorityQueue::size() const {
    int size = 0;
    ListNode* current = front;
    if (front != NULL) {
        size = 1;
        while (current->next != NULL) {
            size++;
            current = current->next;
        }
    }
    return size;
}

void LinkedPriorityQueue::merge(LinkedPriorityQueue& queue) {
    ListNode* merge = queue.front;

    if (queue.front == NULL) {
        return;
    }
    if (front == NULL) {
        front = queue.front;
        return;
    }
    if (front->priority > merge->priority || (front->priority == merge->priority && front->value > merge->value)) {
        front = new ListNode(merge->value, merge->priority, front);
        merge = merge->next;
    }

    ListNode* current = front;

    while (merge != NULL) {
        if (current->next == NULL) {
            current->next = merge;
        } else if (current->next->priority > merge->priority || (current->next->priority == merge->priority && current->next->value > merge->value)) {
            current->next = new ListNode(merge->value, merge->priority, current->next);
            current = current->next;
            merge = merge->next;
        } else {
            current = current->next;
        }
    }
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    out << "{";
    if(queue.front != NULL) {
        out << "\"" << queue.front->value << "\":" << queue.front->priority;
        ListNode* current = queue.front;
        while (current -> next != NULL) {
            current = current->next;
            out << ", \"" << current->value << "\":" << current->priority;
        }
    }
    out << "}";
    return out;
}
