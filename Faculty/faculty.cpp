#include "faculty.h"
#include <iostream>

using namespace std;

Faculty::Faculty() {
    facultyId = 0;
    name = "";
    department = "";
    email = "";
}

Faculty::Faculty(int id, string name, string department, string email) {
    facultyId = id;
    this->name = name;
    this->department = department;
    this->email = email;
}

void Faculty::setFacultyId(int id) {
    facultyId = id;
}

void Faculty::setName(string name) {
    this->name = name;
}

void Faculty::setDepartment(string department) {
    this->department = department;
}

void Faculty::setEmail(string email) {
    this->email = email;
}

void Faculty::setAvailability(vector<string> slots) {
    availableSlots = slots;
}

int Faculty::getFacultyId() const {
    return facultyId;
}

string Faculty::getName() const {
    return name;
}

string Faculty::getDepartment() const {
    return department;
}

string Faculty::getEmail() const {
    return email;
}

vector<string> Faculty::getAvailability() const {
    return availableSlots;
}

void Faculty::displayFaculty() const {
    cout << "Faculty ID: " << facultyId << endl;
    cout << "Name: " << name << endl;
    cout << "Department: " << department << endl;
    cout << "Email: " << email << endl;

    cout << "Available Slots: ";
    for (const string& slot : availableSlots) {
        cout << slot << " ";
    }
    cout << endl;
}
