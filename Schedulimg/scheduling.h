#ifndef TIMESLOT_REQUEST_H
#define TIMESLOT_REQUEST_H
#include<string>

using namespace std;

enum Day { MON,TUE,WED,THU,FRI,SAT};

class timeSlot{
    Day day;
    int period;

    public:
    timeSlot();   //default constructor
    timeSlot(Day d,int p);    //parameterized constructor
    Day getDay() const;
    int getPeriod() const;
    void display() const;
};

class schedulingRequest{
    string subject;
    string section;
    timeSlot slot;
    bool valid;
    int priority;                 // higher number = higher priority

    public:
    schedulingRequest();
    schedulingRequest(string subj,string sec,timeSlot ts,int pri=1);
    bool isValid();
    void process();
    int getPriority() const;
    void display() const;
};

struct node{
    schedulingRequest data;
    node* next;
    node(schedulingRequest r);
};

class requestQueue{
    node* front;
    node* rear;
    int count;

    public:
    requestQueue();
    void enqueue(schedulingRequest r);
    schedulingRequest dequeue();
    bool isEmpty() const;
    ~requestQueue();
};

const int MAX_SLOTS=6;

class circularSlotQueue{
    timeSlot slots[MAX_SLOTS];
    int front, rear, count;

    public:
    circularSlotQueue();
    bool enqueueSlot(timeSlot t);
    timeSlot dequeueSlot();
    bool isFull() const;
    bool isEmpty() const;
};

const int MAX_HEAP_SIZE=100;

// max-heap: the request with the highest priority number sits at index 0
class requestHeap{
    schedulingRequest heapArray[MAX_HEAP_SIZE];
    int heapSize;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void swapRequests(int firstIndex,int secondIndex);

    public:
    requestHeap();
    bool insert(schedulingRequest newRequest);
    schedulingRequest extractMax();
    bool isEmpty() const;
    bool isFull() const;
};

#endif