#include <iostream>
#include <stdexcept>
#include <map>
#include "list.h"

DCLList::DCLList() : start_(nullptr), sz_(0) {}

int DCLList::size() const
{
    return sz_;
}

DCLList::~DCLList()
{
    while (sz_ > 0)
        deleteIndex(0);
}

void DCLList::pushFront(int value)
{
    Node* newNode = new Node;
    newNode->data = value;
    if (start_ == nullptr)
    {   
        newNode->next = newNode->prev = newNode;
        start_ = newNode;
        ++sz_;
        return ;
    }
    newNode->next = start_;
    newNode->prev = start_->prev;
    start_->prev->next = newNode;
    start_->prev = newNode;
    start_ = newNode;
    ++sz_;
}
void DCLList::pushAfter(int after, int value)
{
    if (start_ == nullptr)
    {   
        return ;
    }
    auto it = start_;
    int i = 0;
    while (it->data != after && i++ <= sz_)
        it = it->next;
    if (it->data != after)
        return ;
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = it->next;
    newNode->prev = it;
    it->next->prev = newNode;
    it->next = newNode;
    ++sz_;
}

void DCLList::pushIndex(int index, int value)
{
    if (index < 0 || index > sz_)
    {   
        return ;
    }
    auto it = start_;
    int i = 0;
    while (i++ < index)
        it = it->next;
    Node* newNode = new Node;
    newNode->data = value;
    newNode->next = it;
    newNode->prev = it->prev;
    it->prev->next = newNode;
    it->prev = newNode;
    ++sz_;
    if (index == 0)
        start_ = newNode;
}

void DCLList::pushBack(int value)
{
    Node* newNode = new Node;
    newNode->data = value;
    if (start_ == nullptr)
    {   
        newNode->next = newNode->prev = newNode;
        start_ = newNode;
        ++sz_;
        return ;
    }
    newNode->next = start_;
    newNode->prev = start_->prev;
    start_->prev->next = newNode;
    start_->prev = newNode;
    ++sz_;
}


void DCLList::deleteIndex(int index)
{
    if (index < 0 || index >= sz_)
        return ;
    auto it = start_;
    for (int i=0; i<index; ++i)
        it = it->next;
    if (it == it->next){
        delete it;
        start_ = nullptr;
        sz_ = 0;
        return ;
    }
    it->prev->next = it->next;
    it->next->prev = it->prev;
    if (it == start_)
        start_ = it->next;
    delete it;
    --sz_;
}

void DCLList::deleteRange(int l, int u)
{
    if (u<l)
        return ;
    auto it = start_;
    int i = 0;
    int sz_tmp = sz_;
    while (i < sz_tmp){
        if (l <= it->data && it->data <= u){
            if (it == it->next){
                delete it;
                start_ = nullptr;
                sz_ = 0;
                return ;
            }
            it->prev->next = it->next;
            it->next->prev = it->prev;
            if (it == start_){
                start_ = it->next;
            }
            Node* tmp = it->prev;
            delete it;
            it = tmp;
            --sz_;
        }
        it = it->next;
        ++i;
    }
}

void DCLList::deleteValue(int v)
{
    this->deleteRange(v, v);
}

std::ostream& operator<< (std::ostream& out, const DCLList& list)
{
    auto it = list.start_;
    int i = 0;
    while (i != list.sz_){
        out << "List element nr " << i << ": " << it->data << '\n';
        it = it->next;
        ++i;
    }
    return out;
}

Node DCLList::getElementByValue(int v) const
{
    auto it = start_;
    int i = 0;
    while (i++ < sz_){
        if (v == it->data)
            return *it;
        it = it->next;
    }
    throw std::runtime_error("No such value");
}

Node DCLList::getElementByIndex(int index) const
{
    auto it = start_;
    int i = 0;
    while (i < sz_){
        if (i == index)
            return *it;
        it = it->next;
        ++i;
    }
    throw std::runtime_error("No such index");
}

DCLList DCLList::addList(const DCLList& l) const
{
    DCLList n;
    for (int i=0; i<sz_; ++i)
        n.pushBack(getElementByIndex(i).data);
    for (int i=0; i<l.size(); ++i)
        n.pushBack(l.getElementByIndex(i).data);
    return n;
}

DCLList DCLList::subtractList(const DCLList& l) const
{
    DCLList n;
    for (int i=0; i<sz_; ++i)
        n.pushBack(getElementByIndex(i).data);
    for (int i=0; i<l.size(); ++i)
        n.deleteValue(l.getElementByIndex(i).data);
    return n;
}

DCLList DCLList::unique() const
{
    std::map<int, bool> m;
    DCLList n;
    for (int i=0; i<sz_; ++i){
        if (!m[getElementByIndex(i).data]){
            m[getElementByIndex(i).data] = true;
            n.pushBack(getElementByIndex(i).data);
        }
    }
    return n;
}

bool DCLList::isEqual(const DCLList& l) const
{
    if (this == &l)
        return true;
    if (sz_ != l.sz_)
        return false;
    for (int i=0; i<sz_; ++i)
        if (getElementByIndex(i).data != l.getElementByIndex(i).data)
            return false;
    return true;
}

DCLList::DCLList(const DCLList& l) : sz_(l.sz_)
{
    start_ = nullptr;
    for (int i=0; i<l.sz_; ++i)
        pushBack(l.getElementByIndex(i).data);
}

DCLList& DCLList::operator=(const DCLList& l)
{
    if (this != &l){
        start_ = nullptr;
        for (int i=0; i<l.sz_; ++i)
            pushBack(l.getElementByIndex(i).data);
    }
    return *this;
}

bool DCLList::operator==(const DCLList& l)
{
    return isEqual(l);
}

bool DCLList::operator!=(const DCLList& l)
{
    return !isEqual(l);
}

DCLList DCLList::operator+(const DCLList& l)
{
    return addList(l);
}

DCLList DCLList::operator-(const DCLList& l)
{
    return subtractList(l);
}

Node DCLList::operator[](int i)
{
    return getElementByIndex(i);
}
