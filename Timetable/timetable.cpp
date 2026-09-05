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



 