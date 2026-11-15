#include "timetable.h"
#include <iomanip>
Timetable::Timetable(int initialSectionCapacity) {
    sectionCapacity = initialSectionCapacity > 0 ? initialSectionCapacity : 10;
    sectionCount = 0;
    sections = new SectionGrid[sectionCapacity];

    undoCapacity = 1000;
    undoStack = new UndoItem[undoCapacity];
    top = -1;

    priorityCapacity = 1000;
    priorityRequests = new SchedulingRequest[priorityCapacity];
    priorityQueueSize = 0;

    requestListCapacity = 1000;
    requestList = new SchedulingRequest[requestListCapacity];
    totalRequests = 0;

    requestQueueCapacity = 1000;
    requestQueue = new SchedulingRequest[requestQueueCapacity];
    front = 0;
    rear = -1;
    queueSize = 0;

    roomCapacity = 50;
    availableRooms = new RoomInfo[roomCapacity];
    roomCount = 0;

    workingDays = 5;        
    periodsPerDay = 8;      
    breakAfterPeriod = 6;   
}


Timetable::~Timetable() {
    delete[] sections;
    delete[] undoStack;
    delete[] priorityRequests;
    delete[] requestList;
    delete[] requestQueue;
    delete[] availableRooms;
}

void Timetable::resizeSections(int newCapacity) {
    SectionGrid* newSections = new SectionGrid[newCapacity];
    for (int i = 0; i < sectionCount; i++) {
        newSections[i] = sections[i]; 
    }
    delete[] sections;
    sections = newSections;
    sectionCapacity = newCapacity;
}

void Timetable::resizeUndoStack() {
    int newCap = undoCapacity * 2;
    UndoItem* newStack = new UndoItem[newCap];
    for (int i = 0; i <= top; i++) {
        newStack[i] = undoStack[i];
    }
    delete[] undoStack;
    undoStack = newStack;
    undoCapacity = newCap;
}

void Timetable::resizePriorityQueue() {
    int newCap = priorityCapacity * 2;
    SchedulingRequest* newHeap = new SchedulingRequest[newCap];
    for (int i = 0; i < priorityQueueSize; i++) {
        newHeap[i] = priorityRequests[i];
    }
    delete[] priorityRequests;
    priorityRequests = newHeap;
    priorityCapacity = newCap;
}

void Timetable::resizeRequestList() {
    int newCap = requestListCapacity * 2;
    SchedulingRequest* newList = new SchedulingRequest[newCap];
    for (int i = 0; i < totalRequests; i++) {
        newList[i] = requestList[i];
    }
    delete[] requestList;
    requestList = newList;
    requestListCapacity = newCap;
}

void Timetable::resizeRooms() {
    int newCap = roomCapacity * 2;
    RoomInfo* newRooms = new RoomInfo[newCap];
    for (int i = 0; i < roomCount; i++) {
        newRooms[i] = availableRooms[i];
    }
    delete[] availableRooms;
    availableRooms = newRooms;
    roomCapacity = newCap;
}


void Timetable::addRoom(const string& id, const string& type, int capacity) {
    if (id.empty()) return;
    for (int i = 0; i < roomCount; i++) {
        if (availableRooms[i].id == id) {
            availableRooms[i].type = type.empty() ? "Classroom" : type;
            availableRooms[i].capacity = capacity > 0 ? capacity : 60;
            return;
        }
    }
    if (roomCount >= roomCapacity) {
        resizeRooms();
    }
    availableRooms[roomCount++] = RoomInfo(id, type.empty() ? "Classroom" : type, capacity > 0 ? capacity : 60);
}

int Timetable::getRoomCount() const { return roomCount; }
string Timetable::getRoomId(int idx) const { return (idx >= 0 && idx < roomCount) ? availableRooms[idx].id : ""; }
string Timetable::getRoomType(int idx) const { return (idx >= 0 && idx < roomCount) ? availableRooms[idx].type : ""; }
int Timetable::getRoomCapacity(int idx) const { return (idx >= 0 && idx < roomCount) ? availableRooms[idx].capacity : 60; }


