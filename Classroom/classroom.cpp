
#include "classroom.h"

//------------------------------
// EQUIPMENT DEFINITIONS

Equipment::Equipment(string n)
{
    name = n;
}
string Equipment::getName() 
{
    return name;
}

//----------------------------------
// CLASSROOM DEFINITIONS

// Constructor
Classroom::Classroom(string i, int c, vector<Equipment> e)
{
    id = i;
    capacity = c;
    equipmentList = e;
}

// Return classroom type
string Classroom::getType()
{
    return "Classroom";
}

// Return setup time
int Classroom::getSetupTime()
{
    return 5;
}

// Return classroom ID
string Classroom::getId()
{
    return id;
}

// Return capacity
int Classroom::getCapacity()
{
    return capacity;
}

// Display classroom details
void Classroom::display()
{
    cout << "\nRoom ID: " << id;
    cout << "\nType: " << getType();
    cout << "\nCapacity: " << capacity;
    cout << "\nSetup Time: " << getSetupTime() << " minutes";

    cout << "\nEquipment: ";

    if (equipmentList.empty())
    {
        cout << "None";
    }
    else
    {
        for (int i = 0; i < equipmentList.size(); i++)
        {
            cout << equipmentList[i].getName();

            if (i < equipmentList.size() - 1)
            {
                cout << ", ";
            }
        }
    }

    cout << endl;
}

// Destructor
Classroom::~Classroom()
{
}

//-------------------------------------
// SMART CLASSROOM DEFINITIONS

// Constructor
SmartClassroom::SmartClassroom(string i, int c,vector<Equipment> e, bool p): Classroom(i, c, e)
{
    projector = p;
}

// Return classroom type
string SmartClassroom::getType()
{
    return "Smart Classroom";
}

// Return setup time
int SmartClassroom::getSetupTime()
{
    return 5;
}

// Display details
void SmartClassroom::display()
{
    Classroom::display();

    cout << "\nProjector: ";

    if (projector)
        cout << "Yes";
    else
        cout << "No";

    cout << endl;
}

// Destructor
SmartClassroom::~SmartClassroom()
{
}
   
//------------------------
// LABORATORY DEFINITIONS

// Constructor
Laboratory::Laboratory(string i, int c, vector<Equipment> e, int comp): Classroom(i, c, e)
{
    computers = comp;
}

// Return type
string Laboratory::getType()
{
    return "Laboratory";
}

// Return setup time
int Laboratory::getSetupTime()
{
    return 15;
}

// Display details
void Laboratory::display()
{
    Classroom::display();
    cout << "Number of computers: " << computers << endl;
}

// Destructor
Laboratory::~Laboratory()
{
}


// ------------------------------
// ROOM MANAGER DEFINITIONS

// Add room
void RoomManager::addRoom(Classroom* room)
{
    if (room == nullptr)
    {
        cout << "Invalid room\n";
        return;
    }

    string id = room->getId();

    if (roomHash.find(id) != roomHash.end())
    {
        cout << "Room already exists\n";
        delete room;
        return;
    }

    roomList.push_back(room);
    roomHash[id] = room;

    roomHeap.push({room->getCapacity(), id});

    cout << "Room added successfully\n";
}


// Remove room
void RoomManager::removeRoom(string id)
{
    if (roomHash.find(id) == roomHash.end())
    {
        cout << "Room not found\n";
        return;
    }

    Classroom* room = roomHash[id];

    roomList.remove(room);
    roomHash.erase(id);

    delete room;

    rebuildHeap();

    cout << "Room removed successfully\n";
}


// Search room by ID
void RoomManager::searchRoomByID(string id) 
{
    if (roomHash.find(id) == roomHash.end())
    {
        cout << "Room not found\n";
        return;
    }

    Classroom* room = roomHash[id];
    room->display();
}


// Search room by capacity
void RoomManager::searchRoomByCapacity(int capacity) 
{
    for (auto it = roomList.begin(); it != roomList.end(); it++)
{
    Classroom* room = *it;

    if (room->getCapacity() >= capacity)
    {
        room->display();
        return;
    }
}

    cout << "No suitable room found\n";
}


// Rebuild heap
void RoomManager::rebuildHeap()
{
    while (!roomHeap.empty())
    {
        roomHeap.pop();
    }

    for (Classroom* room : roomList)
    {
        roomHeap.push({room->getCapacity(), room->getId()});
    }
}


// Show highest capacity room
void RoomManager::showHighestCapacityRoom() 
{
    if (roomHeap.empty())
    {
        cout << "No rooms available\n";
        return;
    }

    searchRoomByID(roomHeap.top().second);
}


// Display all rooms
void RoomManager::displayAllRooms()
{
    if (roomList.empty())
    {
        cout << "No rooms available\n";
        return;
    }

    for (auto it = roomList.begin(); it != roomList.end(); it++)
    {
        Classroom* room = *it;
        room->display();
        cout << "-------------------\n";
    }
}


// Destructor
RoomManager::~RoomManager()
{
    for (Classroom* room : roomList)
    {
        delete room;
    }
}
