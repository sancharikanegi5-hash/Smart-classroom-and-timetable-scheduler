#ifndef STUDENT_SECTION_H
#define STUDENT_SECTION_H

#include <iostream>
#include <string>
using namespace std;

#define MAX 50

class Student
{
private:
    int rollNo;
    string name;
    string section;

public:
    Student();
    Student(int r, string n, string s);

    int getRollNo();
    void display();
};

class Section
{
private:
    Student list[MAX];
    int count;

public:
    Section();

    void addStudent();
    void removeStudent(int roll);
    void searchStudent(int roll);
    void displayAllStudents();
};

#endif
