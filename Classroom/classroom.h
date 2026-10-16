#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <queue>

using namespace std;

// EQUIPMENT CLASS

class Equipment
{
private:
    string name;

public:
    Equipment(string n){
        name=n;
    }

    string getName(){
        return name;
    }
};


// CLASSROOM BASE CLASS

class Classroom
{
protected:
    string id;
    int capacity;
    vector<Equipment> equipmentList;

public:
    // Constructor
    Classroom(string i, int c, vector<Equipment> e = {})
    {
        id = i;
        capacity = c;
        equipmentList = e;
    }

    // Getters
    string getId()
    {
        return id;
    }

    int getCapacity()
    {
        return capacity;
    }

    // Virtual functions
    virtual string getType()
    {
        return "Classroom";
    }

    virtual int getSetupTime()
    {
        return 0;
    }

    virtual void display()
    {
        cout << "Classroom ID: " << id << endl;
        cout << "Capacity: " << capacity << endl;
    }

    // Virtual destructor
    virtual ~Classroom()
    {
    }
};


// PHASE 2: SMART CLASSROOM
// Inherits from Classroom


class SmartClassroom : public Classroom
{
private:
    bool projector;

public:
    SmartClassroom(string id, int capacity,
                   const vector<Equipment>& equipment,
                   bool projector);

    string getType() const override;
    int getSetupTime() const override;
    void display() const override;

    ~SmartClassroom() override;
};


// =======================================
// PHASE 2: LABORATORY
// Inherits from Classroom
// =======================================

class Laboratory : public Classroom
{
private:
    int computers;

public:
    Laboratory(string id, int capacity,
               const vector<Equipment>& equipment,
               int computers);

    string getType() const override;
    int getSetupTime() const override;
    void display() const override;

    ~Laboratory() override;
};


// =======================================
// ROOM MANAGER
// Manages all classrooms and laboratories
// =======================================

class RoomManager
{
private:
    // Phase 1: Linked list
    list<Classroom*> roomList;

    // Phase 2: Hash table
    unordered_map<string, Classroom*> roomHash;

    // Phase 2: Max heap
    priority_queue<pair<int, string>> roomHeap;

    // Rebuild heap after deleting a room
    void rebuildHeap();

public:
    // Add and remove rooms
    void addRoom(Classroom* room);
    void removeRoom(string id);

    // Searching
    void searchRoomByID(string id) const;
    void searchRoomByCapacity(int requiredCapacity) const;

    // Heap operation
    void showHighestCapacityRoom() const;

    // Display all rooms
    void displayAllRooms() const;

    // Destructor
    ~RoomManager();

    // Avoid accidental copying of the manager
    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;
};
