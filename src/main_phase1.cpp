#include <iostream>
#include <string>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"

static const int ID_MAX = 1000000000;

static bool g_eof = false;     // input stream ended
static bool g_cancel = false;  // user typed /c at a prompt

static std::string trim(const std::string &s) {
    const char *ws = " \t\r\n";
    std::string::size_type b = s.find_first_not_of(ws);
    if (b == std::string::npos) {
        return "";
    }
    std::string::size_type e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

// Pads or shortens text to exactly w characters (for table columns).
static std::string fit(const std::string &s, std::string::size_type w) {
    if (s.size() > w) {
        return s.substr(0, w - 1) + ".";
    }
    return s + std::string(w - s.size(), ' ');
}

// Reads a whole line. Returns false on EOF or when the user types /c.
static bool readLine(const std::string &label, std::string &out) {
    std::cout << label << " " << std::flush;
    if (!std::getline(std::cin, out)) {
        g_eof = true;
        return false;
    }
    out = trim(out);
    if (out == "/c" || out == "/C") {
        g_cancel = true;
        return false;
    }
    return true;
}

// Whole-number parser: optional sign, then digits only.
static bool parseInt(const std::string &s, long long &out) {
    std::string::size_type i = 0;
    bool neg = false;
    if (i < s.size() && (s[i] == '-' || s[i] == '+')) {
        neg = (s[i] == '-');
        ++i;
    }
    if (i == s.size()) {
        return false;
    }
    long long v = 0;
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
        v = v * 10 + (s[i] - '0');
        if (v > 100000000000LL) {
            return false;
        }
    }
    out = neg ? -v : v;
    return true;
}

// Keeps asking until the answer is a whole number in [lo, hi].
static bool readInt(const std::string &label, int &value, int lo, int hi) {
    std::string line;
    while (true) {
        if (!readLine(label, line)) {
            return false;
        }
        long long v = 0;
        if (line.empty()) {
            std::cout << "Error: please type something.\n";
        } else if (!parseInt(line, v)) {
            std::cout << "Error: '" << line << "' is not a whole number.\n";
        } else if (v < lo || v > hi) {
            std::cout << "Error: value must be between " << lo << " and " << hi << ".\n";
        } else {
            value = static_cast<int>(v);
            return true;
        }
    }
}

// Keeps asking until the answer is non-empty. Spaces are allowed.
static bool readText(const std::string &label, std::string &out) {
    while (true) {
        if (!readLine(label, out)) {
            return false;
        }
        if (!out.empty()) {
            return true;
        }
        std::cout << "Error: please type something.\n";
    }
}

