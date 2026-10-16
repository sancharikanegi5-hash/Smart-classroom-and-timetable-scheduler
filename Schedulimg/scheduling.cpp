#include <iostream>
#include "scheduling.h"
using namespace std;

const string dayNames[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// timeSlot
timeSlot::timeSlot() {
    day = MON;
    period = 0;                        // 0 means "not set yet"
}

timeSlot::timeSlot(Day newDay, int newPeriod) {
    day = newDay;
    period = newPeriod;
}

Day timeSlot::getDay() const {
    return day;
}

int timeSlot::getPeriod() const {
    return period;
}

void timeSlot::display() const {
    cout << dayNames[day] << " Period " << period;
}

// schedulingRequest 
schedulingRequest::schedulingRequest() {
    subject = "";
    section = "";
    valid = false;                     // slot uses timeSlot's default constructor
    priority = 1;
}

schedulingRequest::schedulingRequest(string newSubject, string newSection, timeSlot newSlot, int newPriority) {
    subject = newSubject;
    section = newSection;
    slot = newSlot;
    valid = false;                     // set properly when isValid() runs
    priority = newPriority;
}

bool schedulingRequest::isValid() {
    bool hasSubject = (subject != "");
    bool hasSection = (section != "");
    bool periodInRange = (slot.getPeriod() >= 1 && slot.getPeriod() <= MAX_SLOTS);

    valid = hasSubject && hasSection && periodInRange;
    return valid;
}

void schedulingRequest::process() {
    if (!isValid()) {
        cout << "Rejected: invalid request" << endl;
        return;
    }
    cout << "Processed -> ";
    display();
    cout << endl;
}

void schedulingRequest::display() const {
    cout << subject << " | Section " << section << " | ";
    slot.display();
    cout << " | Priority " << priority;
}

int schedulingRequest::getPriority() const {
    return priority;
}

//  node 
node::node(schedulingRequest newRequest) {
    data = newRequest;
    next = nullptr;                    // a new node always starts with nothing after it
}

// requestQueue (linked list) 
requestQueue::requestQueue() {
    front = nullptr;
    rear = nullptr;
    count = 0;
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

// ---------------- circularSlotQueue (array) ----------------
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

// requestHeap (max-heap by priority) 
requestHeap::requestHeap() {
    heapSize = 0;
}

bool requestHeap::isEmpty() const {
    return heapSize == 0;
}

bool requestHeap::isFull() const {
    return heapSize == MAX_HEAP_SIZE;
}

void requestHeap::swapRequests(int firstIndex, int secondIndex) {
    schedulingRequest temporaryRequest = heapArray[firstIndex];
    heapArray[firstIndex] = heapArray[secondIndex];
    heapArray[secondIndex] = temporaryRequest;
}

void requestHeap::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;

        if (heapArray[index].getPriority() <= heapArray[parentIndex].getPriority()) {
            break;                                  // parent is already bigger, heap is fine
        }

        swapRequests(index, parentIndex);
        index = parentIndex;                        // keep climbing
    }
}

void requestHeap::heapifyDown(int index) {
    while (true) {
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;
        int largestIndex = index;

        if (leftChildIndex < heapSize &&
            heapArray[leftChildIndex].getPriority() > heapArray[largestIndex].getPriority()) {
            largestIndex = leftChildIndex;
        }

        if (rightChildIndex < heapSize &&
            heapArray[rightChildIndex].getPriority() > heapArray[largestIndex].getPriority()) {
            largestIndex = rightChildIndex;
        }

        if (largestIndex == index) {
            break;                                  // node is bigger than both children
        }

        swapRequests(index, largestIndex);
        index = largestIndex;                       // keep sinking
    }
}

bool requestHeap::insert(schedulingRequest newRequest) {
    if (isFull()) {
        return false;
    }

    heapArray[heapSize] = newRequest;               // add at the end
    heapifyUp(heapSize);                            // then bubble it up
    heapSize++;
    return true;
}

schedulingRequest requestHeap::extractMax() {
    if (isEmpty()) {
        return schedulingRequest();
    }

    schedulingRequest highestRequest = heapArray[0];
    heapSize--;
    heapArray[0] = heapArray[heapSize];             // move the last element to the root
    heapifyDown(0);                                 // then sink it down
    return highestRequest;
}