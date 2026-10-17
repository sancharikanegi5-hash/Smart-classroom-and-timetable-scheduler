#include "student-section.h"

#include <limits>

Student::Student() : rollNo(0), name(""), section("") {}

Student::Student(int roll, const string& studentName, const string& sectionCode)
    : rollNo(roll), name(studentName), section(sectionCode) {}

int Student::getRollNo() const {
    return rollNo;
}

string Student::getName() const {
    return name;
}

string Student::getSection() const {
    return section;
}

void Student::display() const {
    cout << "Roll No: " << rollNo
         << ", Name: " << name
         << ", Section: " << section << '\n';
}

Section::Section() : sectionCode(""), studentCount(0), subjects() {}

Section::Section(const string& code)
    : sectionCode(code), studentCount(0), subjects() {}

string Section::getSectionCode() const {
    return sectionCode;
}

bool Section::addStudent(const Student& student) {
    if (studentCount >= MAX_STUDENTS) {
        cout << "Section is full. Cannot add more students.\n";
        return false;
    }

    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getRollNo() == student.getRollNo()) {
            cout << "A student with this roll number already exists in this section.\n";
            return false;
        }
    }

    students[studentCount++] = student;
    cout << "Student added successfully.\n";
    return true;
}

bool Section::removeStudent(int rollNo) {
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getRollNo() == rollNo) {
            for (int j = i; j < studentCount - 1; ++j) {
                students[j] = students[j + 1];
            }
            --studentCount;
            cout << "Student removed successfully.\n";
            return true;
        }
    }

    cout << "Student not found.\n";
    return false;
}

const Student* Section::searchStudent(int rollNo) const {
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].getRollNo() == rollNo) {
            return &students[i];
        }
    }
    return nullptr;
}

void Section::displayAllStudents() const {
    if (studentCount == 0) {
        cout << "No students in section " << sectionCode << ".\n";
        return;
    }

    cout << "\nStudents in section " << sectionCode << ":\n";
    for (int i = 0; i < studentCount; ++i) {
        students[i].display();
    }
}

void Section::linkSubjectsToSection() {
    int numberOfSubjects;

    cout << "How many subjects do you want to link to section "
         << sectionCode << "? ";
    if (!(cin >> numberOfSubjects) || numberOfSubjects < 0) {
        cout << "Invalid number of subjects.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    subjects.clear();

    for (int i = 0; i < numberOfSubjects; ++i) {
        string subjectName;
        cout << "Enter subject " << i + 1 << ": ";
        getline(cin, subjectName);
        subjects.push_back(subjectName);
    }

    cout << "Subjects linked successfully to " << sectionCode << ".\n";
}

void Section::displaySectionDetails() const {
    cout << "\nSection code: " << sectionCode << '\n';

    cout << "Subjects: ";
    if (subjects.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < subjects.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << subjects[i];
        }
    }
    cout << '\n';

    displayAllStudents();
}

Batch::Batch(const string& code, int groupNo)
    : Section(code), labGroupNo(groupNo) {}

int Batch::getLabGroupNo() const {
    return labGroupNo;
}

void Batch::displayBatch() const {
    cout << "\nBatch / Section code: " << getSectionCode() << '\n';
    cout << "Lab group number: " << labGroupNo << '\n';
    displaySectionDetails();
}

SectionNode::SectionNode(const Section& section)
    : data(section), left(nullptr), right(nullptr) {}

SectionBST::SectionBST() : root(nullptr) {}

SectionBST::~SectionBST() {
    deleteTree(root);
}

void SectionBST::deleteTree(SectionNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

SectionNode* SectionBST::insertNode(
    SectionNode* node, const Section& section, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new SectionNode(section);
    }

    if (section.getSectionCode() < node->data.getSectionCode()) {
        node->left = insertNode(node->left, section, inserted);
    } else if (section.getSectionCode() > node->data.getSectionCode()) {
        node->right = insertNode(node->right, section, inserted);
    } else {
        inserted = false;
    }

    return node;
}

bool SectionBST::insertSection(const Section& section) {
    bool inserted = false;
    root = insertNode(root, section, inserted);

    if (!inserted) {
        cout << "Section code already exists. Please use a unique code.\n";
    }
    return inserted;
}

SectionNode* SectionBST::searchNode(SectionNode* node, const string& code) const {
    if (node == nullptr || node->data.getSectionCode() == code) {
        return node;
    }

    if (code < node->data.getSectionCode()) {
        return searchNode(node->left, code);
    }
    return searchNode(node->right, code);
}

Section* SectionBST::searchSection(const string& code) const {
    SectionNode* node = searchNode(root, code);
    return node == nullptr ? nullptr : &node->data;
}

void SectionBST::sortedListing(const SectionNode* node) const {
    if (node == nullptr) return;

    sortedListing(node->left);
    cout << node->data.getSectionCode() << '\n';
    sortedListing(node->right);
}

void SectionBST::sortedSectionListing() const {
    if (root == nullptr) {
        cout << "No sections available.\n";
        return;
    }

    cout << "\nSections in sorted order:\n";
    sortedListing(root);
}

