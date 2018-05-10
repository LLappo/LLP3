#pragma once
#include <iostream>
#include <cstdio>

#include <cstdlib>
#include <stdexcept>
#include <iostream>
using namespace std;

template<typename Key, typename Value>
class RBTreeIter;

template<typename Key, typename Value>
class RBTree
{
    template<typename K, typename V> friend class RBTreeIter;
private:
    enum Color {RED, BLACK};

    struct Node
    {
        Color color;
        Key key;
        Value value;
        Node* parent;
        Node* left;
        Node* right;
    };

    Node* root_;
    int size_;

    void rotateLeft(Node* x)
    {
        Node* y;

        y = x->right;
        x->right = y->left;
        if (y->left)
        {
            y->left->parent = x;
        }

        y->parent = x->parent;
        y->left = x;

        if (!x->parent)
        {
            root_ = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        x->parent = y;
    }
    void rotateRight(Node* y)
    {
        Node *x;

        x = y->left;
        y->left = x->right;
        if (x->right)
        {
            x->right->parent = y;
        }

        x->parent = y->parent;
        x->right = y;

        if (!y->parent)
        {
            root_ = x;
        }
        else if (y == y->parent->left)
        {
            y->parent->left = x;
        }
        else
        {
            y->parent->right = x;
        }

        y->parent = x;
    }
    void transplant(Node* dest, Node* src)
    {
        if (dest->parent == nullptr)
        {
            root_ = src;
        }
        else if (dest == dest->parent->left)
        {
            dest->parent->left = src;
        }
        else
        {
            dest->parent->right = src;
        }

        if (src)
        {
            src->parent = dest->parent;
        }
    }
    Node* minimum(Node* tree)
    {
        if (!root_)
            return nullptr;
        while (tree->left)
        {
            tree = tree->left;
        }

        return tree;
    }
    Node* maximum(Node* tree)
    {
        if (!root_)
            return nullptr;
        while (tree->right)
        {
            tree = tree->right;
        }

        return tree;
    }
    void deleteNode(Node *node)
    {
        if (!node)
        {
            return;
        }

        if (node->left)
        {
            deleteNode(node->left);
        }

        if (node->right)
        {
            deleteNode(node->right);
        }

        delete node;
        size_--;
    }
    Node* successor(Node* x)
    {
        if (x->right)
            return minimum(x->right);
        Node* y = x->parent;
        while (y && x == y->right)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
    Node* predecessor(Node* x)
    {
        if (x == maximum(root_))
            return nullptr;
        if (x->left)
            return maximum(x->left);
        Node* y = x->parent;
        while (y && x == y->left)
        {
            x = y;
            y = y->parent;
        }
        return y;
    }
public:
    RBTree() : root_(nullptr), size_(0) {}
    RBTree(const RBTree& x) : root_(nullptr), size_(0) {
        for (auto i : x)
            insert(i->key, i->value);
    }
    ~RBTree()
    {
        deleteNode(root_);
    }
    int size() { return size_; }
    void insert(const Key& key, const Value& value)
    {
        Node *node, *parent, *z;

        parent = nullptr;
        node = root_;
        while (node)
        {
            parent = node;
            if (key < node->key)
            {
                node = node->left;
            }
            else
            {
                node = node->right;
            }
        }

        if (!parent)
        {
            z = root_ = new Node;
            z->key = key;
            z->value = value;
            z->color = BLACK;
            z->parent = z->left = z->right = nullptr;
        }
        else
        {
            z = new Node;
            z->key = key;
            z->value = value;
            z->color = RED;
            z->parent = parent;
            z->left = z->right = nullptr;

            if (z->key < parent->key)
            {
                parent->left = z;
            }
            else
            {
                parent->right = z;
            }
        }

        Node *uncle;
        bool side;
        while (z->parent && z->parent->color == RED)
        {
            if ((side = (z->parent == z->parent->parent->left)))
            {
                uncle = z->parent->parent->right;
            }
            else
            {
                uncle = z->parent->parent->left;
            }

            if (uncle && uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == (side ? z->parent->right : z->parent->left))
                {
                    z = z->parent;
                    side ? rotateLeft(z) : rotateRight(z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                side ? rotateRight(z->parent->parent) : rotateLeft(z->parent->parent);
            }
        }

        root_->color = BLACK;
        size_++;
    }
    Value& find(const Key& key)
    {
        Node *node = root_;
        while (node)
        {
            if (node->key > key)
            {
                node = node->left;
            }
            else if (node->key < key)
            {
                node = node->right;
            }
            else
            {
                return node->value;
            }
        }

        throw std::runtime_error("Key not found");
    }
    void del(const Key& key)
    {
        Node *node = root_;
        while (node)
        {
            if (node->key > key)
            {
                node = node->left;
            }
            else if (node->key < key)
            {
                node = node->right;
            }
            else
            {
                break;
            }
        }

        if (!node || node->key != key)
        {
            return;
        }

        Color original;
        Node *sub, *old;
        if (!node->left)
        {
            transplant(node, sub = node->right);
        }
        else if (!node->right)
        {
            transplant(node, sub = node->left);
        }
        else
        {
            old = minimum(node->right);
            original = old->color;
            sub = old->right;

            if (old->parent == node)
            {
                if (sub)
                    sub->parent = node;
            }
            else
            {
                transplant(old, old->right);
                old->right = node->right;
                old->right->parent = old;
            }

            transplant(node, old);
            old->left = node->left;
            old->left->parent = old;
            old->color = node->color;
        }

        delete node;
        --size_;
        if (original == BLACK)
        {
            bool side;
            Node *sibling;
            while (old != root_ && old->color == BLACK)
            {
                if ((side = (old == old->parent->left)))
                {
                    sibling = old->parent->right;
                }
                else
                {
                    sibling = old->parent->left;
                }

                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    old->parent->color = RED;
                    side ? rotateLeft(old->parent) : rotateRight(old->parent);
                    sibling = side ? old->parent->right : old->parent->left;
                }

                if (sibling->left->color == BLACK && sibling->right->color == BLACK)
                {
                    sibling->color = RED;
                    old = old->parent;
                }
                else
                {
                    if (BLACK == side ? sibling->right->color : sibling->left->color)
                    {
                        sibling->color = RED;
                        if (side)
                        {
                            sibling->left->color = BLACK;
                            rotateRight(sibling);
                            sibling = old->parent->right;
                        }
                        else
                        {
                            sibling->right->color = BLACK;
                            rotateLeft(sibling);
                            sibling = old->parent->left;
                        }
                    }

                    sibling->color = old->parent->color;
                    old->parent->color = BLACK;
                    if (side)
                    {
                        sibling->left->color = BLACK;
                        rotateLeft(old->parent);
                    }
                    else
                    {
                        sibling->right->color = BLACK;
                        rotateRight(old->parent);
                    }

                    old = root_;
                }
            }
        }
    }
    RBTree operator+(const RBTree& x) const
    {
        RBTree<Key, Value> n;
        Node* mx = maximum(root_);
        Node* i = minimum(root_);
        for (; i != mx; i = successor(i))
            n.insert(i->key, i->value);
        n.insert(i->key, i->value);
        mx = x.maximum(x.root_);
        i = x.minimum(x.root_);
        for (; i != mx; i = successor(i))
            n.insert(i->key, i->value);
        n.insert(i->key, i->value);
        return n;
    }
    RBTree operator-(const RBTree& x) const
    {
        RBTree<Key, Value> n;
        Node* mx = maximum(root_);
        Node* i = minimum(root_);
        for (; i != mx; i = successor(i))
            n.insert(i->key, i->value);
        n.insert(i->key, i->value);
        mx = x.maximum(x.root_);
        i = x.minimum(x.root_);
        for (; i != mx; i = successor(i))
            n.deleteByValue(i->value);
        n.deleteByValue(i->value);
        return n;
    }
    bool operator==(const RBTree& x) const
    {
        if (this == &x)
            return true;
        if (size_ != x.size())
            return false;

        Node *mx1 = maximum(root_), *mx2 = x.maximum(x.root_);
        Node *i1 = minimum(root_), *i2 = x.minimum(x.root_);
        for (; i1 != mx1 && i2 != mx2; i1 = successor(i1), i2=x.successor(i2))
            if(i1->key != i2->key || i1->value != i2->value)
                return false;

        return true;
    }
    bool operator!=(const RBTree& x) const
    {
        return !((*this)==x);
    }
    void deleteByValue(const Value& val) {
        Node* mx = maximum(root_);
        Node* i = minimum(root_);
        bool b = false;
        Key k;
        for (; i != mx; i = successor(i))
            if (i->value == val){
                b = true;
                k = i->key;
            }
        if (i->value == val) {
            b = true;
            k = i->key;
        }
        if (b)
            del(k);
    }
    bool checkForValue(const Value& val) {
        Node* mx = maximum(root_);
        Node* i = minimum(root_);
        bool b = false;
        for (; i != mx; i = successor(i))
            if (i->value == val)
                b = true;
        return b;
    }

    RBTreeIter<Key, Value> begin() const
    {
        return RBTreeIter<Key, Value>(this, 0);
    }

    RBTreeIter<Key, Value> end() const
    {
        return RBTreeIter<Key, Value>(this, size_);
    }

};

template<typename Key, typename Value>
class RBTreeIter
{
private:
    const RBTree<Key, Value>* ptr_;
    int pos_;
public:
    RBTreeIter(const RBTree<Key, Value>* ptr, int pos) : ptr_(ptr), pos_(pos) {}

    bool operator!=(const RBTreeIter<Key, Value>& other) const
    {
        return pos_ != other.pos_;
    }
    typename RBTree<Key, Value>::Node* operator*() const
    {
        typename RBTree<Key, Value>::Node* it = const_cast<RBTree<Key,Value>*>(ptr_)->minimum(ptr_->root_);
        for (int i = 0; i < pos_; ++i)
            it = const_cast<RBTree<Key,Value>*>(ptr_)->successor(it);
        return it;
    }
    const RBTreeIter<Key, Value>& operator++()
    {
        ++pos_;
        return *this;
    }
};
