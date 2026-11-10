#ifndef TIMESLOT_REQUEST_H
#define TIMESLOT_REQUEST_H
#include<string>
#include<vector>
#include<map>
#include<exception>
#include<stdexcept>

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
    string getSubject() const;
    string getSection() const;
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

// Thrown when no timetable can satisfy every request (instead of looping forever).
class SchedulingFailedException : public exception{
    string reason;

    public:
    SchedulingFailedException(string failureReason);
    const char* what() const noexcept override;
};

// Generic max-heap: ONE template reused for requests, faculty, rooms, ...
// Each item is stored together with its own priority number. Backed by an STL vector.
template<class T>
class PriorityQueue{
    struct entry{
        T item;
        int priority;
    };

    vector<entry> entries;

    void swapEntries(int firstIndex,int secondIndex){
        entry temporaryEntry=entries[firstIndex];
        entries[firstIndex]=entries[secondIndex];
        entries[secondIndex]=temporaryEntry;
    }

    void heapifyUp(int index){
        while(index>0){
            int parentIndex=(index-1)/2;
            if(entries[index].priority<=entries[parentIndex].priority){
                break;                              // parent already bigger, heap is fine
            }
            swapEntries(index,parentIndex);
            index=parentIndex;
        }
    }

    void heapifyDown(int index){
        int heapSize=static_cast<int>(entries.size());
        while(true){
            int leftChildIndex=2*index+1;
            int rightChildIndex=2*index+2;
            int largestIndex=index;

            if(leftChildIndex<heapSize && entries[leftChildIndex].priority>entries[largestIndex].priority){
                largestIndex=leftChildIndex;
            }
            if(rightChildIndex<heapSize && entries[rightChildIndex].priority>entries[largestIndex].priority){
                largestIndex=rightChildIndex;
            }
            if(largestIndex==index){
                break;                              // node is bigger than both children
            }
            swapEntries(index,largestIndex);
            index=largestIndex;
        }
    }

    public:
    void insert(T newItem,int newPriority){
        entry newEntry;
        newEntry.item=newItem;
        newEntry.priority=newPriority;
        entries.push_back(newEntry);                // add at the end
        heapifyUp(static_cast<int>(entries.size())-1);
    }

    T extractMax(){
        if(entries.empty()){
            throw out_of_range("PriorityQueue is empty");
        }
        T highestItem=entries[0].item;
        entries[0]=entries[entries.size()-1];       // move the last entry to the root
        entries.pop_back();
        if(!entries.empty()){
            heapifyDown(0);
        }
        return highestItem;
    }

    bool isEmpty() const{
        return entries.empty();
    }

    int size() const{
        return static_cast<int>(entries.size());
    }
};

// Backtracking engine: sorts requests, places them into a week grid, and throws
// SchedulingFailedException if the set of requests is impossible to schedule.
class placementEngine{
    bool occupied[PLACEMENT_DAYS][MAX_SLOTS];     // true = taken or blocked
    vector<schedulingRequest*> requests;
    int furthestRequestIndex;                     // deepest request the search reached

    void markCells(int day,int startPeriod,int periodsNeeded,bool value);
    bool placeRequest(int requestIndex);          // the recursive backtracking function
    void sortRequests();                          // priority first, longer requests first on ties

    public:
    placementEngine();
    void blockSlot(int day,int period);           // e.g. room/faculty unavailable
    bool addRequest(schedulingRequest* newRequest);
    void loadFromHeap(requestHeap& heap);
    void loadFromPriorityQueue(PriorityQueue<schedulingRequest*>& priorityQueue);
    void generateTimetable();                     // throws SchedulingFailedException
    void displayTimetable() const;
    void displayBySection() const;                // uses STL map of vectors
};

#endif