#include "timetable.h"
// Constructor
Timetable::Timetable()
{
    front = 0;
    rear = -1;
    queueSize = 0;
    top = -1;
    heapSize = 0;

    for (int i = 0; i < DAYS; i++)
    {
        for (int j = 0; j < SLOTS; j++)
        {
            occupied[i][j] = false;
        }
    }
}
// Request Queue 

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
        cout << i + 1 << ". "
             << requestQueue[index].subjectCode
             << " - "
             << requestQueue[index].subjectName
             << endl;

        index = (index + 1) % MAX_REQUESTS;
    }
}
//  Priority Queue 
bool Timetable::addPriorityRequest(SchedulingRequest request)
{
    if (heapSize == MAX_REQUESTS)
    {
        cout << "Priority queue is full.\n";
        return false;
    }

    priorityHeap[heapSize] = request;
    heapifyUp(heapSize);
    heapSize++;
    return true;
}

void Timetable::heapifyUp(int index)
{
    while (index > 0)
    {
        int parent = (index - 1) / 2;

        if (priorityHeap[parent].priority >=
            priorityHeap[index].priority)
        {
            break;
        }

        SchedulingRequest temp = priorityHeap[parent];
        priorityHeap[parent] = priorityHeap[index];
        priorityHeap[index] = temp;

        index = parent;
    }
}

void Timetable::heapifyDown(int index)
{
    while (true)
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < heapSize &&
            priorityHeap[left].priority >
            priorityHeap[largest].priority)
        {
            largest = left;
        }

        if (right < heapSize &&
            priorityHeap[right].priority >
            priorityHeap[largest].priority)
        {
            largest = right;
        }

        if (largest == index)
        {
            break;
        }

        SchedulingRequest temp = priorityHeap[index];
        priorityHeap[index] = priorityHeap[largest];
        priorityHeap[largest] = temp;

        index = largest;
    }
}

SchedulingRequest Timetable::getHighestPriorityRequest()
{
    if (priorityQueueEmpty())
    {
        cout << "Priority queue is empty.\n";
        return SchedulingRequest();
    }

    SchedulingRequest request = priorityHeap[0];

    priorityHeap[0] = priorityHeap[heapSize - 1];
    heapSize--;

    if (heapSize > 0)
    {
        heapifyDown(0);
    }

    return request;
}

bool Timetable::priorityQueueEmpty() const
{
    return heapSize == 0;
}
void Timetable::displayPriorityQueue() const
{
    if (priorityQueueEmpty())
    {
        cout << "Priority queue is empty.\n";
        return;
    }

    cout << "\nPriority Queue:\n";

    for (int i = 0; i < heapSize; i++)
    {
        cout << i + 1 << priorityHeap[i].subjectCode << "Priority: "<< priorityHeap[i].priority << ")\n";
    }
}
 