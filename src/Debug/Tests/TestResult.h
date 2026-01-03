#pragma once
#include <string>

enum TestStatus { OK, WARNING, ERROR };

class TestResult {

  TestResult(TestStatus status, std::string errorMessage) {
    _status = status;
    _message = errorMessage;
  }
public:
  static TestResult ok() { return TestResult(OK, ""); }
  static TestResult warning(const char* message) { return TestResult(WARNING, message); }
  static TestResult error(const char* message) { return TestResult(ERROR, message); }

  TestStatus status() const { return _status; }
  std::string getMessage() const { return _message; }

private:
  TestStatus _status = OK;
  std::string _message;
};