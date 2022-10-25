#include <memory.h>
#include <utility>

// T - value type
// HashS - hash functor
template <typename T, typename HashS>
class Linkedhs {

    size_t bucketIdx(size_t h) const {
        return h % this->capacity;
    }

    size_t hash(const T& e) const {
        return HashS()(e);
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

    Entry* get(const T &v) const {
        auto h = this->hash(v);
        auto idx = this->bucketIdx(h);
        if (this->bucket[idx] == nullptr)
            return nullptr;
        auto curr = this->bucket[idx];
        if (curr->hash == h) {
            return curr;
        }
        while (curr != nullptr) {
            if (curr->hash == h) {
                return curr;
            }
            curr = curr->coll;
        }
        return nullptr;
    }

public:
    class iterator {

    friend class Linkedhs;

    Entry *prev;
    Entry *curr;
    
    iterator(Entry *curr, Entry *prev) :
    prev(prev), curr(curr) {}

    public:
        // returns the element to which it points
        T operator*() {
            return this->curr->value;
        }

        // moves the iterator forward
        iterator operator++(int) {
            this->prev = this->curr;
            if (this->curr != nullptr) {
                this->curr = this->curr->next;
            }
            return *this;
        }

        // moves the iterator backwards
        iterator operator++() {
            this->curr = this->prev;
            if (this->curr != nullptr) {
                this->prev = this->curr->prev;
            }
            return *this;
        }

        // check if iterator points to the same elements
        bool operator==(const iterator& other) const {
            return this->curr == other.curr && this->prev == other.prev;
        }

        // check if iterator points to the different elements
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };
    
    // construct hash set with default capacity (100) of hash table
    Linkedhs() {
        this->capacity = 100;
        this->bucket = new Entry*[this->capacity]();
    }

    // construct hash set with given capacity of hash table
    Linkedhs(size_t capacity) : capacity(capacity) {
        this->bucket = new Entry*[this->capacity]();
    }

    // destructor
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

    // copy constructor
    Linkedhs(const Linkedhs<T, HashS> &other) : capacity(other.capacity) { 
        this->bucket = new Entry*[other.capacity]();
        for (auto i = other.begin(); i != other.end(); i++) {
            this->insert(*i);
        }
    }

    // inserts element in set
    bool insert(const T e) {
        auto h = this->hash(e);
        auto bi = this->bucketIdx(h);
        Entry *newEntry = new Entry(e, h);
        if (this->bucket[bi] == nullptr) {
            this->bucket[bi] = newEntry;
        } else {
            auto curr = this->bucket[bi];
            if (curr->hash == h) {
                return false;
            }
            while (curr->coll != nullptr) {
                if (curr->hash == h) {
                    return false;
                }
                curr = curr->coll;
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

    // removes element from set
    bool remove(const T &v) {
        auto h = this->hash(v);
        auto idx = this->bucketIdx(h);
        if (this->bucket[idx] == nullptr)
            return false;
        auto curr = this->bucket[idx];
        Entry *entry = nullptr;
        Entry *prev = nullptr;
        if (curr->hash == h) {
            entry = curr;
            prev = nullptr;
        } else {
            while (curr->coll != nullptr) {
                if (curr->coll->hash == h) {
                    entry = curr->coll;
                    prev = curr;
                    break;
                }
                curr = curr->coll;
            }
            if (entry == nullptr) {
                return false;
            }
        }
        // auto entry = this->get(v);
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
        if (prev != nullptr) {
            prev->coll = entry->coll;
        } else {
            this->bucket[idx] = nullptr;
        }
        delete entry;
        this->count--;
        return true;
    }

    // swaps sets between objects
    void swap(Linkedhs<T, HashS> &other) {
        std::swap(this->first, other.first);
        std::swap(this->last, other.last);

        std::swap(this->bucket, other.bucket);
        std::swap(this->capacity, other.capacity);
        std::swap(this->count, other.count);
    }

    // returns number of elements in set
    size_t size() const {
        return this->count;
    }

    // checks if set's size is 0
    bool empty() const {
        return this->size() == 0;
    }

    // checks if set has element v
    bool contains(const T &v) const {
        auto e = this->get(v);
        return e != nullptr;
    }

    // returns a iterator pointing to v
    // returns end iterator if v doesnt presented in set
    iterator find(const T &v) const {
        auto e = this->get(v);
        if (e == nullptr) {
            return this->end();
        }
        return iterator(e, e->prev);
    }

    // checks if two sets contain same elements
    bool operator==(const Linkedhs<T, HashS> &other) const {
        for (auto i = this->begin(); i != this->end(); i++) {
            if (!other.contains(*i)) {
                return false;
            }
        }
        for (auto i = other.begin(); i != other.end(); i++) {
            if (!this->contains(*i)) {
                return false;
            }
        }
        return true;
    }

    // checks if two sets contain different elements 
    bool operator!=(const Linkedhs<T, HashS> &other) const {
        return !(*this == other);
    }

    // returns iterator pointing to the first element
    iterator begin() const {
        return iterator(this->first, this->first->prev);
    }

    // returns iterator pointing to the element after the last element
    iterator end() const {
        return iterator(nullptr, this->last);
    }

    // makes the set empty
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
        this->count = 0;
    }

};
