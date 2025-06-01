# OpenMPI Prime Generators

## Overview

This repository contains implementation of a multi‐threaded prime‐number generator using OpenMP. The program:

1. Accepts three command‐line arguments:  
   - **M** (lower bound integer),  
   - **N** (upper bound integer),  
   - **t** (number of threads).  
2. Checks each integer in \[M, N\] for primality in parallel.  
3. Stores all primes in a shared array (using a critical section).  
4. Sorts the prime list and writes it to a text file named `N.txt`.  
5. Measures wall‐clock time for the core primality loop using `omp_get_wtime()`.  

Experiments compare runtime and speedup for two input sizes:  
- **Small**: M = 20, N = 5 000  
- **Large**: M = 20, N = 5 000 000  

By varying the thread count (1, 2, 5, 10, 25, 50), we observe how overhead and workload size affect parallel scaling.


## Tech Stack

- **Language:** C (C99)  
- **Parallel Framework:** OpenMP (pragma‐based multithreading)  
- **Compiler & Build Tools:**  
  - `gcc` with `-fopenmp` (tested on GCC 9.4.0)  
  - Makefile (optional)  
- **Execution Environment:** Linux (Ubuntu 22.04 or similar)  
- **Timing Routine:** `omp_get_wtime()`  


## Features

- **Dynamic Thread Scheduling:**  
  - Uses `#pragma omp for schedule(dynamic)` to distribute the primality checks evenly.  
- **Shared Array with Critical Section:**  
  - Each thread checks if `x` is prime; if so, enters `#pragma omp critical` to append `x` to a shared `primes[]` array and increment `count`.  
- **Correctness & Ordering:**  
  - After all threads finish, a call to `qsort(...)` arranges the `primes[]` array in ascending order.  
  - The final prime list is written to `N.txt` (one prime per line).  
- **Configurable Parameters:**  
  - **M** and **N** define the inclusive range of numbers to test.  
  - **t** sets the number of OpenMP threads at runtime.  
- **Performance Measurement:**  
  - Captures start/stop times with `omp_get_wtime()` around the main parallel loop.  
  - Prints the total number of primes found and the elapsed time.


## Key Takeaways
- **Task Granularity Matters:** Small ranges lead to parallel overhead dominating; medium/large ranges see real gains.
- **Critical Section Overhead:** Even though primality checking is expensive for large \(x\), every time a prime is found, all threads serialize briefly. At very high thread counts, that serialization and memory contention limit further speedup.  
- **Dynamic Scheduling:** Helps balance uneven workloads when primes are not uniformly distributed, but carries its own overhead.



## Quick Start

### Prerequisites

- Linux or macOS with GCC ≥ 9.0 (with OpenMP support).  
- No additional libraries beyond the standard C library and OpenMP.

### 1. Clone or Download

```bash
git clone https://github.com/<your-username>/parallel-computing-lab2.git
cd parallel-computing-lab2
