#pragma once

#include <algorithm>

// T - value type
// HashS - hash functor
template <typename T, typename HashS = std::hash<T>>
class Linkedhs {
public:
    class iterator; 

    // construct hash set with given capacity of hash table
    Linkedhs() : buckets(new Entry*[DEFAULT_CAPACITY]()), capacity(this->DEFAULT_CAPACITY) {}

    // copy constructor
    // returned copy is independent copy of other
    Linkedhs(const Linkedhs<T, HashS> &other);

    ~Linkedhs();

    // inserts element in set
    // returns false if e already present in set
    bool insert(const T & e);

    // removes element from set
    bool remove(const T &v);

    // swaps elements between two sets
    void swap(Linkedhs<T, HashS> &other);

    // returns number of elements in set
    size_t size() const;

    // checks if set's size is 0
    bool empty() const;

    // checks if set has element v
    bool contains(const T &v) const;

    // returns an iterator pointing to v
    // returns end iterator if v is not present in set
    iterator find(const T &v) const;

    // checks if two sets contain same elements
    // sets with the same elements but different order of elements are considered equal
    bool operator==(const Linkedhs<T, HashS> &other) const;

    // checks if two sets contain different elements 
    bool operator!=(const Linkedhs<T, HashS> &other) const;

    // copy assignment
    Linkedhs<T, HashS>& operator=(const Linkedhs<T, HashS>& other);

    // returns iterator pointing to the first inserted element
    // iterates based on insertion order
    iterator begin() const;

    // returns iterator pointing to the element after the last element
    iterator end() const;

    // makes the set empty
    void clear();

private:

    static const int DEFAULT_CAPACITY =  100;
    static constexpr double RESIZE_CONDITION = 0.75;

    size_t bucketIdx(size_t h) const {
        return h % this->capacity;
    }

    size_t hash(const T& e) const {
        return HashS()(e);
    }

    struct Entry;

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
    T operator*();

    iterator operator++();

    iterator operator++(int);

    // check if iterators point to the same element in the same Linkedhs
    bool operator==(const iterator& other) const;

    bool operator!=(const iterator& other) const;

private:
    friend class Linkedhs;

    Entry *curr;
    explicit iterator(Entry *curr) : curr(curr) {}
};

template <typename T, typename HashS>
struct Linkedhs<T, HashS>::Entry {

    T value;
    Entry *prevInserted;
    Entry *nextInserted;

    Entry *nextCollision = nullptr;

    explicit Entry(T value, Entry *prevInserted = nullptr, Entry *nextInserted = nullptr) :
        value(value), prevInserted(prevInserted), nextInserted(nextInserted) {}
};
