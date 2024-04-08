#include "smartcalc.h"

#include <locale>

namespace smartcalc {

SmartCalc::SmartCalc() : SmartCalc("", 0) {}
SmartCalc::SmartCalc(std::string infix) : SmartCalc(infix, 0) {}
SmartCalc::SmartCalc(std::string infix, double x)
    : infix_(infix), rpn_(std::string()), x_(x) {}

double SmartCalc::calculate() {
  setlocale(LC_NUMERIC, "C");
  if (rpn_.empty()) generateRpn();
  stack_ = std::stack<double>();

  for (long unsigned i = 0; i < rpn_.length(); ++i) {
    if (isdigit(rpn_[i]) || rpn_[i] == '.') {
      std::string::size_type sz{};
      stack_.push(std::stod(&rpn_[i], &sz));
      i += sz - 1;
    } else if (rpn_[i] != ' ') {
      long unsigned start = i;
      for (; i < rpn_.length() && isalpha(rpn_[i]); ++i) {
      }
      std::string str = rpn_.substr(start, i-- - start);
      if (str.empty()) str = rpn_.substr(++i, 1);

      if (operation_.count(str) == 0 || str == "(") throw("Incorrect input");
      if (stack_.size() < countOfOperands(str)) {
        throw("Incorrect input");
      }
      if (str == "/" && stack_.top() == 0) {
        throw("Division by zero is not possible");
      }

      if (str == "x") {
        stack_.push(x_);
      } else if (countOfOperands(str) == 2) {
        double value = stack_.top();
        stack_.pop();
        stack_.top() = std::get<2>(operation_[str])(value);
      } else {
        stack_.top() = std::get<2>(operation_[str])(stack_.top());
      }
    }
  }

  if (stack_.size() != 1) throw("Incorrect input");
  result_ = stack_.top();
  stack_.pop();

  return result_;
}

std::string SmartCalc::generateRpn() {
  setlocale(LC_NUMERIC, "C");
  if (infix_.empty()) throw("The infix expression not found");

  rpn_.clear();

  for (long unsigned i = 0; i < infix_.length(); ++i) {
    while (isspace(infix_[i])) ++i;

    if (isStart(i) && (infix_[i] == '-' || infix_[i] == '+')) {
      if (infix_[i++] == '-') stackStr_.push("!");
    }

    long unsigned start = i;
    for (; i < infix_.length() && isalpha(infix_[i]); ++i) {
    }

    if (start != i) {
      pushWithCheck(infix_.substr(start, i-- - start));
    } else if (isdigit(infix_[i]) || infix_[i] == '.') {
      std::string::size_type sz{};
      std::stod(&infix_[i], &sz);
      rpn_ += infix_.substr(i, sz) + " ";
      i += sz - 1;
    } else if (!isspace(infix_[i]) && i < infix_.length()) {
      pushWithCheck(infix_.substr(i, 1));
    }
  }

  for (; !stackStr_.empty(); popFromStackToRpn()) {
  }

  return rpn_;
}

void SmartCalc::pushWithCheck(const std::string& str) {
  if (operation_.count(str) == 0) throw("Incorrect input");

  if (str == "x") {
    rpn_ += "x ";
  } else if (str == "(") {
    stackStr_.push("(");
  } else if (str == ")") {
    popBeforeLeftPar();
  } else {
    int priority = priorityOf(str);

    if (priority != function) {
      while (!stackStr_.empty() && needPopOnPriority(priority)) {
        popFromStackToRpn();
      }
    }
    stackStr_.push(str);
  }
}

void SmartCalc::popBeforeLeftPar() {
  while (!stackStr_.empty() && stackStr_.top() != "(") {
    popFromStackToRpn();
  }
  if (stackStr_.empty()) throw("Incorrect input");
  stackStr_.pop();

  if (!stackStr_.empty() && priorityOf(stackStr_.top()) == function) {
    popFromStackToRpn();
  }
}

bool SmartCalc::isStart(long unsigned i) {
  for (; i > 0 && isspace(infix_[i - 1]); --i) {
  }
  return (i == 0 || infix_[i - 1] == '(');
}

void SmartCalc::popFromStackToRpn() {
  rpn_ += stackStr_.top() + " ";
  stackStr_.pop();
}

bool SmartCalc::needPopOnPriority(int priority) {
  int stackPriority = priorityOf(stackStr_.top());

  return (stackPriority > priority ||
          (stackPriority == priority && leftAssoc(stackStr_.top())));
}

}  // namespace smartcalc
