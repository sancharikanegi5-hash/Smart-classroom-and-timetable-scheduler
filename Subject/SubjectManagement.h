#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <string>
using namespace std;

class Subject {
private:
    string code;
    string name;
    int credits;

public:
    Subject();
    Subject(string c, string n, int cr);
    string getCode();
    string getName();
    int getCredits();
    void displaySubject();
    void displaySubject(int minCredits);
};
#ifndef SUBJECTHASH_H
#define SUBJECTHASH_H

#include <string>
using namespace std;

const int TABLE_SIZE = 10;

struct SubjectEntry {
    string code;
    string name;
    SubjectEntry* next;
};

class SubjectHash {
private:
    SubjectEntry* table[TABLE_SIZE];

    int hashFunction(string code);

public:
    SubjectHash();
    ~SubjectHash();

    void insertSubject(string code, string name);
    void searchSubject(string code);
    void deleteSubject(string code);
    void displaySubjects();
};

#endif
#endif
