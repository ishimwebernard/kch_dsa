#include "../include/module3_appointments.h"
#include <iostream>
#include <iomanip>

// Insert appointment slot into BST (recursive)
AppointmentNode* insert_appointment(AppointmentNode* root, int appointmentTime, int patientID) {
    if (root == nullptr) {
        return new AppointmentNode(appointmentTime, patientID);
    }

    if (appointmentTime < root->appointmentTime) {
        root->left = insert_appointment(root->left, appointmentTime, patientID);

    } else if (appointmentTime > root->appointmentTime) {

        root->right = insert_appointment(root->right, appointmentTime, patientID);
    } else {
        std::cerr << "Slot " << appointmentTime << " is already booked.\n";
    }

    return root;
}

// Verify if slot is booked recursively
bool search_appointment(AppointmentNode* root, int appointmentTime) {
    if (root == nullptr) {
        return false;
    }
    if (appointmentTime == root->appointmentTime) {
        return true;
    }

    // go to the left subtree if the root is greater than the time, then recursively keep going down
    // until we pass a root that's a nullptr to the search_appointment function
    if (appointmentTime < root->appointmentTime) {
        return search_appointment(root->left, appointmentTime);
    }

    // same logic for the right side. 
    return search_appointment(root->right, appointmentTime);
}

// In-order traversal: prints the daily schedule in chronological order
void print_schedule_inorder(const AppointmentNode* root) {
    if (root == nullptr) {
        return;
    }

    // we recurse to the left printing each item unitl root is nullptr
    // at that point we now print the roots of the left subtree then the root, after we are done with the left
    // we can now call the right in the same recursive manner
    print_schedule_inorder(root->left);

    int hh = root->appointmentTime / 100;
    int mm = root->appointmentTime % 100;
    std::cout << std::setfill('0')
              << std::setw(4) << root->appointmentTime
              << " (" << std::setw(2) << hh << ":" << std::setw(2) << mm << ")"
              << std::setfill(' ')
              << "  Patient ID: " << root->patientID << "\n";

    print_schedule_inorder(root->right);
}

// Post-order delete of every node in the tree
void free_appointments(AppointmentNode* root) {
    if (root == nullptr) {
        return;
    }
    // Children first, so we never read from a node after deleting it
    free_appointments(root->left);
    free_appointments(root->right);
    delete root;
}
