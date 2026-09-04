
#include "student-section.h"

Student::Student()
{
    rollNo = 0;
    name = "";
    section = "";
}

Student::Student(int r, string n, string s)
{
    rollNo = r;
    name = n;
    section = s;
}

int Student::getRollNo()
{
    return rollNo;
}

void Student::display()
{
    cout << "Roll No: " << rollNo
         << ", Name: " << name
         << ", Section: " << section << endl;
}

Section::Section()
{
    count = 0;
}

void Section::addStudent()
{
    if(count == MAX)
    {
        cout << "Section is full!" << endl;
        return;
    }

    int roll;
    string name, sec;

    cout << "Enter Roll No: ";
    cin >> roll;

    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Section: ";
    getline(cin, sec);

    list[count] = Student(roll, name, sec);
    count++;

    cout << "Student added successfully!" << endl;
}

void Section::removeStudent(int roll)
{
    for(int i = 0; i < count; i++)
    {
        if(list[i].getRollNo() == roll)
        {
            for(int j = i; j < count - 1; j++)
            {
                list[j] = list[j + 1];
            }

            count--;

            cout << "Student removed!" << endl;
            return;
        }
    }

    cout << "Student not found!" << endl;
}

void Section::searchStudent(int roll)
{
    for(int i = 0; i < count; i++)
    {
        if(list[i].getRollNo() == roll)
        {
            cout << "Student Found:" << endl;
            list[i].display();
            return;
        }
    }

    cout << "Student Not Found!" << endl;
}

void Section::displayAllStudents()
{
    if(count == 0)
    {
        cout << "No students available." << endl;
        return;
    }

    cout << "\n--- All Students ---\n";

    for(int i = 0; i < count; i++)
    {
        list[i].display();
    }
}

int main()
{
    Section cseA;

    int n;

    cout << "Enter number of students: ";
    cin >> n;

    for(int i = 0; i < n; i++)
    {
        cout << "\nStudent " << i + 1 << endl;
        cseA.addStudent();
    }

    cseA.displayAllStudents();

    int roll;

    cout << "\nEnter roll number to search: ";
    cin >> roll;

    cseA.searchStudent(roll);

    cout << "\nEnter roll number to remove: ";
    cin >> roll;

    cseA.removeStudent(roll);

    cseA.displayAllStudents();

    return 0;
}
