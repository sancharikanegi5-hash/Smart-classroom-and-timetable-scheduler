#ifndef TIMETABLE_H
#define TIMETABLE_H
#include <iostream>
#include <string>
using namespace std;
const int DAYS = 6;
const int SLOTS = 10;
const int MAX_REQUESTS = 100;

//will delete this structure if member 5 alredy made a class for it

struct SchedulingRequest
{
    string subjectCode;
    string subjectName;
    string facultyName;
    string sectionName;
    string roomName;
    int duration;
    int priority;    // higher number = higher priority
    SchedulingRequest()
    {
        subjectCode = "";
        subjectName = "";
        facultyName = "";
        sectionName = "";
        roomName = "";
        duration = 1;
        priority = 1;
    }
};

struct UndoItem
{
    int day;
    int slot;
    int duration;
    UndoItem()
    {
        day = -1;
        slot = -1;
        duration = 1;
    }
    UndoItem(int d, int s, int dur)
    {
        day = d;
        slot = s;
        duration = dur;
    }
    class Timetable
    {
    private:
        SchedulingRequest requestQueue[MAX_REQUESTS];
        int front;
        int rear;
        int queueSize;
        UndoItem undoStack[MAX_REQUESTS];
        int top;    
        SchedulingRequest priorityHeap[MAX_REQUESTS];
        int heapSize;
        void heapifyUp(int index);
        void heapifyDown(int index);
    public:
        Timetable();
        bool enqueueRequest( SchedulingRequest request);
    
