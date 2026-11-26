#include "../include/module3_appointments.h"
#include <iostream>

// Task 3.1: Insert appointment slot into BST (recursive)
AppointmentNode* insert_appointment(AppointmentNode* root, int appointmentTime, int patientID) {
    // TODO: Base case - return new node if null

    // TODO: Recurse left if key < root key

    // TODO: Recurse right if key > root key

    (void)appointmentTime;
    (void)patientID;
    return root;
}

// Task 3.2: Verify if slot is booked (recursive)
bool search_appointment(AppointmentNode* root, int appointmentTime) {
    // TODO: Return false if null, true if match

    // TODO: Recurse left if key < root key, else right

    (void)root;
    (void)appointmentTime;
    return false;
}

// In-order traversal: prints the daily schedule in chronological order
void print_schedule_inorder(const AppointmentNode* root) {
    // TODO: left subtree, visit node, right subtree
    (void)root;
}

// Post-order delete of every node in the tree
void free_appointments(AppointmentNode* root) {
    // TODO: free left, free right, delete root
    (void)root;
}
