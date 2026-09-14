// Required implementation
// Implement an iterative binary search.
// Implement a recursive binary search.
// Instrument both versions to count element comparisons.
// Test at least five searches, including: first element, last element, middle element, missing value below the range, and missing value inside the range.
// For the recursive version, write and explain the recurrence T(n) = T(n/2) + O(1) and connect it to O(log n).
// Compare binary search to a linear search on the same data.
// Restriction: do not call std::binary_search, lower_bound, or another library search routine to perform the required search.

// Nicolas Holmgren
// 9/13/2026
// Binary Search Implementation
// Data Structures CS 210

#include <iostream>
#include <vector>
using namespace std;


// Iterative binary search function

int iterativeBinarySearch(const vector<int>& arr, int target, int& comparisons) {
    comparisons = 0; // Initialize the comparison count

    // set low index to 0 which is the lowest index
    int low = 0;

    // for the high index, we need to set it to the last index of the array, which is arr.size() - 1
    int high = arr.size() - 1;

    while (low <= high) {

        int mid = low + (high - low) /2;

        comparisons++;

        if (arr[mid] == target) {
            return mid;
        }
        else if (arr[mid] < target) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }
    return -1; // Target not found
}


// Recursive binary search function
int recursiveBinarySearch(const vector<int>& arr, int target, int low, int high, int& comparisons) {

    if (low > high) {
        return -1; // target not found
    }


    int mid = low + (high - low) / 2;
    comparisons++; 

    if (arr[mid] == target) {
        return mid;
    }
// call the recursive function again with updated low and high values based on the comparison
    else if (arr[mid] < target) {
        return recursiveBinarySearch(arr, target, mid + 1, high, comparisons);

    }

    // if the target is less than the middle element, search in the left half of the array
    else {
        return recursiveBinarySearch(arr, target, low, mid - 1, comparisons);
    }
}


// Linear search function
int linearSearch(const vector<int>& arr, int target, int& comparisons) {
    comparisons = 0;

    for (int index = 0; index < static_cast<int>(arr.size()); index++) {
        comparisons++;

        if (arr[index] == target) {
            return index;
        }
    }

    return -1;
}


int main() {
    vector<vector<int>> arrays = {
        {1, 3, 5, 7, 9, 11, 13, 15, 17, 19},
        {1000, 2030, 2465, 3000, 4000, 5000, 6000, 7000, 8000, 9000},
        {-10, -5, 0, 5, 10, 15, 20, 25, 30, 35},
        {2, 4, 6, 8, 10, 12, 14, 16, 18, 20}
    };

    for (int arrayNumber = 0; arrayNumber < arrays.size(); arrayNumber++) {
        vector<int>& arr = arrays[arrayNumber];

        vector<int> targets = {
            arr.front(),              // First element
            arr.back(),               // Last element
            arr[arr.size() / 2],      // Middle element
            arr.front() - 1,          // Missing below range
            arr[1] + 1                 // Missing inside the range
        };

        cout << "===== Test Array " << arrayNumber + 1 << " =====" << endl;

        for (int target : targets) {
            int iterativeComparisons = 0;
            int recursiveComparisons = 0;
            int linearComparisons = 0;

            int iterativeResult = iterativeBinarySearch(
                arr,
                target,
                iterativeComparisons
            );

            int recursiveResult = recursiveBinarySearch(
                arr,
                target,
                0,
                static_cast<int>(arr.size()) - 1,
                recursiveComparisons
            );

            int linearResult = linearSearch(
                arr,
                target,
                linearComparisons
            );

            cout << "Target: " << target << endl;

            cout << "  Iterative result: index "
                 << iterativeResult
                 << ", comparisons: "
                 << iterativeComparisons << endl;

            cout << "  Recursive result: index "
                 << recursiveResult
                 << ", comparisons: "
                 << recursiveComparisons << endl;

              cout << "  Linear result: index "
                  << linearResult
                  << ", comparisons: "
                  << linearComparisons << endl;

            cout << endl;
        }
    }

    return 0;
}