#include <iostream>
#include <string>
#include "../include/module1_staff.h"
#include "../include/module2_triage.h"
#include "../include/module3_appointments.h"
#include "../include/module4_records.h"

static int total = 0, passed = 0;
static void check(bool ok, const std::string &name) {
    ++total;
    if (ok) ++passed;
    std::cout << (ok ? "PASS: " : "FAIL: ") << name << "\n";
}

// ---------------- Module 1 ----------------
static void test_module1() {
    Doctor roster[MAX_DOCTORS];
    int count = 0;
    check(addDoctor(roster, count, 101, "Alex", "Cardiology") &&
          addDoctor(roster, count, 102, "Beatrice", "Neurology") &&
          addDoctor(roster, count, 103, "Charles", "Ortho") && count == 3, "M1 add three doctors");
    check(!addDoctor(roster, count, 102, "Dup", "X") && count == 3, "M1 duplicate doctor rejected");

    addNurse(roster[0], 501, "N1", "Day");
    addNurse(roster[0], 502, "N2", "Night");
    addNurse(roster[1], 503, "N3", "Day");
    check(roster[0].nurseCount == 2 && roster[0].nurseHead->next->nurseID == 502 &&
          roster[0].nurseHead->next->next == nullptr, "M1 nurse tail insertion");

    check(!removeNurse(roster[2], 999), "M1 remove nurse from empty list fails");
    check(!removeNurse(roster[0], 999) && roster[0].nurseCount == 2, "M1 remove missing nurse fails");
    addNurse(roster[0], 504, "N4", "Day");
    check(removeNurse(roster[0], 502) && roster[0].nurseCount == 2 &&
          roster[0].nurseHead->next->nurseID == 504, "M1 delete middle nurse");
    check(removeNurse(roster[0], 504) && roster[0].nurseHead->next == nullptr, "M1 delete tail nurse");
    check(removeNurse(roster[0], 501) && roster[0].nurseHead == nullptr && roster[0].nurseCount == 0,
          "M1 delete head nurse (list becomes empty)");

    check(removeDoctor(roster, count, 102) && count == 2 && roster[0].doctorID == 101 &&
          roster[1].doctorID == 103, "M1 doctor deletion shifts array left");
    check(!removeDoctor(roster, count, 999), "M1 delete missing doctor fails");

    for (int i = 0; i < count; ++i) freeNurseList(roster[i].nurseHead);

    // Full-array condition
    Doctor big[MAX_DOCTORS];
    int bc = 0;
    for (int i = 0; i < MAX_DOCTORS; ++i) addDoctor(big, bc, i + 1, "D", "S");
    check(bc == MAX_DOCTORS && !addDoctor(big, bc, 9999, "Over", "S"), "M1 full roster rejects insert");
}

// ---------------- Module 2 ----------------
static bool heap_ok(const PriorityQueue &pq) {
    for (int i = 1; i < pq.patientCount; ++i)
        if (pq.patientHeap[parent(i)].acuityScore < pq.patientHeap[i].acuityScore) return false;
    return true;
}
static void test_module2() {
    PriorityQueue pq;
    int a[] = {14, 22, 45, 10, 8, 30, 95};
    for (int i = 0; i < 7; ++i) pq_insert(&pq, Patient{i + 1, a[i], "P"});
    int expect[] = {95, 14, 45, 10, 8, 22, 30};
    bool same = true;
    for (int i = 0; i < 7; ++i) if (pq.patientHeap[i].acuityScore != expect[i]) same = false;
    check(same, "M2 final heap array = [95,14,45,10,8,22,30]");
    check(heap_ok(pq), "M2 max-heap dominance after inserts");

    Patient m = extract_max(&pq);
    int after[] = {45, 14, 30, 10, 8, 22};
    bool same2 = pq.patientCount == 6;
    for (int i = 0; i < 6 && same2; ++i) if (pq.patientHeap[i].acuityScore != after[i]) same2 = false;
    check(m.acuityScore == 95 && same2 && heap_ok(pq), "M2 extract_max trace [45,14,30,10,8,22]");

    int order[] = {45, 30, 22, 14, 10, 8};
    bool sorted = true;
    for (int i = 0; i < 6; ++i) if (extract_max(&pq).acuityScore != order[i]) sorted = false;
    check(sorted && is_empty(&pq), "M2 repeated extraction is heapsort order");
    check(extract_max(&pq).patientID == -1, "M2 extract from empty returns sentinel");

    PriorityQueue full;
    for (int i = 0; i < MAX_PATIENTS; ++i) pq_insert(&full, Patient{i, (i % 100) + 1, "P"});
    check(is_full(&full) && heap_ok(full), "M2 fill to capacity stays a heap");
}

