#ifndef TESTLIBRTTTL_H
#define TESTLIBRTTTL_H

#include <gtest/gtest.h>

class TestRTTTL : public ::testing::Test
{
public:
  virtual void SetUp();
  virtual void TearDown();
};

#endif //TESTLIBRTTTL_H
