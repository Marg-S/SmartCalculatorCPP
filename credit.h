#ifndef CPP3_1_CREDITMODEL_H
#define CPP3_1_CREDITMODEL_H

#include <cmath>
#include <list>
#include <map>
#include <stack>
#include <string>

namespace smartcalc {

class Credit {
 public:
  Credit();
  Credit(double sum, int period, bool month, double percent, bool annuitet);

  void calculate();

  std::list<double> getPayment() const { return payment_; }
  double getOverpayment() const { return overpayment_; }
  double getPayout() const { return payout_; }

  void setSum(double sum) { sum_ = sum; }
  void setPeriod(double period) { period_ = period; }
  void setMonth(double month) { month_ = month; }
  void setPercent(double percent) { percent_ = percent; }
  void setAnnuitet(double annuitet) { annuitet_ = annuitet; }

 private:
  double sum_;
  int period_;
  bool month_;
  double percent_;
  bool annuitet_;

  std::list<double> payment_;
  double overpayment_;
  double payout_;
};

}  // namespace smartcalc

#endif  // CPP3_1_CREDITMODEL_H
