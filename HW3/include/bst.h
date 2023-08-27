#ifndef BST_H
#define BST_H

#include <functional>
#include <ostream>


class BST {
public:
    class Node {
    public:
        Node(int value, Node *left, Node *right);

        Node();

        Node(const Node &node);

        bool operator>(int num) const;

        bool operator<(int num) const;

        bool operator>=(int num) const;

        bool operator<=(int num) const;

        bool operator==(int num) const;

        friend bool operator>(int num, const Node& node);

        friend bool operator<(int num, const Node& node);

        friend bool operator>=(int num, const Node& node);

        friend bool operator<=(int num, const Node& node);

        friend std::ostream &operator<<(std::ostream &os, Node &node);

        int value;
        Node *left;
        Node *right;
    };

    BST();

    BST(BST &other);

    BST(BST &&other);

    ~BST();

    void bfs(const std::function<void(Node *&node)>& func);

    Node *&get_root();

    size_t length();

    bool add_node(int value);

    Node **find_node(int value);

    Node **find_parrent(int value);

    Node **find_successor(int value);

    bool delete_node(int value);

    BST operator++();

    BST& operator=(BST &other);

    BST&& operator=(BST &&other);

    friend std::ostream &operator<<(std::ostream &os, BST &bst);

private:
    Node *root;
};

#endif //BST_H