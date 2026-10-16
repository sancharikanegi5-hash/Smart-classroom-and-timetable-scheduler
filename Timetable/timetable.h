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
};
struct CandidateTimetable
{
    int id;
    SchedulingRequest timetable[DAYS][SLOTS];
    bool occupied[DAYS][SLOTS];
    CandidateTimetable()
    {
        id = -1;
        for (int i = 0; i < DAYS; i++)
        {
            for (int j = 0; j < SLOTS; j++)
            {
                occupied[i][j] = false;
            }
        }
    }
};
// Base class for different timetable scoring methods
class EvaluationStrategy
{
public:
    virtual double calculateScore(
        const CandidateTimetable& candidate) const = 0;

    virtual ~EvaluationStrategy() {}
};

// Scores based on request priorities
class PriorityStrategy : public EvaluationStrategy
{
public:
    double calculateScore(
        const CandidateTimetable& candidate) const override;
};

// Scores based on gaps between classes
class GapStrategy : public EvaluationStrategy
{
public:
    double calculateScore(
        const CandidateTimetable& candidate) const override;
};
    class Timetable
    {
    private:
        SchedulingRequest requestQueue[MAX_REQUESTS];
        int front;
        int rear;
        int queueSize;
        UndoItem undoStack[MAX_REQUESTS];
        int top;    

        SchedulingRequest timetable[DAYS][SLOTS];
        bool occupied[DAYS][SLOTS];
     // Priority Queue using Max Heap
    SchedulingRequest priorityRequests[MAX_REQUESTS];
    int priorityQueueSize;
    public:

    Timetable();
    // Request Queue
    bool enqueueRequest(SchedulingRequest request);
    SchedulingRequest dequeueRequest();
    bool requestQueueEmpty() const;
    bool requestQueueFull() const;
    void displayRequestQueue() const;
  // Priority Queue
    bool enqueuePriorityRequest(SchedulingRequest request);
    SchedulingRequest dequeueHighestPriorityRequest();
    bool priorityQueueEmpty() const;
    bool priorityQueueFull() const;
    void displayPriorityQueue() const;
    // Timetable operations
    bool addClass(int day, int slot, SchedulingRequest request);
    bool removeClass(int day, int slot);
    bool isFree(int day, int slot) const;

    // Scheduling
    bool scheduleNextRequest();
    void scheduleAllRequests();

    // Undo
    void pushUndo(int day, int slot, int duration);
    void undo();

    // Display
    void display() const;
    void displayDay(int day) const;
 // Candidate timetable evaluation
    bool validateCandidate(const CandidateTimetable& candidate) const;
   double calculateScore(const CandidateTimetable& candidate) const;
   int selectBestCandidate(const CandidateTimetable candidates[], int count) const;

    ~Timetable();
};
#endif
