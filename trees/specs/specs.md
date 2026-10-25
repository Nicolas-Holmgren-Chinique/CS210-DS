BINARY SEARCH TREE (BST) SPEC
Author: Nicolas Holmgren


OVERVIEW
- We are building a binary search tree of ints in C++17, from scratch.
- Everything lives in one file, binary_tree.cpp, which contains the tree, its
  acceptance tests, a demo program that shows every required operation, and
  the complexity explanation.


LIMITATIONS AND CONSTRAINTS
- C++17 only. Everything lives in a single file, binary_tree.cpp. Do not
  create headers or any additional files.
- Code must compile with no warnings using:
    g++ -std=c++17 -Wall -Wextra -pedantic binary_tree.cpp -o binary_tree
- No library that provides tree, search, insertion, deletion, traversal, or
  height functionality may be used. This includes std::set, std::map,
  std::multiset, std::multimap, and any other tree container.
- Allowed headers: <iostream>, <vector>, <string>, <cassert>.
  <algorithm> is NOT used (no std::sort, std::is_sorted, std::find, std::max).
  Any other header must be approved by Nicolas first.
- Nodes are managed with raw pointers (new/delete). Every node created must be
  deleted exactly once.
- Only implement what the current roadmap step asks for. Do not change the
  public interface without asking Nicolas.


TREE TYPE
- This is a Binary Search Tree (BST) of ints. In a BST:
  - All values in the left subtree are less than the node's value.
  - All values in the right subtree are greater than the node's value.
  - Duplicates are not allowed: inserting a value that already exists leaves
    the tree unchanged and insert returns false.


FILE LAYOUT (binary_tree.cpp, top to bottom)
1. Complexity comment block (see COMPLEXITY section)
2. #include lines
3. struct TreeNode
4. class BinarySearchTree declaration, followed by its member function
   definitions in the same order as the PUBLIC INTERFACE
5. Acceptance test functions, one per section: testTreeNode(),
   testConstructorDestructor(), testInsert(), testSearch(), testTraversals(),
   testHeight(), testDeleteValue(). Each uses assert.
   runAllTests() calls every test function that exists so far, then prints
   "All acceptance tests passed".
6. Demo code: isStrictlyIncreasing() helper and runDemo()
7. main(): calls runAllTests(), then runDemo(). Until the demo exists
   (roadmap step 7), main calls only runAllTests().


PUBLIC INTERFACE
```cpp
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    explicit TreeNode(int value);   // data = value, left = right = nullptr
};

class BinarySearchTree {
public:
    BinarySearchTree();             // empty tree: root = nullptr
    ~BinarySearchTree();            // frees every node
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    bool insert(int value);         // false if duplicate
    bool search(int value) const;
    bool deleteValue(int value);    // false if not found
    std::vector<int> inorder() const;
    std::vector<int> preorder() const;
    std::vector<int> postorder() const;
    int height() const;

private:
    TreeNode* root;
    // private recursive helpers, described in each section below
};
```


MISC AND CLEARING UP AMBIGUITIES
- height() counts edges: an empty tree returns -1, a single node returns 0.
- Traversals return std::vector<int>. They never print; printing happens in
  runDemo().
- Empty tree behavior: search returns false, deleteValue returns false,
  traversals return an empty vector, height returns -1.
- Two-children deletion uses the inorder successor (smallest value in the
  right subtree).
- Copying a tree is disabled (copy constructor and copy assignment are deleted).
- Recursive work is done in private helpers. Public functions never take or
  return TreeNode*, and root is never exposed.


TREENODE
- Holds int data and two pointers to TreeNode, left and right.
- Constructor TreeNode(int value) sets data to value and both pointers to nullptr.
- Contains no tree logic.

Acceptance tests (testTreeNode):
1. TreeNode n(5): n.data == 5, n.left == nullptr, n.right == nullptr.


CONSTRUCTOR AND DESTRUCTOR
- Constructor sets root to nullptr (empty tree).
- Destructor frees every node using a private postorder helper:
    void destroy(TreeNode* node);
  It deletes the left subtree, then the right subtree, then the node itself.
  Base case: if node is nullptr, return.

Acceptance tests (testConstructorDestructor):
1. A new tree is empty (inorder returns {} and height returns -1 once those exist).
2. A tree containing nodes is destroyed with no crash and no leaks
   (verify by adding -fsanitize=address to the build command if available).


INSERT
- bool insert(int value)
- If the tree is empty, the new node becomes the root; return true.
- Otherwise start at the root. If value is less than the node's value, move
  left; if greater, move right. Repeat until reaching an empty (nullptr) spot,
  create the new node there, and return true.
- If value equals any node's value along the way, return false and leave the
  tree unchanged.
- Implemented iteratively (a loop walking down from the root).

