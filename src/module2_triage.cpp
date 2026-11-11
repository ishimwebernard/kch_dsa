#include "../include/module2_triage.h"
#include <iostream>

// Helper swap function
static void swap_patients(Patient& a, Patient& b) {
    Patient temp = a;
    a = b;
    b = temp;
}

// Recursive sift-up to maintain max-heap property
void bubble_up(PriorityQueue* pq, int index) {
    if (index <= 0) {
        return;
    }

    int p = parent(index);
    if (pq->patientHeap[index].acuityScore > pq->patientHeap[p].acuityScore) {
        swap_patients(pq->patientHeap[index], pq->patientHeap[p]);
        bubble_up(pq, p);
    }
}

// Recursive sift-down to maintain max-heap property
void bubble_down(PriorityQueue* pq, int index) {
    int l = left_child(index);
    int r = right_child(index);
    int largest = index;

    if (l < pq->patientCount && pq->patientHeap[l].acuityScore > pq->patientHeap[largest].acuityScore) {
        largest = l;
    }

    if (r < pq->patientCount && pq->patientHeap[r].acuityScore > pq->patientHeap[largest].acuityScore) {
        largest = r;
    }

    if (largest != index) {
        swap_patients(pq->patientHeap[index], pq->patientHeap[largest]);
        bubble_down(pq, largest);
    }
}

// 1. Insertion Algorithm (void return type, recursive sift-up)
void pq_insert(PriorityQueue* pq, Patient patientRecord) {
    if (is_full(pq)) {
        std::cerr << "Triage queue is full.\n";
        return;
    }

    pq->patientHeap[pq->patientCount] = patientRecord;
    pq->patientCount++;
    bubble_up(pq, pq->patientCount - 1);
}

// 2. Extraction Algorithm (extract root, move last leaf to root, recursive sift-down)
Patient extract_max(PriorityQueue* pq) {
    if (is_empty(pq)) {
        std::cerr << "Triage queue is empty.\n";
        return Patient{-1, -1, ""};
    }

    Patient max = pq->patientHeap[0];
    pq->patientHeap[0] = pq->patientHeap[pq->patientCount - 1];
    pq->patientCount--;

    if (pq->patientCount > 0) {
        bubble_down(pq, 0);
    }

    return max;
}

// Peek at the highest priority patient
Patient peek_max(const PriorityQueue* pq) {
    if (is_empty(pq)) {
        return Patient{-1, -1, ""};
    }
    return pq->patientHeap[0];
}

// Check if priority queue is empty
bool is_empty(const PriorityQueue* pq) {
    return pq->patientCount == 0;
}

// Check if priority queue is full
bool is_full(const PriorityQueue* pq) {
    return pq->patientCount == MAX_PATIENTS;
}

// Print priority queue elements in array order
void print_priority_queue(const PriorityQueue* pq) {
    if (is_empty(pq)) {
        std::cout << "Triage queue is empty.\n";
        return;
    }

    for (int i = 0; i < pq->patientCount; i++) {
        std::cout << "[" << i << "] ID: " << pq->patientHeap[i].patientID
                  << ", Name: " << pq->patientHeap[i].name
                  << ", Acuity: " << pq->patientHeap[i].acuityScore << "\n";
    }
}