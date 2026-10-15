#include<iostream>
#include<scheduling.h>
using namespace std;

requestQueue::requestQueue(){
    front=nullptr;
    rear=nullptr;
    count=0;
}

void requestQueue::enqueue(schedulingRequest r) {
    node* newNode = new node(r);   // step 1: make the new node

    if (front == nullptr) {
        // step 2: queue is empty — newNode becomes both front and rear
        front = newNode;
        rear  = newNode;
    } else {
        // step 3: queue has stuff — attach newNode after current rear,
        //         then move rear forward to it
        rear->next = newNode;
        rear = newNode;
    }
    count++;   // step 4: don't forget to update count
}

schedulingRequest requestQueue::dequeue() {
    if (front == nullptr) {
        return schedulingRequest();   // queue empty, return a default (invalid) request
    }

    node* nodeToDelete = front;
    schedulingRequest removedRequest = nodeToDelete->data;

    front = front->next;              // move front forward

    if (front == nullptr) {
        rear = nullptr;               // that was the last node, so rear must reset too
    }

    delete nodeToDelete;              // free the old front node
    count--;
    return removedRequest;
}

bool requestQueue::isEmpty() const {
    return front == nullptr;
}

requestQueue::~requestQueue() {
    node* current = front;

    while (current != nullptr) {
        node* nextNode = current->next;   // save the next node first
        delete current;                   // free the current node
        current = nextNode;               // move forward
    }

    front = nullptr;
    rear = nullptr;
    count = 0;
}

circularSlotQueue::circularSlotQueue() {
    front = -1;
    rear = -1;
    count = 0;
}

bool circularSlotQueue::isFull() const {
    return count == MAX_SLOTS;
}

bool circularSlotQueue::isEmpty() const {
    return count == 0;
}

bool circularSlotQueue::enqueueSlot(timeSlot newSlot) {
    if (isFull()) {
        return false;                        // no space left
    }

    if (isEmpty()) {
        front = 0;                           // first element goes at index 0
    }

    rear = (rear + 1) % MAX_SLOTS;           // wraparound
    slots[rear] = newSlot;
    count++;
    return true;
}

timeSlot circularSlotQueue::dequeueSlot() {
    if (isEmpty()) {
        return timeSlot();                   // default slot, nothing to remove
    }

    timeSlot removedSlot = slots[front];
    front = (front + 1) % MAX_SLOTS;         // wraparound
    count--;

    if (isEmpty()) {
        front = -1;                          // reset so the next enqueue starts clean
        rear = -1;
    }

    return removedSlot;
}