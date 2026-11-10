#include <cassert>
#include <iostream>
#include <string>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"

int totalTests = 0;
int passedTests = 0;

void runTest(bool condition, const std::string& testName) {
    totalTests++;
    if (condition) {
        passedTests++;
        std::cout << "PASS: " << testName << "\n";
    } else {
        std::cout << "FAIL: " << testName << "\n";
    }
}

void test_module1_staff() {
    Doctor roster[MAX_DOCTORS];
    int docCount = 0;

    // Doctor Array Insertion
    bool addDoc1 = addDoctor(roster, docCount, 101, "Alex", "Cardiology");
    bool addDoc2 = addDoctor(roster, docCount, 102, "Beatrice", "Neurology");
    bool docInsertCheck = addDoc1 && addDoc2 && (docCount == 2) && (roster[0].doctorID == 101) && (roster[1].doctorID == 102);
    runTest(docInsertCheck, "doctor array insertion");

    // Nurse Tail Insertion
    bool addNurse1 = addNurse(roster[0], 501, "Nurse1", "Day");
    bool addNurse2 = addNurse(roster[0], 502, "Nurse2", "Night");
    bool nurseHeadCheck = (roster[0].nurseHead != nullptr && roster[0].nurseHead->nurseID == 501);
    bool nurseNextCheck = (roster[0].nurseHead != nullptr && roster[0].nurseHead->next != nullptr && roster[0].nurseHead->next->nurseID == 502);
    bool nurseInsertCheck = addNurse1 && addNurse2 && (roster[0].nurseCount == 2) && nurseHeadCheck && nurseNextCheck;
    runTest(nurseInsertCheck, "nurse tail insertion updates count");

    // Nurse Head Deletion
    bool remNurseHead = removeNurse(roster[0], 501);
    bool headDelCheck = remNurseHead && (roster[0].nurseCount == 1) && (roster[0].nurseHead != nullptr) && (roster[0].nurseHead->nurseID == 502);
    runTest(headDelCheck, "delete nurse at head");

    // Nurse Tail Deletion
    bool remNurseTail = removeNurse(roster[0], 502);
    bool tailDelCheck = remNurseTail && (roster[0].nurseCount == 0) && (roster[0].nurseHead == nullptr);
    runTest(tailDelCheck, "delete nurse at tail");

    // Doctor Deletion & Array Left-Shifting
    addDoctor(roster, docCount, 103, "Charles", "Orthopedics");
    bool remDoc = removeDoctor(roster, docCount, 102);
    bool shiftCheck = remDoc && (docCount == 2) && (roster[0].doctorID == 101) && (roster[1].doctorID == 103);
    runTest(shiftCheck, "doctor deletion shifts array left");

    for (int i = 0; i < docCount; ++i) {
        freeNurseList(roster[i].nurseHead);
    }
}

void test_module2_triage() {
PriorityQueue pq;
    pq.patientCount = 0; // Ensures clean initial state

    runTest(is_empty(&pq) == true, "triage queue initial empty check");

    // Heap Insert Dominance Assertion
    int acuities[] = {14, 22, 45, 10, 8, 30, 95};
    bool insertAllOk = true;
    for (int i = 0; i < 7; ++i) {
        Patient p{i + 1, acuities[i], "P" + std::to_string(i + 1)};
        pq_insert(&pq, p); // Call void pq_insert function
        
        if (pq.patientCount != i + 1) {
            insertAllOk = false;
        }
    }

    runTest(insertAllOk && (pq.patientCount == 7), "heap insert patient records");
    runTest(peek_max(&pq).acuityScore == 95, "max-heap root dominance check");

    // Extraction & Sift-down Dominance Assertions
    Patient max1 = extract_max(&pq);
    bool max1Check = (max1.acuityScore == 95) && (pq.patientCount == 6) && (peek_max(&pq).acuityScore == 45);
    runTest(max1Check, "extract max patient 1");

    Patient max2 = extract_max(&pq);
    runTest(max2.acuityScore == 45, "extract max patient 2");

    Patient max3 = extract_max(&pq);
    runTest(max3.acuityScore == 30, "extract max patient 3");

    Patient max4 = extract_max(&pq);
    runTest(max4.acuityScore == 22, "extract max patient 4");
}

int main() {
    test_module1_staff();
    test_module2_triage();

    std::cout << "Summary: " << passedTests << "/" << totalTests << " tests passed\n";
    return 0;
}