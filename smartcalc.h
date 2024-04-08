#ifndef CPP3_1_SMARTCALCMODEL_H
#define CPP3_1_SMARTCALCMODEL_H

#include <cmath>
#include <list>
#include <map>
#include <stack>
#include <string>

namespace smartcalc {

class SmartCalc {
 public:
  SmartCalc();
  SmartCalc(std::string infix);
  SmartCalc(std::string infix, double x);

  const std::string &getInfix() const { return infix_; }
  const std::string &getRpn() const { return rpn_; }
  double getX() const { return x_; }
  double getResult() const { return result_; }

  void setInfix(const std::string &infix) { infix_ = infix; }
  void setRpn(const std::string &rpn) { rpn_ = rpn; }
  void setX(double x) { x_ = x; }

  std::string generateRpn();
  double calculate();

 private:
  int priorityOf(const std::string &s) { return std::get<0>(operation_[s]); }
  bool leftAssoc(const std::string &s) { return (std::get<1>(operation_[s])); }
  size_t countOfOperands(std::string &s) { return std::get<3>(operation_[s]); }
  bool isStart(long unsigned i);
  bool needPopOnPriority(int prior);
  void popBeforeLeftPar();
  void popFromStackToRpn();
  void pushWithCheck(const std::string &str);

  static double mySum(double value) { return stack_.top() + value; }
  static double myMinus(double value) { return stack_.top() - value; }
  static double myMul(double value) { return stack_.top() * value; }
  static double myDiv(double value) { return stack_.top() / value; }
  static double myMod(double value) { return fmod(stack_.top(), value); }
  static double myPow(double value) { return pow(stack_.top(), value); }
  static double myNegativ(double value) { return -value; }

  enum priority {
    leftPar,
    rightPar,
    low,
    middle,
    high,
    function,
    unaryMinus,
    varX
  };
  enum associativity { right, left };

  using map_tuple = std::tuple<priority, bool, double (*)(double), int>;
  using map_pair = std::pair<std::string, map_tuple>;

  static inline std::map<std::string, map_tuple> operation_{
      map_pair("(", map_tuple(leftPar, left, nullptr, 0)),
      map_pair(")", map_tuple(rightPar, left, nullptr, 0)),
      map_pair("+", map_tuple(low, left, mySum, 2)),
      map_pair("-", map_tuple(low, left, myMinus, 2)),
      map_pair("*", map_tuple(middle, left, myMul, 2)),
      map_pair("/", map_tuple(middle, left, myDiv, 2)),
      map_pair("mod", map_tuple(middle, left, myMod, 2)),
      map_pair("^", map_tuple(high, right, myPow, 2)),
      map_pair("cos", map_tuple(function, left, cos, 1)),
      map_pair("sin", map_tuple(function, left, sin, 1)),
      map_pair("tan", map_tuple(function, left, tan, 1)),
      map_pair("acos", map_tuple(function, left, acos, 1)),
      map_pair("asin", map_tuple(function, left, asin, 1)),
      map_pair("atan", map_tuple(function, left, atan, 1)),
      map_pair("sqrt", map_tuple(function, left, sqrt, 1)),
      map_pair("log", map_tuple(function, left, log10, 1)),
      map_pair("ln", map_tuple(function, left, log, 1)),
      map_pair("!", map_tuple(unaryMinus, left, myNegativ, 1)),
      map_pair("x", map_tuple(varX, left, nullptr, 0))};

  static inline std::stack<double> stack_;
  std::stack<std::string> stackStr_;

  std::string infix_;
  std::string rpn_;

  double x_;
  double result_;
};

}  // namespace smartcalc

#endif  // CPP3_1_SMARTCALCMODEL_H
