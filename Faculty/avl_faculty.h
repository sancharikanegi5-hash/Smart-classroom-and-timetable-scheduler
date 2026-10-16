#ifndef AVL_FACULTY_H
#define AVL_FACULTY_H

#include "faculty.h"

class AVLFacultyTree {
private:
    struct Node {
        Faculty* faculty;
        Node* left;
        Node* right;
        int height;

        Node(Faculty* f) {
            faculty = f;
            left = nullptr;
            right = nullptr;
            height = 1;
        }
    };

    Node* root;

    int height(Node* node);
    int getBalance(Node* node);
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* insertNode(Node* node, Faculty* faculty);
    Faculty* searchNode(Node* node, int id) const;

public:
    AVLFacultyTree();
    void insert(Faculty* faculty);
    Faculty* search(int id) const;
};

#endif