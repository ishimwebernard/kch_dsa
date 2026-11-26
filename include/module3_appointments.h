#ifndef MODULE3_APPOINTMENTS_H
#define MODULE3_APPOINTMENTS_H

#include <iostream>

struct AppointmentNode {
    int appointmentTime;   // Key: 24-hr timestamp (e.g., 0800, 1030)
    int patientID;         // Patient National ID
    AppointmentNode* left;
    AppointmentNode* right;

    AppointmentNode(int time, int id)
        : appointmentTime(time), patientID(id), left(nullptr), right(nullptr) {}
};

// BST Core Operations
AppointmentNode* insert_appointment(AppointmentNode* root, int appointmentTime, int patientID);
bool search_appointment(AppointmentNode* root, int appointmentTime);

// Utility / Display
void print_schedule_inorder(const AppointmentNode* root);
void free_appointments(AppointmentNode* root);

#endif
