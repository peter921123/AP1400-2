#include "bst.h"
#include <iomanip>
#include <iostream>
#include <queue>

/******* BST::Node *******/
BST::Node::Node(int value,  Node* left, Node* right) : value(value), left(left), right(right) {}
BST::Node::Node() : value(0), left(nullptr), right(nullptr) {}
BST::Node::Node(const Node& node) : value(node.value), left(node.left), right(node.right) {
    // By unit test, they might want shallow copy
    /*
    right = node.right == nullptr ? nullptr : new Node(*node.right);
    left = node.left == nullptr ? nullptr : new Node(*node.left);
    */
}

std::ostream& operator<<(std::ostream& os, const BST::Node& node) {
    std::cout << std::left
              << std::setw(18) << &node
              << "=> "
              << "value:" << std::setw(10) << node.value
              << "left:" << std::setw(18) << node.left
              << "right:" << node.right
              << std::endl;
    return os;
}

bool operator==(const int value, const BST::Node& node) {
    return value == node.value;
}

bool operator<(const int value, const BST::Node& node) {
    return value < node.value;
}

bool operator<=(const int value, const BST::Node& node) {
    return value <= node.value;
}

bool operator>=(const int value, const BST::Node& node) {
    return value >= node.value;
}

bool operator>(const int value, const BST::Node& node) {
    return value > node.value;
}

bool operator==(const BST::Node& node, const int value) {
    return node.value == value;
}

bool operator<(const BST::Node& node, const int value) {
    return node.value < value;
}

bool operator<=(const BST::Node& node, const int value) {
    return node.value <= value;
}

bool operator>=(const BST::Node& node, const int value) {
    return node.value >= value;
}
bool operator>(const BST::Node& node, const int value) {
    return node.value > value;
}
/******* BST::Node *******/

/********** BST **********/
BST::Node*& BST::get_root() {
    return root;
}

void BST::bfs(std::function<void(Node*& node)> func) const {
    if (root == nullptr) return;
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* currNode = q.front();
        q.pop();
        if (currNode->left != nullptr) q.push(currNode->left);
        if (currNode->right != nullptr) q.push(currNode->right);
        func(currNode);
    }
    return;
}

size_t BST::length() const {
    size_t numNode = 0;
    auto counter = [&numNode](Node*& node){ ++numNode; };
    bfs(counter);
    return numNode;
}

bool BST::add_node(int value) {
    Node* newNode = new Node{value, nullptr, nullptr};
    Node* currNode = root;
    
    if (root == nullptr) {
        root = newNode;
        return true;
    }

    while (currNode != nullptr) {
        if (currNode->value == value) return false;
        else if (value > currNode->value) {
            if (currNode->right == nullptr) {
                currNode->right = newNode;
                return true;
            } else {
                currNode = currNode->right;
            }
        } else {
            if (currNode->left == nullptr) {
                currNode->left = newNode;
                return true;
            } else {
                currNode = currNode->left;
            }
        }
    }

    return false;
}

BST::Node** BST::find_node(int value) {
    Node** currNode = &root;
    while (*currNode != nullptr) {
        if ((*currNode)->value == value) return currNode;
        currNode = (value > (*currNode)->value) ? &((*currNode)->right) : &((*currNode)->left);
    }
    return nullptr;
}

BST::Node** BST::find_parrent(int value) {
    Node** currNode = &root;
    Node** prevNode = nullptr;

    while (*currNode != nullptr) {
        if ((*currNode)->value == value) return prevNode;
        prevNode = currNode;
        currNode = (value > (*currNode)->value) ? &((*currNode)->right) : &((*currNode)->left);
    }
    return nullptr;
}

BST::Node** BST::find_successor(int value) {
    Node** target = find_node(value);
    if (target == nullptr || *target == nullptr) return nullptr;

    if ((*target)->left != nullptr) {
        Node** currNode = &((*target)->left);
        while ((*currNode)->right != nullptr) {
            currNode = &((*currNode)->right);
        }
        return currNode;
    }

    Node** pred = find_parrent(value);
    while (*pred != nullptr && *target == (*pred)->left) {
        target = pred;
        pred = find_parrent((*pred)->value);
    }
    return pred;
}

bool BST::delete_node(int value) {
    Node** temp = find_node(value);
    if (temp == nullptr) return false;

    Node* target = *temp;

    Node* deletedNode;
    Node* child;
    if (target->left == nullptr || target->right == nullptr) {
        deletedNode = target;
    } else {
        deletedNode = *find_successor(value);
    }

    if (deletedNode->left != nullptr) {
        child = deletedNode->left;
    } else {
        child = deletedNode->right;
    }

    Node* deletedNodeParant = *find_parrent(deletedNode->value);
    if (deletedNodeParant == nullptr) {
        root = child;
    } else if (deletedNode == deletedNodeParant->left) {
        deletedNodeParant->left = child;
    } else {
        deletedNodeParant->right = child;
    }

    if (deletedNode != target) {
        target->value = deletedNode->value;
    }

    delete deletedNode;
    deletedNode = nullptr;
    return true;
}

std::ostream& operator<<(std::ostream& os, const BST& bst) {
    size_t numNode = 0;
    auto func = [&numNode, &os](BST::Node*& node) {
        ++numNode;
        os << *node;
    };
    bst.bfs(func);
    os << " binary search tree size: " << numNode << "\n";
    return os;
}

BST& BST::operator++() {
    auto func = [](Node*& node) { ++(node->value); };
    bfs(func);
    return *this;
}

BST BST::operator++(int) {
    BST temp = *this;
    auto func = [](Node*& node) { ++(node->value); };
    this->bfs(func);
    return temp; 
}

BST::BST() : root(nullptr) {}

BST::BST(const BST& bst) : root(nullptr) {
    auto func = [&](Node*& node) {
        this->add_node(node->value);
    };
    bst.bfs(func);
}

BST& BST::operator=(const BST& bst) {
    if (this != &bst) {

        auto deleteNode = [](Node*& node) {
            delete node;
        };
        this->bfs(deleteNode);
        root = nullptr;

        auto func = [&](Node*& node) {
            this->add_node(node->value);
        };
        bst.bfs(func);
    }
    return *this;
}

BST::BST(BST&& bst) : root(nullptr) {
    std::swap(this->root, bst.root);
}

BST& BST::operator=(BST&& bst) {
    if (this != &bst)
        std::swap(this->root, bst.root);
    return *this;
}

BST::BST(std::initializer_list<int> list) : root(nullptr) {
    for (int value : list) {
        add_node(value);
    }
}

BST::~BST() {
    auto deleteNode = [](Node*& node) {
        delete node;
    };
    this->bfs(deleteNode);
    root = nullptr;
}
/********** BST **********/