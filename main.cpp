// ============================================================
//  Student Management System
//  Language  : C++ (C++11 Standard)
//  Concepts  : Abstract Base Class, Pure Virtual Functions,
//              Virtual Function Dispatch (Polymorphism),
//              'this' Pointer, File I/O with <fstream>
//  Storage   : students.txt (pipe-delimited flat file)
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

// ============================================================
// ABSTRACT BASE CLASS
// ============================================================
class Person {
public:
    // Pure Virtual Function — makes Person an Abstract Class.
    // Every concrete derived class MUST override this function.
    // A pointer of type Person* can call this method, and C++
    // will dispatch to the correct override at runtime (vtable).
    virtual void displayDetails() const = 0;

    // Virtual destructor: ensures the derived class destructor
    // is called when deleting via a base class pointer.
    virtual ~Person() {}
};

// ============================================================
// DERIVED CLASS — Student
// ============================================================
class Student : public Person {
private:
    // Data members are PRIVATE — enforcing Encapsulation.
    // They cannot be directly accessed from outside this class.
    int    rollNo;
    string name;
    string course;
    float  marks;

public:
    // --------------------------------------------------------
    // CONSTRUCTOR — uses 'this' pointer to resolve the
    // variable shadowing caused by identical parameter names.
    // 'this->rollNo' refers to the member variable.
    // 'rollNo' (RHS) refers to the constructor parameter.
    // --------------------------------------------------------
    Student(int rollNo, string name, string course, float marks) {
        this->rollNo  = rollNo;
        this->name    = name;
        this->course  = course;
        this->marks   = marks;
    }

    // --------------------------------------------------------
    // GETTERS — public read-only accessors for private members.
    // --------------------------------------------------------
    int    getRollNo() const { return this->rollNo;  }
    string getName()   const { return this->name;    }
    string getCourse() const { return this->course;  }
    float  getMarks()  const { return this->marks;   }

    // --------------------------------------------------------
    // OVERRIDE of Pure Virtual Function.
    // The 'override' keyword is C++11 best practice — it tells
    // the compiler to verify that we are actually overriding a
    // virtual function in the base class.
    // --------------------------------------------------------
    void displayDetails() const override {
        cout << left;
        cout << "  +-----------------------------------------+" << endl;
        cout << "  | Roll No : " << setw(30) << this->rollNo   << "|" << endl;
        cout << "  | Name    : " << setw(30) << this->name     << "|" << endl;
        cout << "  | Course  : " << setw(30) << this->course   << "|" << endl;
        cout << "  | Marks   : " << setw(30) << fixed << setprecision(2)
           << this->marks << "|" << endl;
        cout << "  +-----------------------------------------+" << endl;
    }
};

// ============================================================
// GLOBAL DATA STORE
// A vector of base class pointers — this is what enables
// polymorphism. The vector does not know the concrete type;
// it only sees Person*. Virtual dispatch handles the rest.
// ============================================================
vector<Person*> students;
const string FILENAME = "students.txt";

// ============================================================
// FILE I/O — Save all records to students.txt
// Uses pipe '|' as delimiter to handle names with spaces.
// ============================================================
void saveToFile() {
    ofstream outFile(FILENAME);
    if (!outFile) {
        cerr << "[ERROR] Could not open file for writing." << endl;
        return;
    }
    for (const auto& p : students) {
        // Dynamic cast to access Student-specific getters.
        // Safe because we know all Person* in our vector are Student*.
        Student* s = dynamic_cast<Student*>(p);
        if (s) {
            outFile << s->getRollNo() << "|"
                    << s->getName()   << "|"
                    << s->getCourse() << "|"
                    << s->getMarks()  << endl;
        }
    }
    outFile.close();
    cout << "  [OK] Records saved to " << FILENAME << endl;
}

// ============================================================
// FILE I/O — Load all records from students.txt on startup
// ============================================================
void loadFromFile() {
    ifstream inFile(FILENAME);
    if (!inFile) {
        cout << "  [INFO] No existing data file found. Starting fresh." << endl;
        return;
    }
    string line;
    int loadedCount = 0;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        // Parse the pipe-delimited record
        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        if (p1 == string::npos || p2 == string::npos || p3 == string::npos)
            continue; // Skip malformed lines
        int    rollNo = stoi(line.substr(0, p1));
        string name   = line.substr(p1 + 1, p2 - p1 - 1);
        string course = line.substr(p2 + 1, p3 - p2 - 1);
        float  marks  = stof(line.substr(p3 + 1));
        students.push_back(new Student(rollNo, name, course, marks));
        loadedCount++;
    }
    inFile.close();
    cout << "  [INFO] " << loadedCount << " record(s) loaded from " << FILENAME << "." << endl;
}

