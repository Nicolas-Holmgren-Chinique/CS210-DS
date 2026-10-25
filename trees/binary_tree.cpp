// Binary Search Tree
// Using Spec Driven Devleopment (SDD) Refercence specs.md
// Claude code Opus 5.5 High, was used for the writting of code

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// One node of the tree. Holds a value and links to its two children.
// No tree logic lives here.
struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}
};

// Binary search tree of ints. Left subtree < node < right subtree, no duplicates.
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

    void destroy(TreeNode* node);
    TreeNode* deleteValue(TreeNode* node, int value, bool& removed);
    void inorder(TreeNode* node, std::vector<int>& out) const;
    void preorder(TreeNode* node, std::vector<int>& out) const;
    void postorder(TreeNode* node, std::vector<int>& out) const;
    int height(TreeNode* node) const;
};

BinarySearchTree::BinarySearchTree() : root(nullptr) {}

BinarySearchTree::~BinarySearchTree() {
    destroy(root);
}

// Postorder delete: left subtree, right subtree, then the node itself.
void BinarySearchTree::destroy(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

// Walks down from the root until it finds an empty spot for the value.
// Returns false (tree unchanged) if the value is already in the tree.
bool BinarySearchTree::insert(int value) {
    if (root == nullptr) {
        root = new TreeNode(value);
        return true;
    }

    TreeNode* current = root;
    while (true) {
        if (value == current->data) {
            return false;
        }
        if (value < current->data) {
            if (current->left == nullptr) {
                current->left = new TreeNode(value);
                return true;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new TreeNode(value);
                return true;
            }
            current = current->right;
        }
    }
}

// Follows one path down from the root, going left or right by comparison.
// Returns false on reaching an empty spot without finding the value.
bool BinarySearchTree::search(int value) const {
    TreeNode* current = root;
    while (current != nullptr) {
        if (value == current->data) {
            return true;
        }
        if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

bool BinarySearchTree::deleteValue(int value) {
    bool removed = false;
    root = deleteValue(root, value, removed);
    return removed;
}

// Removes value from the subtree rooted at node and returns the subtree's new
// root, so the caller can relink its pointer. Sets removed when a node is freed.
TreeNode* BinarySearchTree::deleteValue(TreeNode* node, int value, bool& removed) {
    if (node == nullptr) {
        return nullptr;
    }
    if (value < node->data) {
        node->left = deleteValue(node->left, value, removed);
        return node;
    }
    if (value > node->data) {
        node->right = deleteValue(node->right, value, removed);
        return node;
    }

    // Leaf: the parent's pointer becomes nullptr.
    if (node->left == nullptr && node->right == nullptr) {
        delete node;
        removed = true;
        return nullptr;
    }

    // One child: the parent's pointer now points to that child.
    if (node->left == nullptr || node->right == nullptr) {
        TreeNode* child = (node->left != nullptr) ? node->left : node->right;
        delete node;
        removed = true;
        return child;
    }

    // Two children: copy in the inorder successor (smallest value in the right
    // subtree), then delete the successor from the right subtree.
    TreeNode* successor = node->right;
    while (successor->left != nullptr) {
        successor = successor->left;
    }
    node->data = successor->data;
    node->right = deleteValue(node->right, successor->data, removed);
    return node;
}

std::vector<int> BinarySearchTree::inorder() const {
    std::vector<int> out;
    inorder(root, out);
    return out;
}

// Left subtree, then the node, then right subtree.
void BinarySearchTree::inorder(TreeNode* node, std::vector<int>& out) const {
    if (node == nullptr) {
        return;
    }
    inorder(node->left, out);
    out.push_back(node->data);
    inorder(node->right, out);
}

std::vector<int> BinarySearchTree::preorder() const {
    std::vector<int> out;
    preorder(root, out);
    return out;
}

// The node, then left subtree, then right subtree.
void BinarySearchTree::preorder(TreeNode* node, std::vector<int>& out) const {
    if (node == nullptr) {
        return;
    }
    out.push_back(node->data);
    preorder(node->left, out);
    preorder(node->right, out);
}

std::vector<int> BinarySearchTree::postorder() const {
    std::vector<int> out;
    postorder(root, out);
    return out;
}

// Left subtree, then right subtree, then the node.
void BinarySearchTree::postorder(TreeNode* node, std::vector<int>& out) const {
    if (node == nullptr) {
        return;
    }
    postorder(node->left, out);
    postorder(node->right, out);
    out.push_back(node->data);
}

int BinarySearchTree::height() const {
    return height(root);
}

// Counts edges on the longest path down: empty is -1, a single node is 0.
int BinarySearchTree::height(TreeNode* node) const {
    if (node == nullptr) {
        return -1;
    }
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    if (leftHeight > rightHeight) {
        return 1 + leftHeight;
    }
    return 1 + rightHeight;
}

// ---------------------------------------------------------------------------
// Acceptance tests
// ---------------------------------------------------------------------------

// TREENODE 1
void testTreeNode() {
    TreeNode n(5);
    assert(n.data == 5);
    assert(n.left == nullptr);
    assert(n.right == nullptr);
}

void testConstructorDestructor() {
    // CONSTRUCTOR AND DESTRUCTOR 1: a new tree is empty.
    {
        BinarySearchTree tree;
        assert(tree.inorder().empty());
        assert(tree.height() == -1);
    }

    // CONSTRUCTOR AND DESTRUCTOR 2: trees containing nodes are destroyed
    // without crashing when they go out of scope.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.inorder().size() == 10);
    }
    {
        BinarySearchTree tree;
        const std::vector<int> values = {10, 20, 30, 40, 50};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.inorder().size() == 5);
    }
}

void testInsert() {
    // INSERT 1: insert into an empty tree.
    {
        BinarySearchTree tree;
        assert(tree.insert(50));
        assert(tree.inorder() == std::vector<int>({50}));
    }

    // INSERT 2: demo tree, every insert succeeds.
    // INSERT 3: inserting a duplicate fails and leaves the tree unchanged.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            assert(tree.insert(value));
        }
        const std::vector<int> expected = {10, 20, 30, 40, 45, 50, 60, 65, 70, 80};
        assert(tree.inorder() == expected);

        assert(!tree.insert(40));
        assert(tree.inorder() == expected);
        assert(tree.inorder().size() == 10);
    }

    // INSERT 4: degenerate tree (sorted insertion order).
    {
        BinarySearchTree tree;
        const std::vector<int> values = {10, 20, 30, 40, 50};
        for (int value : values) {
            assert(tree.insert(value));
        }
        assert(tree.inorder() == std::vector<int>({10, 20, 30, 40, 50}));
    }
}

