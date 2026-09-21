// Assignment 03: isSorted and Sorting Study
// Programming Task: isSorted and Sorting Study
// Build a small C++17 sorting test bench based on the sorting work used in prior CS 210 assignments.

// Required implementation
// Implement bool isSorted(const std::vector<int>& values).
// Implement bubble sort, selection sort, insertion sort, and quicksort from scratch.
// Verify each algorithm with isSorted after sorting.
// Benchmark all four algorithms on random, already sorted, and reverse-sorted input.
// Use at least three input sizes. Choose sizes large enough to show a meaningful trend without causing unreasonable run time.
// Report a small timing table and explain the observed best, average, and worst behavior.
// Restriction: do not use std::sort to perform the required sorts. You may use it only in a separate verification step if you clearly label that use.


// Nicolas Holmgren
// Sorting Algorithm using bubble sort, selection sort, insertion sort, and quicksort


// Bubble sort 


#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

// Targeted imports instead of a global "using namespace std;"
using std::vector;

// Forward declaration so quickSortHelper can be defined before partition
int partition(vector<int>& arr, int low, int high);


bool isSorted(const vector<int>& arr) {

    // we check if the array is sorted if it is sorted then we return true
    // We will do this by looping through the array using the current index and the next index to check
    // so we check if the value in index 0 is greater than that of index 1, if yes then we continue to loop
    //


    for (std::size_t i = 0; i + 1 < arr.size(); i++) {

        // Now we check if the current index we are on is less than the index next to it

        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }

    // Only after every neighbouring pair has been checked do we know it is sorted
    return true;
}



// *************************************************
// Bubble sort
// Bubble sort compares the current index to the next, if the current is larger then next,
// then the value of the two indicies are swapped, we then check again if the swapped position is less
// 
// 

void bubbleSort( vector<int>& arr) {

// Time complexity is going to O(n^2) due to the nested for loop
    for (int i = 0; i < arr.size(); ++i) {

        for (int j = 0; j < arr.size() - 1; ++j) {

            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

            }
        }
    }

}

// **********************************************

// Selection Sort -this sort is also O(n^2)
void selectionSort(vector<int>& arr) {
    
    for (std::size_t i = 0; i + 1 < arr.size(); ++i) {

        std::size_t minIndex = i;

        for (std::size_t j = i + 1; j < arr.size(); ++j) {


            if (arr[j] < arr[minIndex]) {

                minIndex = j;
            }
        }

        if (minIndex != i) {
            int temp = arr[i];

            arr[i] = arr[minIndex];
            arr[minIndex] = temp;

        }
    }
}



// **************************************

// Insertion Sort

void insertionSort(vector<int>& arr) {

    for (int i = 0; i < arr.size(); ++i) {

        int key = arr[i];
        
        int j = i - 1;

        while(j >= 0 && arr[j] > key) {

            arr[j + 1] = arr[j];
            j = j - 1;
        }

        arr[j + 1] = key;
    }
}


// ****************************************************

// Quick sort

// 1. THE WORKER (Define this first)
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Pick the last element as the pivot
    int i = (low - 1);     // The boundary for numbers smaller than the pivot

    for (int j = low; j < high; ++j) {
        // If the current number is smaller than the pivot
        if (arr[j] < pivot) {
            i++; // Expand the small-number boundary
            std::swap(arr[i], arr[j]); // Throw the smaller number into that boundary
        }
    }

    // Finally, put the pivot exactly at the boundary line
    std::swap(arr[i + 1], arr[high]);
    
    // Return the pivot's final, sorted position
    return (i + 1);
}

// 2. THE MANAGER (Uses the worker)
void quickSortHelper(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high); // Call the worker!

        quickSortHelper(arr, low, pivotIndex - 1);  // Sort the left side
        quickSortHelper(arr, pivotIndex + 1, high); // Sort the right side
    }
}

// 3. THE WRAPPER (What main() actually calls)
void quickSort(vector<int>& arr) {
    if (arr.empty()) return; 
    quickSortHelper(arr, 0, static_cast<int>(arr.size()) - 1);
}


