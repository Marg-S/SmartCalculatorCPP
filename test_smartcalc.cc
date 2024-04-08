#include <cmath>
#include <exception>

#include "credit.h"
#include "gtest/gtest.h"
#include "smartcalc.h"

TEST(TestCreate, Test1) {
  smartcalc::SmartCalc sm;

  EXPECT_STREQ(sm.getInfix().c_str(), "");
  EXPECT_STREQ(sm.getRpn().c_str(), "");
  EXPECT_DOUBLE_EQ(sm.getX(), 0);
}

TEST(TestCreate, Test2) {
  smartcalc::SmartCalc sm("2+3");

  EXPECT_STREQ(sm.getInfix().c_str(), "2+3");
  EXPECT_STREQ(sm.getRpn().c_str(), "");
  EXPECT_DOUBLE_EQ(sm.getX(), 0);
}

TEST(TestCreate, Test3) {
  std::string str = "2+x";
  smartcalc::SmartCalc sm(str, .5);

  EXPECT_STREQ(sm.getInfix().c_str(), str.c_str());
  EXPECT_STREQ(sm.getRpn().c_str(), "");
  EXPECT_DOUBLE_EQ(sm.getX(), .5);
}

TEST(TestCreate, Test4) {
  const std::string str = "2+3";
  double x = 1.23;
  smartcalc::SmartCalc sm(str, x);

  EXPECT_STREQ(sm.getInfix().c_str(), str.c_str());
  EXPECT_STREQ(sm.getRpn().c_str(), "");
  EXPECT_DOUBLE_EQ(sm.getX(), 1.23);
}

TEST(TestCalculate, Test1) {
  std::string str = "1.5*2";
  std::string strGoal = "1.5 2 * ";
  double valueGoal = 1.5 * 2;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test2) {
  std::string str = "0";
  std::string strGoal = "0 ";
  double valueGoal = 0;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test3) {
  std::string str = "cos(2*1)";
  std::string strGoal = "2 1 * cos ";
  double valueGoal = cos(2 * 1);

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test4) {
  std::string str = "+2*1*sin(cos(25*x))";
  std::string strGoal = "2 1 * 25 x * cos sin * ";

  double x_1 = 0;
  double valueGoal1 = +2 * 1 * sin(cos(25 * x_1));

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = +2 * 1 * sin(cos(25 * x_2));

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test5) {
  std::string str = "002*1*sin(cos(25*x))";
  std::string strGoal = "002 1 * 25 x * cos sin * ";

  double x_1 = 0;
  double valueGoal1 = 002 * 1 * sin(cos(25 * x_1));

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = 002 * 1 * sin(cos(25 * x_2));

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test6) {
  std::string str = "ln(2*x*1)";
  std::string strGoal = "2 x * 1 * ln ";

  double x_1 = 0;
  double valueGoal1 = log(2 * x_1 * 1);

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = log(2 * x_2 * 1);

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test7) {
  std::string str = "sqrt(-2.2*x*(-1))";
  std::string strGoal = "2.2 ! x * 1 ! * sqrt ";

  double x_1 = 0;
  double valueGoal1 = sqrt(-2.2 * x_1 * (-1));

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = sqrt(-2.2 * x_2 * (-1));

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);

  double x_3 = -10;
  double valueGoal3 = sqrt(-2.2 * x_3 * (-1));

  sm.setX(x_3);

  EXPECT_TRUE(std::isnan(sm.calculate()));
  EXPECT_TRUE(std::isnan(valueGoal3));
}

TEST(TestCalculate, Test8) {
  std::string str = "0.5+ln(-2*x*(-1.9))";
  std::string strGoal = "0.5 2 ! x * 1.9 ! * ln + ";

  double x_1 = 0;
  double valueGoal1 = 0.5 + log(-2 * x_1 * (-1.9));

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = 0.5 + log(-2 * x_2 * (-1.9));

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test9) {
  std::string str = "-2mod3";
  std::string strGoal = "2 ! 3 mod ";
  double valueGoal = fmod(-2, 3);

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test10) {
  std::string str = "2+cos(sin(25)mod(sin(cos(3*x))))";
  std::string strGoal = "2 25 sin 3 x * cos sin mod cos + ";

  double x_1 = 0;
  double valueGoal1 = 2 + cos(fmod(sin(25), (sin(cos(3 * x_1)))));

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = 2 + cos(fmod(sin(25), (sin(cos(3 * x_2)))));

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test11) {
  std::string str = "-3^(-2)";
  std::string strGoal = "3 ! 2 ! ^ ";
  double valueGoal = pow(-3, -2);

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test12) {
  std::string str = " .123     *  5 *x    ";
  std::string strGoal = ".123 5 * x * ";

  double x_1 = 0;
  double valueGoal1 = .123 * 5 * x_1;

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal1);

  double x_2 = 3.14 / 2;
  double valueGoal2 = .123 * 5 * x_2;

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test13) {
  std::string str = "(100/x)";
  std::string strGoal = "100 x / ";

  double x_1 = 0;

  smartcalc::SmartCalc sm(str, x_1);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_ANY_THROW(sm.calculate());
  try {
    sm.calculate();
  } catch (const char* s) {
    EXPECT_STREQ(s, "Division by zero is not possible");
  }

  double x_2 = 3.14 / 2;
  double valueGoal2 = 100 / x_2;

  sm.setX(x_2);
  EXPECT_EQ(sm.calculate(), valueGoal2);
}

