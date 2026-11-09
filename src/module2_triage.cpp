#include "../include/module2_triage.h"


void bubble_up(PriorityQueue* pq, int index) {
    while (index > 0){
        int p = parent(index);
        if (pq->patientHeap[index].acuityScore > pq->patientHeap[p].acuityScore){
            Patient temp = pq->patientHeap[p];
            pq->patientHeap[p] = pq->patientHeap[index];
            pq->patientHeap[index] = temp;
            index = p;
        } else {
            break;
        }
        
    }
    
}

void bubble_down(PriorityQueue* pq, int index) {
    while (true){
        int l = left_child(index);
        int r = right_child(index);
        int largest = index;

        if (l < pq->patientCount && pq->patientHeap[l].acuityScore > pq->patientHeap[largest].acuityScore){
            largest = l;
        }

        if (r < pq->patientCount && pq->patientHeap[r].acuityScore > pq->patientHeap[largest].acuityScore){
            largest = r;
        }

        if (largest == index){
            break;
        }
        
        Patient temp = pq->patientHeap[index];
        pq->patientHeap[index] = pq->patientHeap[largest];
        pq->patientHeap[largest] = temp;
        index = largest;
        
    }
    
}

bool pq_insert(PriorityQueue* pq, Patient patientRecord) {
    if (is_full(pq)){
        return false;
    }
    pq->patientHeap[pq->patientCount] = patientRecord;
    pq->patientCount++;
    bubble_up(pq, pq->patientCount-1);
    return true;
}

Patient extract_max(PriorityQueue* pq) {
    if (is_empty(pq)){
        throw std::underflow_error("the array is emptu");
    }

    Patient max = pq->patientHeap[0];
    pq->patientHeap[0] = pq->patientHeap[pq->patientCount-1];
    pq->patientCount--;

    if (pq->patientCount > 0){
        bubble_down(pq, 0);
    }

    return max;
}

Patient peek_max(const PriorityQueue* pq) {
    if (pq->patientCount == 0){
        return Patient{-1, -1, ""};
    }
    return pq->patientHeap[0];
}

bool is_empty(const PriorityQueue* pq) {
    return pq->patientCount == 0;
}

bool is_full(const PriorityQueue* pq) {
    return pq->patientCount == MAX_PATIENTS;
}

void print_priority_queue(const PriorityQueue* pq) {
    if (pq->patientCount == 0){
        std::cout << "Triage queue is empty.\n";
        return;
    }
    for (int i = 0; i < pq->patientCount; i++){
        std::cout << "[" << i << "] ID: " << pq->patientHeap[i].patientID
                  << ", Name: " << pq->patientHeap[i].name
                  << ", Acuity: " << pq->patientHeap[i].acuityScore << "\n";
    }
}