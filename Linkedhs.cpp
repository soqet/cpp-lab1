#include <stdlib.h>
#include <memory.h>
#include <string>
#include <vector>
#include <iostream>

template <typename T, size_t (*hashS)(const T&)> // TODO: change to functor
class Linkedhs {
    size_t bucketIdx(const T& e) {
        // if hashS == 0 {
        //     return e.hash() % this.capacity;
        // }
        // else {
        //     return hashS(e) % this->capacity;
        // }
        return hashS(e) % this->capacity;
    }

    class Entry {
    
        friend class Linkedhs;
        friend class iterator;

        T value;
        size_t hash;

        Entry *prev;
        Entry *next;

        Entry *coll = nullptr;

        Entry(T value, size_t hash, Entry *prev , Entry *next) : 
        value(value), hash(hash), prev(prev), next(next) {}

        Entry(T value, size_t hash) :
        value(value), hash(hash) {
            this->prev = nullptr;
            this->next = nullptr;
        }
    };

    Entry **bucket;
    size_t capacity;
    size_t count = 0;

    Entry *first = nullptr;
    Entry *last = nullptr;

    Entry* get(const T &v) {
        auto idx = this->bucketIdx(v);
        if (this->bucket[idx] == nullptr)
            return nullptr;
        auto curr = this->bucket[idx];
        if (curr->value == v) {
            return curr;
        }
        while (curr->coll != nullptr) {
            if (curr->value == v) {
                return curr;
            }
            curr = curr->coll;
        }
        return nullptr;
    }

public:
    class iterator {

    friend class Linkedhs;
    
    iterator(Entry *curr, Entry *prev) :
    prev(prev), curr(curr) {}

    public:
        T operator*() {
            return this->curr->value;
        }

        iterator operator++() {
            this->prev = this->curr;
            if (this->curr != nullptr) {
                this->curr = this->curr->next;
            }
            return *this;
        }

        iterator operator--() {
            this->curr = this.prev;
            if (this->curr != nullptr) {
                this->prev = this->curr->prev;
            }
            return *this;
        }

        bool operator==(const iterator& other) const {
            return this->curr == other.curr;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    private:
        Entry *prev;
        Entry *curr;
    };

    Linkedhs() {
        this->capacity = 100;
        this->bucket = new Entry*[this->capacity]();
    }

    Linkedhs(size_t capacity) : capacity(capacity) {
        this->bucket = new Entry*[this->capacity]();
    }

    ~Linkedhs() {
        delete[] this->bucket;
        if (this->first == nullptr) {
            return;
        }
        auto curr = this->first;
        while (curr->next != nullptr) {
            curr = curr->next;
            delete curr->prev;
        }
    }

    Linkedhs(const Linkedhs<T, hashS> &other) : capacity(other.capacity) { 
        this->bucket = new Entry*[other.capacity]();
        for (auto& i = other.begin(); i != other.end(); ++i) {
            this->insert(*i);
        }
    }


    bool insert(const T e) {
        size_t h = this->bucketIdx(e);
        Entry *newEntry = new Entry(e, h);
        if (this->bucket[h] == NULL) {
            this->bucket[h] = newEntry;
        } else {
            auto curr = this->bucket[h];
            if (curr->value == e) {
                return false;
            }
            while (curr->coll != NULL) {
                if (curr->value == e) {
                    return false;
                }
                curr = this->bucket[h]->coll;
            }
            curr->coll = newEntry;
        }
        if (this->last != nullptr) {
            this->last->next = newEntry;
        } else {
            this->first = newEntry;
        }
        newEntry->prev = this->last;
        this->last = newEntry;
        this->count++;
        return true;
    }

    bool remove(const T &v) {
        auto entry = this->get(v);
        if (entry == nullptr) {
            return false;
        }
        if (entry->prev != nullptr) {
            entry->prev->next = entry->next;
        } else {
            this->first = entry->next;
        }
        if (entry->next != nullptr) {
            entry->next->prev = entry->prev;
        } else {
            this->last = entry->prev;
        }
        this->count--;
        return true;
    }

    void swap(Linkedhs &other) {
        
    }

    size_t size() const {
        return this->count;
    }

    bool empty() const {
        return this->size() == 0;
    }

    bool contains(const T &v) const {
        auto e = this->get(v);
        return e != nullptr;
    }

    iterator find(const T &v) const { // what if set doesnt contain v?
        auto e = this->get(v);
        if e == nullptr {
            return iterator(this->last, nullptr);
        }
        return iterator(e->prev, e);
    }

    bool operator==(const Linkedhs<T, hashS> &other) const {
        for (auto& i = this->begin(); i != this->end(); ++i) {
            if (!other.contains(*i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Linkedhs<T, hashS> &other) const {
        return !(*this == other);
    }

    iterator begin() const {
        return iterator(this->first, this->first->prev);
    }

    iterator end() const {
        return iterator(nullptr, this->last);
    }

    void clear() {
        if (this->first == nullptr) {
            return;
        }
        auto curr = this->first;
        while (curr->next != nullptr) {
            curr = curr->next;
            delete curr->prev;
        }
        this->first = nullptr;
        this->last = nullptr;
        memset(this->bucket, 0, this->capacity * sizeof(Entry**));
    }

};

size_t hashInt(const int &a) {
    return a;
}

int main() {
    auto test = Linkedhs<int, hashInt>(10);
    test.insert(10);
    test.insert(11);
    test.insert(100);
    auto test2 = new Linkedhs<int, hashInt>(test);
    test2->insert(-2);
    test.remove(11);
    for (auto& i = test.begin(); i != test.end(); ++i) {
        std::cout << *i << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    for (auto& i = test2->begin(); i != test2->end(); ++i) {
        std::cout << *i << std::endl;
    }
    return 0;
}