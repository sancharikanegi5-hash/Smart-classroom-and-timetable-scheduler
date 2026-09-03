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
