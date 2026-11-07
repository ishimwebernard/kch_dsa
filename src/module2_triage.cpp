#include "../include/module2_triage.h"


void bubble_up(PriorityQueue* pq, int index) {
    (void)pq; (void)index;
}

void bubble_down(PriorityQueue* pq, int index) {
    (void)pq; (void)index;
}

bool pq_insert(PriorityQueue* pq, Patient patientRecord) {
    (void)pq; (void)patientRecord;
    return false;
}

Patient extract_max(PriorityQueue* pq) {
    (void)pq;
    return Patient{-1, -1, ""};
}

Patient peek_max(const PriorityQueue* pq) {
    (void)pq;
    return Patient{-1, -1, ""};
}

bool is_empty(const PriorityQueue* pq) {
    (void)pq;
    return true;
}

bool is_full(const PriorityQueue* pq) {
    (void)pq;
    return false;
}

void print_priority_queue(const PriorityQueue* pq) {
    (void)pq;
}