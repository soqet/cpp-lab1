#pragma once

#include <algorithm>

static const int DEFAULT_CAPACITY =  100;

// T - value type
// HashS - hash functor
template <typename T, typename HashS = std::hash<T>>
class Linkedhs {
public:
    class iterator; 

    // construct hash set with given capacity of hash table
    Linkedhs(size_t capacity = DEFAULT_CAPACITY);

    // copy constructor
    Linkedhs(const Linkedhs<T, HashS> &other);

    ~Linkedhs();

    // inserts element in set
    bool insert(const T & e);

   // removes element from set
    bool remove(const T &v);

    // swaps sets between objects
    void swap(Linkedhs<T, HashS> &other);

    // returns number of elements in set
    size_t size() const;

    // checks if set's size is 0
    bool empty() const;

    // checks if set has element v
    bool contains(const T &v) const;

    // returns a iterator pointing to v
    // returns end iterator if v doesnt presented in set
    iterator find(const T &v) const;

    // checks if two sets contain same elements
    bool operator==(const Linkedhs<T, HashS> &other) const;

    // checks if two sets contain different elements 
    bool operator!=(const Linkedhs<T, HashS> &other) const;

    // copy assignment
    Linkedhs<T, HashS>& operator=(const Linkedhs<T, HashS>& other);

    // returns iterator pointing to the first element
    iterator begin() const;

    // returns iterator pointing to the element after the last element
    iterator end() const;

    // makes the set empty
    void clear();

private:
    size_t bucketIdx(size_t h) const {
        return h % this->capacity;
    }

    size_t hash(const T& e) const {
        return HashS()(e);
    }

    class Entry;

    Entry **buckets;
    size_t capacity;
    size_t count = 0;

    Entry *first = nullptr;
    Entry *last = nullptr;

    Entry* get(const T &v) const;

    void resize();

    bool insertEntry(Entry *entry);

};

template <typename T, typename HashS>
class Linkedhs<T, HashS>::iterator {
public:
    // returns the element to which it points
    T operator*();

    // moves the iterator forward
    iterator operator++();

    // moves the iterator forward
    iterator operator++(int);

    // check if iterator points to the same elements
    bool operator==(const iterator& other) const;

    // check if iterator points to the different elements
    bool operator!=(const iterator& other) const;

private:
    friend class Linkedhs;

    Entry *curr;
    
    iterator(Entry *curr) : curr(curr) {}
};

template <typename T, typename HashS>
class Linkedhs<T, HashS>::Entry {

    friend class Linkedhs;
    friend class iterator;

    T value;
    Entry *prevInserted;
    Entry *nextInserted;

    Entry *nextCollision = nullptr;

    Entry(T value, Entry *prevInserted = nullptr, Entry *nextInserted = nullptr) :
        value(value), prevInserted(prevInserted), nextInserted(nextInserted) {}
};