void testSearch() {
    // SEARCH 1: empty tree.
    {
        BinarySearchTree tree;
        assert(!tree.search(10));
    }

    // SEARCH 2-7: demo tree.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }
        const std::vector<int> shapeBefore = tree.preorder();

        assert(tree.search(45));    // SEARCH 2: path 50 -> 30 -> 40 -> 45
        assert(tree.search(50));    // SEARCH 3: the root
        assert(tree.search(10));    // SEARCH 4: deepest left leaf
        assert(!tree.search(55));   // SEARCH 5: absent, between existing values
        assert(!tree.search(5));    // SEARCH 6: smaller than every value
        assert(!tree.search(100));  // SEARCH 7: larger than every value

        // search does not modify the tree
        assert(tree.preorder() == shapeBefore);
    }
}

void testTraversals() {
    // TRAVERSALS 1: empty tree.
    {
        BinarySearchTree tree;
        assert(tree.inorder().empty());
        assert(tree.preorder().empty());
        assert(tree.postorder().empty());
    }

    // TRAVERSALS 2: single node.
    {
        BinarySearchTree tree;
        tree.insert(50);
        assert(tree.inorder() == std::vector<int>({50}));
        assert(tree.preorder() == std::vector<int>({50}));
        assert(tree.postorder() == std::vector<int>({50}));
    }

    // TRAVERSALS 3: demo tree.
    // TRAVERSALS 5: calling a traversal twice returns identical results, and
    // preorder before and after the calls matches (the tree is unchanged).
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }
        const std::vector<int> shapeBefore = tree.preorder();

        assert(tree.inorder() == std::vector<int>({10, 20, 30, 40, 45, 50, 60, 65, 70, 80}));
        assert(tree.preorder() == std::vector<int>({50, 30, 20, 10, 40, 45, 70, 60, 65, 80}));
        assert(tree.postorder() == std::vector<int>({10, 20, 45, 40, 30, 65, 60, 80, 70, 50}));

        assert(tree.inorder() == tree.inorder());
        assert(tree.preorder() == tree.preorder());
        assert(tree.postorder() == tree.postorder());

        assert(tree.preorder() == shapeBefore);
    }

    // TRAVERSALS 4: degenerate tree.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {10, 20, 30, 40, 50};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.inorder() == std::vector<int>({10, 20, 30, 40, 50}));
        assert(tree.preorder() == std::vector<int>({10, 20, 30, 40, 50}));
        assert(tree.postorder() == std::vector<int>({50, 40, 30, 20, 10}));
    }
}

void testHeight() {
    // HEIGHT 1: empty tree.
    {
        BinarySearchTree tree;
        assert(tree.height() == -1);
    }

    // HEIGHT 2: single node.
    {
        BinarySearchTree tree;
        tree.insert(50);
        assert(tree.height() == 0);
    }

    // HEIGHT 3: demo tree.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.height() == 3);
    }

    // HEIGHT 4: degenerate tree.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {10, 20, 30, 40, 50};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.height() == 4);
    }
}

