// Nicolas Holmgren
// 10/15/2026


// A hashtable is a data structure which stores a key and a value


// we get the key and then we pass it through a hashfunction,

// We use modulo (%) to divide by the array size and then we get the remainder which comes from
// the % and with that remainder it determines the index that the key value pair will be stored

// because their can be collisions we will use a linked list in each of the buckets (indices)
// If an already occupied bucket(index) requires another key value pair to be placed then we send that
// key value pair to the linked list that is pointing to at the that array index


#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
using namespace std; 


// A hashtable is an array which stores key values pairs

// we get a key and a value from the user, we then hash the key by passsing the key through a hashfunction
// If the key is an int then we can simply key % array.size() to then get the remainder
// which the remainder will be the index where we will place the key and value


uint64_t demoHash(const std::string& s) {
    uint64_t h = 1469598103934665603ULL;
    for (unsigned char c : s) {
        h = (h ^ c) * 1099511628211ULL;
    }
    return h;
}

// create a struct for defining the key and value, both of which are strings
struct Account {
    // the key will hold the username, which is a string
    string userName;
    // the value is the salt the is added to the password and will also be a string
    string salt;

    uint64_t passwordHash;

    Account(string k, string v, uint64_t pashas) {
        userName = k;
        salt = v;
        passwordHash = pashas;

    } 
};


// hashtable class
class HashTable {

    private:
    // create the hash table using a vecotr and populating with account
        vector<vector<Account>> table;
        size_t numElements;


        size_t getIndex(string username) {
            return demoHash(username) % table.size();
        }

    public:

        HashTable(size_t initialSize = 10) {
            table.resize(initialSize);
            numElements = 0;
        }


        void insert(string rawUsername, string rawPassword) {


            string saltTest = "38fhHjj29Al931jNmndw";
            string pepperTest = "pe41janfe9249tuKhdhkja355";


            string combinedString = saltTest + rawPassword + pepperTest;


            uint64_t secureHash = demoHash(combinedString);


            size_t index = getIndex(rawUsername);

            Account newBox(rawUsername, saltTest, secureHash);
            table[index].push_back(newBox);

            numElements++;
            cout << "Successfully inserted user: " << rawUsername << endl;

            if (loadFactor() > 0.75) {
                rehash();
            }

        }

        bool login(string enteredUsername, string enteredPassword) {

            size_t index = getIndex(enteredUsername);
            
            for (size_t i = 0; i < table[index].size(); i++) {
                if (table[index][i].userName == enteredUsername) {

                    string myPepper = "pe41janfe9249tuKhdhkja355";
                    string combinedString = table[index][i].salt + enteredPassword + myPepper;

                    // + mySuperSeCreEtpasSSwoRD + pe41janfe9249tuKhdhkja355
                    uint64_t calculateHash = demoHash(combinedString);

                    if (calculateHash == table[index][i].passwordHash) {
                        cout << "Login successful for " << enteredUsername << "!" << endl;
                        return true;
                    } else {
                        cout << "Login failed: incorrect password for " << enteredUsername << "!" << endl;
                        return false;
                    }
                }
            }

            cout << "Login failed: User " << enteredUsername << " does not exist." << endl;
            return false; 

        }


        bool remove(string username) {
            size_t index = getIndex(username);

            for (size_t i = 0; i < table[index].size(); i++) {
                if (table[index][i].userName == username) {
                    table[index].erase(table[index].begin() + i);
                    numElements--;

                    cout << "User " << username << " removed successfully." << endl;
                    return true;
                }
            }

            cout << "Removal failed: User " << username << " not found." << endl;
            return false;
        }


        double loadFactor() {
            return static_cast<double>(numElements) / table.size();
        }


        void rehash() {

            cout << "Rehashing" << endl;

            size_t newSize = table.size() * 2;
            vector<vector<Account>> newTable;
            newTable.resize(newSize);


            for (size_t i = 0; i < table.size(); i++) {
                for (size_t j = 0; j < table[i].size(); j++) {
                    Account oldBox = table[i][j];

                    size_t newIndex = demoHash(oldBox.userName) % newSize;

                    newTable[newIndex].push_back(oldBox);
                }
            }

            table = newTable;

            cout << "rehash complete." << endl;
        }
};


class LinearProbingTable {
    vector<Account*> table; 
public:
    LinearProbingTable(size_t size = 10) { table.assign(size, nullptr); }
    void insert(string username, string salt, uint64_t hash) {
        size_t index = demoHash(username) % table.size();
        size_t originalIndex = index;
        while (table[index] != nullptr) {
            index = (index + 1) % table.size();
            if (index == originalIndex) return; 
        }
        table[index] = new Account(username, salt, hash);
    }
};

// 2. QUADRATIC PROBING
class QuadraticProbingTable {
    vector<Account*> table;
public:
    QuadraticProbingTable(size_t size = 10) { table.assign(size, nullptr); }
    void insert(string username, string salt, uint64_t hash) {
        size_t index = demoHash(username) % table.size();
        size_t originalIndex = index;
        size_t i = 1;
        while (table[index] != nullptr) {
            index = (originalIndex + (i * i)) % table.size();
            i++;
            if (i > table.size()) return; 
        }
        table[index] = new Account(username, salt, hash);
    }
};

// 3. DOUBLE HASHING
class DoubleHashingTable {
    vector<Account*> table;
    size_t hash2(string key) {
        size_t step = 0;
        for (char c : key) step += c;
        return (step % 7) + 1; 
    }
public:
    DoubleHashingTable(size_t size = 10) { table.assign(size, nullptr); }
    void insert(string username, string salt, uint64_t hash) {
        size_t index = demoHash(username) % table.size();
        size_t stepSize = hash2(username);
        size_t originalIndex = index;
        while (table[index] != nullptr) {
            index = (index + stepSize) % table.size();
            if (index == originalIndex) return; 
        }
        table[index] = new Account(username, salt, hash);
    }
};


int main() {
    HashTable myTable(5);
    cout << "Building constructed successfully." << endl;


    myTable.insert("nholmgren", "mySuperSeCreEtpasSSwoRD");
    myTable.insert("jDoe", "password123");
    myTable.insert("jimmySim", "j1QJ34f2hH!HFHi838");

    cout << "------ Testing logins------" << endl;

    myTable.login("nholmgren", "mySuperSeCreEtpasSSwoRD");
    myTable.login("nholmgren", "lolWorngPassjhfigoi");


    myTable.login("ghostUser", "password123");


    // By the 4th user, Load Factor is 4/5 = 0.8. This triggers the rehash!
    myTable.insert("sarahConnor", "skynetSucks2026"); 
    myTable.insert("bruceWayne", "imBatman99");

    cout << "\n------ Testing logins ------" << endl;
    myTable.login("nholmgren", "mySuperSeCreEtpasSSwoRD"); // Success
    myTable.login("nholmgren", "lolWorngPassjhfigoi");     // Fail (Wrong pass)
    myTable.login("ghostUser", "password123");             // Fail (Unknown user)

    cout << "\n------ Testing Removal ------" << endl;
    myTable.remove("jDoe"); // Remove an existing user
    myTable.remove("jDoe"); // Try to remove them again (should fail)
    
    // Prove they are gone by trying to log them in
    myTable.login("jDoe", "password123");
    
    return 0;
}
    