Acceptance tests (testInsert):
1. insert(50) on an empty tree returns true; inorder = {50}.
2. Demo tree, insert 50 30 70 20 40 60 80 10 45 65: every call returns true;
   inorder = 10 20 30 40 45 50 60 65 70 80.
3. insert(40) again returns false; inorder is unchanged (still 10 values).
4. Degenerate tree, insert 10 20 30 40 50: inorder = 10 20 30 40 50.


SEARCH
- bool search(int value) const
- Starts at the root. If the value equals the current node's value, return true.
  If the value is less, move to the left child; if greater, move to the right
  child. Repeat.
- If the search reaches an empty spot (nullptr) without finding the value,
  return false. Searching an empty tree returns false.
- Does not modify the tree. Implemented iteratively.

Acceptance tests (testSearch, on the demo tree unless noted):
1. Empty tree: search(10) returns false.
2. search(45) returns true (path: 50 -> 30 -> 40 -> 45).
3. search(50) returns true (the root).
4. search(10) returns true (deepest left leaf).
5. search(55) returns false (absent, would sit between existing values).
6. search(5) returns false (smaller than every value).
7. search(100) returns false (larger than every value).


TRAVERSALS (inorder, preorder, postorder)
- Return every value in the tree in a defined order. Traversals do NOT search
  for a value and take no target.
- Inorder is how we prove the tree stays sorted. Preorder is how we prove the
  tree has the correct shape.
- Behavior:
  - inorder:   left subtree, then the node, then right subtree.
  - preorder:  the node, then left subtree, then right subtree.
  - postorder: left subtree, then right subtree, then the node.
  - Every node is visited exactly once; the returned vector's size equals the
    number of nodes in the tree.
  - An empty tree returns an empty vector.
- Each public traversal calls a private recursive helper, starting at root:
    void inorder(TreeNode* node, std::vector<int>& out) const;
    (same pattern for preorder and postorder)
  - Base case: if node is nullptr, return immediately.
  - out is passed by reference so every recursive call appends to the same vector.
- Do NOT sort the output. The order must come from the tree structure alone;
  sorting would hide insert/delete bugs.
- Recursive implementation only; iterative versions are out of scope.

Acceptance tests (testTraversals):
1. Empty tree: all three traversals return {}.
2. Single node (insert 50): all three return {50}.
3. Demo tree, insert 50 30 70 20 40 60 80 10 45 65:
     inorder   = 10 20 30 40 45 50 60 65 70 80
     preorder  = 50 30 20 10 40 45 70 60 65 80
     postorder = 10 20 45 40 30 65 60 80 70 50
4. Degenerate tree, insert 10 20 30 40 50:
     inorder   = 10 20 30 40 50
     preorder  = 10 20 30 40 50
     postorder = 50 40 30 20 10
5. Calling any traversal twice returns identical results, and preorder before
   and after the calls matches (the tree is unchanged).


HEIGHT
- int height() const
- Calls a private recursive helper: int height(TreeNode* node) const;
  - If node is nullptr, return -1.
  - Otherwise return 1 + the larger of height(left) and height(right).
    (Compare the two heights by hand; <algorithm> is not used.)

Acceptance tests (testHeight):
1. Empty tree: height = -1.
2. Single node: height = 0.
3. Demo tree: height = 3.
4. Degenerate tree (10 20 30 40 50): height = 4.


DELETEVALUE
- bool deleteValue(int value)
- If the value is not in the tree (including an empty tree), return false and
  leave the tree unchanged.
- If found, remove the node, free its memory, and return true. Three cases:
  - Leaf (no children): the parent's pointer to it becomes nullptr.
  - One child: the parent's pointer now points to that child.
  - Two children: find the inorder successor (go right once, then keep going
    left until the left pointer is nullptr). Copy the successor's value into
    this node, then delete the successor from the right subtree. The successor
    never has a left child, so its removal is always the leaf or one-child case.
  - Deleting the root follows the same rules, and root is updated.
- Implemented with a private recursive helper that returns the new root of the
  subtree:
    TreeNode* deleteValue(TreeNode* node, int value, bool& removed);
  The public function does: root = deleteValue(root, value, removed); return removed;

Acceptance tests (testDeleteValue):
1. Empty tree: deleteValue(10) returns false.
2. Single node (insert 50): deleteValue(50) returns true; inorder = {}; height = -1.
3. Demo tree: deleteValue(55) returns false; inorder is unchanged.
4. Demo tree, in this order:
     deleteValue(10)  leaf:          returns true
       inorder  = 20 30 40 45 50 60 65 70 80
       preorder = 50 30 20 40 45 70 60 65 80
       height   = 3
     deleteValue(40)  one child:     returns true
       inorder  = 20 30 45 50 60 65 70 80
       preorder = 50 30 20 45 70 60 65 80
       height   = 3
     deleteValue(50)  two children:  returns true
       inorder  = 20 30 45 60 65 70 80
       preorder = 60 30 20 45 70 65 80
       height   = 2
