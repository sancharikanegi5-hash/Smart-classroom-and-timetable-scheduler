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
    Subject() {
        code = "";
        name = "";
        credits = 0;
    }

    Subject(string c, string n, int cr) {
        code = c;
        name = n;
        credits = cr;
    }

    string getCode() {
        return code;
    }

    string getName() {
        return name;
    }

    int getCredits() {
        return credits;
    }

    void displaySubject() {
        cout << code << " " << name << " " << credits << endl;
    }

    void displaySubject(int minCredits) {
        if (credits >= minCredits)
            cout << code << " " << name << " " << credits << endl;
    }
};
#ifndef SUBJECTHASH_H
#define SUBJECTHASH_H

#include <string>
using namespace std;

const int TABLE_SIZE = 10;

struct Subject {
    string code;
    string name;
    Subject* next;
};

class SubjectHash {
private:
    Subject* table[TABLE_SIZE];

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
