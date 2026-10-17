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
    SmartClassroom(string i, int c, vector<Equipment> e, bool p) : Classroom(i, c, e)
    {
        projector = p;
    }
    string getType() override
    {
        return "Smart Classroom";
    }
    int getSetupTime() override
    {
        return 10;
    }
    void display() override
    {
        Classroom::display();

        cout << "Projector: ";
        if (projector)
            cout << "Available" << endl;
        else
            cout << "Not Available" << endl;
    }

    ~SmartClassroom() override
    {
    }
};


// PHASE 2: LABORATORY
// Inherits from Classroom

class Laboratory : public Classroom
{
private:
    int computers;
public:
    Laboratory(string i, int c, vector<Equipment> e, int comp) : Classroom(i, c, e)
    {
        computers = comp;
    }
    string getType() override
    {
        return "Laboratory";
    }
    int getSetupTime() override
    {
        return 20;
    }
    void display() override
    {
        Classroom::display();
        cout << "Computers: " << computers << endl;
    }
    ~Laboratory() override
    {
    }
};

// ROOM MANAGER
// Manages all classrooms and laboratories

class RoomManager
{
private:
    list<Classroom*> roomList;
    unordered_map<string, Classroom*> roomHash;
    priority_queue<pair<int, string>> roomHeap;

    void rebuildHeap()
    {
    }

public:
    // Add and remove rooms
    void addRoom(Classroom* room)
    {
    }

    void removeRoom(string id)
    {
    }

    // Search rooms
    void searchRoomByID(string id)
    {
    }

    void searchRoomByCapacity(int capacity)
    {
    }

    void showHighestCapacityRoom()
    {
    }

    // Display all rooms
    void displayAllRooms()
    {
    }

    // Destructor
    ~RoomManager()
    {
    }
};
