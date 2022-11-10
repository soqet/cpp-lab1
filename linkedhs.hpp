#pragma once

#include "linkedhs.h"
#include <algorithm>

template <typename T, typename HashS>
T Linkedhs<T, HashS>::iterator::operator*() {
    return this->curr->value;
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::iterator Linkedhs<T, HashS>::iterator::operator++() {
    if (this->curr != nullptr) {
        this->curr = this->curr->nextInserted;
    }
    return *this;
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::iterator Linkedhs<T, HashS>::iterator::operator++(int) {
    auto savedCopy = *this;
    ++*this;
    return savedCopy;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::iterator::operator==(const iterator& other) const {
    return this->curr == other.curr;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::iterator::operator!=(const iterator& other) const {
    return !(*this == other);
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::Linkedhs(size_t capacity) : capacity(capacity) { //???
    this->buckets = new Entry*[this->capacity]();
}


template <typename T, typename HashS>
Linkedhs<T, HashS>::Linkedhs(const Linkedhs<T, HashS> &other) : capacity(other.capacity) { 
    this->buckets = new Entry*[other.capacity]();
    for (auto i = other.begin(); i != other.end(); i++) {
        this->insert(*i);
    }
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::~Linkedhs() {
    this->clear();
    delete[] this->buckets;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::insert(const T & e) {
    const double resizeCondition = 0.75;
    if (this->size() + 1 >= resizeCondition * this->capacity) {
        this->resize();
    }
    Entry *newEntry = new Entry(e);
    if (!this->insertEntry(newEntry)) {
        delete newEntry;
        return false;
    }
    if (this->last != nullptr) { // size > 0
        this->last->nextInserted = newEntry;
    } else {
        this->first = newEntry;
    }
    newEntry->prevInserted = this->last;
    this->last = newEntry;
    this->count++;
    return true;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::remove(const T &v) {
    auto h = this->hash(v);
    auto idx = this->bucketIdx(h);
    if (this->buckets[idx] == nullptr)
        return false;
    auto curr = this->buckets[idx];
    Entry *entry = nullptr;
    Entry *prev = nullptr;
    if (curr->value == v) {
        entry = curr;
        prev = nullptr;
    } else {
        while (curr->nextCollision != nullptr) {
            if (curr->nextCollision->value == v) {
                entry = curr->nextCollision;
                prev = curr;
                break;
            }
            curr = curr->nextCollision;
        }
        if (entry == nullptr) {
            return false;
        }
    }
    if (entry == nullptr) {
        return false;
    }
    if (prev != nullptr) { // if this element have collision
        prev->nextCollision = entry->nextCollision;
    } else {
        this->buckets[idx] = nullptr;
    }
    
    if (entry->prevInserted != nullptr) { // if not first
        entry->prevInserted->nextInserted = entry->nextInserted;
    } else {
        this->first = entry->nextInserted;
    }
    if (entry->nextInserted != nullptr) { // if not last
        entry->nextInserted->prevInserted = entry->prevInserted;
    } else {
        this->last = entry->prevInserted;
    }
    delete entry;
    this->count--;
    return true;
}

template <typename T, typename HashS>
void Linkedhs<T, HashS>::swap(Linkedhs<T, HashS> &other) {
    std::swap(this->first, other.first);
    std::swap(this->last, other.last);

    std::swap(this->buckets, other.buckets);
    std::swap(this->capacity, other.capacity);
    std::swap(this->count, other.count);
}

template <typename T, typename HashS>
size_t Linkedhs<T, HashS>::size() const {
    return this->count;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::empty() const {
    return this->size() == 0;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::contains(const T &v) const {
    return this->end() != this->find(v);
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::iterator Linkedhs<T, HashS>::find(const T &v) const {
    auto e = this->get(v);
    if (e == nullptr) {
        return this->end();
    }
    return iterator(e);
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::operator==(const Linkedhs<T, HashS> &other) const {
    if (this->size() != other.size()) {
        return false;
    }
    for (T element : *this) {
        if (!other.contains(element)) {
            return false;
        }
    }
    return true;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::operator!=(const Linkedhs<T, HashS> &other) const {
    return !(*this == other);
}

template <typename T, typename HashS>
Linkedhs<T, HashS>& Linkedhs<T, HashS>::operator=(const Linkedhs<T, HashS>& other) {
    if (this == &other) {
        return *this;
    }
    this->clear();
    auto copy = Linkedhs(other);
    this->swap(copy);
    return *this;
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::iterator Linkedhs<T, HashS>::begin() const {
    return iterator(this->first);
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::iterator Linkedhs<T, HashS>::end() const {
    return iterator(nullptr);
}

template <typename T, typename HashS>
void Linkedhs<T, HashS>::clear() {
    if (this->first == nullptr) {
        return;
    }
    auto curr = this->first; 
    while (curr->nextInserted != nullptr) {
        curr = curr->nextInserted;
        delete curr->prevInserted;
    }
    delete curr; 
    this->first = nullptr;
    this->last = nullptr;
    std::fill_n(buckets, capacity, nullptr);
    this->count = 0;
}

template <typename T, typename HashS>
Linkedhs<T, HashS>::Entry* Linkedhs<T, HashS>::get(const T &v) const {
    auto h = this->hash(v);
    auto idx = this->bucketIdx(h);
    if (this->buckets[idx] == nullptr)
        return nullptr;
    auto curr = this->buckets[idx];
    if (curr->value == v) {
        return curr;
    }
    while (curr != nullptr) {
        if (curr->value == v) {
            return curr;
        }
        curr = curr->nextCollision;
    }
    return nullptr;
}

template <typename T, typename HashS>
void Linkedhs<T, HashS>::resize() {
    const int maxIncrease = 1024;
    if (this->capacity >= maxIncrease) {
        this->capacity += maxIncrease;
    } else {
        this->capacity *= 2;
    }
    delete[] this->buckets;
    this->buckets = new Entry*[this->capacity]();
    if (this->first == nullptr) {
        return;
    }
    auto curr = this->first; 
    while (curr->nextInserted != nullptr) {
        curr = curr->nextInserted;
        this->insertEntry(curr->prevInserted);
    }
    this->insertEntry(curr);
    return;
}

template <typename T, typename HashS>
bool Linkedhs<T, HashS>::insertEntry(Entry *entry) {
    auto bi = this->bucketIdx(this->hash(entry->value));
    if (this->buckets[bi] == nullptr) {
        this->buckets[bi] = entry;
    } else { // if bucket has elements
        auto curr = this->buckets[bi];
        if (curr->value == entry->value) {
            return false;
        }
        while (curr->nextCollision != nullptr) {
            if (curr->value == entry->value) {
                return false;
            }
            curr = curr->nextCollision;
        }
        curr->nextCollision = entry;
    }
    return true;
}