TEST(TestCalculate, Test14) {
  std::string str = "2^3^2";
  std::string strGoal = "2 3 2 ^ ^ ";
  double valueGoal = pow(2, pow(3, 2));

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test15) {
  std::string str = "2*cos(2)*.2";
  std::string strGoal = "2 2 cos * .2 * ";
  double valueGoal = 2 * cos(2) * .2;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test16) {
  std::string str = "1e6-1e4";
  std::string strGoal = "1e6 1e4 - ";
  double valueGoal = 1e6 - 1e4;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test17) {
  std::string str = "2e-2";
  std::string strGoal = "2e-2 ";
  double valueGoal = 2e-2;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_EQ(sm.calculate(), valueGoal);
}

TEST(TestCalculate, Test18) {
  std::string str = "ln 2.71828182845904";
  std::string strGoal = "2.71828182845904 ln ";
  double valueGoal = 1;

  smartcalc::SmartCalc sm(str);
  EXPECT_STREQ(sm.generateRpn().c_str(), strGoal.c_str());

  EXPECT_NEAR(sm.calculate(), valueGoal, 7);
}

TEST(TestCalculate, Test19) {
  std::string str = "cos cos cos";

  smartcalc::SmartCalc sm(str);
  EXPECT_ANY_THROW(sm.calculate());
  try {
    sm.calculate();
  } catch (const char* s) {
    EXPECT_STREQ(s, "Incorrect input");
  }
}

TEST(TestCalculate, Test20) {
  std::string str = "cos x * sin ( -2 )";

  smartcalc::SmartCalc sm;
  sm.setInfix(str);
  sm.generateRpn();

  smartcalc::SmartCalc sm2;
  sm2.setX(1.);
  sm2.setRpn(sm.getRpn());
  sm2.calculate();

  EXPECT_NEAR(sm2.getResult(), -0.491295, 7);

  smartcalc::SmartCalc sm3(str, 1.);
  sm3.calculate();
  EXPECT_NEAR(sm3.getResult(), -0.491295, 7);
}

TEST(TestCalculate, Test21) {
  std::string str = "(";

  smartcalc::SmartCalc sm(str);
  EXPECT_ANY_THROW(sm.calculate());
  try {
    sm.calculate();
  } catch (const char* s) {
    EXPECT_STREQ(s, "Incorrect input");
  }
}

TEST(TestCalculate, Test22) {
  std::string str = ")";

  smartcalc::SmartCalc sm(str);
  EXPECT_ANY_THROW(sm.calculate());
  try {
    sm.calculate();
  } catch (const char* s) {
    EXPECT_STREQ(s, "Incorrect input");
  }
}

TEST(TestCredit, Test1) {
  smartcalc::Credit credit(100000., 1, false, 10., true);
  credit.calculate();
  EXPECT_NEAR(credit.getPayment().front(), 8791.59, 2);
  EXPECT_NEAR(credit.getOverpayment(), 5499.08, 2);
  EXPECT_NEAR(credit.getPayout(), 105499.08, 2);
}

TEST(TestCredit, Test2) {
  smartcalc::Credit credit;

  credit.setSum(100000.);
  credit.setPeriod(12);
  credit.setMonth(true);
  credit.setPercent(10.);
  credit.setAnnuitet(false);

  credit.calculate();

  EXPECT_NEAR(credit.getPayment().front(), 9166.67, 2);
  EXPECT_NEAR(credit.getOverpayment(), 5416.67, 2);
  EXPECT_NEAR(credit.getPayout(), 105416.67, 2);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
