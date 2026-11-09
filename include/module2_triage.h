#ifndef MODULE2_TRIAGE_H
#define MODULE2_TRIAGE_H

#include <string>
#include <iostream>
#include<stdexcept>

constexpr int MAX_PATIENTS = 100;

struct Patient {
    int patientID;
    int acuityScore; // 1 to 100 (higher score indicates higher urgency)
    std::string name;
};

struct PriorityQueue {
    Patient patientHeap[MAX_PATIENTS];
    int patientCount = 0;
};

// Index Mapping Formulas
inline int parent(int i) { return (i - 1) / 2; }
inline int left_child(int i) { return 2 * i + 1; }
inline int right_child(int i) { return 2 * i + 2; }

// Priority Queue Core Operations
void bubble_up(PriorityQueue* pq, int index);
void bubble_down(PriorityQueue* pq, int index);
bool pq_insert(PriorityQueue* pq, Patient patientRecord);
Patient extract_max(PriorityQueue* pq);
Patient peek_max(const PriorityQueue* pq);
bool is_empty(const PriorityQueue* pq);
bool is_full(const PriorityQueue* pq);

// Utility / Display
void print_priority_queue(const PriorityQueue* pq);
#endif