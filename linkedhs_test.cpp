#include "linkedhs.hpp"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>

struct Student {
    size_t age;
    size_t name;
    bool operator==(Student &other) {
        return this->age == other.age && this->name == other.name;
    }
};

struct fHashStudent {
    size_t operator()(const Student &v) {
        return v.age + (v.name << 32);
    }
};


struct fHashInt {
    size_t operator()(const int &v) {
        return v;
    }
};


TEST(Insert, IsEmpty) {
    Linkedhs<int> lhs(10);
    lhs.insert(10);
    EXPECT_FALSE(lhs.empty());
}

TEST(insert, size) {
    Linkedhs<int, fHashInt> lhs(10);
    int elements[13] = { 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (int i = 0; i < 13; i++) {
        lhs.insert(elements[i]);
        EXPECT_EQ(lhs.size(), i + 1);
        EXPECT_TRUE(lhs.contains(elements[i]));
    }
}

TEST(insert, sameElement) {
    Linkedhs<int, fHashInt> lhs(10);
    int elements[7] = { 0, 0, 0, 0, 0, 0, 0}; 
    for (auto e : elements) {
        lhs.insert(e);
        EXPECT_EQ(lhs.size(), 1);
    }
}

TEST(insert, resize) {
    Linkedhs<int, fHashInt> lhs(2);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (int i = 0; i < 13; i++) {
        lhs.insert(v[i]);
        EXPECT_EQ(lhs.size(), i + 1);
        EXPECT_TRUE(lhs.contains(v[i]));
    }
}


TEST(iterator, basic) {
    Linkedhs<int, fHashInt> lhs(10);
    int elements[13] = { 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 }; 
    for (auto e : elements) {
        lhs.insert(e);
    }
    EXPECT_EQ(lhs.size(), 13);
    auto iter = lhs.begin();
    for (int i = 0; i < 13; i++) {
        ASSERT_NE(iter, lhs.end());
        EXPECT_EQ(*iter, elements[i]);
        iter++;
        
    }
    EXPECT_EQ(iter, lhs.end());
}

TEST(iterator, find) {
    Linkedhs<int, fHashInt> lhs(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs.insert(e);
    }
    EXPECT_EQ(lhs.size(), 13);
    for (auto t : v) {
        auto iter = lhs.find(t);
        ASSERT_EQ(iter != lhs.end(), true);
        for (auto i = std::find(v.begin(), v.end(), t) ; i != v.end(); i++) {
            ASSERT_EQ(iter != lhs.end(), true);
            EXPECT_EQ(*iter, *i);
            iter++;
        }
        ASSERT_EQ(iter == lhs.end(), true);
    }
}

TEST(remove, first) {
    Linkedhs<int, fHashInt> lhs(10);
    lhs.insert(10);
    EXPECT_EQ(lhs.contains(10), true);
    lhs.remove(10);
    EXPECT_EQ(lhs.contains(10), false);
}

TEST(remove, advanced) {
    Linkedhs<int, fHashInt> lhs(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs.insert(e);
    }
    EXPECT_EQ(lhs.size(), 13);
    for (auto i = std::find(v.begin(), v.end(), 2); i != v.end(); i++) {
        lhs.remove(*i);
        EXPECT_EQ(lhs.contains(*i), false);
    }
}

TEST(base, equality) {
    Linkedhs<int, fHashInt> lhs1(10);
    Linkedhs<int, fHashInt> lhs2(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs1.insert(e);
    }
    for (auto i = v.rbegin(); i != v.rend(); ++i) {
        lhs2.insert(*i);
    }
    EXPECT_EQ(lhs1 == lhs2, true);
    EXPECT_EQ(lhs1 != lhs2, false);
}

TEST(base, notEquality) {
    Linkedhs<int, fHashInt> lhs1(10);
    Linkedhs<int, fHashInt> lhs2(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs1.insert(e);
    }
    for (auto i = v.rbegin(); i != v.rend(); ++i) {
        lhs2.insert(*i);
    }
    lhs2.remove(11);
    EXPECT_EQ(lhs1 == lhs2, false);
    EXPECT_EQ(lhs1 != lhs2, true);
}

TEST(base, copy) {
    Linkedhs<int, fHashInt> lhs(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs.insert(e);
    }
    auto cpy = Linkedhs<int, fHashInt>(lhs);
    EXPECT_EQ(lhs == cpy, true);
}

TEST(base, swap) {
    Linkedhs<int, fHashInt> lhs1(10);
    Linkedhs<int, fHashInt> lhs2(10);
    std::vector<int> v1({ 0, 10, 100, 11 });
    std::vector<int> v2({ 0, 10, 110, 11 });
    for (auto e : v1) {
        lhs1.insert(e);
    }
    for (auto e : v2) {
        lhs2.insert(e);
    }
    lhs1.swap(lhs2);
    EXPECT_EQ(lhs1.contains(100), false);
    EXPECT_EQ(lhs1.contains(110), true);
    EXPECT_EQ(lhs2.contains(100), true);
    EXPECT_EQ(lhs2.contains(110), false);
}

TEST(base, clear) {
    Linkedhs<int, fHashInt> lhs(10);
    std::vector<int> v({ 0, 10, 100, 11, 101, 2, 3, 4, 5, 6, 7, 8, 9 });
    for (auto e : v) {
        lhs.insert(e);
    }
    lhs.clear();
    EXPECT_EQ(lhs.empty(), true);
}

TEST(base, complexType) {
    Linkedhs<Student, fHashStudent> lhs(10);
    Student s1;
    s1.age = 1;
    s1.name = 10;
    Student s2;
    s2.age = 2;
    s2.name = 20;
    lhs.insert(s1);
    EXPECT_FALSE(lhs.empty());
    lhs.insert(s2);
    EXPECT_EQ(lhs.size(), 2);
    lhs.insert(s1);
    EXPECT_EQ(lhs.size(), 2);
}


TEST(base, assign) {
    Linkedhs<int> lhs1;
    Linkedhs<int> lhs2;
    lhs1.insert(0);
    lhs2 = lhs1;
    EXPECT_EQ(lhs1.size(), lhs2.size());
    lhs1.insert(1);
    EXPECT_NE(lhs1.size(), lhs2.size());
}
