#include <gtest/gtest.h>
#include "linkedhs.hpp"

struct fHashInt {
    size_t operator()(const int &v) {
        return v;
    }
};


TEST(insert, firstInsert) {
  Linkedhs<int, fHashInt> lhs(10);
  lhs.insert(10);
  ASSERT_EQ(lhs.empty(), false);
}
