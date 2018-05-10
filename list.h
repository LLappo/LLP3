#pragma once

struct Node
{
    int data;
    Node *next, *prev;
};

class DCLList
{
private:
    Node *start_;
    int sz_;
public:
    DCLList();
    ~DCLList();
    DCLList(const DCLList& l);
    DCLList& operator=(const DCLList& l);
    int size() const;
    void pushFront(int);
    void pushAfter(int, int);
    void pushIndex(int, int);
    void pushBack(int);
    void deleteIndex(int);
    void deleteRange(int, int);
    void deleteValue(int);
    Node getElementByValue(int) const;
    Node getElementByIndex(int) const;
    DCLList addList(const DCLList&) const;
    DCLList subtractList(const DCLList&) const;
    DCLList unique() const;
    bool isEqual(const DCLList&) const;
    bool operator==(const DCLList&);
    bool operator!=(const DCLList&);
    DCLList operator+(const DCLList&);
    DCLList operator-(const DCLList&);
    Node operator[](int);
    friend std::ostream& operator<< (std::ostream& out, const DCLList& list);
};
