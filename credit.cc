#include "credit.h"

namespace smartcalc {

Credit::Credit() : Credit(0., 0, false, 0, false) {}
Credit::Credit(double sum, int period, bool month, double percent,
               bool annuitet)
    : sum_(sum),
      period_(period),
      month_(month),
      percent_(percent),
      annuitet_(annuitet) {}

void Credit::calculate() {
  if (!month_) period_ *= 12;
  double monthPercent = percent_ / 1200;

  if (annuitet_) {
    double p = sum_ * monthPercent * pow(1 + monthPercent, period_) /
               (pow(1 + monthPercent, period_) - 1);
    payment_.push_back(p);
    payout_ = std::round(p * 100) / 100 * period_;

  } else {
    for (int month = 1; month <= period_; ++month) {
      double averageSum = sum_ / period_;
      double p = averageSum + (sum_ - (month - 1) * averageSum) * monthPercent;
      payment_.push_back(p);
      payout_ += p;
    }
  }

  overpayment_ = payout_ - sum_;
}

}  // namespace smartcalc
