#include <iostream>
#include <string>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"

void displayMenu() {
    std::cout << "\n===================================================\n";
    std::cout << "        INTERACTIVE MANAGEMENT SYSTEM MENU        \n";
    std::cout << "===================================================\n";
    std::cout << "--- Module 1: Staff Management ---\n";
    std::cout << " 1. Add Doctor\n";
    std::cout << " 2. Remove Doctor\n";
    std::cout << " 3. Add Nurse to Doctor\n";
    std::cout << " 4. Remove Nurse from Doctor\n";
    std::cout << " 5. Display Doctor Roster\n";
    std::cout << "--- Module 2: Emergency Room Triage Engine ---\n";
    std::cout << " 6. Insert Triage Patient (Max-Heap Insert)\n";
    std::cout << " 7. Extract Most Urgent Patient (Max-Heap Extract)\n";
    std::cout << " 8. Peek Highest Acuity Patient\n";
    std::cout << " 9. Display Triage Priority Queue\n";
    std::cout << "10. Exit System\n";
    std::cout << "===================================================\n";
    std::cout << "Enter choice (1-10): ";
}

int main() {
    std::cout << "===================================================\n";
    std::cout << " Kigali Central Hospital - PMTS (Phase 1 Driver)   \n";
    std::cout << "===================================================\n\n";

    // --- Module 1: Staff Management Demo ---
    std::cout << ">>> 1. STAFF MANAGEMENT MODULE DEMO <<<\n";
    Doctor demoRoster[MAX_DOCTORS];
    int demoDoctorCount = 0;

    addDoctor(demoRoster, demoDoctorCount, 101, "Alex", "Cardiology");
    addDoctor(demoRoster, demoDoctorCount, 102, "Beatrice", "Neurology");
    addDoctor(demoRoster, demoDoctorCount, 103, "Charles", "Orthopedics");

    addNurse(demoRoster[0], 501, "N1_Grace", "Day");
    addNurse(demoRoster[0], 502, "N2_David", "Night");
    addNurse(demoRoster[1], 503, "N3_Claire", "Day");

    printDoctorRoster(demoRoster, demoDoctorCount);

    std::cout << "\n[Action] Removing Nurse 501 from Dr. Alex...\n";
    removeNurse(demoRoster[0], 501);

    std::cout << "[Action] Removing Doctor 102 (Dr. Beatrice)...\n";
    removeDoctor(demoRoster, demoDoctorCount, 102);

    printDoctorRoster(demoRoster, demoDoctorCount);

    for (int i = 0; i < demoDoctorCount; ++i) {
        freeNurseList(demoRoster[i].nurseHead);
    }
    demoDoctorCount = 0;

    // --- Module 2: Emergency Room Triage Engine Demo ---
    std::cout << "\n>>> 2. EMERGENCY ROOM TRIAGE ENGINE DEMO <<<\n";
    PriorityQueue demoPQ;
    demoPQ.patientCount = 0;

    int acuities[] = {14, 22, 45, 10, 8, 30, 95};
    int numPatients = 7;

    std::cout << "Inserting patients with acuities: [14, 22, 45, 10, 8, 30, 95]...\n";
    for (int i = 0; i < numPatients; ++i) {
        Patient p{i + 1, acuities[i], "Patient_" + std::to_string(i + 1)};
        pq_insert(&demoPQ, p);
    }

    print_priority_queue(&demoPQ);

    std::cout << "\nExtracting highest acuity patient (extract_max)...\n";
    Patient urgent = extract_max(&demoPQ);
    std::cout << "Extracted Patient ID: " << urgent.patientID 
              << ", Name: " << urgent.name 
              << ", Acuity: " << urgent.acuityScore << "\n\n";

    print_priority_queue(&demoPQ);

    std::cout << "\nPhase 1 Automated Demo Completed.\n";
    std::cout << "Starting Interactive Menu System...\n";

    // --- Interactive System ---
    Doctor doctorRoster[MAX_DOCTORS];
    int doctorCount = 0;
    PriorityQueue triageQueue;
    triageQueue.patientCount = 0;

    int choice = 0;
    while (choice != 10) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                std::string name, spec;
                std::cout << "Enter Doctor ID: ";
                std::cin >> id;
                std::cout << "Enter Doctor Name: ";
                std::cin >> name;
                std::cout << "Enter Specialization: ";
                std::cin >> spec;
                if (addDoctor(doctorRoster, doctorCount, id, name, spec)) {
                    std::cout << "Doctor added successfully.\n";
                }
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter Doctor ID to remove: ";
                std::cin >> id;
                if (removeDoctor(doctorRoster, doctorCount, id)) {
                    std::cout << "Doctor removed and associated nurses freed.\n";
                }
                break;
            }
            case 3: {
                int docID, nurseID;
                std::string name, shift;
                std::cout << "Enter Supervising Doctor ID: ";
                std::cin >> docID;
                int docIdx = findDoctorIndex(doctorRoster, doctorCount, docID);
                if (docIdx == -1) {
                    std::cout << "Error: Doctor ID not found.\n";
                    break;
                }
                std::cout << "Enter Nurse ID: ";
                std::cin >> nurseID;
                std::cout << "Enter Nurse Name: ";
                std::cin >> name;
                std::cout << "Enter Shift (Day/Night): ";
                std::cin >> shift;
                if (addNurse(doctorRoster[docIdx], nurseID, name, shift)) {
                    std::cout << "Nurse assigned successfully.\n";
                }
                break;
            }
            case 4: {
                int docID, nurseID;
                std::cout << "Enter Supervising Doctor ID: ";
                std::cin >> docID;
                int docIdx = findDoctorIndex(doctorRoster, doctorCount, docID);
                if (docIdx == -1) {
                    std::cout << "Error: Doctor ID not found.\n";
                    break;
                }
                std::cout << "Enter Nurse ID to remove: ";
                std::cin >> nurseID;
                if (removeNurse(doctorRoster[docIdx], nurseID)) {
                    std::cout << "Nurse removed successfully.\n";
                } else {
                    std::cout << "Error: Nurse ID not found.\n";
                }
                break;
            }
            case 5:
                printDoctorRoster(doctorRoster, doctorCount);
                break;

            case 6: {
                int pid, acuity;
                std::string name;
                std::cout << "Enter Patient ID: ";
                std::cin >> pid;
                std::cout << "Enter Acuity Score (1-100): ";
                std::cin >> acuity;
                std::cout << "Enter Patient Name: ";
                std::cin >> name;
                Patient p{pid, acuity, name};
                if (is_full(&triageQueue)) {
                    std::cout << "Triage queue is full. Cannot insert patient.\n";
                } else {
                    pq_insert(&triageQueue, p);
                    std::cout << "Patient added to triage Max-Heap.\n";
                }
                break;
            }
            case 7: {
                if (is_empty(&triageQueue)) {
                    std::cout << "Triage queue is empty.\n";
                } else {
                    Patient p = extract_max(&triageQueue);
                    std::cout << "Extracted Patient -> ID: " << p.patientID 
                              << ", Name: " << p.name 
                              << ", Acuity: " << p.acuityScore << "\n";
                }
                break;
            }
            case 8: {
                if (is_empty(&triageQueue)) {
                    std::cout << "Triage queue is empty.\n";
                } else {
                    Patient p = peek_max(&triageQueue);
                    std::cout << "Highest Urgency Patient -> ID: " << p.patientID 
                              << ", Name: " << p.name 
                              << ", Acuity: " << p.acuityScore << "\n";
                }
                break;
            }
            case 9:
                print_priority_queue(&triageQueue);
                break;

            case 10:
                std::cout << "Cleaning dynamic memory and exiting...\n";
                for (int i = 0; i < doctorCount; ++i) {
                    freeNurseList(doctorRoster[i].nurseHead);
                }
                doctorCount = 0;
                std::cout << "Shutdown complete.\n";
                break;

            default:
                std::cout << "Invalid choice. Select an option between 1 and 10.\n";
                break;
        }
    }

    return 0;
}