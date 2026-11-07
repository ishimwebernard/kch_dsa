#include <cassert>
#include <iostream>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"

void test_module1_staff() {
    std::cout << "[RUNNING] Module 1 Staff Management Tests...\n";
    Doctor roster[MAX_DOCTORS];
    int docCount = 0;

    // Doctor Array Insertion
    assert(addDoctor(roster, docCount, 101, "Alex", "Cardiology") == true);
    assert(addDoctor(roster, docCount, 102, "Beatrice", "Neurology") == true);
    assert(docCount == 2);
    assert(roster[0].doctorID == 101);
    assert(roster[1].doctorID == 102);

    // Nurse Tail Insertion
    assert(addNurse(roster[0], 501, "Nurse1", "Day") == true);
    assert(addNurse(roster[0], 502, "Nurse2", "Night") == true);
    assert(roster[0].nurseCount == 2);
    assert(roster[0].nurseHead->nurseID == 501);
    assert(roster[0].nurseHead->next->nurseID == 502);

    // Nurse Head Deletion
    assert(removeNurse(roster[0], 501) == true);
    assert(roster[0].nurseCount == 1);
    assert(roster[0].nurseHead->nurseID == 502);

    // Nurse Tail Deletion
    assert(removeNurse(roster[0], 502) == true);
    assert(roster[0].nurseCount == 0);
    assert(roster[0].nurseHead == nullptr);

    // Doctor Deletion & Array Left-Shifting
    addDoctor(roster, docCount, 103, "Charles", "Orthopedics");
    assert(docCount == 3);
    assert(removeDoctor(roster, docCount, 102) == true);
    assert(docCount == 2);
    assert(roster[0].doctorID == 101);
    assert(roster[1].doctorID == 103);

    for (int i = 0; i < docCount; ++i) {
        freeNurseList(roster[i].nurseHead);
    }
    std::cout << "[PASS] Module 1 Staff Management Tests Passed.\n";
}

void test_module2_triage() {
    std::cout << "[RUNNING] Module 2 Triage Max-Heap Tests...\n";
    PriorityQueue pq;

    assert(is_empty(&pq) == true);

    // Heap Insert Dominance Assertion
    int acuities[] = {14, 22, 45, 10, 8, 30, 95};
    for (int i = 0; i < 7; ++i) {
        Patient p{i + 1, acuities[i], "P" + std::to_string(i + 1)};
        assert(pq_insert(&pq, p) == true);
    }

    assert(pq.patientCount == 7);
    assert(peek_max(&pq).acuityScore == 95);

    // Extraction & Sift-down Dominance Assertions
    Patient max1 = extract_max(&pq);
    assert(max1.acuityScore == 95);
    assert(pq.patientCount == 6);
    assert(peek_max(&pq).acuityScore == 45);

    Patient max2 = extract_max(&pq);
    assert(max2.acuityScore == 45);

    Patient max3 = extract_max(&pq);
    assert(max3.acuityScore == 30);

    Patient max4 = extract_max(&pq);
    assert(max4.acuityScore == 22);

    std::cout << "[PASS] Module 2 Triage Max-Heap Tests Passed.\n";
}

int main() {
    test_module1_staff();
    test_module2_triage();
    std::cout << "\nALL PHASE 1 UNIT TESTS PASSED SUCCESSFULLY!\n";
    return 0;
}