#include <iostream>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"

int main() {
    std::cout << "===================================================\n";
    std::cout << " Kigali Central Hospital - PMTS (Phase 1 Driver)  \n";
    std::cout << "===================================================\n\n";

    // --- Module 1: Staff Management Demo ---
    std::cout << ">>> 1. STAFF MANAGEMENT MODULE DEMO <<<\n";
    Doctor doctorRoster[MAX_DOCTORS];
    int doctorCount = 0;

    addDoctor(doctorRoster, doctorCount, 101, "Alex", "Cardiology");
    addDoctor(doctorRoster, doctorCount, 102, "Beatrice", "Neurology");
    addDoctor(doctorRoster, doctorCount, 103, "Charles", "Orthopedics");

    addNurse(doctorRoster[0], 501, "N1_Grace", "Day");
    addNurse(doctorRoster[0], 502, "N2_David", "Night");
    addNurse(doctorRoster[1], 503, "N3_Claire", "Day");

    printDoctorRoster(doctorRoster, doctorCount);

    std::cout << "\n[Action] Removing Nurse 501 from Dr. Alex...\n";
    removeNurse(doctorRoster[0], 501);

    std::cout << "[Action] Removing Doctor 102 (Dr. Beatrice)...\n";
    removeDoctor(doctorRoster, doctorCount, 102);

    printDoctorRoster(doctorRoster, doctorCount);

    for (int i = 0; i < doctorCount; ++i) {
        freeNurseList(doctorRoster[i].nurseHead);
    }
    doctorCount = 0;

    // --- Module 2: Emergency Room Triage Engine Demo ---
    std::cout << "\n>>> 2. EMERGENCY ROOM TRIAGE ENGINE DEMO <<<\n";
    PriorityQueue pq;

    int acuities[] = {14, 22, 45, 10, 8, 30, 95};
    int numPatients = 7;

    std::cout << "Inserting patients with acuities: [14, 22, 45, 10, 8, 30, 95]...\n";
    for (int i = 0; i < numPatients; ++i) {
        Patient p{i + 1, acuities[i], "Patient_" + std::to_string(i + 1)};
        pq_insert(&pq, p);
    }

    print_priority_queue(&pq);

    std::cout << "\nExtracting highest acuity patient (extract_max)...\n";
    Patient urgent = extract_max(&pq);
    std::cout << "Extracted Patient ID: " << urgent.patientID 
              << ", Name: " << urgent.name 
              << ", Acuity: " << urgent.acuityScore << "\n\n";

    print_priority_queue(&pq);

    std::cout << "\nPhase 1 CLI execution finished.\n";
    return 0;
}