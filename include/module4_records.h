#ifndef MODULE4_RECORDS_H
#define MODULE4_RECORDS_H

#include <string>
#include <iostream>

constexpr int TABLE_SIZE = 9;          // m = 9 slots (0..8), per the specification
constexpr int EMPTY_SLOT = -1;         // sentinel for an unused slot in the key-only table

// ---------------------------------------------------------------
// Q4.1 / Q4.3: Key generation, modulo hash and linear probing
// (original logic by Gerald, module4/HashTable.*, merged here)
// ---------------------------------------------------------------
bool is_valid_citizen_id(const std::string &citizenID);       // 8 digits + 1 letter
int  generate_key(const std::string &citizenID);              // sum of ASCII codes
int  hash_function(int key, int m);                           // key mod m
bool insert_patient_record(int hashTable[], int m, int key);  // open addressing, linear probing
int  search_patient_key(const int hashTable[], int m, int key); // slot index or -1
void init_key_table(int hashTable[], int m);                  // fills with EMPTY_SLOT
void print_key_table(const int hashTable[], int m);

// ---------------------------------------------------------------
// Full patient-record dictionary: Separate Chaining (linked list buckets)
// ---------------------------------------------------------------
struct PatientRecord {
    std::string citizenID;   // e.g. 11998801A
    std::string name;
    std::string history;     // short medical-history note
    int key = 0;             // generate_key(citizenID)
};

struct RecordNode {
    PatientRecord record;
    RecordNode* next = nullptr;
};

struct RecordTable {
    RecordNode* buckets[TABLE_SIZE] = {nullptr};
    int recordCount = 0;
};

bool  record_insert(RecordTable &table, const std::string &citizenID,
                    const std::string &name, const std::string &history);
const PatientRecord* record_find(const RecordTable &table, const std::string &citizenID,
                                 int *comparisons = nullptr);
bool  record_remove(RecordTable &table, const std::string &citizenID);
void  record_table_print(const RecordTable &table);
void  record_table_free(RecordTable &table);

#endif