#ifndef SMARTCALCCONTROLLER_H
#define SMARTCALCCONTROLLER_H

#include <QString>

#include "credit.h"
#include "smartcalc.h"

namespace smartcalc {

class SmartCalcController {
 public:
  SmartCalcController() : SmartCalcController("", 0) {}
  SmartCalcController(QString s) : SmartCalcController(s, 0) {}
  SmartCalcController(QString expression, double x);

  QString getError() { return error_; }
  double getResult() { return calculator_.getResult(); }

  void setExpression(QString s) { calculator_.setInfix(s.toStdString()); }
  void calculate();
  void calculate(double x);

 private:
  SmartCalc calculator_;
  QString error_;
};

class CreditController {
 public:
  CreditController() : CreditController(0., 0, false, 0., false) {}
  CreditController(double sum, int period, bool month, double percent,
                   bool annuitet)
      : credit_(sum, period, month, percent, annuitet) {}

  void calculate() { credit_.calculate(); }

  std::list<double> getPayment() const { return credit_.getPayment(); }
  double getOverpayment() const { return credit_.getOverpayment(); }
  double getPayout() const { return credit_.getPayout(); }

  void setSum(double sum) { credit_.setSum(sum); }
  void setperiod(double period) { credit_.setPeriod(period); }
  void setmonth(double month) { credit_.setMonth(month); }
  void setpercent(double percent) { credit_.setPercent(percent); }
  void setannuitet(double annuitet) { credit_.setAnnuitet(annuitet); }

 private:
  Credit credit_;
};

#endif  // SMARTCALCCONTROLLER_H

}  // namespace smartcalc
