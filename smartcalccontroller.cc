#include "smartcalccontroller.h"

namespace smartcalc {

SmartCalcController::SmartCalcController(QString expression, double x)
    : calculator_(expression.toStdString(), x) {
  try {
    calculator_.generateRpn();
    calculator_.calculate();
  } catch (const std::exception& e) {
    error_ = "Incorrect input";
  } catch (const char* s) {
    error_ = s;
  }
}

void SmartCalcController::calculate(double x) {
  calculator_.setX(x);
  calculate();
}

void SmartCalcController::calculate() {
  error_.clear();
  try {
    calculator_.calculate();
  } catch (const std::exception& e) {
    error_ = "Incorrect input";
  } catch (const char* s) {
    error_ = s;
  }
}

}  // namespace smartcalc
