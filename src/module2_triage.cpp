#include "../include/module2_triage.h"
#include <iostream>

// Helper swap function (no STL <algorithm> / std::swap)
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

    // we recursively bubble up until index acuityScore is no longer greater than the parent's
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

    // if the left child exists and its score is greater than that of the parent
    if (l < pq->patientCount && pq->patientHeap[l].acuityScore > pq->patientHeap[largest].acuityScore) {
        largest = l; // we temporarily set largest to l before we check right
    }

    // if the right child exists and is greater than the current largest (parent or left), it becomes the largest
    if (r < pq->patientCount && pq->patientHeap[r].acuityScore > pq->patientHeap[largest].acuityScore) {
        largest = r;
    }

    // if a child is larger, swap it with the parent and keep sifting down from the child's position
    // stop once the node is larger than both children (or is a leaf)
    if (largest != index) {
        swap_patients(pq->patientHeap[index], pq->patientHeap[largest]);
        bubble_down(pq, largest);
    }
}

// Insertion Algorithm (void return type, recursive sift-up)
void pq_insert(PriorityQueue* pq, Patient patientRecord) {
    // if the heap is already at MAX_PATIENTS we can't insert anymore
    if (is_full(pq)) {
        std::cerr << "Triage queue is full.\n";
        return;
    }

    // we place the new patient at the next open leaf (end of the array)
    pq->patientHeap[pq->patientCount] = patientRecord;
    pq->patientCount++;

    // then we bubble it up from the last index until the max-heap property holds again
    bubble_up(pq, pq->patientCount - 1);
}

// Extraction Algorithm (extract root, move last leaf to root, recursive sift-down)
Patient extract_max(PriorityQueue* pq) {
    // if there are no patients we return a sentinel patient with -1 values
    if (is_empty(pq)) {
        std::cerr << "Triage queue is empty.\n";
        return Patient{-1, -1, ""};
    }

    // the root always holds the highest acuity patient, so we save it to return
    Patient max = pq->patientHeap[0];

    // we move the last leaf to the root and shrink the heap by one
    pq->patientHeap[0] = pq->patientHeap[pq->patientCount - 1];
    pq->patientCount--;

    // if patients are left, we bubble the new root down to restore the max-heap property
    if (pq->patientCount > 0) {
        bubble_down(pq, 0);
    }

    return max;
}

// Peek at the highest priority patient
Patient peek_max(const PriorityQueue* pq) {
    // if there are no patients we return a sentinel patient with -1 values
    if (is_empty(pq)) {
        return Patient{-1, -1, ""};
    }

    // the root is the max, we return it without removing it
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

    // we loop through the array in index order (heap layout, not sorted by acuity)
    for (int i = 0; i < pq->patientCount; i++) {
        std::cout << "[" << i << "] ID: " << pq->patientHeap[i].patientID
                  << ", Name: " << pq->patientHeap[i].name
                  << ", Acuity: " << pq->patientHeap[i].acuityScore << "\n";
    }
}