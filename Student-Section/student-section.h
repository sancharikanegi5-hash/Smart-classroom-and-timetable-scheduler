#ifndef STUDENT_SECTION_H
#define STUDENT_SECTION_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int MAX_STUDENTS = 100;

class Student {
private:
    int rollNo;
    string name;
    string section;

public:
    Student();
    Student(int roll, const string& studentName, const string& sectionCode);

    int getRollNo() const;
    string getName() const;
    string getSection() const;
    void display() const;
};

class Section {
private:
    string sectionCode;
    Student students[MAX_STUDENTS];
    int studentCount;
    vector<string> subjects;

public:
    Section();
    explicit Section(const string& code);

    string getSectionCode() const;

    bool addStudent(const Student& student);
    bool removeStudent(int rollNo);
    const Student* searchStudent(int rollNo) const;
    void displayAllStudents() const;

    void linkSubjectsToSection();
    void displaySectionDetails() const;
};

class Batch : public Section {
private:
    int labGroupNo;

public:
    Batch(const string& code, int groupNo);
    int getLabGroupNo() const;
    void displayBatch() const;
};

struct SectionNode {
    Section data;
    SectionNode* left;
    SectionNode* right;

    explicit SectionNode(const Section& section);
};

class SectionBST {
private:
    SectionNode* root;

    SectionNode* insertNode(SectionNode* node, const Section& section, bool& inserted);
    SectionNode* searchNode(SectionNode* node, const string& code) const;
    void sortedListing(const SectionNode* node) const;
    void deleteTree(SectionNode* node);

public:
    SectionBST();
    ~SectionBST();

    SectionBST(const SectionBST&) = delete;
    SectionBST& operator=(const SectionBST&) = delete;

    bool insertSection(const Section& section);
    Section* searchSection(const string& code) const;
    void sortedSectionListing() const;
};

#endif
