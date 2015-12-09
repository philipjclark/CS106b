/*
 * Authors: Philip Clark and Susannah Meyer
 * Date: 11/9/15
 * Description: The ExtraPriorityQueue3 class is a doubly linked list implementation of the priority queue. Drawing on the ListNode2
 * object (which has both a front and back pointer), it modifies the linked list to have a front and back pointer, and a next and prev
 * operation. The class is thus able to modify a number of the functions (especially dequeue and changePriority) with these
 * new features to improve their efficiency.
 */

#include "ExtraPriorityQueue3.h"

ExtraPriorityExtra3::ExtraPriorityExtra3() {
    front = NULL;
    back = NULL;
}

ExtraPriorityExtra3::~ExtraPriorityExtra3() {
clear();
}

void ExtraPriorityExtra3::changePriority(string value, int newPriority) { //uses back and prev pointer to move backwards once value is found and reinsert rather than having to enqueue completely
    bool found = false;
    bool first = false;
    ListNode2*trash;
    if(front->value == value) { //changes value in linked list if the front is the value being changed
        if(front->priority < newPriority) throw("Error: new priority cannot be greater than old priority");
        front ->priority = newPriority;
        found = true;
    }
    else {
        ListNode2* current = front;
        while(current->next != NULL) { //loop through list
            //cout << current->next->value;
            if(current->next->value== value) { //find value
                if(newPriority > current->next->priority) throw("Error: new priority cannot be greater than old priority");
                else {
                    trash = current->next;
                    //if(current->next->next != NULL) current->next = current->next->next;
                    current->next = current->next->next;
               //    cout << "trash " << trash->value;
                    if(current->next != NULL) current->next->prev = current; //sets prev pointer
                    while(true) { //works backward to find place where value must be inserted
                        if(current->prev->priority < newPriority || current->prev->priority == newPriority && current->prev->value < value) break;
                        if(current->prev == front) {
                            first = true;
                            break;
                        }
                        current = current->prev;
                    }
              //      cout << current->prev->value;
                    if(first) {
                        front = new ListNode2(value, newPriority, front, back); //if it's the front do the special case to update the back pointer
                        front->next->prev = front;
                    }
                    else { current->prev = new ListNode2(value, newPriority, current, current->prev); //otherwise just insert a correct node
                    current->prev->prev->next = current->prev; //update prev pointer
                    }
                    found = true;
                    break;
                }
            }
            current = current->next;
        }
    }
    if(!found) throw("Error: The given value is not found in this priority queue");
}

void ExtraPriorityExtra3::clear() {
    ListNode2* trash;
    ListNode2* current = front;
    while(current != NULL) {
        trash = current;
        current = current -> next;
        delete trash;
    }
    front = current;
}

string ExtraPriorityExtra3::dequeue() {
    if(front == NULL) throw("Error: priority queue is empty");
    ListNode2* trash = front;
    string dequeueString = front->value;
    front = front->next;
    if(!front==NULL) front->prev = back; //set new front to back point to back
    delete trash;
    return dequeueString;
}

void ExtraPriorityExtra3::enqueue(string value, int priority) {
    if(front == NULL || front->priority > priority || front->priority == priority && front->value > value) {
        front = new ListNode2(value, priority, front, back);
        if(front->next!=NULL) front->next->prev = front; //make correct prev pointer
      //  cout << 92;
    } else{
        ListNode2* current = front;
        while(current->next!=NULL) {
            if(current->next->priority > priority || current->next->priority == priority && current->next->value > value) {
                current->next = new ListNode2(value, priority, current->next, current);
                current->next->next->prev = current->next; //implement correct prev pointer
                return;
            }
            current = current->next;
        }
       current->next = new ListNode2(value, priority, NULL, current); //correctly enqueue new value
    //   cout << current->next->value << " previous " << current->next->prev->value;
       back = current->next;
       front->prev = back; //update back
    //   cout << front ->prev->value << " back" << endl;
    }
}

bool ExtraPriorityExtra3::isEmpty() const {
    return front == NULL;
}

string ExtraPriorityExtra3::peek() const {
    if(front == NULL) throw("Error: priority queue is empty");
    return front->value;
}

int ExtraPriorityExtra3::peekPriority() const {
    if(front == NULL) throw("Error: priority queue is empty");
    return front->priority;
}

int ExtraPriorityExtra3::size() const {
    int size = 0;
    ListNode2* current = front;
    if(front != NULL) {
        size = 1;
        while(current->next != NULL) {
            size++;
            current = current->next;
        }
    }
    return size;
}

ostream& operator<<(ostream& out, const ExtraPriorityExtra3& queue) {

    out << "{";
    if(queue.front != NULL) {
        out << "\"" << queue.front->value << "\":" << queue.front->priority;// << " my previous is " << queue.front->prev->value;
        ListNode2* current = queue.front;
        while (current -> next != NULL) {
            current = current->next;
            out << ", \"" << current->value << "\":" << current->priority;// << " my previous is " << current->prev->value;
          //  if(current->next!=NULL) out << " my next is " << current->next->value;
        }
    }
  //  out << endl << "back is " << queue.back->value;
    out << "}";
    return out;
}
