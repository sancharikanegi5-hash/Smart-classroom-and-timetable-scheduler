#include "Subject.h"

Subject::Subject() {
    code = "";
    name = "";
    credits = 0;
}

Subject::Subject(string c, string n, int cr) {
    code = c;
    name = n;
    credits = cr;
}

string Subject::getCode() {
    return code;
}

string Subject::getName() {
    return name;
}

int Subject::getCredits() {
    return credits;
}

void Subject::displaySubject() {
    cout << code << " " << name << " " << credits << endl;
}

void Subject::displaySubject(int minCredits) {
    if (credits >= minCredits)
        cout << code << " " << name << " " << credits << endl;
}
#include "SubjectHash.h"
#include <iostream>
using namespace std;

SubjectHash::SubjectHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        table[i] = nullptr;
    }
}

int SubjectHash::hashFunction(string code) {
    int sum = 0;

    for (int i = 0; i < code.length(); i++) {
        sum += code[i];
    }

    return sum % TABLE_SIZE;
}


void SubjectHash::insertSubject(string code, string name) {
    int index = hashFunction(code);

    Subject* temp = table[index];
    while (temp != nullptr) {
        if (temp->code == code) {
            temp->name = name;
            cout << "Subject updated successfully.\n";
            return;
        }
        temp = temp->next;
    }

    Subject* newSubject = new Subject;
    newSubject->code = code;
    newSubject->name = name;
    newSubject->next = table[index];

    table[index] = newSubject;

    cout << "Subject inserted successfully.\n";
}

void SubjectHash::searchSubject(string code) {
    int index = hashFunction(code);
    Subject* temp = table[index];

    while (temp != nullptr) {
        if (temp->code == code) {
            cout << "Code: " << temp->code << endl;
            cout << "Name: " << temp->name << endl;
            return;
        }

        temp = temp->next;
    }

    cout << "Subject not found.\n";
}

void SubjectHash::deleteSubject(string code) {
    int index = hashFunction(code);
    Subject* temp = table[index];
    Subject* prev = nullptr;

    while (temp != nullptr) {
        if (temp->code == code) {
            if (prev == nullptr)
                table[index] = temp->next;
            else
                prev->next = temp->next;

            delete temp;
            cout << "Subject deleted successfully.\n";
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    cout << "Subject not found.\n";
}

void SubjectHash::displaySubjects() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        cout << "Index " << i << ": ";

        Subject* temp = table[i];

        while (temp != nullptr) {
            cout << "[" << temp->code << ", "
                 << temp->name << "] -> ";

            temp = temp->next;
        }

        cout << "NULL\n";
    }
}

SubjectHash::~SubjectHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Subject* temp = table[i];

        while (temp != nullptr) {
            Subject* del = temp;
            temp = temp->next;
            delete del;
        }
    }
}
