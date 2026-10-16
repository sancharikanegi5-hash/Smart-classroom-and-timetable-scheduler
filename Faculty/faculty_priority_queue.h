#ifndef FACULTY_PRIORITY_QUEUE_H
#define FACULTY_PRIORITY_QUEUE_H

#include <queue>
#include <string>

using namespace std;

struct AllocationRequest {
    int facultyId;
    string slot;
    int priority;

    bool operator<(const AllocationRequest& other) const {
        return priority < other.priority;
    }
};

class FacultyPriorityQueue {
private:
    priority_queue<AllocationRequest> requests;

public:
    void addRequest(int id, string slot, int priority) {
        requests.push({id, slot, priority});
    }

    bool empty() const {
        return requests.empty();
    }

    AllocationRequest getNextRequest() {
        AllocationRequest request = requests.top();
        requests.pop();
        return request;
    }
};

#endif