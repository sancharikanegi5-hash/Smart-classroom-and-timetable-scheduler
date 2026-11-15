#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Equipment Class
class Equipment {
private:
    string name;

public:
    explicit Equipment(string name);
    string getName();
};

// Classroom Base Class
class Classroom {
protected:
    string id;
    int capacity;
    vector<Equipment> equipmentList;

public:
    Classroom(string id, int capacity, vector<Equipment> equipment = {});

    virtual ~Classroom();

    string getId();
    int getCapacity();

    virtual string getType();
    virtual int getSetupTime();
    virtual void display();
};

// SmartClassroom Derived Class
class SmartClassroom : public Classroom {
private:
    bool projector;

public:
    SmartClassroom(string id, int capacity, vector<Equipment> equipment, bool projector);

    ~SmartClassroom() override;

    string getType() override;
    int getSetupTime() override;
    void display() override;
};

// Laboratory Derived Class
class Laboratory : public Classroom {
private:
    int computers;

public:
    Laboratory(string id, int capacity, vector<Equipment> equipment, int computers);

    ~Laboratory() override;

    string getType() override;
    int getSetupTime() override;
    void display() override;
};

// RoomManager Class
class RoomManager {
private:
    list<Classroom*> roomList;
    unordered_map<string, Classroom*> roomHash;
    priority_queue<pair<int, string>> roomHeap;

    void rebuildHeap();

public:
    RoomManager() = default;

    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;

    void addRoom(Classroom* room);
    void removeRoom(string id);
    void searchRoomByID(string id);
    void searchRoomByCapacity(int capacity);
    void showHighestCapacityRoom();
    void displayAllRooms();

    ~RoomManager();
};

#endif
