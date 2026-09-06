// Nicolas Holmgren 09/01/26
// 2sum


#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

// Pre start notes and thoughts:

// // In this problem we have target ie. = 10 and array of ints = [0, 2, 4, 8]  we need to find two numbers in the array that add up
// to equal the target so in this case the target is 10. We need current and we need temp vars,
// we start the search current index is placed in that field, then we place the index next to curr into temp, we add them together to see if they equal target.


// update: so we actually will be using a hash map, the above desrvcibed would be a brute force. A hash map has key value pairs.
// we will get target - current ie. 10 - 4 we then check if any values within the HM will equal 10 - 4 so 6, if not then we add it in to the hash map for later, so target_minus_current_get_value = target - current 



// We use a vector which is a dynamic array (size can grow and shrink as needed), the values will be int

// We create the twoSum function which has a constant vector type int the const prevents the array from being changed
// using the & passes the vector arry nums as a reference instead of making a full copy
// We also declare int target this is the number we are looking for 

vector<int> twoSum(const vector<int>& nums, int target);
vector<int> twoSumBruteForce(const vector<int>& nums, int target);

int main() {
    vector<int> nums = {15, 4, 18, 8, 19, 22, 24, 59, 59, 20, 18, 12, 36, 42, 9};

    // Test 1: Hash Map
    vector<int> results1 = twoSum(nums, 24);
    if (results1.size() == 2) {
        cout << "Hash Map found indices: [" << results1[0] << ", " << results1[1] << "]" << endl;  
    }

    // Test 2: Brute Force
    vector<int> results2 = twoSumBruteForce(nums, 24);
    if (results2.size() == 2) {
        cout << "Brute Force found indices: [" << results2[0] << ", " << results2[1] << "]" << endl;  
    }

    return 0;
};

// Hash map approach for 2sum, the time complexity is O(n) as we are only looping through the array once,
// This is more efficent compared to the brute force approach which has a time complexity of O(n^2).

vector<int> twoSum(const vector<int>& nums, int target) {

    // We use the unordered_map, which is our hashmap, we pass in two ints 
    unordered_map<int, int> index;

    // we loop through and search for our needed value that will give us target, target - the value of the index we are on in nums vector

    for(int i = 0; i < nums.size(); ++i) {
        int needed = target - nums[i];


        // we then check if needed is already in our hashmap, if it is we return index of needed
        if (index.count(needed)) {
            return {index[needed], i};
        }

        // in the case we didn't find the index where the correct value is stored we save that index spot for future reference

        index[nums[i]] = i;

    }
    
    return {};
};



    // Brute force 2sums


// Here we use a vector, we decleare it as a const so it doesnt get changed accidently
// We pass by reference to not make a copy of the entire vecotr

// the time complexity for brute force is O(n^2) as we are using nested for loops

vector<int> twoSumBruteForce(const vector<int>& nums, int target) {

// Loop through the whole length of the vector
    for (int i=0; i < nums.size(); ++i) {


        for (int j = i + 1; j < nums.size(); ++j) {

            if (nums[i] + nums[j] == target) {

                return {i, j};
            }
        }
    }
    return {};
};