int Timetable::addSection(const string& name, const string& course, const string& sem, const string& coord, int strength) {
    if (name.empty()) return -1;


    for (int i = 0; i < sectionCount; i++) {
        if (sections[i].name == name) {
            if (!course.empty()) sections[i].course = course;
            if (!sem.empty()) sections[i].semester = sem;
            if (!coord.empty()) sections[i].coordinator = coord;
            if (strength > 0) sections[i].studentStrength = strength;
            return i;
        }
    }

    // Dynamic expansion if capacity reached
    if (sectionCount >= sectionCapacity) {
        resizeSections(sectionCapacity * 2);
    }

    int idx = sectionCount++;
    sections[idx].name = name;
    sections[idx].course = course.empty() ? "B.Tech C.S.E" : course;
    sections[idx].semester = sem.empty() ? "III" : sem;
    sections[idx].coordinator = coord;
    sections[idx].studentStrength = strength > 0 ? strength : 60;
    return idx;
}

int Timetable::getSectionIndex(const string& name) const {
    for (int i = 0; i < sectionCount; i++) {
        if (sections[i].name == name) return i;
    }
    return -1;
}

int Timetable::getSectionCount() const { return sectionCount; }
string Timetable::getSectionName(int idx) const { return (idx >= 0 && idx < sectionCount) ? sections[idx].name : ""; }
string Timetable::getSectionCourse(int idx) const { return (idx >= 0 && idx < sectionCount) ? sections[idx].course : ""; }
string Timetable::getSectionSemester(int idx) const { return (idx >= 0 && idx < sectionCount) ? sections[idx].semester : ""; }
string Timetable::getSectionCoordinator(int idx) const { return (idx >= 0 && idx < sectionCount) ? sections[idx].coordinator : ""; }
int Timetable::getSectionStrength(int idx) const { return (idx >= 0 && idx < sectionCount) ? sections[idx].studentStrength : 60; }
void Timetable::setSectionStrength(int idx, int strength) {
    if (idx >= 0 && idx < sectionCount && strength > 0) {
        sections[idx].studentStrength = strength;
    }
}

void Timetable::setWorkingDays(int days) {
    if (days >= 1 && days <= DAYS) workingDays = days;
}

void Timetable::setPeriodsPerDay(int periods) {
    if (periods >= 1 && periods <= SLOTS) periodsPerDay = periods;
}

void Timetable::setBreakAfterPeriod(int period) {
    breakAfterPeriod = period;
}

int Timetable::getWorkingDays() const { return workingDays; }
int Timetable::getPeriodsPerDay() const { return periodsPerDay; }
int Timetable::getBreakAfterPeriod() const { return breakAfterPeriod; }

//  Circular Queue (Request Buffer) 
bool Timetable::enqueueRequest(SchedulingRequest request) {
    if (requestQueueFull()) {
        return false;
    }
    rear = (rear + 1) % requestQueueCapacity;
    requestQueue[rear] = request;
    queueSize++;
    return true;
}

SchedulingRequest Timetable::dequeueRequest() {
    if (requestQueueEmpty()) {
        return SchedulingRequest();
    }
    SchedulingRequest req = requestQueue[front];
    front = (front + 1) % requestQueueCapacity;
    queueSize--;
    return req;
}

bool Timetable::requestQueueEmpty() const { return queueSize == 0; }
bool Timetable::requestQueueFull() const { return queueSize == requestQueueCapacity; }

void Timetable::displayRequestQueue() const {
    if (requestQueueEmpty()) {
        cout << "Request queue is empty." << endl;
        return;
    }
    cout << "\nRequest Queue (" << queueSize << " items):" << endl;
    int idx = front;
    for (int i = 0; i < queueSize; i++) {
        cout << i + 1 << ". " << requestQueue[idx].subjectCode << " ("
             << requestQueue[idx].sectionName << ") - "
             << requestQueue[idx].facultyName << endl;
        idx = (idx + 1) % requestQueueCapacity;
    }
}

// ============ Priority Queue (Max-Heap) ============
bool Timetable::priorityQueueEmpty() const { return priorityQueueSize == 0; }
bool Timetable::priorityQueueFull() const { return false; /* resizes dynamically */ }

bool Timetable::enqueuePriorityRequest(SchedulingRequest request) {
    if (priorityQueueSize >= priorityCapacity) {
        resizePriorityQueue();
    }

    int i = priorityQueueSize++;
    priorityRequests[i] = request;

    // Heapify Up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (priorityRequests[i].priority > priorityRequests[parent].priority) {
            SchedulingRequest temp = priorityRequests[i];
            priorityRequests[i] = priorityRequests[parent];
            priorityRequests[parent] = temp;
            i = parent;
        } else {
            break;
        }
    }
    return true;
}

