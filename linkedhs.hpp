#include <algorithm>

static const int DEFAULT_CAPACITY =  100;

// CR: separate definition and declaration
// T - value type
// HashS - hash functor
template <typename T, typename HashS = std::hash<T>>
class Linkedhs {
private:
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
        Entry *prevInserted;
        Entry *nextInserted;

        Entry *nextCollision = nullptr;

        Entry(T value, Entry *prevInserted = nullptr, Entry *nextInserted = nullptr) :
            value(value), prevInserted(prevInserted), nextInserted(nextInserted) {}
    };

    Entry **buckets;
    size_t capacity;
    size_t count = 0;

    Entry *first = nullptr;
    Entry *last = nullptr;

    Entry* get(const T &v) const {
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

    void resize() {
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

    bool insertEntry(Entry *entry) {
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

public:
    class iterator {

    friend class Linkedhs;

    Entry *curr;
    
    iterator(Entry *curr) : curr(curr) {}

    public:
        // returns the element to which it points
        T operator*() {
            return this->curr->value;
        }

        // moves the iterator forward
        iterator operator++(int) {
            auto savedCopy = *this;
            ++*this;
            return savedCopy;
        }

        // moves the iterator forward
        iterator operator++() {
            if (this->curr != nullptr) {
                this->curr = this->curr->nextInserted;
            }
            return *this;
        }

        // check if iterator points to the same elements
        bool operator==(const iterator& other) const {
            return this->curr == other.curr;
        }

        // check if iterator points to the different elements
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // construct hash set with given capacity of hash table
    Linkedhs(size_t capacity = DEFAULT_CAPACITY) : capacity(capacity) {
        this->buckets = new Entry*[this->capacity]();
    }

    // copy constructor
    Linkedhs(const Linkedhs<T, HashS> &other) : capacity(other.capacity) { 
        this->buckets = new Entry*[other.capacity]();
        for (auto i = other.begin(); i != other.end(); i++) {
            this->insert(*i);
        }
    }

    // destructor
    ~Linkedhs() {
        this->clear();
        delete[] this->buckets;
    }

    // inserts element in set
    bool insert(const T & e) {
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

    // removes element from set
    bool remove(const T &v) {
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

    // swaps sets between objects
    void swap(Linkedhs<T, HashS> &other) {
        std::swap(this->first, other.first);
        std::swap(this->last, other.last);

        std::swap(this->buckets, other.buckets);
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
        return this->end() != this->find(v);
    }

    // returns a iterator pointing to v
    // returns end iterator if v doesnt presented in set
    iterator find(const T &v) const {
        auto e = this->get(v);
        if (e == nullptr) {
            return this->end();
        }
        return iterator(e);
    }

    // checks if two sets contain same elements
    bool operator==(const Linkedhs<T, HashS> &other) const {
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

    // checks if two sets contain different elements 
    bool operator!=(const Linkedhs<T, HashS> &other) const {
        return !(*this == other);
    }

    Linkedhs<T, HashS>& operator=(const Linkedhs<T, HashS>& other) {
        if (this == &other) {
            return *this;
        }
        this->clear();
        auto copy = Linkedhs(other);
        this->swap(copy);
        return *this;
    }

    // returns iterator pointing to the first element
    iterator begin() const {
        return iterator(this->first);
    }

    // returns iterator pointing to the element after the last element
    iterator end() const {
        return iterator(nullptr);
    }

    // makes the set empty
    void clear() {
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
};
