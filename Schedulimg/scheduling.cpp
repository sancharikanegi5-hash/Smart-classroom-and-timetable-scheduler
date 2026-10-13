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