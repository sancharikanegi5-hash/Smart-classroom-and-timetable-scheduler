#include "timetable.h"
Timetable::Timetable()
{
    front = 0;
    rear = -1;
    queueSize = 0;
    top = -1;
    //priority queue
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
    // Check if Priority Queue is empty
bool Timetable::priorityQueueEmpty() const
{
    return priorityQueueSize == 0;
}

// to check if priority queue is full
bool Timetable::priorityQueueFull() const
{
    return priorityQueueSize == MAX_REQUESTS;
}
// to insert request according to priority
bool Timetable::enqueuePriorityRequest(SchedulingRequest request)
{
    if (priorityQueueFull())
    {
        cout << "Priority queue is full\n";
        return false;
    }
    int i = priorityQueueSize;
    priorityRequests[i] = request;
    priorityQueueSize++;
    // Heapify up
    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (priorityRequests[i].priority >
            priorityRequests[parent].priority)
        {
            swap(priorityRequests[i], priorityRequests[parent]);
            i = parent;
        }
        else
        {
            break;
        }
    }

    return true;
}
// Remove highest-priority request
SchedulingRequest Timetable::dequeueHighestPriorityRequest()
{
    if (priorityQueueEmpty())
    {
        cout << "Priority queue is empty\n";
        return SchedulingRequest();
    }

    SchedulingRequest highest = priorityRequests[0];
    priorityRequests[0] = priorityRequests[priorityQueueSize - 1];
    priorityQueueSize--;

    // Heapify down
    int i = 0;
    while (true)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < priorityQueueSize &&
            priorityRequests[left].priority >
            priorityRequests[largest].priority)
        {
            largest = left;
        }

        if (right < priorityQueueSize &&
            priorityRequests[right].priority >
            priorityRequests[largest].priority)
        {
            largest = right;
        }

        if (largest == i)
            break;

        swap(priorityRequests[i], priorityRequests[largest]);
        i = largest;
    }
    return highest;
}
// Display Priority Queue
void Timetable::displayPriorityQueue() const
{
    if (priorityQueueEmpty())
    {
        cout << "Priority queue is empty\n";
        return;
    }

    cout << "\nPriority Queue:\n";

    for (int i = 0; i < priorityQueueSize; i++)
    {
        cout << "Subject: " << priorityRequests[i].subjectCode<< " | Name: "<< priorityRequests[i].subjectName<< " | Priority: "<< priorityRequests[i].priority<< endl;
    }
}
}
