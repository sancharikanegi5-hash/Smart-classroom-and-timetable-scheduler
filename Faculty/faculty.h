#ifndef FACULTY_H
#define FACULTY_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Faculty {
protected:
    int facultyId;
    string name;
    string department;
    string email;
    vector<string> availableSlots;

public:
    Faculty();
    Faculty(int id, string name, string department, string email);
    virtual ~Faculty() = default;

    void setFacultyId(int id);
    void setName(string name);
    void setDepartment(string department);
    void setEmail(string email);
    void setAvailability(vector<string> slots);

    int getFacultyId() const;
    string getName() const;
    string getDepartment() const;
    string getEmail() const;
    vector<string> getAvailability() const;

    virtual bool checkAvailability(const string& slot) const;

    void displayFaculty() const;
};

class FullTimeFaculty : public Faculty {
public:
    FullTimeFaculty(int id, string name, string department, string email);
    bool checkAvailability(const string& slot) const override;
};

class VisitingFaculty : public Faculty {
public:
    VisitingFaculty(int id, string name, string department, string email);
    bool checkAvailability(const string& slot) const override;
};

#endif
