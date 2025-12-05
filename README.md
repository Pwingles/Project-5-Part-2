CS 315 — Project 5 Part 2: Hash Tables Open Addressing with LRU Profile

Student: Kevin Rodriguez
Instructor: Prof. Ali Kooshesh
Semester: Fall 2025
Student ID: 008858727
Course: CS 315 – Data Structures
Repository: https://github.com/Pwingles/Project-5-Part-2 

Overview

This project compares the performance of Single Hashing and Double Hashing (with compaction) under an LRU (Least Recently Used) profile. It implements an Open Addressing Hash Table and simulates an LRU cache workload to analyze the efficiency of different probing strategies in terms of elapsed time and average probes.

The project includes:
- A `HashTableDictionary` implementation supporting both Single and Double hashing.
- An `LRUTraceGenerator` that creates trace files simulating LRU cache behavior.
- A harness (`main.cpp`) to execute traces and measure performance.
- A data collection script (`collect_data.sh`) to automate experiments across various sizes (N).
- A final report analyzing the results, including the impact of CPU cache locality on performance.

Project Status: complete

Completed:

✓ `HashTableDictionary` class with Insert/Remove/Find
✓ Single Hashing (Linear Probing) and Double Hashing implementations
✓ Tombstone mechanism and Compaction (re-hashing) logic
✓ `LRUTraceGenerator` for creating realistic LRU workloads
✓ `main.cpp` harness with command-line argument parsing
✓ `collect_data.sh` script for automated building and data collection
✓ Performance analysis and report generation (`Rodriguez_Kevin_project5_part2_report.md`)
✓ D3.js visualization for analyzing results

Collaboration & Sources

Original Work:
- `HashTableDictionary` core logic (probing, compaction).
- `LRUTraceGenerator` implementation.
- `main.cpp` harness modifications for timing and CSV output.
- Analysis and Report. 

AI-Assisted Development:
- Used AI for generating the `collect_data.sh` script.
- Used AI for formatting README.

Testing & Status

Current Status

Implementation Progress: 100% complete

Files:

- `main.cpp`: Entry point for the test harness.
- `HashTableDictionary.cpp/hpp`: Core Hash Table implementation.
- `LRUTraceGenerator.cpp`: Generates trace files for testing.
- `collect_data.sh`: Automates the build and test process.
- `CMakeLists.txt`: Build configuration.

Testing Plan:

1. **Trace Generation**: Run `LRUTraceGenerator` to create trace files for N=1024 to N=1048576.
2. **Correctness**: Verify that the Hash Table handles Insert/Remove operations correctly without crashing.
3. **Performance**: Run the harness on all trace files for both Single and Double hashing.
4. **Analysis**: Collect data into `LRU_csv/lru_results.csv` and verify trends (Linear Time, Probe counts).

Expected Behavior

Running `bash collect_data.sh` should:
1. Build the project.
2. Generate trace files (if not present).
3. Run the harness for all defined sizes.
4. Output `Data collection complete!` and save results to `LRU_csv/lru_results.csv`.

File Structure

Project 5 part 2/
├── build/                      # Compiled executables
├── LRU_csv/                    # Output CSV data
├── LRUTraceGenerator_output/   # Generated trace files
├── src/
│   ├── main.cpp                # Harness entry point
│   ├── HashTableDictionary.cpp # Hash Table implementation
│   ├── HashTableDictionary.hpp
│   ├── LRUTraceGenerator.cpp   # Trace Generator
│   └── ...
├── collect_data.sh             # Automation script
├── CMakeLists.txt              # Build config
├── Rodriguez_Kevin_project5_part2_report.md # Final Report
└── README.md                   # This file

Build Instructions

To compile the project manually:

>**Make sure you're in /build directory first**
```bash
cd build
make
```

To run the Trace Generator:


```bash
cd build
./LRUTraceGenerator
```
This will generate trace files in the `LRUTraceGenerator_output/` directory.

To run the full experiment (Recommended):

```bash
bash collect_data.sh
```
This script will:
1. Build the project.
2. Run the harness for all sizes (N=1024 to 1M) for both Single and Double hashing.
3. Save the results to `LRU_csv/lru_results.csv`.

Implementation Details

LRU Profile Simulation

The `LRUTraceGenerator` simulates a cache with a working set. It generates a sequence of operations where:
- Some items are accessed frequently (Hot items).
- Some items are accessed rarely (Cold items).
- The simulation ensures a mix of Hits and Misses to stress the Hash Table's Insert and Remove (Eviction) logic.

Hash Table Design

- **Open Addressing**: Uses a single array for storage.
- **Tombstones**: Deleted items are marked with a special `TOMBSTONE` value to maintain probe chains.
- **Compaction**: When the table gets too full of tombstones or elements, it triggers a compaction (re-hash) to clean up tombstones and resize if necessary.
- **Probing**:
    - **Single Hashing**: Linear probing (`index + i`).
    - **Double Hashing**: Uses a second hash function (`index + i * h2(key)`).

Notes

- The experiment shows that while Double Hashing reduces the number of probes, Single Hashing is often faster in practice due to CPU cache locality (sequential memory access).
- Compaction is critical for maintaining performance in an Open Addressing table with frequent deletions.
