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

    public:
    schedulingRequest();
    schedulingRequest(string subj,string sec,timeSlot ts);
    bool isValid();
    void process();
    void display() const;
};

struct node{
    schedulingRequest date;
    node* next;
    node(schedulingRequet r);
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
    ~reuquestQueue();
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

#endif
