#ifndef MODULE3_APPOINTMENTS_H
#define MODULE3_APPOINTMENTS_H

#include <iostream>

struct AppointmentNode {
    int appointmentTime;  
    int patientID;        
    AppointmentNode* left;
    AppointmentNode* right;

    AppointmentNode(int time, int id)
        : appointmentTime(time), patientID(id), left(nullptr), right(nullptr) {}
};

// core operation functions
AppointmentNode* insert_appointment(AppointmentNode* root, int appointmentTime, int patientID);
bool search_appointment(AppointmentNode* root, int appointmentTime);

// Utility / Display
void print_schedule_inorder(const AppointmentNode* root);
void free_appointments(AppointmentNode* root);

#endif
