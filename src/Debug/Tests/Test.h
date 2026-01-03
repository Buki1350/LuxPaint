#pragma once
#include "TestResult.h"

class Test {
  public:
  virtual TestResult check() = 0;
  virtual ~Test() {}
};