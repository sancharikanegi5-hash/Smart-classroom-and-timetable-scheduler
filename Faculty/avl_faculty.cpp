#include "avl_faculty.h"
#include<algorithm>
AVLFacultyTree::AVLFacultyTree() {
    root = nullptr;
}

int AVLFacultyTree::height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int AVLFacultyTree::getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

AVLFacultyTree::Node* AVLFacultyTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

AVLFacultyTree::Node* AVLFacultyTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

AVLFacultyTree::Node* AVLFacultyTree::insertNode(Node* node, Faculty* faculty) {
    if (node == nullptr)
        return new Node(faculty);

    int id = faculty->getFacultyId();
    int nodeId = node->faculty->getFacultyId();

    if (id < nodeId)
        node->left = insertNode(node->left, faculty);
    else if (id > nodeId)
        node->right = insertNode(node->right, faculty);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && id < node->left->faculty->getFacultyId())
        return rotateRight(node);

    if (balance < -1 && id > node->right->faculty->getFacultyId())
        return rotateLeft(node);

    if (balance > 1 && id > node->left->faculty->getFacultyId()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && id < node->right->faculty->getFacultyId()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void AVLFacultyTree::insert(Faculty* faculty) {
    if (faculty != nullptr)
        root = insertNode(root, faculty);
}

Faculty* AVLFacultyTree::searchNode(Node* node, int id) const {
    if (node == nullptr)
        return nullptr;

    int currentId = node->faculty->getFacultyId();

    if (id == currentId)
        return node->faculty;

    if (id < currentId)
        return searchNode(node->left, id);

    return searchNode(node->right, id);
}

Faculty* AVLFacultyTree::search(int id) const {
    return searchNode(root, id);
}