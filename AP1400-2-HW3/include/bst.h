#ifndef BST_H
#define BST_H

#include <iostream>
#include <functional>

class BST
{
public:
    class Node
    {
    public:
        Node(int value, Node* left, Node* right);
        Node();
        Node(const Node& node);

        int value;
        Node* left;
        Node* right;

        friend std::ostream& operator<<(std::ostream&, const Node&);
        friend bool operator==(const int, const BST::Node&);
        friend bool operator<(const int, const BST::Node&);
        friend bool operator<=(const int, const BST::Node&);
        friend bool operator>=(const int, const BST::Node&);
        friend bool operator>(const int, const BST::Node&);

        friend bool operator==(const BST::Node&, const int);
        friend bool operator<(const BST::Node&, const int);
        friend bool operator<=(const BST::Node&, const int);
        friend bool operator>=(const BST::Node&, const int);
        friend bool operator>(const BST::Node&, const int);
    };

    Node*& get_root();
    void bfs(std::function<void(Node*& node)> func) const;
    size_t length() const;
    bool add_node(int value);
    Node** find_node(int value);
    Node** find_parrent(int value);
    Node** find_successor(int value);
    bool delete_node(int value);

    friend std::ostream& operator<<(std::ostream&, const BST&);

    BST& operator++();
    BST operator++(int);

    BST();
    BST(std::initializer_list<int> list);
    BST(const BST&);
    BST(BST&&);

    ~BST();
    
    BST& operator=(const BST&);
    BST& operator=(BST&&);
private:
    Node* root;
};

#endif //BST_H