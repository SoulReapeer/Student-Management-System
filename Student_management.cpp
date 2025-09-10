#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdio>
using namespace std;

class Student {
private:
    int IDNo;
    string name;
    float cgpa;
    string email, phone, course;
    int semester;

public:
    Student() : IDNo(0), cgpa(0), semester(0) {}

    Student(int r, string n, float c, string e, string p, string crs, int sem)
        : IDNo(r), name(n), cgpa(c), email(e), phone(p), course(crs), semester(sem) {}

    int getIDNo() const { return IDNo; }
    string getName() const { return name; }
    float getCGPA() const { return cgpa; }

    void input() {
        cin.ignore();
        cout << "Name: "; getline(cin, name);
        cout << "CGPA: "; cin >> cgpa; cin.ignore();
        cout << "Email: "; getline(cin, email);
        cout << "Phone: "; getline(cin, phone);
        cout << "Course: "; getline(cin, course);
        cout << "Semester: "; cin >> semester;
    }

    void display(bool detailed = false) const {
        if (detailed) {
            cout << "\nID No: " << IDNo
                 << "\nName: " << name
                 << "\nCGPA: " << cgpa
                 << "\nEmail: " << email
                 << "\nPhone: " << phone
                 << "\nCourse: " << course
                 << "\nSemester: " << semester << "\n";
        } else {
            cout << left << setw(10) << IDNo
                 << setw(20) << name
                 << setw(10) << cgpa << endl;
        }
    }

    friend ofstream& operator<<(ofstream& ofs, const Student& s) {
        ofs << s.IDNo << "\n" << s.name << "\n" << s.cgpa << "\n"
            << s.email << "\n" << s.phone << "\n" << s.course << "\n" << s.semester << "\n";
        return ofs;
    }

    friend ifstream& operator>>(ifstream& ifs, Student& s) {
        ifs >> s.IDNo; if (ifs.fail()) return ifs; ifs.ignore();
        getline(ifs, s.name); ifs >> s.cgpa; ifs.ignore();
        getline(ifs, s.email); getline(ifs, s.phone); getline(ifs, s.course);
        ifs >> s.semester; ifs.ignore();
        return ifs;
    }

    void setData(const Student& s) {
        IDNo = s.IDNo; name = s.name; cgpa = s.cgpa;
        email = s.email; phone = s.phone; course = s.course; semester = s.semester;
    }
};

class StudentManager {
private:
    string filename;

    bool exists(int IDNo) {
        ifstream inFile(filename);
        Student s;
        while (inFile >> s) if (s.getIDNo() == IDNo) return true;
        return false;
    }

public:
    StudentManager(string fname) : filename(fname) {}

    void addStudent() {
        int IDNo;
        cout << "\nID No: "; cin >> IDNo;
        if (exists(IDNo)) { cout << "Student exists!\n"; return; }

        Student s(IDNo, "", 0, "", "", "", 0);
        s.input();

        ofstream outFile(filename, ios::app);
        outFile << s;
        cout << "Student added successfully!\n";
    }

    void displayAll() {
        ifstream inFile(filename);
        Student s;
        cout << "\n" << left << setw(10) << "IDNo" << setw(20) << "Name" << setw(10) << "CGPA\n";
        cout << string(40, '-') << endl;
        while (inFile >> s) s.display();
    }

    void searchByRoll() {
        int IDNo; cout << "\nEnter Roll No: "; cin >> IDNo;
        ifstream inFile(filename);
        Student s; bool found = false;
        while (inFile >> s) if (s.getIDNo() == IDNo) { s.display(true); found = true; break; }
        if (!found) cout << "Not found!\n";
    }

    void searchByName() {
        cin.ignore();
        string name; cout << "\nEnter Name: "; getline(cin, name);
        ifstream inFile(filename);
        Student s; bool found = false;
        while (inFile >> s) if (s.getName() == name) { s.display(true); found = true; }
        if (!found) cout << "Not found!\n";
    }

    void updateStudent() {
        int IDNo; cout << "\nEnter Roll No to update: "; cin >> IDNo;
        ifstream inFile(filename); ofstream temp("temp.txt"); Student s; bool found = false;

        while (inFile >> s) {
            if (s.getIDNo() == IDNo) { found = true; s.input(); }
            temp << s;
        }

        inFile.close(); temp.close();
        remove(filename.c_str()); rename("temp.txt", filename.c_str());
        cout << (found ? "Updated successfully!\n" : "Student not found!\n");
    }

    void deleteStudent() {
        int IDNo; cout << "\nEnter Roll No to delete: "; cin >> IDNo;
        ifstream inFile(filename); ofstream temp("temp.txt"); Student s; bool found = false;

        while (inFile >> s) {
            if (s.getIDNo() == IDNo) { found = true; continue; }
            temp << s;
        }

        inFile.close(); temp.close();
        remove(filename.c_str()); rename("temp.txt", filename.c_str());
        cout << (found ? "Deleted successfully!\n" : "Student not found!\n");
    }

    void showStatistics() {
        ifstream inFile(filename); Student s;
        int count = 0; float total = 0, high = -1e9, low = 1e9;
        while (inFile >> s) {
            float c = s.getCGPA();
            total += c; count++;
            if (c > high) high = c;
            if (c < low) low = c;
        }
        if (count == 0) { cout << "No records!\n"; return; }
        cout << "\nTotal Students: " << count
             << "\nAverage CGPA: " << total / count
             << "\nHighest CGPA: " << high
             << "\nLowest CGPA: " << low << endl;
    }
};

void displayMenu() {
    cout << "\n1. Add Student\n2. Display All\n3. Search by Roll\n4. Search by Name\n"
         << "5. Update Student\n6. Delete Student\n7. Statistics\n8. Exit\nChoice: ";
}

int main() {
    StudentManager sm("students.txt"); int choice;
    do {
        displayMenu(); cin >> choice;
        switch(choice){
            case 1: sm.addStudent(); break;
            case 2: sm.displayAll(); break;
            case 3: sm.searchByRoll(); break;
            case 4: sm.searchByName(); break;
            case 5: sm.updateStudent(); break;
            case 6: sm.deleteStudent(); break;
            case 7: sm.showStatistics(); break;
            case 8: cout << "System Ended!\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while(choice != 8);
}
