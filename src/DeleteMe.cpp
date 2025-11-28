#include "DeleteMe.h"

DeleteMe::DeleteMe() {
  m_lastResult = 0;
}

DeleteMe::~DeleteMe() {}

bool DeleteMe::add(int addend1, int addend2) {
  m_lastResult = addend1 + addend2;
  return true;
}

// This is for checking that code coverage will not be <100%
bool DeleteMe::mul(int factor1, int factor2) {
  m_lastResult = factor1 * factor2;
  return true;
}

bool DeleteMe::div(int dividend, int divisor) {
  if (divisor == 0) {
    m_lastResult = 0;
    return false;
  }

  m_lastResult = dividend / divisor;
  return true;
}

int DeleteMe::get_last_result() {
  return m_lastResult;
}