// ****************************************************************
// Data Generation and Benchmarking Orchestrator
// ****************************************************************

// Any sorting routine with the shape void f(std::vector<int>&) can be benchmarked
using SortFunction = std::function<void(vector<int>&)>;

// Number of timed runs averaged for each (algorithm, input) pair
const int NUM_RUNS = 5;

// ---------- 1. Data generation helpers ----------

// Random values in [0, 1,000,000]. Fixed seed so every run of the program
// (and every report you write from it) uses the same "random" data.
vector<int> generateRandom(int size) {
    std::mt19937 rng(210);
    std::uniform_int_distribution<int> dist(0, 1000000);

    vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = dist(rng);
    }
    return data;
}

// Ascending: 1, 2, ..., size  (best case for insertion sort / worst case for naive quicksort)
vector<int> generateSorted(int size) {
    vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = i + 1;
    }
    return data;
}

// Descending: size, size-1, ..., 1  (worst case for insertion sort and last-element-pivot quicksort)
vector<int> generateReverseSorted(int size) {
    vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = size - i;
    }
    return data;
}

// ---------- 2. Benchmark runner ----------

// Times sortFn on NUM_RUNS fresh copies of originalData, prints one table row
// (algorithm name + average ms), and returns the average in milliseconds.
double runBenchmark(const SortFunction& sortFn, const std::string& name, const vector<int>& originalData) {
    double totalMs = 0.0;

    for (int run = 0; run < NUM_RUNS; ++run) {

        // Fresh copy BEFORE the timer starts, so runs 2-5 never sort already-sorted data
        // and the copy itself is not part of the measurement.
        vector<int> working = originalData;

        auto start = std::chrono::high_resolution_clock::now();
        sortFn(working);
        auto end = std::chrono::high_resolution_clock::now();

        // Verification happens after the timer has stopped
        if (!isSorted(working)) {
            std::cout << "  ERROR: " << name << " did not produce a sorted array (run "
                      << run + 1 << " of " << NUM_RUNS << ")\n";
        }

        std::chrono::duration<double, std::milli> elapsed = end - start;
        totalMs += elapsed.count();
    }

    double avgMs = totalMs / NUM_RUNS;

    std::cout << "  | " << std::left << std::setw(16) << name << " | "
              << std::right << std::setw(13) << std::fixed << std::setprecision(3) << avgMs << " |\n";

    return avgMs;
}

// ---------- 3. Main orchestrator ----------

int main() {
    const int sizes[] = { 2000, 4000, 8000 };

    struct SortEntry {
        const char* name;
        void (*fn)(vector<int>&);
    };
    const SortEntry algorithms[] = {
        { "Bubble Sort",    bubbleSort },
        { "Selection Sort", selectionSort },
        { "Insertion Sort", insertionSort },
        { "Quick Sort",     quickSort },
    };

    std::cout << "Sorting benchmark: average of " << NUM_RUNS << " runs per cell, times in milliseconds\n";

    for (int size : sizes) {

        // Generate each input type once per size; runBenchmark copies it for every run
        struct InputEntry {
            const char* label;
            vector<int> data;
        };
        const InputEntry inputs[] = {
            { "Random",   generateRandom(size) },
            { "Sorted",   generateSorted(size) },
            { "Reverse",  generateReverseSorted(size) },
        };

        for (const InputEntry& input : inputs) {
            std::cout << "\nN = " << size << "  |  Input: " << input.label << "\n";
            std::cout << "  +------------------+---------------+\n";
            std::cout << "  | " << std::left << std::setw(16) << "Algorithm" << " | "
                      << std::right << std::setw(13) << "Avg time (ms)" << " |\n";
            std::cout << "  +------------------+---------------+\n";

            for (const SortEntry& algo : algorithms) {
                runBenchmark(algo.fn, algo.name, input.data);
            }

            std::cout << "  +------------------+---------------+\n";
        }
    }

    return 0;
}