// ============================================================
// DISPLAY MAIN MENU
// ============================================================
void displayMenu() {
    cout << endl;
    cout << "  =========================================" << endl;
    cout << "    STUDENT MANAGEMENT SYSTEM  v1.0        " << endl;
    cout << "  =========================================" << endl;
    cout << "  [1]  Add New Student                     " << endl;
    cout << "  [2]  Display All Students                " << endl;
    cout << "  [3]  Search Student by Roll No.          " << endl;
    cout << "  [4]  Delete Student by Roll No.          " << endl;
    cout << "  [5]  Exit                                " << endl;
    cout << "  =========================================" << endl;
    cout << "  Enter your choice: ";
}

// ============================================================
// OPERATION: Add New Student
// ============================================================
void addStudent() {
    int    rollNo;
    string name, course;
    float  marks;
    cout << endl << "  --- ADD NEW STUDENT ---" << endl;
    cout << "  Enter Roll No  : "; cin >> rollNo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  Enter Name     : "; getline(cin, name);
    cout << "  Enter Course   : "; getline(cin, course);
    cout << "  Enter Marks    : "; cin >> marks;
    students.push_back(new Student(rollNo, name, course, marks));
    saveToFile();
    cout << "  [SUCCESS] Student added successfully!" << endl;
}

// ============================================================
// OPERATION: Display All Students
// Uses polymorphism — calls virtual displayDetails() on
// each Person* in the vector.
// ============================================================
void displayAllStudents() {
    cout << endl << "  --- ALL STUDENT RECORDS ---" << endl;
    if (students.empty()) {
        cout << "  [INFO] No records found in the database." << endl;
        return;
    }
    cout << "  Total Records: " << students.size() << endl << endl;
    for (const auto& p : students) {
        p->displayDetails(); // Virtual dispatch — polymorphism in action
        cout << endl;
    }
}

// ============================================================
// OPERATION: Search Student by Roll Number
// ============================================================
void searchStudent() {
    int searchRoll;
    cout << endl << "  --- SEARCH STUDENT ---" << endl;
    cout << "  Enter Roll No to search: "; cin >> searchRoll;
    bool found = false;
    for (const auto& p : students) {
        Student* s = dynamic_cast<Student*>(p);
        if (s && s->getRollNo() == searchRoll) {
            cout << endl << "  [FOUND] Record Details:" << endl;
            s->displayDetails();
            found = true;
            break;
        }
    }
    if (!found)
        cout << "  [NOT FOUND] No student with Roll No " << searchRoll << " exists." << endl;
}

// ============================================================
// OPERATION: Delete Student by Roll Number
// ============================================================
void deleteStudent() {
    int delRoll;
    cout << endl << "  --- DELETE STUDENT ---" << endl;
    cout << "  Enter Roll No to delete: "; cin >> delRoll;
    bool found = false;
    for (auto it = students.begin(); it != students.end(); ++it) {
        Student* s = dynamic_cast<Student*>(*it);
        if (s && s->getRollNo() == delRoll) {
            delete *it;        // Free heap memory
            students.erase(it); // Remove pointer from vector
            saveToFile();
            cout << "  [SUCCESS] Student with Roll No " << delRoll << " has been deleted." << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "  [NOT FOUND] No student with Roll No " << delRoll << " exists." << endl;
}

// ============================================================
// MAIN FUNCTION — Program Entry Point
// ============================================================
int main() {
    cout << "  =============================================" << endl;
    cout << "   Welcome to the Student Management System   " << endl;
    cout << "   Developed using OOP in C++                 " << endl;
    cout << "  =============================================" << endl;

    loadFromFile(); // Load persisted data on startup

    int choice;
    do {
        displayMenu();
        // Input validation loop
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  [ERROR] Invalid input. Please enter a number (1-5): ";
        }
        switch (choice) {
            case 1: addStudent();        break;
            case 2: displayAllStudents(); break;
            case 3: searchStudent();     break;
            case 4: deleteStudent();     break;
            case 5:
                cout << endl << "  Releasing memory and exiting..." << endl;
                for (auto& p : students) delete p; // Prevent memory leaks
                students.clear();
                cout << "  Thank you for using SMS. Goodbye!" << endl;
                break;
            default:
                cout << "  [ERROR] Invalid option. Please select 1-5." << endl;
        }
    } while (choice != 5);

    return 0;
}