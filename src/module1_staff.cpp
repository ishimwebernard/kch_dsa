#include "../include/module1_staff.h"

// Boilerplate declarations & stubs

int findDoctorIndex(const Doctor doctorRoster[], int doctorCount, int doctorID) {
    (void)doctorRoster; (void)doctorCount; (void)doctorID;
    return -1;
}

bool addDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID, const std::string &name, const std::string &specialization) {
    (void)doctorRoster; (void)doctorCount; (void)doctorID; (void)name; (void)specialization;
    return false;
}

void freeNurseList(Nurse* &head) {
    (void)head;
}

bool removeDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID) {
    (void)doctorRoster; (void)doctorCount; (void)doctorID;
    return false;
}

bool addNurse(Doctor &doc, int nurseID, const std::string &name, const std::string &shift) {
    (void)doc; (void)nurseID; (void)name; (void)shift;
    return false;
}

bool removeNurse(Doctor &doc, int nurseID) {
    (void)doc; (void)nurseID;
    return false;
}

void printDoctorRoster(const Doctor doctorRoster[], int doctorCount) {
    (void)doctorRoster; (void)doctorCount;
}

void printNurseList(const Doctor &doc) {
    (void)doc;
}