SchedulingRequest Timetable::dequeueHighestPriorityRequest() {
    if (priorityQueueEmpty()) return SchedulingRequest();

    SchedulingRequest highest = priorityRequests[0];
    priorityQueueSize--;
    priorityRequests[0] = priorityRequests[priorityQueueSize];

    // Heapify Down
    int i = 0;
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < priorityQueueSize && priorityRequests[left].priority > priorityRequests[largest].priority) {
            largest = left;
        }
        if (right < priorityQueueSize && priorityRequests[right].priority > priorityRequests[largest].priority) {
            largest = right;
        }
        if (largest == i) break;

        SchedulingRequest temp = priorityRequests[i];
        priorityRequests[i] = priorityRequests[largest];
        priorityRequests[largest] = temp;
        i = largest;
    }
    return highest;
}

void Timetable::displayPriorityQueue() const {
    if (priorityQueueEmpty()) {
        cout << "Priority queue is empty." << endl;
        return;
    }
    cout << "\nPriority Queue (" << priorityQueueSize << " items):" << endl;
    for (int i = 0; i < priorityQueueSize; i++) {
        cout << "Sub: " << priorityRequests[i].subjectCode
             << " | Sec: " << priorityRequests[i].sectionName
             << " | Fac: " << priorityRequests[i].facultyName
             << " | Pri: " << priorityRequests[i].priority << endl;
    }
}

// 1. Faculty conflict- Same faculty cannot teach 2 different sections at the same time
bool Timetable::hasFacultyConflict(int day, int slot, const string& facultyName) const {
    if (facultyName.empty() || facultyName == "TBD") return false;
    for (int s = 0; s < sectionCount; s++) {
        if (sections[s].occupied[day][slot] && sections[s].grid[day][slot].facultyName == facultyName) {
            return true;
        }
    }
    return false;
}

// 2. Classroom conflict: Same classroom cannot host 2 different sections at the same time
bool Timetable::hasRoomConflict(int day, int slot, const string& roomName) const {
    if (roomName.empty() || roomName == "TBD") return false;
    for (int s = 0; s < sectionCount; s++) {
        if (sections[s].occupied[day][slot] && sections[s].grid[day][slot].roomName == roomName) {
            return true;
        }
    }
    return false;
}

// 3. Section conflict: Same section cannot have 2 classes at the same time
bool Timetable::hasSectionConflict(int secIdx, int day, int slot) const {
    if (secIdx < 0 || secIdx >= sectionCount) return true;
    return sections[secIdx].occupied[day][slot];
}

// Check if a section already has this subject on this day (to make class evenly spread
bool Timetable::subjectAlreadyOnDay(int secIdx, int day, const string& subjectCode) const {
    if (secIdx < 0 || secIdx >= sectionCount || subjectCode.empty()) return false;
    for (int slot = 0; slot < periodsPerDay; slot++) {
        if (sections[secIdx].occupied[day][slot] && sections[secIdx].grid[day][slot].subjectCode == subjectCode) {
            return true;
        }
    }
    return false;
}

// Check section availability, faculty availability, duration, and lunch break (without room check)
bool Timetable::isValidSlotBasic(int secIdx, int day, int slot, int duration, const string& facultyName) const {
    if (secIdx < 0 || secIdx >= sectionCount) return false;
    if (day < 0 || day >= workingDays) return false;
    if (slot < 0 || slot + duration > periodsPerDay) return false;

    // Break check: class cannot span or touch the lunch break
    if (breakAfterPeriod > 0) {
        for (int k = 0; k < duration; k++) {
            if (slot + k == breakAfterPeriod) {
                return false;
            }
        }
    }

    // Check all required consecutive slots
    for (int k = 0; k < duration; k++) {
        int currentSlot = slot + k;

        // Condition 1: Section must be free at this period
        if (hasSectionConflict(secIdx, day, currentSlot)) {
            return false;
        }

        // Condition 2: Faculty must not be teaching any section at this period
        if (hasFacultyConflict(day, currentSlot, facultyName)) {
            return false;
        }
    }

    return true;
}

