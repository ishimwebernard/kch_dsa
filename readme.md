
# Kigali Central Hospital Patient Management & Triage System (KCH-PMTS) - Phase 1

## Overview
Kigali Central Hospital (KCH) PMTS Phase 1 backend architecture implemented in standard C++.

- **Module 1 (Staff Management & Dynamic Rostering)**: Manages a fixed array of up to 50 doctors and linked lists of assigned nurses. Handles contiguous left-shifting on doctor removal and dynamic memory deallocation.
- **Module 2 (Emergency Room Triage Engine)**: Array-backed Max-Heap Priority Queue (up to 100 patients) sorted by clinical acuity score. Performs logarithmic `bubble_up` (sift-up) insertions and `bubble_down` (sift-down) extractions.

## Building and Running

### Compilation
Build the main application binary (`kch_phase1`):
```bash
make
