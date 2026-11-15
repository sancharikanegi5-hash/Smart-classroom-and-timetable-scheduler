#include "SubjectManagement.h"
#include <sstream>

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

    SubjectEntry* temp = table[index];
    while (temp != nullptr) {
        if (temp->code == code) {
            temp->name = name;
            cout << "Subject updated successfully.\n";
            return;
        }
        temp = temp->next;
    }

    SubjectEntry* newSubject = new SubjectEntry;
    newSubject->code = code;
    newSubject->name = name;
    newSubject->next = table[index];

    table[index] = newSubject;

    cout << "Subject inserted successfully.\n";
}

void SubjectHash::searchSubject(string code) {
    int index = hashFunction(code);
    SubjectEntry* temp = table[index];

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
    SubjectEntry* temp = table[index];
    SubjectEntry* prev = nullptr;

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

        SubjectEntry* temp = table[i];

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
        SubjectEntry* temp = table[i];

        while (temp != nullptr) {
            SubjectEntry* del = temp;
            temp = temp->next;
            delete del;
        }
    }
}
Resource::Resource(string i, string n, string t) {
    id = i;
    name = n;
    type = t;
}
template <class T>
void Repository<T>::add(T item) {
    items.push_back(item);
}

template <class T>
T* Repository<T>::get(int index) {
    if (index < 0 || index >= items.size())
        return nullptr;

    return &items[index];
}

template <class T>
int Repository<T>::size() {
    return items.size();
}
void ResourceGraph::addConflict(string a, string b) {
    adj[a].push_back(b);
    adj[b].push_back(a);
}

string ResourceGraph::displayGraph() {
    stringstream ss;

    for (auto &p : adj) {
        ss << p.first << " -> ";

        for (auto &x : p.second) {
            ss << x << " ";
        }

        ss << "\n";
    }

    return ss.str();
}

// Disjoint Set
void DisjointSet::add(string x) {
    parent[x] = x;
    rankValue[x] = 0;
}

string DisjointSet::find(string x) {
    if (parent[x] != x)
        parent[x] = find(parent[x]);

    return parent[x];
}

void DisjointSet::unite(string a, string b) {
    string rootA = find(a);
    string rootB = find(b);

    if (rootA == rootB)
        return;

    if (rankValue[rootA] < rankValue[rootB]) {
        parent[rootA] = rootB;
    }
    else if (rankValue[rootA] > rankValue[rootB]) {
        parent[rootB] = rootA;
    }
    else {
        parent[rootB] = rootA;
        rankValue[rootA]++;
    }
}

bool DisjointSet::isSameSet(string a, string b) {
    return find(a) == find(b);
}
template class Repository<Resource>;

