#include "student-section.h"

#include <fstream>
#include <iomanip>
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



Section::Section() : sectionCode(""), students(), subjects() {}

Section::Section(const string& code)
    : sectionCode(code), students(), subjects() {}

string Section::getSectionCode() const {
    return sectionCode;
}

const vector<Student>& Section::getStudents() const {
    return students;
}

const vector<string>& Section::getSubjects() const {
    return subjects;
}

void Section::setSubjects(const vector<string>& subjectList) {
    subjects = subjectList;
}


int Section::findStudentIndex(int rollNo) const {
    int low = 0;
    int high = static_cast<int>(students.size()) - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int midRoll = students[mid].getRollNo();

        if (midRoll == rollNo) {
            return mid;
        } else if (midRoll < rollNo) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

bool Section::addStudent(const Student& student) {
    
    if (findStudentIndex(student.getRollNo()) != -1) {
        cout << "A student with this roll number already exists in this section.\n";
        return false;
    }

    
    vector<Student>::iterator position = students.begin();
    while (position != students.end() &&
           position->getRollNo() < student.getRollNo()) {
        ++position;
    }
    students.insert(position, student);

    cout << "Student added successfully.\n";
    return true;
}

bool Section::removeStudent(int rollNo) {
    int index = findStudentIndex(rollNo);
    if (index == -1) {
        cout << "Student not found.\n";
        return false;
    }

    students.erase(students.begin() + index);
    cout << "Student removed successfully.\n";
    return true;
}

const Student* Section::searchStudent(int rollNo) const {
    int index = findStudentIndex(rollNo);
    if (index == -1) {
        return nullptr;
    }
    return &students[index];
}

void Section::displayAllStudents() const {
    if (students.empty()) {
        cout << "No students in section " << sectionCode << ".\n";
        return;
    }

    cout << "\nStudents in section " << sectionCode << ":\n";
    for (size_t i = 0; i < students.size(); ++i) {
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

void SectionBST::collectSections(const SectionNode* node, vector<Section>& result) const {
    if (node == nullptr) return;
    collectSections(node->left, result);
    result.push_back(node->data);
    collectSections(node->right, result);
}

vector<Section> SectionBST::getAllSections() const {
    vector<Section> result;
    collectSections(root, result);
    return result;
}



bool SectionBST::saveStudents(const string& filename) const {
    ofstream fout(filename.c_str());
    if (!fout) {
        cerr << "Error: Could not open " << filename << " for writing.\n";
        return false;
    }

    vector<Section> allSections = getAllSections();
    fout << allSections.size() << '\n';

    for (size_t i = 0; i < allSections.size(); ++i) {
        const Section& section = allSections[i];
        const vector<string>& subjectList = section.getSubjects();
        const vector<Student>& studentList = section.getStudents();

        
        fout << quoted(section.getSectionCode()) << ' '
             << subjectList.size() << ' ' << studentList.size() << '\n';

        for (size_t j = 0; j < subjectList.size(); ++j) {
            fout << quoted(subjectList[j]) << '\n';
        }

        for (size_t j = 0; j < studentList.size(); ++j) {
            fout << studentList[j].getRollNo() << ' '
                 << quoted(studentList[j].getName()) << ' '
                 << quoted(studentList[j].getSection()) << '\n';
        }
    }

    cout << allSections.size() << " section record(s) saved to " << filename << ".\n";
    return true;
}

bool SectionBST::loadStudents(const string& filename) {
    ifstream fin(filename.c_str());
    if (!fin) {
        return false; 
    }

    size_t numberOfSections;
    if (!(fin >> numberOfSections)) {
        cerr << "Error: The saved file has an invalid format.\n";
        return false;
    }

    SectionBST loadedTree;

    for (size_t i = 0; i < numberOfSections; ++i) {
        string code;
        size_t subjectCount, studentCount;

        if (!(fin >> quoted(code) >> subjectCount >> studentCount)) {
            cerr << "Error: Incomplete section data in file.\n";
            return false;
        }

        Section section(code);
        vector<string> subjectList;

        for (size_t j = 0; j < subjectCount; ++j) {
            string subject;
            if (!(fin >> quoted(subject))) {
                cerr << "Error: Incomplete subject data in file.\n";
                return false;
            }
            subjectList.push_back(subject);
        }
        section.setSubjects(subjectList);

        for (size_t j = 0; j < studentCount; ++j) {
            int roll;
            string name, studentSection;
            if (!(fin >> roll >> quoted(name) >> quoted(studentSection))) {
                cerr << "Error: Incomplete student data in file.\n";
                return false;
            }
            
            section.addStudent(Student(roll, name, studentSection));
        }

        if (!loadedTree.insertSection(section)) {
            cerr << "Error: Duplicate section code found in saved file.\n";
            return false;
        }
    }

    
    deleteTree(root);
    root = loadedTree.root;
    loadedTree.root = nullptr;

    cout << "Student and section records loaded successfully from " << filename << ".\n";
    return true;
}



void SectionOverlapGraph::buildGraph(const SectionBST& sectionTree) {
    vector<Section> allSections = sectionTree.getAllSections();
    sectionCodes.clear();
    adjacency.clear();

    for (size_t i = 0; i < allSections.size(); ++i) {
        sectionCodes.push_back(allSections[i].getSectionCode());
        adjacency.push_back(vector<int>());
    }

    
    for (size_t i = 0; i < allSections.size(); ++i) {
        for (size_t j = i + 1; j < allSections.size(); ++j) {
            bool sharesSubject = false;
            const vector<string>& subjectsA = allSections[i].getSubjects();
            const vector<string>& subjectsB = allSections[j].getSubjects();

            for (size_t a = 0; a < subjectsA.size() && !sharesSubject; ++a) {
                for (size_t b = 0; b < subjectsB.size(); ++b) {
                    if (subjectsA[a] == subjectsB[b]) {
                        sharesSubject = true;
                        break;
                    }
                }
            }

            if (sharesSubject) {
                adjacency[i].push_back(static_cast<int>(j));
                adjacency[j].push_back(static_cast<int>(i));
            }
        }
    }
}

void SectionOverlapGraph::displayGraph() const {
    if (sectionCodes.empty()) {
        cout << "No sections available to build the overlap graph.\n";
        return;
    }

    cout << "\n===== SECTION OVERLAP GRAPH =====\n";
    cout << "An edge means the two sections share at least one subject/elective/lab.\n";

    bool hasEdge = false;
    for (size_t i = 0; i < sectionCodes.size(); ++i) {
        cout << sectionCodes[i] << " -> ";
        if (adjacency[i].empty()) {
            cout << "No shared subject";
        } else {
            for (size_t j = 0; j < adjacency[i].size(); ++j) {
                if (j > 0) cout << ", ";
                cout << sectionCodes[adjacency[i][j]];
                if (static_cast<size_t>(adjacency[i][j]) > i) hasEdge = true;
            }
        }
        cout << '\n';
    }

    if (!hasEdge) {
        cout << "No section overlaps found.\n";
    }
}



int main() {
    SectionBST sectionTree;
    const string dataFile = "students.txt";

    
    bool dataLoaded = sectionTree.loadStudents(dataFile);

    if (!dataLoaded) {
        int numberOfSections;
        cout << "How many sections do you want to create? ";
        if (!(cin >> numberOfSections) || numberOfSections < 0) {
            cout << "Invalid number of sections.\n";
            return 1;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < numberOfSections; ++i) {
            string code;
            cout << "Enter section code " << i + 1 << " (e.g. CSE-A): ";
            getline(cin, code);

            if (code.empty()) {
                cout << "Section code cannot be empty. Please try again.\n";
                --i;
                continue;
            }

            Section section(code);
            if (!sectionTree.insertSection(section)) {
                --i;
            }
        }
    }

    int choice;
    do {
        cout << "\n===== SMART CLASSROOM: STUDENTS & SECTIONS =====\n";
        cout << "1. Add student to section\n";
        cout << "2. Search student in section\n";
        cout << "3. Remove student from section\n";
        cout << "4. Link subjects to section\n";
        cout << "5. Display section details\n";
        cout << "6. Display all sections in sorted order\n";
        cout << "7. Create and display a Batch\n";
        cout << "8. Save student/section records to file\n";
        cout << "9. Load student/section records from file\n";
        cout << "10. Build and display section-overlap graph\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice >= 1 && choice <= 5) {
            string code;
            cout << "Enter section code: ";
            getline(cin, code);

            Section* section = sectionTree.searchSection(code);
            if (section == nullptr) {
                cout << "Section not found.\n";
                continue;
            }

            if (choice == 1) {
                int roll;
                string name;

                cout << "Enter student roll number: ";
                if (!(cin >> roll)) {
                    cout << "Invalid roll number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "Enter student name: ";
                getline(cin, name);

                if (roll <= 0 || name.empty()) {
                    cout << "Roll number must be positive and name cannot be empty.\n";
                    continue;
                }

                section->addStudent(Student(roll, name, code));
            } else if (choice == 2) {
                int roll;
                cout << "Enter roll number to search: ";
                if (!(cin >> roll)) {
                    cout << "Invalid roll number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                const Student* student = section->searchStudent(roll);
                if (student != nullptr) {
                    student->display();
                } else {
                    cout << "Student not found.\n";
                }
            } else if (choice == 3) {
                int roll;
                cout << "Enter roll number to remove: ";
                if (!(cin >> roll)) {
                    cout << "Invalid roll number.\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                section->removeStudent(roll);
            } else if (choice == 4) {
                section->linkSubjectsToSection();
            } else if (choice == 5) {
                section->displaySectionDetails();
            }
        } else if (choice == 6) {
            sectionTree.sortedSectionListing();
        } else if (choice == 7) {
            string batchCode;
            int groupNo;

            cout << "Enter batch/section code: ";
            getline(cin, batchCode);

            int groupNoInput;
            cout << "Enter lab group number: ";
            if (!(cin >> groupNoInput)) {
                cout << "Invalid lab group number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            groupNo = groupNoInput;
            Batch batch(batchCode, groupNo);
            batch.displayBatch();
        } else if (choice == 8) {
            sectionTree.saveStudents(dataFile);
        } else if (choice == 9) {
            if (!sectionTree.loadStudents(dataFile)) {
                cout << "No valid saved data could be loaded.\n";
            }
        } else if (choice == 10) {
            SectionOverlapGraph overlapGraph;
            overlapGraph.buildGraph(sectionTree);
            overlapGraph.displayGraph();
        } else if (choice == 0) {
            cout << "Program ended.\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}
