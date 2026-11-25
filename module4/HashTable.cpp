#include "HashTable.h"
#include <string>

// Returns the sum of the ASCII values of all characters in the Citizen ID.
int generateKey(std::string citizenID)
{
    int key = 0; // creates our running total

    // For each character in the ID
    for (char character : citizenID)
    {
        // Add the character's ASCII value to the running total
        key = key + character;
    }

      return key;
}

// Calculates the array index of the returned key above
int hashFunction(int key, int m)
{
    return key % m;
}

bool insertPatientRecord(int hashTable[], int m, int key)
{
    // Calculate the key's prefered slot
    int initialSlot = hashFunction(key, m);

    // Try at most m slots, starting with the preferred one

    for (int i = 0; i < m; i++)
    {
        // Move i positions from the initial slot, Module wraps the position back to 0 after the last slot
        int slot = (initialSlot + i) % m;

        // Is slot empty?

        if (hashTable[slot] == -1)
        {
            // Store the key in the empty slot
            hashTable[slot] = key;
            
            return true;
        }

    }

    return false;
}