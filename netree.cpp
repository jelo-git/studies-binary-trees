#include <iostream>
#include <limits>
#include <math.h>
#include <windows.h>
using namespace std;

#define ARRAY_SIZE 10                                             //  change to 10 when using static ARRAY
int const ARRAY[10] = {44, 23, 45, 22, 46, 875, 455, 24, 57, 83}; // ciąg liczb naturalnych zapisany pod zmienną
int arr[ARRAY_SIZE];
LARGE_INTEGER freq, start, endt;
/// @brief tree class
class Node
{
public:
    int value;
    Node *left;
    Node *right;
    Node(int x)
    {
        value = x;
        left = nullptr;
        right = nullptr;
    }
};
Node *root;
/// @brief calculate height of child nodes
/// @param node parent node
/// @return height of nodes
int height(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    else
    {
        int x = height(node->left);
        int y = height(node->right);
        return (max(x, y) + 1);
    }
}
/// @brief calculate balance of node
/// @param node node to calculate
/// @return balance of node
int getBalance(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return height(node->left) - height(node->right);
}
/// @brief start measuring clock
void startClock()
{
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
}
/// @brief stop measuring clock
/// @return number of microseconds from start to now
double exeTime()
{
    QueryPerformanceCounter(&endt);
    return (double)(endt.QuadPart - start.QuadPart) / (freq.QuadPart / 1000000.0);
}
/// @brief heap sort
/// @param arr array to sort
/// @param end number of items in array
/// @param i max
void heap(int *arr, int end, int i)
{
    int max = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < end && arr[l] > arr[max])
        max = l;
    if (r < end && arr[r] > arr[max])
        max = r;
    if (max != i)
    {
        int tmp = arr[max];
        arr[max] = arr[i];
        arr[i] = tmp;
        heap(arr, end, max);
    }
}
/// @brief sort array using heap function
/// @param arr array to sort
/// @param end number of items in array
void sort(int *arr, int end)
{
    for (int i = end / 2 - 1; i >= 0; i--)
    {
        heap(arr, end, i);
    }
    for (int i = end - 1; i >= 0; i--)
    {
        int tmp = arr[i];
        arr[i] = arr[0];
        arr[0] = tmp;
        heap(arr, i, 0);
    }
}
/// @brief generate descending list of numbers
/// @return pointer to usable arr
int *generate()
{
    for (int i = ARRAY_SIZE; i > 0; i--)
    {
        arr[ARRAY_SIZE - i] = i;
    }
    return arr;
}
/// @brief generate descending list of numbers
/// @param k how many numbers
/// @return pointer to usable arr
int *generate(int k)
{
    for (int i = k; i > 0; i--)
    {
        arr[k - i] = i;
    }
    return arr;
}
/// @brief copy elements from ARRAY to usable arr
/// @return pointer to arr
int *copy()
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        arr[i] = ARRAY[i];
    }
    return arr;
}
/// @brief wipe root for new tree inplementation
void destroy()
{
    delete (root);
    root = nullptr;
}
/// @brief squishes tree for balancing
/// @param dummy dummy root
/// @param n how many times
void squish(Node *dummy, int n)
{
    Node *tmp = dummy->right;
    for (int i = 0; i < n; i++)
    {
        Node *ttmp = tmp;
        tmp = tmp->right;
        dummy->right = tmp;
        ttmp->right = tmp->left;
        tmp->left = ttmp;
        dummy = tmp;
        tmp = tmp->right;
    }
}
/// @brief finds node with given key
/// @param root root to start from
/// @param key key to find
/// @return node with key or nullptr if not found
Node *findNode(Node *root, int key)
{
    Node *node = root;
    while (node != nullptr && node->value != key)
    {
        if (key > node->value)
        {
            node = node->right;
        }
        else
        {
            node = node->left;
        }
    }
    if (node != nullptr)
    {
        return node;
    }
    return nullptr;
}
/// @brief rotate in left direction
/// @param node node to rotate
/// @return new "parent" node
Node *left(Node *node)
{
    Node *tmp = node->right;
    Node *parent = tmp->left;
    tmp->left = node;
    node->right = parent;
    return tmp;
}
/// @brief rotate in right direction
/// @param node node to rotate
/// @return new "parent" node
Node *right(Node *node)
{
    Node *tmp = node->left;
    Node *parent = tmp->right;
    tmp->right = node;
    node->left = parent;
    return tmp;
}
/// @brief minimal value from node
/// @param node tree node
/// @return node with minimal value
Node *minVal(Node *node)
{
    Node *point = node;
    while (point->left != nullptr)
    {
        point = point->left;
    }
    return point;
}
/// @brief finds min and max values in tree
/// @param node tree root
void minmax(Node *node)
{
    Node *point = root;
    while (point->left != nullptr)
    {
        // cout << point->value << (point->left->left != nullptr ? "->" : ""); // output suppressed
        point = point->left;
    }
    // cout << "| min: " << point->value << endl; // output suppressed
    point = root;
    while (point->right != nullptr)
    {
        // cout << point->value << (point->right->right != nullptr ? "->" : ""); // output suppressed
        point = point->right;
    }
    // cout << "| max: " << point->value << endl; // output suppressed
}
/// @brief remove node from tree and balance it
/// @param val value of key to find
Node *remove(Node *root, int val)
{
    if (root == nullptr)
    {
        return root;
    }
    if (val < root->value)
    {
        root->left = remove(root->left, val);
    }
    else if (val > root->value)
    {
        root->right = remove(root->right, val);
    }
    else
    {
        if (root->left == nullptr || root->right == nullptr)
        {
            Node *tmp = root->left ? root->left : root->right;
            if (tmp == nullptr)
            {
                tmp = root;
                root = nullptr;
            }
            else
            {
                *root = *tmp;
            }
            free(tmp);
        }
        else
        {
            Node *tmp = minVal(root->right);
            root->value = tmp->value;
            root->right = remove(root->right, tmp->value);
        }
    }
    if (root == nullptr)
    {
        return root;
    }
    int bf = getBalance(root);
    if (bf > 1)
    {
        if (getBalance(root->left) >= 0)
        {
            return right(root);
        }
        else
        {
            root->left = left(root->left);
            return right(root);
        }
    }
    if (bf < -1)
    {
        if (getBalance(root->right) <= 0)
        {
            return left(root);
        }
        else
        {
            root->right = right(root->right);
            return left(root);
        }
    }
    return root;
}
/// @brief remove node for each key given
/// @param node tree node (parent  of all nodes)
/// @param n size of keys array
/// @param keys keys to find and delete
void removeKeys(Node *node, int n, int *keys)
{
    for (int i = 0; i < n; i++)
    {
        root = remove(root, keys[i]);
    }
}
/// @brief prompt for user to input how much keys and what keys they want deleted
void promptRemoveKeys()
{
    int n, q;
    bool validInput = false;
    while (!validInput)
    {
        cout << "Ile elementow: ";
        cin >> n;
        if (cin.fail() || n <= 0)
        {
            cout << "Nieprawidlowe dane. " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            validInput = true;
        }
    }
    int arr[n];
    for (int i = 0; i < n; i++)
    {
        validInput = false;
        while (!validInput)
        {
            cout << (i + 1) << ". Klucz: ";
            cin >> q;
            if (cin.fail() || q <= 0)
            {
                cout << "Nieprawidlowe dane. " << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else
            {
                validInput = true;
            }
        }
        arr[i] = q;
    }
    removeKeys(root, n, arr);
}
/// @brief remove nodes in tree
/// @param node tree node (parent of all nodes)
void removePostOrder(Node *node)
{
    if (node == nullptr)
        return;
    removePostOrder(node->left);
    removePostOrder(node->right);
    cout << node->value << " ";
    node->left = nullptr;
    node->right = nullptr;
    delete node;
}
/// @brief barebones of constucting binary tree
/// @param value value to put in new node
void constructTree(int value)
{
    Node *node = new Node(value);
    if (root == nullptr)
    {
        root = node;
    }
    else
    {
        Node *point = root;
        bool right = true;
        while (point != NULL)
        {
            if (value >= point->value)
            {
                if (point->right == NULL)
                {
                    right = true;
                    break;
                }
                point = point->right;
            }
            else
            {
                if (point->left == NULL)
                {
                    right = false;
                    break;
                }
                point = point->left;
            }
        }
        if (right)
        {
            point->right = node;
        }
        else
        {
            point->left = node;
        }
    }
}
/// @brief binary search and input node to tree (creates balanced AVL tree)
/// @param arr input array
/// @param left left element in array
/// @param right right element in array
void AVLrec(int *arr, int left, int right)
{
    if (left <= right)
    {
        int p = (right + left) / 2;
        constructTree(arr[p]);
        AVLrec(arr, left, p - 1);
        AVLrec(arr, p + 1, right);
    }
}
/// @brief start constructing AVL from nothing
/// @param array input array
/// @param size size of array
/// @return time of execute in microsec
double constructAVL(int *array, int size = ARRAY_SIZE)
{
    sort(array, size);
    destroy();
    startClock();
    AVLrec(array, 0, size - 1);
    return exeTime();
}
/// @brief start constructing BST from nothing
/// @param array input array
/// @param size size of array
/// @return time of execute in microsec
double constructBST(int *array, int size = ARRAY_SIZE)
{
    destroy();
    startClock();
    for (int i = 0; i < size; i++)
    {
        constructTree(array[i]);
    }
    return exeTime();
}
/// @brief turn BST to vine
/// @param dummy dummy root
/// @return count of nodes
int bstToVine(Node *dummy)
{
    int n = 0;
    Node *tmp = dummy->right;
    while (tmp != nullptr)
    {
        if (tmp->left != nullptr)
        {
            Node *ttmp = tmp;
            tmp = tmp->left;
            ttmp->left = tmp->right;
            tmp->right = ttmp;
            dummy->right = tmp;
        }
        else
        {
            dummy = tmp;
            tmp = tmp->right;
            n++;
        }
    }
    return n;
}
/// @brief print tree
/// @param node tree root
/// @param showBalance true: print AVL balance per node
/// @param prefix print prefix
/// @param end true: print 'L'
/// @param nth height start
void print(Node *node, bool showBalance = false, string prefix = "", bool end = true, int nth = 0)
{
    if (node != nullptr)
    {
        cout << prefix << (end ? "L" : "P") << to_string(nth) << ": " << node->value;
        if (showBalance)
        {
            cout << ";" << getBalance(node);
        }
        cout << endl;
        print(node->left, showBalance, prefix + (end ? "|  " : "   "), true, nth += 1);
        print(node->right, showBalance, prefix + (end ? "|  " : "   "), false, nth);
    }
}
/// @brief print tree IN order
/// @param node tree root
void printInOrder(Node *node)
{
    if (node == nullptr)
        return;
    printInOrder(node->left);
    // cout << node->value << " "; // output suppressed
    printInOrder(node->right);
}
/// @brief print tree PRE order
/// @param node tree root
void printPreOrder(Node *node)
{
    if (node == nullptr)
        return;
    // cout << node->value << " "; // output suppressed
    printPreOrder(node->left);
    printPreOrder(node->right);
}
/// @brief print tree POST order
/// @param node tree root
void printPostOrder(Node *node)
{
    if (node == nullptr)
        return;
    printPostOrder(node->left);
    printPostOrder(node->right);
    cout << node->value << " ";
}

/// @brief prompt for user to print tree POST order from key
void promptPrintPostOrder()
{
    int n;
    bool validInput = false;
    while (!validInput)
    {
        cout << "Jaki klucz: ";
        cin >> n;
        if (cin.fail() || n <= 0)
        {
            cout << "Nieprawidlowe dane. " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            validInput = true;
        }
    }
    Node *found = findNode(root, n);
    if (found != nullptr)
    {
        printPostOrder(found);
    }
    else
    {
        cout << "Nieznaleziono klucza" << endl;
    }
}

/// @brief balanced tree with DSW algorithm
/// @param root tree root
void balanceDSW(Node *root)
{
    // cout << "Old: "; // output suppressed
    printPreOrder(root);
    Node *dummy = new Node(-1);
    dummy->right = root;
    int c = bstToVine(dummy);
    int n = pow(2, log2(c + 1)) - 1;
    squish(dummy, c - n);
    for (n = n / 2; n > 0; n /= 2)
    {
        squish(dummy, n);
    }
    root = dummy->right;
    dummy->right = nullptr;
    delete dummy;
    // cout << "\nNew: "; // output suppressed
    printPreOrder(root);
}

int main()
{
    // double time = constructAVL(generate()); // create AVL tree using decreasing numbers
    // double time = constructBST(copy()); // create BST tree using hardcoded values (in ARRAY constant)
    // print(root);              // print tree
    // promptRemoveKeys();       // interactive remove keys dialog
    // removePostOrder(root);    // remove data and print in POST ORDER
    // print(root, true); // print tree with balance
    // printInOrder(root);       // print tree IN ORDER
    // printPreOrder(root);      // print tree PRE ORDER
    // promptPrintPostOrder();   // interactive prompt to choose key to print POST ORDER
    // balanceDSW(root);         // balance tree using DSW algo

    // testing zone :D //

    // AVL tree
    string create, min, inorder;
    for (int i = 1; i <= 10; i++)
    {
        int size = (ARRAY_SIZE / 10) * i;
        create += "(" + to_string(size) + "," + to_string(constructAVL(generate(size), size)) + ")";
        startClock();
        minmax(root); // output suppressed
        min += "(" + to_string(size) + "," + to_string(exeTime()) + ")";
        startClock();
        printInOrder(root); // output suppressed
        inorder += "(" + to_string(size) + "," + to_string(exeTime()) + ")";
    }

    // BST tree
    string create2, min2, inorder2, ball;
    for (int i = 1; i <= 10; i++)
    {
        int size = (ARRAY_SIZE / 10) * i;
        create2 += "(" + to_string(size) + "," + to_string(constructBST(generate(size), size)) + ")";
        startClock();
        minmax(root); // output suppressed
        min2 += "(" + to_string(size) + "," + to_string(exeTime()) + ")";
        startClock();
        printInOrder(root); // output suppressed
        inorder2 += "(" + to_string(size) + "," + to_string(exeTime()) + ")";
        startClock();
        balanceDSW(root); // output suppressed
        ball += "(" + to_string(size) + "," + to_string(exeTime()) + ")";
    }
    // output suppressed for latex graphs
    cout << "BST\nBalance: " << ball << "\nCreate: " << create2 << "\nMinMax: " << min2 << "\nInOrder: " << inorder2 << endl;
    cout << "AVL\nCreate: " << create << "\nMinMax: " << min << "\nInOrder: " << inorder << endl;
    return 0;
}