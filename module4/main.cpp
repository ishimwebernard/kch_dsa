#include <iostream>
#include "HashTable.h"

int main()
{
    // The table will has 9 slots, numbered 0 through 8
    const int TABLE_SIZE = 9;
    
    int hashTable[TABLE_SIZE]; // Array with the table size of 9

    // Visit each slot and mark it as empty
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTable[i] = -1;
    }

    // Convert a Citizen ID into an integer key.
    std::string citizenID = "11998801A";
    int generatedKey = generateKey(citizenID);

    // Expected result: 486
    std::cout << "Citizen ID: " << citizenID << '\n';
    std::cout << "Generated key: " << generatedKey << "\n\n";

    // Store the example keys in their arrival order
    int patientKeys[] = {15,28,19,20,33,12,10};

    // Attempts to insert key into the hash table
    
    for(int key : patientKeys)
    {
        // Insert the generated key into the table
        bool inserted = insertPatientRecord(hashTable, TABLE_SIZE, key);

        // Report if insertion succeeded
        if (inserted)
            {
                std::cout << "Inserted key " << key << '\n';
            }
            else
            {
                std::cout << "Table is full.\n";
            }
    }


    // Display the completed table
    std::cout << "\nFinal hash table:\n";
    std::cout << "______________________________________________\n\n";

    for (int i = 0; i < TABLE_SIZE; i++)
    {
        std::cout << "Slot " << i << ": "
                  << hashTable[i] << '\n';
    }

    return 0;
}