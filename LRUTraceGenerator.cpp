#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

// Function Prototype
void generateTrace(int N, int seed, const std::vector<std::string> &allWords);

int main() {
  int seed = 23;
  int maxN = 1048576;
  int numWordsNeeded = 4 * maxN;

  std::vector<std::string> allWords;
  std::ifstream in;
  in.open("../20980712_uniq_words.txt", std::ios::in);
  if (!in.is_open()) {
    std::cout << "Unable to open 20980712_uniq_words.txt Terminating...\n";
    return 2;
  }

  std::cout << "Reading " << numWordsNeeded << " words..." << std::endl;
  std::string line;
  while (allWords.size() < numWordsNeeded && std::getline(in, line)) {
    allWords.push_back(line);
  }
  std::cout << "Finished reading words." << std::endl;

  // Loop from N=1024 up to 1048576 did this to avoid compile parameters
  // simplicity sake
  for (int N = 1024; N <= maxN; N *= 2) {
    generateTrace(N, seed, allWords);
  }

  return 0;
}

// trace gen helper
void generateTrace(int N, int seed, const std::vector<std::string> &allWords) {
  std::vector<std::string> bag;
  std::unordered_map<std::string, std::list<std::string>::iterator>
      resident_map;
  std::list<std::string> LRU_list;

  // Add the words to the bag doing the 1N 1N5 2N3N
  // First N words 1 of each
  for (int i = 0; i < N; i++) {
    bag.push_back(allWords[i]);
  }

  // Next add the second set of N words 5x each
  for (int i = N; i < 2 * N; i++) {
    for (int j = 0; j < 5; j++) {
      bag.push_back(allWords[i]);
    }
  }

  // Next add the third set of N words 3x each
  for (int i = 2 * N; i < 4 * N; i++) {
    for (int j = 0; j < 3; j++) {
      bag.push_back(allWords[i]);
    }
  }

  // Shuffle the bag with given seed
  std::mt19937 rng(seed);
  std::shuffle(bag.begin(), bag.end(), rng);

  // Output trace file
  // follows lru_profile_N_1024_S_23.trace format
  std::string outputFileName = "../LRUTraceGenerator_output/lru_profile_N_" +
                               std::to_string(N) + "_S_" +
                               std::to_string(seed) + ".trace";
  std::ofstream out(outputFileName);
  if (!out.is_open()) {
    std::cout << "Failed to open output file " << outputFileName << '\n';
    return;
  }

  out << "lru_profile " << N << " " << seed << '\n';

  // Loop through the bag
  for (const auto &w : bag) {
    auto iterator = resident_map.find(w);

    // If no word is found it will return the .end(), thus this statement checks
    // if there is a hit
    if (iterator != resident_map.end()) {
      LRU_list.erase(
          iterator->second);  // remove the word from where its at in the list
      LRU_list.push_front(w); // Put it to the front
      resident_map[w] = LRU_list.begin();
      out << "I " << w << '\n';
    } else {
      // Miss, but space available.
      if (resident_map.size() < (size_t)N) {
        LRU_list.push_front(w);
        resident_map[w] = LRU_list.begin();
        out << "I " << w << '\n';
      }
      // Miss and no space available (evict LRU)
      else {
        // Delete LRU
        std::string rip = LRU_list.back();
        LRU_list.pop_back();
        resident_map.erase(rip);
        out << "E " << rip << '\n';

        // Insert new
        LRU_list.push_front(w);
        resident_map[w] = LRU_list.begin();
        out << "I " << w << '\n';
      }
    }
  }
  std::cout << "Generated " << outputFileName << std::endl;
}
