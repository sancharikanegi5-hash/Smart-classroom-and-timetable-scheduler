#ifndef TIMESLOT_REQUEST_H
#define TIMESLOT_REQUEST_H
#include<string>

using namespace std;

enum Day { MON,TUE,WED,THU,FRI,SAT};

const int MAX_SLOTS=6;           // periods per day (period numbers run 1..MAX_SLOTS)
const int PLACEMENT_DAYS=6;      // MON..SAT
const int MAX_HEAP_SIZE=100;

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

// A plain schedulingRequest behaves like a lecture (1 period).
// labRequest (below) overrides the virtual functions for 2 consecutive periods.
class schedulingRequest{
    string subject;
    string section;
    timeSlot slot;
    bool valid;
    int priority;                 // higher number = higher priority

    public:
    schedulingRequest();
    schedulingRequest(string subj,string sec,timeSlot ts,int pri=1);
    virtual ~schedulingRequest();
    bool isValid();
    void process();
    int getPriority() const;
    timeSlot getSlot() const;
    void setSlot(timeSlot newSlot);
    void display() const;

    // polymorphic part (Phase 2)
    virtual int getPeriodsNeeded() const;
    virtual bool generatePlacement(int day,int startPeriod,const bool occupied[][MAX_SLOTS]) const;
};

class labRequest : public schedulingRequest{
    public:
    labRequest(string subj,string sec,timeSlot ts,int pri=1);
    int getPeriodsNeeded() const override;
    bool generatePlacement(int day,int startPeriod,const bool occupied[][MAX_SLOTS]) const override;
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

// max-heap: the request with the highest priority number sits at index 0.
// It stores POINTERS so labRequest objects keep their own behaviour (no slicing).
class requestHeap{
    schedulingRequest* heapArray[MAX_HEAP_SIZE];
    int heapSize;

    void heapifyUp(int index);
    void heapifyDown(int index);
    void swapRequests(int firstIndex,int secondIndex);

    public:
    requestHeap();
    bool insert(schedulingRequest* newRequest);
    schedulingRequest* extractMax();       // nullptr if the heap is empty
    bool isEmpty() const;
    bool isFull() const;
};

// Backtracking engine: places requests (highest priority first) into a week grid.
class placementEngine{
    bool occupied[PLACEMENT_DAYS][MAX_SLOTS];     // true = taken or blocked
    schedulingRequest* requests[MAX_HEAP_SIZE];
    int requestCount;

    void markCells(int day,int startPeriod,int periodsNeeded,bool value);
    bool placeRequest(int requestIndex);          // the recursive backtracking function

    public:
    placementEngine();
    void blockSlot(int day,int period);           // e.g. room/faculty unavailable
    bool addRequest(schedulingRequest* newRequest);
    void loadFromHeap(requestHeap& heap);
    bool generateTimetable();                     // true = every request placed
    void displayTimetable() const;
};

#endif