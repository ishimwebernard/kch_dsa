#include "../include/module1_staff.h"
#include <iostream>

// Binary search implementation in O(log N) time complexity
int findDoctorIndex(const Doctor doctorRoster[], int doctorCount, int doctorID) {
    int left = 0;
    int right = doctorCount - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (doctorRoster[mid].doctorID == doctorID) {
            return mid;
        }
        if (doctorRoster[mid].doctorID < doctorID) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

// Insert Doctor into array maintaining sorted order by doctorID (enables binary search)
bool addDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID, const std::string &name, const std::string &specialization) {
    if (doctorCount >= MAX_DOCTORS) {
        std::cerr << "Doctor roster already full (MAX_DOCTORS = " << MAX_DOCTORS << ").\n";
        return false;
    }

    // Find insertion index to maintain sorted array property
    int pos = 0;
    while (pos < doctorCount && doctorRoster[pos].doctorID < doctorID) {
        ++pos;
    }

    // Check for duplicate ID
    if (pos < doctorCount && doctorRoster[pos].doctorID == doctorID) {
        std::cerr << "Doctor ID " << doctorID << " already exists.\n";
        return false;
    }

    // Shift elements right to create space for sorted insertion
    for (int i = doctorCount; i > pos; --i) {
        doctorRoster[i] = doctorRoster[i - 1];
    }

    Doctor &d = doctorRoster[pos];
    d.doctorID = doctorID;
    d.name = name;
    d.specialization = specialization;
    d.nurseHead = nullptr;
    d.nurseCount = 0;
    ++doctorCount;

    return true;
}

// Free all nurse linked list nodes and reset head pointer
void freeNurseList(Nurse* &head) {
    Nurse* current = head;
    while (current != nullptr) {
        Nurse* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// Delete doctor by ID using findDoctorIndex and shifting elements
bool removeDoctor(Doctor doctorRoster[], int &doctorCount, int doctorID) {
    int index = findDoctorIndex(doctorRoster, doctorCount, doctorID);

    if (index == -1) {
        std::cerr << "Doctor ID " << doctorID << " not found.\n";
        return false;
    }

    // Free associated linked list memory
    freeNurseList(doctorRoster[index].nurseHead);
    doctorRoster[index].nurseCount = 0;

    // Shift remaining elements left
    for (int i = index; i < doctorCount - 1; ++i) {
        doctorRoster[i] = doctorRoster[i + 1];
    }

    --doctorCount;
    doctorRoster[doctorCount].nurseHead = nullptr;
    doctorRoster[doctorCount].nurseCount = 0;

    return true;
}

// Nurse tail insertion for a specific doctor
bool addNurse(Doctor &doc, int nurseID, const std::string &name, const std::string &shift) {
    Nurse* node = new Nurse;
    node->nurseID = nurseID;
    node->name = name;
    node->shift = shift;
    node->next = nullptr;

    if (doc.nurseHead == nullptr) {
        doc.nurseHead = node;
    } else {
        Nurse* tail = doc.nurseHead;
        while (tail->next != nullptr) {
            tail = tail->next;
        }
        tail->next = node;
    }

    ++doc.nurseCount;
    return true;
}

// Remove nurse by ID from a doctor's linked list
bool removeNurse(Doctor &doc, int nurseID) {
    if (doc.nurseHead == nullptr) {
        std::cerr << "Nurse list is empty.\n";
        return false;
    }

    Nurse* previous = nullptr;
    Nurse* current = doc.nurseHead;

    while (current != nullptr && current->nurseID != nurseID) {
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        std::cerr << "Nurse ID " << nurseID << " not found.\n";
        return false;
    }

    if (previous == nullptr) {
        doc.nurseHead = current->next;
    } else {
        previous->next = current->next;
    }

    delete current;
    --doc.nurseCount;

    return true;
}

// Print all doctors currently in the roster
void printDoctorRoster(const Doctor doctorRoster[], int doctorCount) {
    if (doctorCount == 0) {
        std::cout << "Doctor roster is empty.\n";
        return;
    }
    for (int i = 0; i < doctorCount; ++i) {
        std::cout << "ID: " << doctorRoster[i].doctorID 
                  << " | Name: " << doctorRoster[i].name 
                  << " | Specialization: " << doctorRoster[i].specialization 
                  << " | Nurse Count: " << doctorRoster[i].nurseCount << "\n";
    }
}

// Print assigned nurses for a specific doctor
void printNurseList(const Doctor &doc) {
    std::cout << "Nurses for Dr. " << doc.name << " (ID: " << doc.doctorID << "):\n";
    Nurse* current = doc.nurseHead;
    if (current == nullptr) {
        std::cout << "  No nurses assigned.\n";
        return;
    }
    while (current != nullptr) {
        std::cout << "  Nurse ID: " << current->nurseID 
                  << " | Name: " << current->name 
                  << " | Shift: " << current->shift << "\n";
        current = current->next;
    }
}