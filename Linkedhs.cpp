#include <stdlib.h>
#include <string>
#include <vector>


// class HashStrat {

// };

template <typename T, size_t (*hashS)(const T&) = 0>
class Linkedhs {
    size_t hash(const T& e) {
        if hashS == 0 {
            return e.hash() % this.capacity;
        }
        else {
            return hashS(e) % this->capacity;
        }
    }

    class Entry {
    public:
        T value;
        long long hash;
        bool initialized;

        Entry *next;
        Entry *prev;

        Entry *coll;

        Entry() {
            this->initialized = false;
        }

        Entry(T value) {
            this->value = value;
            this->initialized = true;
        }
    };
    Entry **bucket;
    size_t capacity;
    Entry *first;
    Entry *last;

public:
    class iterator {
    public:
        T operator*() {
            return this->curr.value;
        }

        iterator operator++() {
            this->prev = this->curr;
            if (this->curr != nullptr) {
                this->curr = this->curr.next;
            }
            return this;
        }

        iterator operator--() {
            this->curr = this.prev;
            if (this->curr != nullptr) {
                this.prev = this.curr.prev;
            }
            return this;
        }

        bool operator==(const iterator& other) const {
            return this->curr == other.curr;
        }

        bool operator!=(const iterator& other) const {
            return !(this == other);
        }
    private:
        Entry *curr;
        Entry *prev;
    };

    Linkedhs() {
        const int capacity = 100;
        this->bucket = new Entry*[capacity]();
        this->capacity = capacity;
        this->first = nullptr;
        this->last = nullptr;
    }

    Linkedhs(size_t capacity) {
        this->bucket = new Entry*[capacity]();
        this->capacity = capacity;
    }

    ~Linkedhs() {
        delete[] this->bucket;
    }

    Linkedhs(const Linkedhs &other) {}

    bool insert(const T e) {
        long long h = this->hash(e) % this->capacity;
        Entry *newEntry = new Entry(e);
        this->last->next = newEntry;
        newEntry->prev = this->last;
        this->last = newEntry;
        if (this->bucket[h] == nullptr || !this->bucket[h]->initialized) {
            this->bucket[h] = newEntry;
            return true;
        }
        auto curr = this->bucket[h];
        while (this->bucket[h]->coll != nullptr) {
            curr = this->bucket[h]->coll;
        }
        curr->coll = newEntry;
        return true;
    }

    bool remove(const T &e) {}

    void swap(Linkedhs &other) {
        // Linkedhs<T> temp;
    }

    size_t size() const;

    bool empty() const {
        return this->size() == 0;
    }

    bool contains(const T &e) const;
    iterator find(const T &e) const;

    bool operator==(const Linkedhs &other) const;
    bool operator!=(const Linkedhs &other) const;

    iterator begin() {

    }

    iterator end() {
        
    } 
};

size_t hashInt(const int &a) {
    return a;
}

int main() {
    auto test = Linkedhs<int, hashInt>(10);
    test.insert(10);
    return 0;
}