// ---------------- Module 3 ----------------
static void test_module3() {
    AppointmentNode* root = nullptr;
    int t[] = {1030, 815, 1400, 900, 1130, 745, 1530};
    for (int i = 0; i < 7; ++i) root = insert_appointment(root, t[i], 101 + i);
    check(root->appointmentTime == 1030 && root->left->appointmentTime == 815 &&
          root->right->appointmentTime == 1400, "M3 BST root/children shape");
    check(root->left->right->appointmentTime == 900 && root->left->left->appointmentTime == 745 &&
          root->right->left->appointmentTime == 1130 && root->right->right->appointmentTime == 1530,
          "M3 BST leaves shape");
    check(search_appointment(root, 900) && search_appointment(root, 1530), "M3 search finds booked slots");
    check(!search_appointment(root, 1000) && !search_appointment(nullptr, 900), "M3 search misses / empty tree");
    int before = 0;
    root = insert_appointment(root, 900, 999);           // duplicate slot is rejected
    (void)before;
    check(root->left->right->patientID == 104, "M3 duplicate slot keeps original booking");
    free_appointments(root);
}

// ---------------- Module 4 ----------------
static void test_module4() {
    check(generate_key("11998801A") == 486, "M4 generate_key(\"11998801A\") == 486");
    check(is_valid_citizen_id("11998801A") && !is_valid_citizen_id("1199880A") &&
          !is_valid_citizen_id("119988011") && !is_valid_citizen_id("1199880AA"), "M4 citizen ID validation");
    check(hash_function(15, 9) == 6 && hash_function(28, 9) == 1 && hash_function(486, 9) == 0, "M4 modulo hash");

    int table[TABLE_SIZE];
    init_key_table(table, TABLE_SIZE);
    int keys[] = {15, 28, 19, 20, 33, 12, 10};
    bool ok = true;
    for (int k : keys) ok = ok && insert_patient_record(table, TABLE_SIZE, k);
    int expect[TABLE_SIZE] = {-1, 28, 19, 20, 12, 10, 15, 33, -1};
    bool same = true;
    for (int i = 0; i < TABLE_SIZE; ++i) if (table[i] != expect[i]) same = false;
    check(ok && same, "M4 linear probing layout [-1,28,19,20,12,10,15,33,-1]");
    check(search_patient_key(table, TABLE_SIZE, 10) == 5 && search_patient_key(table, TABLE_SIZE, 99) == -1,
          "M4 linear probing search");

    // wrap-around + full table
    int small[3];
    init_key_table(small, 3);
    check(insert_patient_record(small, 3, 2) && insert_patient_record(small, 3, 5) &&
          small[0] == 5, "M4 probing wraps to slot 0");
    check(insert_patient_record(small, 3, 8) && !insert_patient_record(small, 3, 11), "M4 full table returns false");

    RecordTable rt;
    check(record_insert(rt, "11998801A", "Alice", "Asthma") &&
          record_insert(rt, "11998802B", "Bob", "None") &&
          record_insert(rt, "A1199880" /*invalid but allowed by core API*/, "Eve", "x") &&
          rt.recordCount == 3, "M4 chained insert");
    int cmp = 0;
    const PatientRecord* r = record_find(rt, "11998802B", &cmp);
    check(r != nullptr && r->name == "Bob" && cmp >= 1, "M4 chained find");
    check(record_find(rt, "00000000Z") == nullptr, "M4 chained find miss");
    check(!record_insert(rt, "11998801A", "Dup", "x"), "M4 duplicate citizen ID rejected");
    // anagram IDs share the same ASCII-sum key -> same bucket, still distinguishable
    check(generate_key("11998801A") == generate_key("11998810A") &&
          record_insert(rt, "11998810A", "Carl", "Flu") &&
          record_find(rt, "11998810A")->name == "Carl" &&
          record_find(rt, "11998801A")->name == "Alice", "M4 colliding keys resolved via chain");
    check(record_remove(rt, "11998801A") && record_find(rt, "11998801A") == nullptr &&
          record_find(rt, "11998810A") != nullptr && !record_remove(rt, "11998801A"), "M4 chained delete");
    record_table_free(rt);
    check(rt.recordCount == 0, "M4 table freed");
}

int main() {
    test_module1();
    test_module2();
    test_module3();
    test_module4();
    std::cout << "Summary: " << passed << "/" << total << " tests passed\n";
    return passed == total ? 0 : 1;
}