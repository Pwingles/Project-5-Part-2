#!/bin/bash

# Ensure we are in the project root
# (This script assumes it is located in /Users/kevin/Documents/Project 5 part 2/)

# 1. Build the project 
echo "Building project..."
cd build
make
if [ $? -ne 0 ]; then
    echo "Build failed! Exiting."
    exit 1
fi
cd ..

# 2. Setup paths
OUT_CSV="LRU_csv/lru_results.csv"
TRACE_DIR="LRUTraceGenerator_output"
EXE="build/HashTablesOpenAddressing"

# Create output directory if it doesn't exist
mkdir -p LRU_csv

# 3. Initialize CSV with Header
# We run the smallest trace just to extract the header line
echo "Initializing $OUT_CSV..."
if [ -f "$TRACE_DIR/lru_profile_N_1024_S_23.trace" ]; then
    $EXE "$TRACE_DIR/lru_profile_N_1024_S_23.trace" | grep "impl,profile" > "$OUT_CSV"
else
    echo "Error: Base trace file (N=1024) not found. Did you run the generator?"
    exit 1
fi

# 4. Run Harness on all sizes in order, This wil take a few minutes.
SIZES=(1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576)

for N in "${SIZES[@]}"; do
    TRACE_FILE="$TRACE_DIR/lru_profile_N_${N}_S_23.trace"
    
    if [ -f "$TRACE_FILE" ]; then
        echo "Running harness for N=$N (Double)..."
        $EXE "$TRACE_FILE" double | grep "hash_map_double," >> "$OUT_CSV"
        
        echo "Running harness for N=$N (Single)..."
        $EXE "$TRACE_FILE" single | grep "hash_map_single," >> "$OUT_CSV"
    else
        echo "Warning: Trace file for N=$N not found at $TRACE_FILE"
    fi
done

echo "---------------------------------------------------"
echo "Data collection complete!"
echo "Results saved to: $OUT_CSV"
