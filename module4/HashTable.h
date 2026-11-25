#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>

// Converts a citizen ID into an integer key by adding character coes.
int generateKey(std::string citizenID);

// Calculates the primary array index
int hashFunction(int key, int m);

// Insert a key using linear probing & Return true if inserted, or false if the table is full
bool insertPatientRecord(int hashTable[], int m, int key);

#endif