// Accepts day / night in any letter case and normalises it.
static bool readShift(std::string &shift) {
    std::string line;
    while (true) {
        if (!readLine("Shift (Day/Night):", line)) {
            return false;
        }
        std::string low;
        for (std::string::size_type i = 0; i < line.size(); ++i) {
            char c = line[i];
            low += static_cast<char>((c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c);
        }
        if (low == "day" || low == "d") {
            shift = "Day";
            return true;
        }
        if (low == "night" || low == "n") {
            shift = "Night";
            return true;
        }
        std::cout << "Error: please type Day or Night.\n";
    }
}

// Yes/no question. Returns false on EOF/cancel; answer goes into `yes`.
static bool readYesNo(const std::string &label, bool &yes) {
    std::string line;
    while (true) {
        if (!readLine(label + " (y/n):", line)) {
            return false;
        }
        if (line == "y" || line == "Y" || line == "yes" || line == "YES") {
            yes = true;
            return true;
        }
        if (line == "n" || line == "N" || line == "no" || line == "NO") {
            yes = false;
            return true;
        }
        std::cout << "Error: please answer y or n.\n";
    }
}

static void pauseForMenu() {
    std::cout << "\nPress Enter to return to the main menu..." << std::flush;
    std::string dummy;
    if (!std::getline(std::cin, dummy)) {
        g_eof = true;
    }
}

static void screenTitle(const std::string &title, bool showCancelHint) {
    std::cout << "\n" << title << "\n";
    if (showCancelHint) {
        std::cout << "(type /c at any prompt to cancel)\n";
    }
    std::cout << "\n";
}

// ===================================================================
//  Display helpers
// ===================================================================
static std::string levelLabel(int acuity) {
    if (acuity >= 70) return "CRITICAL";
    if (acuity >= 40) return "MODERATE";
    return "LOW";
}

static void printPatient(const std::string &heading, const Patient &p) {
    std::cout << heading << "\n";
    std::cout << "Patient ID: " << p.patientID << "\n";
    std::cout << "Name: " << p.name << "\n";
    std::cout << "Acuity: " << p.acuityScore << " (" << levelLabel(p.acuityScore) << ")\n";
}

static void showRoster(const Doctor roster[], int count) {
    if (count == 0) {
        std::cout << "The doctor roster is empty.\n";
        return;
    }
    std::cout << fit("ID", 8) << fit("Name", 18) << fit("Specialization", 20) << "Nurses\n";
    for (int i = 0; i < count; ++i) {
        std::cout << fit(std::to_string(roster[i].doctorID), 8)
                  << fit(roster[i].name, 18) << fit(roster[i].specialization, 20)
                  << roster[i].nurseCount << "\n";
        for (const Nurse *n = roster[i].nurseHead; n != nullptr; n = n->next) {
            std::cout << "    Nurse " << fit(std::to_string(n->nurseID), 6)
                      << fit(n->name, 16) << n->shift << "\n";
        }
    }
    std::cout << count << " of " << MAX_DOCTORS << " doctor slots used\n";
}

static void showQueue(const PriorityQueue *pq) {
    if (is_empty(pq)) {
        std::cout << "The triage queue is empty.\n";
        return;
    }
    std::cout << fit("Idx", 5) << fit("ID", 8) << fit("Name", 16) << fit("Acuity", 8) << "Level\n";
    for (int i = 0; i < pq->patientCount; ++i) {
        const Patient &p = pq->patientHeap[i];
        std::cout << fit(std::to_string(i), 5) << fit(std::to_string(p.patientID), 8)
                  << fit(p.name, 16) << fit(std::to_string(p.acuityScore), 8)
                  << levelLabel(p.acuityScore) << "\n";
    }
    std::cout << pq->patientCount << " of " << MAX_PATIENTS << " patients waiting\n";
}

void displayMenu(int doctorCount, const PriorityQueue *pq) {
    std::cout << "\nKigali Central Hospital - PMTS (Phase 1)\n";
    std::cout << "Doctors: " << doctorCount << "/" << MAX_DOCTORS
              << "  Patients waiting: " << pq->patientCount << "/" << MAX_PATIENTS << "\n\n";
    std::cout << "Staff Management\n";
    std::cout << " 1. Add Doctor\n";
    std::cout << " 2. Remove Doctor\n";
    std::cout << " 3. Add Nurse to Doctor\n";
    std::cout << " 4. Remove Nurse from Doctor\n";
    std::cout << " 5. Display Doctor Roster\n\n";
    std::cout << "Emergency Room Triage\n";
    std::cout << " 6. Insert Triage Patient\n";
    std::cout << " 7. Extract Most Urgent Patient\n";
    std::cout << " 8. Peek Highest Acuity Patient\n";
    std::cout << " 9. Display Triage Priority Queue\n\n";
    std::cout << "10. Exit System\n\n";
}

// ===================================================================
//  main
// ===================================================================
int main() {
    std::cout << "Kigali Central Hospital - PMTS (Phase 1 Driver)\n\n";

    // --- Module 1: Staff Management Demo ---
    std::cout << "1. STAFF MANAGEMENT MODULE DEMO\n";
    Doctor demoRoster[MAX_DOCTORS];
    int demoDoctorCount = 0;

    addDoctor(demoRoster, demoDoctorCount, 101, "Alex", "Cardiology");
    addDoctor(demoRoster, demoDoctorCount, 102, "Beatrice", "Neurology");
    addDoctor(demoRoster, demoDoctorCount, 103, "Charles", "Orthopedics");

    addNurse(demoRoster[0], 501, "N1_Grace", "Day");
    addNurse(demoRoster[0], 502, "N2_David", "Night");
    addNurse(demoRoster[1], 503, "N3_Claire", "Day");

    printDoctorRoster(demoRoster, demoDoctorCount);

    std::cout << "\nRemoving Nurse 501 from Dr. Alex...\n";
    removeNurse(demoRoster[0], 501);

    std::cout << "Removing Doctor 102 (Dr. Beatrice)...\n";
    removeDoctor(demoRoster, demoDoctorCount, 102);

    printDoctorRoster(demoRoster, demoDoctorCount);

    for (int i = 0; i < demoDoctorCount; ++i) {
        freeNurseList(demoRoster[i].nurseHead);
    }
    demoDoctorCount = 0;

    // --- Module 2: Emergency Room Triage Engine Demo ---
    std::cout << "\n2. EMERGENCY ROOM TRIAGE ENGINE DEMO\n";
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

    std::cout << "\nPhase 1 automated demo completed.\n";
    std::cout << "Press Enter to continue to the main menu..." << std::flush;
    {
        std::string dummy;
        if (!std::getline(std::cin, dummy)) {
            g_eof = true;
        }
    }

    // --- Interactive System ---
    Doctor doctorRoster[MAX_DOCTORS];
    int doctorCount = 0;
    PriorityQueue triageQueue;
    triageQueue.patientCount = 0;

    int choice = -1;
    while (choice != 10 && !g_eof) {
        displayMenu(doctorCount, &triageQueue);

        if (!readInt("Enter choice (1-10):", choice, 1, 10)) {
            if (g_cancel) {  // /c on the main menu simply redraws it
                g_cancel = false;
                choice = -1;
            }
            continue;
        }

        switch (choice) {
            case 1: {
                screenTitle("ADD DOCTOR", true);
                if (doctorCount >= MAX_DOCTORS) {
                    std::cout << "Error: the roster is full (" << MAX_DOCTORS << " doctors). Remove a doctor first.\n";
                    break;
                }
                int id;
                std::string name, spec;
                if (!readInt("Doctor ID:", id, 1, ID_MAX)) break;
                if (findDoctorIndex(doctorRoster, doctorCount, id) != -1) {
                    std::cout << "Error: Doctor ID " << id << " already exists.\n";
                    break;
                }
                if (!readText("Doctor name:", name)) break;
                if (!readText("Specialization:", spec)) break;
                if (addDoctor(doctorRoster, doctorCount, id, name, spec)) {
                    std::cout << "Dr. " << name << " (ID " << id << ") added to the roster.\n";
                }
                break;
            }
            case 2: {
                screenTitle("REMOVE DOCTOR", true);
                if (doctorCount == 0) {
                    std::cout << "The roster is empty. Nothing to remove.\n";
                    break;
                }
                showRoster(doctorRoster, doctorCount);
                std::cout << "\n";
                int id;
                if (!readInt("Doctor ID to remove:", id, 1, ID_MAX)) break;
                int idx = findDoctorIndex(doctorRoster, doctorCount, id);
                if (idx == -1) {
                    std::cout << "Error: no doctor with ID " << id << ".\n";
                    break;
                }
                bool yes = false;
                if (!readYesNo("Remove Dr. " + doctorRoster[idx].name + " and their " +
                               std::to_string(doctorRoster[idx].nurseCount) + " nurse(s)?", yes)) break;
                if (!yes) {
                    std::cout << "Removal cancelled. Nothing was changed.\n";
                    break;
                }
                std::string removedName = doctorRoster[idx].name;
                if (removeDoctor(doctorRoster, doctorCount, id)) {
                    std::cout << "Dr. " << removedName << " removed and associated nurses freed.\n";
                }
                break;
            }
            case 3: {
                screenTitle("ADD NURSE TO DOCTOR", true);
                if (doctorCount == 0) {
                    std::cout << "There are no doctors yet. Add a doctor first (option 1).\n";
                    break;
                }
                showRoster(doctorRoster, doctorCount);
                std::cout << "\n";
                int docID, nurseID;
                std::string name, shift;
                if (!readInt("Supervising Doctor ID:", docID, 1, ID_MAX)) break;
                int docIdx = findDoctorIndex(doctorRoster, doctorCount, docID);
                if (docIdx == -1) {
                    std::cout << "Error: no doctor with ID " << docID << ".\n";
                    break;
                }
                if (!readInt("Nurse ID:", nurseID, 1, ID_MAX)) break;
                if (!readText("Nurse name:", name)) break;
                if (!readShift(shift)) break;
                if (addNurse(doctorRoster[docIdx], nurseID, name, shift)) {
                    std::cout << "Nurse " << name << " (" << shift << " shift) assigned to Dr. "
                              << doctorRoster[docIdx].name << ".\n";
                }
                break;
            }
            case 4: {
                screenTitle("REMOVE NURSE FROM DOCTOR", true);
                if (doctorCount == 0) {
                    std::cout << "There are no doctors yet.\n";
                    break;
                }
                showRoster(doctorRoster, doctorCount);
                std::cout << "\n";
                int docID, nurseID;
                if (!readInt("Supervising Doctor ID:", docID, 1, ID_MAX)) break;
                int docIdx = findDoctorIndex(doctorRoster, doctorCount, docID);
                if (docIdx == -1) {
                    std::cout << "Error: no doctor with ID " << docID << ".\n";
                    break;
                }
                if (doctorRoster[docIdx].nurseHead == nullptr) {
                    std::cout << "Dr. " << doctorRoster[docIdx].name << " has no nurses assigned.\n";
                    break;
                }
                if (!readInt("Nurse ID to remove:", nurseID, 1, ID_MAX)) break;
                if (removeNurse(doctorRoster[docIdx], nurseID)) {
                    std::cout << "Nurse " << nurseID << " removed from Dr. " << doctorRoster[docIdx].name << ".\n";
                } else {
                    std::cout << "Error: nurse ID " << nurseID << " not found for this doctor.\n";
                }
                break;
            }
            case 5:
                screenTitle("DOCTOR ROSTER", false);
                showRoster(doctorRoster, doctorCount);
                break;

            case 6: {
                screenTitle("INSERT TRIAGE PATIENT", true);
                if (is_full(&triageQueue)) {
                    std::cout << "Error: the triage queue is full (" << MAX_PATIENTS << "). Treat a patient first.\n";
                    break;
                }
                int pid, acuity;
                std::string name;
                if (!readInt("Patient ID:", pid, 1, ID_MAX)) break;
                if (!readText("Patient name:", name)) break;
                std::cout << "1-39 low, 40-69 moderate, 70-100 critical\n";
                if (!readInt("Acuity score (1-100):", acuity, 1, 100)) break;
                Patient p{pid, acuity, name};
                pq_insert(&triageQueue, p);
                std::cout << name << " added to the triage queue.\n\n";
                printPatient("Admitted patient", p);
                std::cout << "\nMost urgent right now: " << triageQueue.patientHeap[0].name
                          << " (acuity " << triageQueue.patientHeap[0].acuityScore << ")\n";
                break;
            }
            case 7: {
                screenTitle("EXTRACT MOST URGENT PATIENT", false);
                if (is_empty(&triageQueue)) {
                    std::cout << "The triage queue is empty.\n";
                } else {
                    Patient p = extract_max(&triageQueue);
                    printPatient("Now treating", p);
                    if (is_empty(&triageQueue)) {
                        std::cout << "\nThe queue is now empty.\n";
                    } else {
                        std::cout << "\nNext in line: " << triageQueue.patientHeap[0].name
                                  << " (acuity " << triageQueue.patientHeap[0].acuityScore << "), "
                                  << triageQueue.patientCount << " waiting.\n";
                    }
                }
                break;
            }
            case 8: {
                screenTitle("PEEK HIGHEST ACUITY PATIENT", false);
                if (is_empty(&triageQueue)) {
                    std::cout << "The triage queue is empty.\n";
                } else {
                    Patient p = peek_max(&triageQueue);
                    printPatient("Highest urgency (not removed)", p);
                }
                break;
            }
            case 9:
                screenTitle("TRIAGE PRIORITY QUEUE", false);
                showQueue(&triageQueue);
                break;

            case 10:
                std::cout << "Cleaning dynamic memory and exiting...\n";
                break;
        }

        if (g_cancel) {
            g_cancel = false;
            std::cout << "Action cancelled. Nothing was changed.\n";
        }

        if (choice != 10 && !g_eof) {
            pauseForMenu();
        }
    }

    // Always release nurse lists, whether the user chose 10 or the input ended.
    for (int i = 0; i < doctorCount; ++i) {
        freeNurseList(doctorRoster[i].nurseHead);
    }
    doctorCount = 0;
    std::cout << "Shutdown complete.\n";

    return 0;
}