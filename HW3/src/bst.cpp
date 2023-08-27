#include "bst.h"
#include <iomanip>
#include <vector>
#include <queue>
#include <iostream>


BST::Node::Node() {
    this->value = 0;
    this->left = nullptr;
    this->right = nullptr;
}

BST::Node::Node(const Node &node) {
    this->value = node.value;
    this->left = node.left;
    this->right = node.right;
}

BST::Node::Node(int value, Node *left, Node *right) {
    this->value = value;
    this->left = left;
    this->right = right;
}

std::ostream &operator<<(std::ostream &os, BST::Node &node) {
    os << &node << "value: " << node.value << " left: " << node.left << " right: " << node.right;
    return os;
}

bool BST::Node::operator>(int num) const {
    return value > num;
}

bool BST::Node::operator<(int num) const {
    return value < num;
}

bool BST::Node::operator>=(int num) const {
    return value >= num;
}

bool BST::Node::operator<=(int num) const {
    return value <= num;
}

bool BST::Node::operator==(int num) const {
    return value == num;
}

bool operator>(int a, const BST::Node &node) {
    return node < a;
}

bool operator>=(int a, const BST::Node &node) {
    return node <= a;
}

bool operator<(int a, const BST::Node &node) {
    return node > a;
}

bool operator<=(int a, const BST::Node &node) {
    return node >= a;
}

bool operator==(int a, const BST::Node &node) {
    return node == a;
}

BST::BST() {
    root = nullptr;
}

BST::BST(BST &other) {
    root = nullptr;
    add_node(other.get_root()->value);

    std::vector<int> nodes;
    other.bfs([&nodes](BST::Node *&node) {
        nodes.push_back(node->value);
    });

    for (auto node: nodes) {
        add_node(node);
    }
}

BST::BST(BST &&other) {
    root = other.root;
    other.root = nullptr;
}

BST::~BST() {
    std::vector<Node *> nodes;
    bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
    for (auto &node: nodes)
        delete node;
}


BST::Node *&BST::get_root() {
    return this->root;
}

size_t BST::length() {
    std::vector<int> ans;
    bfs([&ans](BST::Node *&node) { ans.push_back(node->value); });
    return ans.size();
}

void BST::bfs(const std::function<void(Node *&)> &func) {
    if (root == nullptr) {
        func(root);
        return;
    }

    std::queue<Node *> queue;
    queue.push(root);

    while (!queue.empty()) {
        auto head = queue.front();
        queue.pop();
        func(head);
        if (head->left != nullptr) {
            queue.push(head->left);
        }
        if (head->right != nullptr) {
            queue.push(head->right);
        }
    }
}

bool BST::add_node(int value) {
    bool ans = false;
    bool repeated = false;
    Node *pnode = root;

    while (!ans && !repeated) {
        if (pnode == nullptr) {
            root = new BST::Node(value, nullptr, nullptr);
            ans = true;
            break;
        }
        if (value < pnode->value) {
            if (pnode->left == nullptr) {
                pnode->left = new BST::Node(value, nullptr, nullptr);
                ans = true;
            } else {
                pnode = pnode->left;
            }
        } else if (value > pnode->value) {
            if (pnode->right == nullptr) {
                pnode->right = new BST::Node(value, nullptr, nullptr);
                ans = true;
            } else {
                pnode = pnode->right;
            }
        } else {
            repeated = true;
        }
    }

    return ans && !repeated;
}

BST::Node **BST::find_node(int value) {
    static Node *result = nullptr;

    bool find = false;
    bfs([value, &find](Node *&node) {
        if (node->value == value) {
            result = node;
            find = true;
        }
    });
    if (find) {
        return &result;

    } else {
        return nullptr;
    }
}

BST::Node **BST::find_parrent(int value) {
    static Node *parent_node = nullptr;
    bool find = false;

    bfs([value, &find](Node *&node) {
        if (node->left != nullptr && node->left->value == value) {
            parent_node = node;
            find = true;
        }
        if (node->right != nullptr && node->right->value == value) {
            parent_node = node;
            find = true;
        }
    });
    if (find) {
        return &parent_node;

    } else {
        return nullptr;
    }
}

BST::Node **BST::find_successor(int value) {
    auto succ_node = find_node(value);
    if (succ_node == nullptr) {
        return nullptr;
    }
    if ((*succ_node)->left == nullptr) {
        return nullptr;
    }

    *succ_node = (*succ_node)->left;
    while ((*succ_node)->right != nullptr) {
        (*succ_node) = (*succ_node)->right;
    }
    return succ_node;
}

bool BST::delete_node(int value) {
    auto pnode = find_node(value);
    if (pnode == nullptr) {
        return false;
    }
    Node *pnode_v = *pnode;

    auto parent = find_parrent(value);
    if (parent == nullptr) {
        parent = &get_root();
    }

    Node *parent_v = *parent;

    // 只存在单侧节点
    if (pnode_v->left == nullptr) {
        parent_v->right = pnode_v->right;
        return true;
    }
    if (pnode_v->right == nullptr) {
        parent_v->left = pnode_v->left;
        return true;
    }

    auto succ = find_successor(value);
    if (succ == nullptr) {
        if (parent_v->right->value == value) {
            parent_v->right = pnode_v->right;

        } else {
            parent_v->left = pnode_v->right;
        }
        return true;
    }
    Node *succ_v = *succ;

    auto succ_parent = find_parrent(succ_v->value);

    pnode_v->value = succ_v->value;

    if (succ_v->left != nullptr) {
        (*succ_parent)->right = (*succ)->left;
    } else {
        (*succ_parent)->right = nullptr;
    }

    return true;
}

BST BST::operator++() {
    bfs([](BST::Node *&node) {
        node->value++;
    });
    std::cout << *this << std::endl;
    return *this;
}

BST &BST::operator=(BST &bst) {
    if (get_root() == bst.get_root()) {
        return *this;
    }

    std::vector<int> nodes;
    bst.bfs([&nodes](BST::Node *&node) {
        nodes.push_back(node->value);
    });

    for (auto node: nodes) {
        add_node(node);
    }
    return *this;
}

BST &&BST::operator=(BST &&bst) {
    root = bst.root;
    bst.root = nullptr;

    return std::move(*this);
}

std::ostream &operator<<(std::ostream &os, BST &bst) {
    os << std::string(80, '*') << std::endl;
    bst.bfs([&os](BST::Node *&node) {
        os << std::setiosflags(std::ios::left);
        os << std::setw(20) << node << "=> value:";
        os << std::setw(10) << node->value << "left:";
        os << std::setw(20) << node->left << "right:";
        os << std::setw(20) << node->right << std::endl;
    });
    os << std::string(80, '*') << std::endl;
    return os;
}




