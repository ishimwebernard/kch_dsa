#include "../include/module4_records.h"
#include <iomanip>

// ---------------- Q4.1: key generation ----------------
// Array indices must be integers, so the alphanumeric ID is reduced to an integer
// by summing the ASCII code of every character. Example: "11998801A" -> 486.
bool is_valid_citizen_id(const std::string &id) {
    if (id.size() != 9) return false;
    for (int i = 0; i < 8; ++i) {
        if (id[i] < '0' || id[i] > '9') return false;
    }
    char c = id[8];
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int generate_key(const std::string &citizenID) {
    int key = 0;
    for (unsigned char ch : citizenID) {   // unsigned: never adds a negative value
        key += ch;
    }
    return key;
}

// ---------------- Q4.3a: modulo hash ----------------
int hash_function(int key, int m) {
    if (m <= 0) return -1;
    int h = key % m;
    return h < 0 ? h + m : h;
}

// ---------------- Q4.3b: linear probing insertion ----------------
// h(k, i) = (h(k) + i) mod m. Returns false when the table is full (or key invalid).
bool insert_patient_record(int hashTable[], int m, int key) {
    if (m <= 0 || key < 0) return false;
    int initialSlot = hash_function(key, m);
    for (int i = 0; i < m; ++i) {
        int slot = (initialSlot + i) % m;
        if (hashTable[slot] == EMPTY_SLOT) {
            hashTable[slot] = key;
            return true;
        }
    }
    return false;
}

int search_patient_key(const int hashTable[], int m, int key) {
    if (m <= 0 || key < 0) return -1;
    int initialSlot = hash_function(key, m);
    for (int i = 0; i < m; ++i) {
        int slot = (initialSlot + i) % m;
        if (hashTable[slot] == key) return slot;
        if (hashTable[slot] == EMPTY_SLOT) return -1;  // an empty slot ends the probe chain
    }
    return -1;
}

void init_key_table(int hashTable[], int m) {
    for (int i = 0; i < m; ++i) hashTable[i] = EMPTY_SLOT;
}

void print_key_table(const int hashTable[], int m) {
    for (int i = 0; i < m; ++i) {
        std::cout << "Slot " << i << ": ";
        if (hashTable[i] == EMPTY_SLOT) std::cout << "(empty)\n";
        else std::cout << hashTable[i] << "\n";
    }
}

// ---------------- Separate chaining dictionary ----------------
// Different IDs can share one ASCII-sum key (e.g. anagrams), so records are
// compared by full citizenID, never by key alone.
bool record_insert(RecordTable &table, const std::string &citizenID,
                   const std::string &name, const std::string &history) {
    if (record_find(table, citizenID) != nullptr) {
        std::cerr << "Citizen ID " << citizenID << " already has a record.\n";
        return false;
    }
    RecordNode* node = new RecordNode;
    node->record.citizenID = citizenID;
    node->record.name = name;
    node->record.history = history;
    node->record.key = generate_key(citizenID);
    node->next = nullptr;

    int slot = hash_function(node->record.key, TABLE_SIZE);
    if (table.buckets[slot] == nullptr) {
        table.buckets[slot] = node;
    } else {                                 // append at the chain tail
        RecordNode* tail = table.buckets[slot];
        while (tail->next != nullptr) tail = tail->next;
        tail->next = node;
    }
    ++table.recordCount;
    return true;
}

const PatientRecord* record_find(const RecordTable &table, const std::string &citizenID,
                                 int *comparisons) {
    int count = 0;
    int slot = hash_function(generate_key(citizenID), TABLE_SIZE);
    for (RecordNode* cur = table.buckets[slot]; cur != nullptr; cur = cur->next) {
        ++count;
        if (cur->record.citizenID == citizenID) {
            if (comparisons) *comparisons = count;
            return &cur->record;
        }
    }
    if (comparisons) *comparisons = count;
    return nullptr;
}

bool record_remove(RecordTable &table, const std::string &citizenID) {
    int slot = hash_function(generate_key(citizenID), TABLE_SIZE);
    RecordNode* prev = nullptr;
    RecordNode* cur = table.buckets[slot];
    while (cur != nullptr && cur->record.citizenID != citizenID) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == nullptr) return false;
    if (prev == nullptr) table.buckets[slot] = cur->next;
    else prev->next = cur->next;
    delete cur;
    --table.recordCount;
    return true;
}

void record_table_print(const RecordTable &table) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        std::cout << "Slot " << i << ": ";
        if (table.buckets[i] == nullptr) { std::cout << "(empty)\n"; continue; }
        for (RecordNode* cur = table.buckets[i]; cur != nullptr; cur = cur->next) {
            std::cout << "[" << cur->record.citizenID << " k=" << cur->record.key
                      << " " << cur->record.name << "]";
            if (cur->next) std::cout << " -> ";
        }
        std::cout << "\n";
    }
}

void record_table_free(RecordTable &table) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        RecordNode* cur = table.buckets[i];
        while (cur != nullptr) {
            RecordNode* next = cur->next;
            delete cur;
            cur = next;
        }
        table.buckets[i] = nullptr;
    }
    table.recordCount = 0;
}