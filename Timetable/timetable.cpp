#include "timetable.h"
Timetable::Timetable()
{
    front = 0;
    rear = -1;
    queueSize = 0;
    top = -1;

    for (int i = 0; i < DAYS; i++)
    {
        for (int j = 0; j < SLOTS; j++)
        {
            occupied[i][j] = false;
        }
    }
}
//priority queue
Timetable::Timetable()
{
    front = 0;
    rear = -1;
    queueSize = 0;
    top = -1;

    priorityQueueSize = 0;

    for (int i = 0; i < DAYS; i++)
    {
        for (int j = 0; j < SLOTS; j++)
        {
            occupied[i][j] = false;
        }
    }
}
//request Q

bool Timetable::enqueueRequest(SchedulingRequest request)
{
    if (requestQueueFull())
    {
        cout << "Request queue is full\n";
        return false;
    }
    rear = (rear + 1) % MAX_REQUESTS;
    requestQueue[rear] = request;
    queueSize++;
    return true;
}
SchedulingRequest Timetable::dequeueRequest()
{
    if (requestQueueEmpty())
    {
        cout << "Request queue is empty.\n";
        return SchedulingRequest();
    }

    SchedulingRequest request = requestQueue[front];
    front = (front + 1) % MAX_REQUESTS;
    queueSize--;
    return request;
}

bool Timetable::requestQueueEmpty() const
{
    return queueSize == 0;
}

bool Timetable::requestQueueFull() const
{
    return queueSize == MAX_REQUESTS;
} 

void Timetable::displayRequestQueue() const
{
    if (requestQueueEmpty())
    {
        cout << "Request queue is empty.\n";
        return;
    }

    cout << "\nRequest Queue:\n";

    int index = front;

    for (int i = 0; i < queueSize; i++)
    {
        cout << i + 1 << ". "<< requestQueue[index].subjectCode<< " - "<< requestQueue[index].subjectName << endl;

        index = (index + 1) % MAX_REQUESTS;
    }
}
bool Timetable::isFree(int day, int slot) const
{
    if (day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS)
        return false;

    return !occupied[day][slot];
}

bool Timetable::addClass(int day, int slot, SchedulingRequest request)
{
    if (day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS)
    {
        cout << "Invalid day or slot.\n";
        return false;
    }

    if (request.duration <= 0 || slot + request.duration > SLOTS)
    {
        cout << "Invalid duration.\n";
        return false;
    }

    for (int i = slot; i < slot + request.duration; i++)
    {
        if (occupied[day][i])
        {
            cout << "Slot already occupied\n";
            return false;
        }
    }

    for (int i = slot; i < slot + request.duration; i++)
    {
        timetable[day][i] = request;
        occupied[day][i] = true;
    }

    pushUndo(day, slot, request.duration);

    cout << "Class added successfully\n";
    return true;
}

bool Timetable::removeClass(int day, int slot)
{
    if (day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS)
    {
        cout << "Invalid day or slot.\n";
        return false;
    }

    if (!occupied[day][slot])
    {
        cout << "No class in this slot.\n";
        return false;
    }

    SchedulingRequest request = timetable[day][slot];
    int start = slot;

    while (start > 0 &&
           occupied[day][start - 1] &&
           timetable[day][start - 1].subjectCode == request.subjectCode &&
           timetable[day][start - 1].sectionName == request.sectionName)
    {
        start--;
    }

    for (int i = start; i < start + request.duration && i < SLOTS; i++)
    {
        if (occupied[day][i] &&
            timetable[day][i].subjectCode == request.subjectCode &&
            timetable[day][i].sectionName == request.sectionName)
        {
            timetable[day][i] = SchedulingRequest();
            occupied[day][i] = false;
        }
    }

    cout << "Class removed successfully\n";
    return true;
}