5. Deleting every remaining value one at a time: each call returns true, and
   the tree ends empty (inorder = {}, height = -1) with no leaks.
6. Sorted invariant: after every successful deletion, inorder is strictly increasing.
7. One child on the LEFT: fresh demo tree, deleteValue(20) (only child is 10)
   returns true; 10 moves up into 20's place:
     inorder  = 10 30 40 45 50 60 65 70 80
     preorder = 50 30 10 40 45 70 60 65 80
     height   = 3


DEMO PROGRAM (runDemo in binary_tree.cpp)
- Uses only the public interface of BinarySearchTree.
- Includes a hand-written helper, bool isStrictlyIncreasing(const std::vector<int>&),
  used to print "sorted: yes/no". Do not use std::is_sorted.
- Steps, in order:
  1. Insert 50 30 70 20 40 60 80 10 45 65. Print inorder, preorder, postorder, height.
  2. Search for 45 (present) and 55 (absent). Print the results.
  3. Delete 10 (leaf). Print inorder and the sorted check.
  4. Delete 40 (one child). Print inorder and the sorted check.
  5. Delete 50 (two children). Print inorder and the sorted check.
- Expected full program output (spacing may differ; values and order must match):
    All acceptance tests passed
    Inserted:  50 30 70 20 40 60 80 10 45 65
    Inorder:   10 20 30 40 45 50 60 65 70 80
    Preorder:  50 30 20 10 40 45 70 60 65 80
    Postorder: 10 20 45 40 30 65 60 80 70 50
    Height:    3
    Search 45: found
    Search 55: not found
    Delete 10 (leaf):         20 30 40 45 50 60 65 70 80   sorted: yes
    Delete 40 (one child):    20 30 45 50 60 65 70 80      sorted: yes
    Delete 50 (two children): 20 30 45 60 65 70 80         sorted: yes


COMPLEXITY (comment block at the top of binary_tree.cpp)
- The comment block must explain, in plain language:
  - search, insert, deleteValue: average O(log n), worst case O(n). Each one
    walks a single path from the root, so its cost is O(h), where h is the
    height. When the tree is reasonably balanced (e.g. random insertion
    order), h is about log2(n). When it degenerates into a chain (e.g. sorted
    insertion order like 10 20 30 40 50), h = n - 1.
  - Traversals and height: O(n) always, since every node is visited once.
  - Extra space for recursion: O(h).


SUCCESS CRITERIA (definition of done for the whole project)
- binary_tree.cpp compiles with the build command and no warnings.
- Running the program passes every acceptance test (prints
  "All acceptance tests passed") and then prints the demo output exactly as
  described in the DEMO PROGRAM section.
- No memory leaks (verify by adding -fsanitize=address to the build command
  if available).
- The complexity comment block covers the COMPLEXITY section.
- Assignment requirement checklist:
  1. Node type and BinarySearchTree class   -> TREENODE, PUBLIC INTERFACE
  2. All operations                         -> INSERT, SEARCH, DELETEVALUE,
                                               TRAVERSALS, HEIGHT
  3. At least 10 distinct integers          -> DEMO step 1
  4. Present and absent search              -> DEMO step 2
  5. All three removal cases                -> DEMO steps 3-5
  6. Inorder sorted after each removal      -> DEMO steps 3-5
  7. Average and worst-case complexity      -> complexity comment block


ROADMAP
Review gate (applies after EVERY step): stop development and hand it over to
me (Nicolas, the engineer) with the acceptance test results. Do not start the
next step until I approve. If not approved, we work through any problems and
make changes where needed.

STEP 1: Create binary_tree.cpp with TreeNode, the BinarySearchTree skeleton
        (constructor, destructor with destroy helper, deleted copy
        operations), runAllTests(), and main(). Tests: TREENODE.
STEP 2: insert and inorder, built together because inorder is how insert is
        verified. Tests: INSERT 1-4, TRAVERSALS 1-5 for inorder only.
STEP 3: preorder and postorder. Tests: TRAVERSALS 1-5 for both.
STEP 4: search. Tests: SEARCH 1-7.
STEP 5: height. Tests: HEIGHT 1-4, CONSTRUCTOR AND DESTRUCTOR 1-2.
STEP 6: deleteValue. Tests: DELETEVALUE 1-6.
STEP 7: runDemo(); main now calls runAllTests() then runDemo().
        Output must match DEMO PROGRAM.
STEP 8: Write understanable comments that are both meant for human engineers and AI coding agents, to help understand what code block does and the purpose.