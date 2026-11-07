#ifndef MODULE1_STAFF_H
#define MODULE1_STAFF_H

#include <string>
#include <iostream>

constexpr int MAX_DOCTORS = 50;

struct Nurse {
    int nurseID;
    std::string name;
    std::string shift; // e.g., "Day", "Night"
    Nurse* next = nullptr;
};

struct Doctor {
    int doctorID;
    std::string name;
    std::string specialization;
    Nurse* nurseHead = nullptr;
    int nurseCount = 0;
};

// Doctor Array Operations
bool addDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID, const std::string &name, const std::string &specialization);
bool removeDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID);
int findDoctorIndex(const Doctor doctorRoster[], int doctorCount, int doctorID);

// Dynamic Nurse List Operations
bool addNurse(Doctor &doc, int nurseID, const std::string &name, const std::string &shift);
bool removeNurse(Doctor &doc, int nurseID);
void freeNurseList(Nurse* &head);

// Utility / Display
void printDoctorRoster(const Doctor doctorRoster[], int doctorCount);
void printNurseList(const Doctor &doc);

#endif