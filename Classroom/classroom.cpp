
#include "classroom.h"

// =======================================
// EQUIPMENT DEFINITIONS
// =======================================

Equipment::Equipment(string name)
{
    this->name = name;
}

string Equipment::getName() const
{
    return name;
}


// =======================================
// CLASSROOM DEFINITIONS
// =======================================

Classroom::Classroom(string id, int capacity,
                     const vector<Equipment>& equipment)
{
    this->id = id;
    this->capacity = capacity;
    equipmentList = equipment;
}

string Classroom::getId() const
{
    return id;
}

int Classroom::getCapacity() const
{
    return capacity;
}

string Classroom::getType() const
{
    return "Classroom";
}

int Classroom::getSetupTime() const
{
    return 5;
}

void Classroom::display() const
{
    cout << "\nRoom ID: " << id;
    cout << "\nType: " << getType();
    cout << "\nCapacity: " << capacity;
    cout << "\nSetup Time: " << getSetupTime()
         << " minutes";

    cout << "\nEquipment: ";

    if (equipmentList.empty())
    {
        cout << "None";
    }

    for (int i = 0; i < equipmentList.size(); i++)
    {
        cout << equipmentList[i].getName();

        if (i != equipmentList.size() - 1)
        {
            cout << ", ";
        }
    }

    cout << endl;
}

Classroom::~Classroom()
{
    // Equipment vector cleans itself automatically
}


// =======================================
// SMART CLASSROOM DEFINITIONS
// =======================================

SmartClassroom::SmartClassroom(
    string id, int capacity,
    const vector<Equipment>& equipment,
    bool projector)
    : Classroom(id, capacity, equipment)
{
    this->projector = projector;
}

string SmartClassroom::getType() const
{
    return "Smart Classroom";
}

int SmartClassroom::getSetupTime() const
{
    return 5;
}

void SmartClassroom::display() const
{
    Classroom::display();

    cout << "Projector: ";

    if (projector)
        cout << "Yes\n";
    else
        cout << "No\n";
}

SmartClassroom::~SmartClassroom()
{
    // Base class destructor runs automatically
}


// =======================================
// LABORATORY DEFINITIONS
// =======================================

Laboratory::Laboratory(
    string id, int capacity,
    const vector<Equipment>& equipment,
    int computers)
    : Classroom(id, capacity, equipment)
{
    this->computers = computers;
}

string Laboratory::getType() const
{
    return "Laboratory";
}

int Laboratory::getSetupTime() const
{
    return 15;
}

void Laboratory::display() const
{
    Classroom::display();

    cout << "Number of computers: "
         << computers << endl;
}

Laboratory::~Laboratory()
{
    // Base class destructor runs automatically
}


// =======================================
// ROOM MANAGER DEFINITIONS
// =======================================

// Add room to linked list, hash table and heap

void RoomManager::addRoom(Classroom* room)
{
    if (room == nullptr)
    {
        cout << "Invalid room.\n";
        return;
    }

    string roomId = room->getId();

    // Check duplicate ID
    if (roomHash.find(roomId) != roomHash.end())
    {
        cout << "Room ID already exists.\n";
        delete room;
        return;
    }

    // Phase 1: Linked list insertion
    roomList.push_back(room);

    // Phase 2: Hash table insertion
    roomHash[roomId] = room;

    // Phase 2: Heap insertion
    roomHeap.push(
        make_pair(room->getCapacity(), roomId)
    );

    cout << "Room added successfully.\n";
}


// Remove room

void RoomManager::removeRoom(string id)
{
    auto it = roomHash.find(id);

    if (it == roomHash.end())
    {
        cout << "Room not found.\n";
        return;
    }

    Classroom* room = it->second;

    // Remove from linked list
    roomList.remove(room);

    // Remove from hash table
    roomHash.erase(it);

    // Delete the room object
    delete room;

    // Rebuild heap
    rebuildHeap();

    cout << "Room removed successfully.\n";
}


// Search by ID using hash table

void RoomManager::searchRoomByID(string id) const
{
    auto it = roomHash.find(id);

    if (it == roomHash.end())
    {
        cout << "Room not found.\n";
        return;
    }

    cout << "Room found:\n";
    it->second->display();
}


// Search by capacity using linked list

void RoomManager::searchRoomByCapacity(
    int requiredCapacity) const
{
    for (Classroom* room : roomList)
    {
        if (room->getCapacity() >= requiredCapacity)
        {
            cout << "Suitable room found:\n";
            room->display();
            return;
        }
    }

    cout << "No suitable room found.\n";
}


// Rebuild heap after room deletion

void RoomManager::rebuildHeap()
{
    while (!roomHeap.empty())
    {
        roomHeap.pop();
    }

    for (auto it = roomHash.begin();
         it != roomHash.end(); it++)
    {
        Classroom* room = it->second;

        roomHeap.push(
            make_pair(room->getCapacity(),
                      room->getId())
        );
    }
}


// Show highest capacity room

void RoomManager::showHighestCapacityRoom() const
{
    if (roomHeap.empty())
    {
        cout << "No rooms available.\n";
        return;
    }

    string id = roomHeap.top().second;

    searchRoomByID(id);
}


// Display all rooms

void RoomManager::displayAllRooms() const
{
    if (roomList.empty())
    {
        cout << "No rooms available.\n";
        return;
    }

    for (Classroom* room : roomList)
    {
        room->display();

        cout << "-------------------\n";
    }
}


// RoomManager destructor

RoomManager::~RoomManager()
{
    for (Classroom* room : roomList)
    {
        delete room;
    }

    roomList.clear();
    roomHash.clear();
}

