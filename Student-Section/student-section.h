#ifndef STUDENT_SECTION_H
#define STUDENT_SECTION_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

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
    vector<Student> students;       
    vector<string> subjects;

    int findStudentIndex(int rollNo) const;

public:
    Section();
    explicit Section(const string& code);

    string getSectionCode() const;
    const vector<Student>& getStudents() const;
    const vector<string>& getSubjects() const;
    void setSubjects(const vector<string>& subjectList);

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
    void collectSections(const SectionNode* node, vector<Section>& result) const;
    void deleteTree(SectionNode* node);

public:
    SectionBST();
    ~SectionBST();

    SectionBST(const SectionBST&) = delete;
    SectionBST& operator=(const SectionBST&) = delete;

    bool insertSection(const Section& section);
    Section* searchSection(const string& code) const;
    void sortedSectionListing() const;
    vector<Section> getAllSections() const;

    
    bool saveStudents(const string& filename) const;
    bool loadStudents(const string& filename);
};


class SectionOverlapGraph {
private:
    vector<string> sectionCodes;
    vector<vector<int> > adjacency;

public:
    void buildGraph(const SectionBST& sectionTree);
    void displayGraph() const;
};

#endif