void testDeleteValue() {
    auto isStrictlyIncreasing = [](const std::vector<int>& values) {
        for (std::size_t i = 1; i < values.size(); ++i) {
            if (values[i - 1] >= values[i]) {
                return false;
            }
        }
        return true;
    };

    // DELETEVALUE 1: empty tree.
    {
        BinarySearchTree tree;
        assert(!tree.deleteValue(10));
    }

    // DELETEVALUE 2: single node.
    {
        BinarySearchTree tree;
        tree.insert(50);
        assert(tree.deleteValue(50));
        assert(tree.inorder().empty());
        assert(tree.height() == -1);
    }

    // DELETEVALUE 3-6: demo tree.
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }

        // DELETEVALUE 3: absent value, tree unchanged.
        const std::vector<int> inorderBefore = tree.inorder();
        const std::vector<int> preorderBefore = tree.preorder();
        assert(!tree.deleteValue(55));
        assert(tree.inorder() == inorderBefore);
        assert(tree.preorder() == preorderBefore);

        // DELETEVALUE 4 and 6: the three removal cases, sorted after each.
        assert(tree.deleteValue(10));   // leaf
        assert(tree.inorder() == std::vector<int>({20, 30, 40, 45, 50, 60, 65, 70, 80}));
        assert(tree.preorder() == std::vector<int>({50, 30, 20, 40, 45, 70, 60, 65, 80}));
        assert(tree.height() == 3);
        assert(isStrictlyIncreasing(tree.inorder()));

        assert(tree.deleteValue(40));   // one child
        assert(tree.inorder() == std::vector<int>({20, 30, 45, 50, 60, 65, 70, 80}));
        assert(tree.preorder() == std::vector<int>({50, 30, 20, 45, 70, 60, 65, 80}));
        assert(tree.height() == 3);
        assert(isStrictlyIncreasing(tree.inorder()));

        assert(tree.deleteValue(50));   // two children (the root)
        assert(tree.inorder() == std::vector<int>({20, 30, 45, 60, 65, 70, 80}));
        assert(tree.preorder() == std::vector<int>({60, 30, 20, 45, 70, 65, 80}));
        assert(tree.height() == 2);
        assert(isStrictlyIncreasing(tree.inorder()));

        // DELETEVALUE 5 and 6: delete every remaining value until empty.
        const std::vector<int> remaining = tree.inorder();
        for (int value : remaining) {
            assert(tree.deleteValue(value));
            assert(!tree.search(value));
            assert(isStrictlyIncreasing(tree.inorder()));
        }
        assert(tree.inorder().empty());
        assert(tree.height() == -1);
    }

    // DELETEVALUE 7: one child on the left (20's only child is 10).
    {
        BinarySearchTree tree;
        const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
        for (int value : values) {
            tree.insert(value);
        }
        assert(tree.deleteValue(20));
        assert(tree.inorder() == std::vector<int>({10, 30, 40, 45, 50, 60, 65, 70, 80}));
        assert(tree.preorder() == std::vector<int>({50, 30, 10, 40, 45, 70, 60, 65, 80}));
        assert(tree.height() == 3);
        assert(isStrictlyIncreasing(tree.inorder()));
    }
}

void runAllTests() {
    testTreeNode();
    testConstructorDestructor();
    testInsert();
    testSearch();
    testTraversals();
    testHeight();
    testDeleteValue();

    std::cout << "All acceptance tests passed\n";
}

// ---------------------------------------------------------------------------
// Demo
// ---------------------------------------------------------------------------

// True if every value is larger than the one before it.
bool isStrictlyIncreasing(const std::vector<int>& values) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i - 1] >= values[i]) {
            return false;
        }
    }
    return true;
}

void runDemo() {
    // "10 20 30" style text for a list of values.
    auto toText = [](const std::vector<int>& values) {
        std::string text;
        for (std::size_t i = 0; i < values.size(); ++i) {
            if (i > 0) {
                text += ' ';
            }
            text += std::to_string(values[i]);
        }
        return text;
    };

    // Pads text with spaces on the right so columns line up.
    auto padRight = [](std::string text, std::size_t width) {
        if (text.size() < width) {
            text.append(width - text.size(), ' ');
        }
        return text;
    };

    BinarySearchTree tree;

    // 1. Insert 10 distinct values and show all traversals and the height.
    const std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 45, 65};
    for (int value : values) {
        tree.insert(value);
    }
    std::cout << "Inserted:  " << toText(values) << '\n';
    std::cout << "Inorder:   " << toText(tree.inorder()) << '\n';
    std::cout << "Preorder:  " << toText(tree.preorder()) << '\n';
    std::cout << "Postorder: " << toText(tree.postorder()) << '\n';
    std::cout << "Height:    " << tree.height() << '\n';

    // 2. Search for a present value and an absent one.
    std::cout << "Search 45: " << (tree.search(45) ? "found" : "not found") << '\n';
    std::cout << "Search 55: " << (tree.search(55) ? "found" : "not found") << '\n';

    // 3-5. One removal of each kind, checking inorder stays sorted.
    struct Removal {
        int value;
        std::string label;
    };
    const std::vector<Removal> removals = {
        {10, "Delete 10 (leaf):"},
        {40, "Delete 40 (one child):"},
        {50, "Delete 50 (two children):"},
    };
    for (const Removal& removal : removals) {
        tree.deleteValue(removal.value);
        const std::vector<int> inorder = tree.inorder();
        std::cout << padRight(removal.label, 26)
                  << padRight(toText(inorder), 29)
                  << "sorted: " << (isStrictlyIncreasing(inorder) ? "yes" : "no") << '\n';
    }
}

int main() {
    runAllTests();
    runDemo();
    return 0;
}
