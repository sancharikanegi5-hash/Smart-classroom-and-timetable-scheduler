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