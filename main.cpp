#include <iostream>
#include <string>
#include <cassert>
#include "tree.h"
#include "list.h"

int main()
{
    //int
    RBTree<int, int> tree;
    for (int i = 1; i < 10; ++i)
    {
        tree.insert(i, i*2);
    }
    RBTree<int, int> t2(tree), t3 = t2 + tree;
    tree.del(9);
    tree.del(4);
    RBTree<int, int> t4 = t2 - tree, t5(t4);
    assert(tree.size() == 7);
    assert(t2.size() == 9);
    assert(t3.size() == 18);
    assert(t4.size() == 2);
    tree.deleteByValue(2);
    assert(tree.size() == 6);
    assert(tree.size() == 6);
    assert(tree.find(2) == 4);
    assert(tree != t2);
    assert(t4   == t5);
    assert(tree.checkForValue(4));
    assert(!tree.checkForValue(2));
    int g=0;
    for (auto i : tree){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    g=0;
    for (auto i : t2){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    //String
    RBTree<int, std::string> stree;
    for (int i = 1; i < 10; ++i)
    {
        stree.insert(i, std::to_string(i*20000) + "abcc");
    }
    RBTree<int, std::string> st2(stree), st3 = st2 + stree;
    stree.del(9);
    stree.del(4);
    RBTree<int, std::string> st4 = st2 - stree, st5(st4);
    assert(stree.size() == 7);
    assert(st2.size() == 9);
    assert(st3.size() == 18);
    assert(st4.size() == 2);
    stree.deleteByValue("20000abcc");
    assert(stree.size() == 6);
    assert(stree.size() == 6);
    assert(stree.find(2) == "40000abcc");
    assert(stree != st2);
    assert(st4   == st5);
    assert(stree.checkForValue("40000abcc"));
    assert(!stree.checkForValue("20000abcc"));
    g=0;
    for (auto i : stree){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    g=0;
    for (auto i : st2){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    //DCLList
    RBTree<int, DCLList> dtree;
    DCLList l[9];
    for (int i = 1; i < 10; ++i)
    {
        for (int k=0; k<i; ++k)
            l[i-1].pushBack(2*i);
        dtree.insert(i, l[i-1]);
    }
    RBTree<int, DCLList> dt2(dtree), dt3 = dt2 + dtree;
    dtree.del(9);
    dtree.del(4);
    RBTree<int, DCLList> dt4 = dt2 - dtree, dt5(dt4);
    assert(dtree.size() == 7);
    assert(dt2.size() == 9);
    assert(dt3.size() == 18);
    assert(dt4.size() == 2);
    dtree.deleteByValue(l[0]);
    assert(dtree.size() == 6);
    assert(dtree.size() == 6);
    assert(dtree.find(2) == l[1]);
    assert(dtree != dt2);
    assert(dt4   == dt5);
    assert(dtree.checkForValue(l[2]));
    assert(!dtree.checkForValue(l[0]));
    g=0;
    for (auto i : dtree){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    g=0;
    for (auto i : dt2){
        std::cout << g++ << ' ' << i->key << ' ' << i->value << '\n';
    }
    return 0;
}