// Check if a specific room is suitable for the class type (Theory/Lab), capacity, and free for all consecutive slots
bool Timetable::isRoomSuitableAndFree(int roomIdx, int day, int slot, int duration, bool isLab, int requiredStrength) const {
    if (roomIdx < 0 || roomIdx >= roomCount) return false;

    // Capacity check: if requiredStrength > 0, room capacity must be >= requiredStrength
    if (requiredStrength > 0 && availableRooms[roomIdx].capacity < requiredStrength) {
        return false;
    }

    // Identify required classroom type
    string reqType = isLab ? "Laboratory" : "Classroom";

    // Check if rooms of the exact required type exist in the pool
    bool poolHasReqType = false;
    for (int i = 0; i < roomCount; i++) {
        if (availableRooms[i].type == reqType) {
            poolHasReqType = true;
            break;
        }
    }

    // If rooms of the required type exist, the selected room must match that type
    if (poolHasReqType && availableRooms[roomIdx].type != reqType) {
        return false;
    }

    // Check whether the room is already occupied during the proposed time slot across all sections
    for (int k = 0; k < duration; k++) {
        if (hasRoomConflict(day, slot + k, availableRooms[roomIdx].id)) {
            return false;
        }
    }

    return true;
}

// Validate whether a class session can be scheduled at (day, slot) for section secIdx
bool Timetable::isValidSlot(int secIdx, int day, int slot, const SchedulingRequest& request) const {
    int duration = request.duration;

    // Boundary check
    if (secIdx < 0 || secIdx >= sectionCount) return false;
    if (day < 0 || day >= workingDays) return false;
    if (slot < 0 || slot + duration > periodsPerDay) return false;

    // Break check: class cannot span or touch the lunch break
    if (breakAfterPeriod > 0) {
        for (int k = 0; k < duration; k++) {
            if (slot + k == breakAfterPeriod) {
                return false;
            }
        }
    }

    // Check all required consecutive slots
    for (int k = 0; k < duration; k++) {
        int currentSlot = slot + k;

        
        if (hasSectionConflict(secIdx, day, currentSlot)) {
            return false;
        }

       
        if (hasFacultyConflict(day, currentSlot, request.facultyName)) {
            return false;
        }

        if (hasRoomConflict(day, currentSlot, request.roomName)) {
            return false;
        }
    }

    return true;
}


bool Timetable::addClass(int secIdx, int day, int slot, SchedulingRequest request) {
    if (secIdx < 0 || secIdx >= sectionCount || day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS) {
        return false;
    }
    for (int k = 0; k < request.duration; k++) {
        sections[secIdx].grid[day][slot + k] = request;
        sections[secIdx].occupied[day][slot + k] = true;
    }
    pushUndo(secIdx, day, slot, request.duration);
    return true;
}

bool Timetable::addCombinedClass(const int secIndices[], int count, int day, int slot, SchedulingRequest request) {
    if (count <= 0 || day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS) {
        return false;
    }
    for (int c = 0; c < count; c++) {
        int secIdx = secIndices[c];
        if (secIdx < 0 || secIdx >= sectionCount) return false;
    }

    for (int c = 0; c < count; c++) {
        int secIdx = secIndices[c];
        for (int k = 0; k < request.duration; k++) {
            sections[secIdx].grid[day][slot + k] = request;
            sections[secIdx].occupied[day][slot + k] = true;
        }
    }

    pushCombinedUndo(secIndices, count, day, slot, request.duration);
    return true;
}

bool Timetable::isFree(int secIdx, int day, int slot) const {
    if (secIdx < 0 || secIdx >= sectionCount || day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS) {
        return false;
    }
    return !sections[secIdx].occupied[day][slot];
}

const SchedulingRequest& Timetable::getCell(int secIdx, int day, int slot) const {
    return sections[secIdx].grid[day][slot];
}

bool Timetable::isOccupied(int secIdx, int day, int slot) const {
    if (secIdx < 0 || secIdx >= sectionCount || day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS) {
        return false;
    }
    return sections[secIdx].occupied[day][slot];
}


void Timetable::pushUndo(int secIdx, int day, int slot, int duration) {
    if (top >= undoCapacity - 1) {
        resizeUndoStack();
    }
    top++;
    undoStack[top] = UndoItem(secIdx, day, slot, duration);
}

