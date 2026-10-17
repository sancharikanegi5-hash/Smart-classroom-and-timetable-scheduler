#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

class Equipment {
private:
    std::string name;
public:
    explicit Equipment(std::string name);
    std::string getName();
};

class Classroom {
protected:
    std::string id;
    int capacity;
    std::vector<Equipment> equipmentList;
public:
    Classroom(std::string id, int capacity, std::vector<Equipment> equipment = {});
    virtual ~Classroom();
    std::string getId();
    int getCapacity();
    virtual std::string getType();
    virtual int getSetupTime();
    virtual void display();
};

class SmartClassroom : public Classroom {
private:
    bool projector;
public:
    SmartClassroom(std::string id, int capacity, std::vector<Equipment> equipment, bool projector);
    ~SmartClassroom() override;
    std::string getType() override;
    int getSetupTime() override;
    void display() override;
};

class Laboratory : public Classroom {
private:
    int computers;
public:
    Laboratory(std::string id, int capacity, std::vector<Equipment> equipment, int computers);
    ~Laboratory() override;
    std::string getType() override;
    int getSetupTime() override;
    void display() override;
};

class RoomManager {
private:
    std::list<Classroom*> roomList;
    std::unordered_map<std::string, Classroom*> roomHash;
    std::priority_queue<std::pair<int, std::string>> roomHeap;
    void rebuildHeap();
public:
    RoomManager() = default;
    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;
    void addRoom(Classroom* room);
    void removeRoom(std::string id);
    void searchRoomByID(std::string id);
    void searchRoomByCapacity(int capacity);
    void showHighestCapacityRoom();
    void displayAllRooms();
    ~RoomManager();
};

#endif
