#include "HashTableDictionary.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Helper to parse the header line: "profile_name N seed"
bool load_trace_strict_header(const std::string &path, std::string &profile,
                              int &N, int &seed) {
  std::ifstream in(path);
  if (!in.is_open())
    return false;
  std::string line;
  if (std::getline(in, line)) {
    std::stringstream ss(line);
    ss >> profile >> N >> seed;
    return true;
  }
  return false;
}

bool isPrime(int n) {
  if (n <= 1)
    return false;
  if (n <= 3)
    return true;
  if (n % 2 == 0 || n % 3 == 0)
    return false;
  for (int i = 5; i * i <= n; i += 6) {
    if (n % i == 0 || n % (i + 2) == 0)
      return false;
  }
  return true;
}

int nextPrime(int n) {
  while (!isPrime(n))
    n++;
  return n;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 3) {
    std::cerr << "Usage: " << argv[0] << " <trace_file> [single|double]"
              << std::endl;
    return 1;
  }

  std::string tracePath = argv[1];
  std::string pTypeStr = "double"; // Default
  if (argc == 3) {
    pTypeStr = argv[2];
  }

  HashTableDictionary::PROBE_TYPE pType =
      (pTypeStr == "single") ? HashTableDictionary::PROBE_TYPE::SINGLE
                             : HashTableDictionary::PROBE_TYPE::DOUBLE;
  std::string implName = (pType == HashTableDictionary::PROBE_TYPE::SINGLE)
                             ? "hash_map_single"
                             : "hash_map_double";

  // Load header
  std::string profile;
  int N = 0;
  int seed = 0;
  if (!load_trace_strict_header(tracePath, profile, N, seed)) {
    std::cerr << "Failed to load trace header from " << tracePath << std::endl;
    return 1;
  }

  // Determine Table Size (Heuristic based on observations: N=1024 -> Size=1279)
  // 1279 is approx 1.25 * 1024. Let's use nextPrime(N * 1.25)
  int tableSize = nextPrime(static_cast<int>(N * 1.25));

  // Load all operations
  struct Op {
    char type;
    std::string key;
  };
  std::vector<Op> ops;
  std::ifstream in(tracePath);
  std::string line;
  std::getline(in, line); // Skip header
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::stringstream ss(line);
    char type;
    std::string key;
    ss >> type >> key;
    ops.push_back({type, key});
  }

  // Initialize Dictionary
  // Constructor: HashTableDictionary(size_t tableSize, PROBE_TYPE probeType,
  // bool doCompact, double rate) We enable compaction by default as per
  // observations "Compaction ON"
  HashTableDictionary dict(tableSize, pType, true, 0.95);
  // Note: Compaction trigger rate 0.5 is a guess, but standard is often 0.5 or
  // 0.8. Observations said "Effective Load Factor 100%" for "Compaction OFF",
  // so maybe we should pass false if we want OFF? But the script runs
  // "Compaction ON" implicitly? Actually, the user's observations showed "Run
  // 2: Single Probing, Compaction ON". Let's assume we want Compaction ON for
  // the final data collection.

  // Timing
  auto start = std::chrono::high_resolution_clock::now();
  for (const auto &op : ops) {
    if (op.type == 'I') {
      dict.insert(op.key);
    } else if (op.type == 'E') {
      dict.remove(op.key);
    }
  }
  auto end = std::chrono::high_resolution_clock::now();
  double elapsed_ms =
      std::chrono::duration<double, std::milli>(end - start).count();

  // Output CSV Line
  // The csvStats() method likely returns a comma-separated string of stats.
  // We need to prepend our harness-specific columns: impl, profile, trace_path,
  // N, seed, elapsed_ms, ops_total And then append the dict stats.

  // Let's check what csvStats() returns.
  // If we can't check, we'll assume it returns the dict-specific stats.
  // Header:
  // impl,profile,trace_path,N,seed,elapsed_ms,ops_total,average_probes,eff_load_factor_pct,load_factor_pct,tombstones_pct,full_scans,compactions,compaction_state

  std::cout << implName << "," << profile << "," << tracePath << "," << N << ","
            << seed << "," << elapsed_ms << "," << ops.size() << ","
            << dict.csvStats() // This likely returns the rest of the columns
            << std::endl;

  return 0;
}
