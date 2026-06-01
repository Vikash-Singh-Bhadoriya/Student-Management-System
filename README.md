# Student Management System
### B.Tech IT — 2nd Semester | Fast Learner Activity Project

A console-based **Student Management System** written in **C++**, demonstrating core Object-Oriented Programming principles including Abstract Classes, Pure Virtual Functions, Runtime Polymorphism, the `this` pointer, and File I/O.

---

## Features

- Add new student records
- Display all students (with polymorphic dispatch)
- Search student by Roll Number
- Delete student by Roll Number
- Persistent storage — data saved to `students.txt` automatically

---

## OOP Concepts Demonstrated

| Concept | Where Used |
|---|---|
| Abstract Base Class | `Person` class |
| Pure Virtual Function | `Person::displayDetails() = 0` |
| Inheritance | `Student` inherits from `Person` |
| Runtime Polymorphism | `vector<Person*>` + virtual dispatch |
| `this` Pointer | `Student` constructor |
| Encapsulation | Private members + public getters |
| File I/O (`<fstream>`) | `saveToFile()` / `loadFromFile()` |

---

## How to Compile & Run

**Requirements:** GCC (g++) with C++11 support

```bash
# Clone the repository
git clone https://github.com/Vikash-Singh-Bhadoriya/Student-Management-System.git
cd student-management-system

# Compile
g++ -std=c++11 -o sms main.cpp

# Run (Linux/macOS)
./sms

# Run (Windows)
sms.exe
```

---

## Sample Data (students.txt format)

The program auto-generates `students.txt` in pipe-delimited format:

```
101|Rahul Sharma|B.Tech IT|87.50
102|Priya Patel|B.Tech CS|91.00
104|Sneha Joshi|MCA|94.50
```

---

## Project Structure

```
student-management-system/
├── main.cpp          # Complete source code (single file)
├── students.txt      # Auto-generated data file (created on first run)
└── README.md
```

---

## Academic Details

- **Institution:** Amity University
- **Department:** Information Technology
- **Semester:** 2nd Semester, B.Tech
- **Subject:** Object-Oriented Programming using C++
- **Guide:** Dr. Ganesh Gupta

---

*Submitted as part of the Fast Learner Activity assignment, Academic Year 2025–26.*