void Timetable::pushCombinedUndo(const int secIndices[], int count, int day, int slot, int duration) {
    if (top >= undoCapacity - 1) {
        resizeUndoStack();
    }
    top++;
    undoStack[top] = UndoItem(secIndices, count, day, slot, duration);
}

void Timetable::undo() {
    if (top < 0) return;

    UndoItem item = undoStack[top--];
    int d = item.day;
    int s = item.slot;
    int dur = item.duration;

    for (int c = 0; c < item.sectionIndicesCount; c++) {
        int sec = item.sectionIndices[c];
        if (sec >= 0 && sec < sectionCount && d >= 0 && d < DAYS) {
            for (int k = 0; k < dur && (s + k) < SLOTS; k++) {
                sections[sec].grid[d][s + k] = SchedulingRequest();
                sections[sec].occupied[d][s + k] = false;
            }
        }
    }
}
void Timetable::displaySection(int secIdx) const {
    if (secIdx < 0 || secIdx >= sectionCount) return;

    cout << "\n========================================================" << endl;
    cout << "  TIMETABLE: SECTION " << sections[secIdx].name
         << " (" << sections[secIdx].course << " - Sem " << sections[secIdx].semester << ")" << endl;
    cout << "========================================================" << endl;

    cout << setw(8) << "DAY";
    for (int j = 0; j < periodsPerDay; j++) {
        if (breakAfterPeriod > 0 && j == breakAfterPeriod) {
            cout << setw(10) << "[BREAK]";
        }
        cout << setw(10) << ("P" + to_string(j + 1));
    }
    cout << endl;

    for (int i = 0; i < workingDays; i++) {
        cout << setw(8) << DAY_NAMES[i];
        for (int j = 0; j < periodsPerDay; j++) {
            if (breakAfterPeriod > 0 && j == breakAfterPeriod) {
                cout << setw(10) << "---";
            }
            if (sections[secIdx].occupied[i][j]) {
                cout << setw(10) << sections[secIdx].grid[i][j].subjectCode;
            } else {
                cout << setw(10) << "---";
            }
        }
        cout << endl;
    }
}

void Timetable::display() const {
    for (int s = 0; s < sectionCount; s++) {
        displaySection(s);
    }
}

// ============ Candidate Evaluation Strategies (Polymorphism) ============
double PriorityStrategy::calculateScore(const CandidateTimetable& candidate) const {
    double score = 0.0;
    for (int day = 0; day < DAYS; day++) {
        for (int slot = 0; slot < SLOTS; slot++) {
            if (candidate.occupied[day][slot]) {
                score += candidate.timetable[day][slot].priority * 2.0;
            }
        }
    }
    return score;
}

double GapStrategy::calculateScore(const CandidateTimetable& candidate) const {
    double score = 0.0;
    for (int day = 0; day < DAYS; day++) {
        int first = -1;
        int last = -1;
        for (int slot = 0; slot < SLOTS; slot++) {
            if (candidate.occupied[day][slot]) {
                if (first == -1) first = slot;
                last = slot;
            }
        }
        if (first != -1) {
            for (int slot = first; slot <= last; slot++) {
                if (!candidate.occupied[day][slot]) {
                    score -= 3.0;
                }
            }
        }
    }
    return score;
}

bool Timetable::validateCandidate(const CandidateTimetable& candidate) const {
    for (int day = 0; day < DAYS; day++) {
        for (int slot = 0; slot < SLOTS; slot++) {
            if (!candidate.occupied[day][slot]) continue;
            const SchedulingRequest& req = candidate.timetable[day][slot];
            if (req.duration <= 0 || slot + req.duration > SLOTS) return false;
            for (int k = 0; k < req.duration; k++) {
                if (!candidate.occupied[day][slot + k]) return false;
            }
        }
    }
    return true;
}

double Timetable::calculateScore(const CandidateTimetable& candidate) const {
    if (!validateCandidate(candidate)) return -1.0;
    PriorityStrategy priorityStrat;
    GapStrategy gapStrat;

    EvaluationStrategy* strategy = &priorityStrat;
    double pScore = strategy->calculateScore(candidate);

    strategy = &gapStrat;
    double gScore = strategy->calculateScore(candidate);

    return pScore + gScore;
}
