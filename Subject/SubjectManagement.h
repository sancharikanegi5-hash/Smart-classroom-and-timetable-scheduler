#ifndef SUBJECT_H
#define SUBJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
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
}

struct Resource {
    string id;
    string name;
    string type;

    Resource(string, string, string);
};

template <class T>
class Repository {
private:
    vector<T> items;

public:
    void add(T item);
    T* get(int index);
    int size();
};

class ResourceGraph {
private:
    unordered_map<string, vector<string>> adj;

public:
    void addConflict(string a, string b);
    string displayGraph();
};

class DisjointSet {
private:
    unordered_map<string, string> parent;
    unordered_map<string, int> rankValue;

public:
    void add(string x);
    string find(string x);
    void unite(string a, string b);
    bool isSameSet(string a, string b);
};

#endif

