#include "../model/model.h"
#include "gtest/gtest.h"

#define EPS_CALC 1e-8
#define EPS 1e-3

TEST(TestSmartCalc, Test_1) {
  s21::Model m1("sin(x)+cos(2*x)+0.01");
  for (long double x = -10; x < 10; x += 0.1) {
    long double res = m1.Calc(x);
    long double fres = sinl(x) + cosl(2 * x) + 0.01;
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_2) {
  s21::Model m1("sin(cos(2*x))+sqrt(x*123.321)+1.1e-14");
  for (long double x = 1; x < 10; x += 0.1) {
    long double res = m1.Calc(x);
    long double fres = sinl(cosl(2 * x)) + sqrtl(x * 123.321) + 1.1e-14;
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_3) {
  s21::Model m1("asin(x) / acos(x) + (x + 3) ^ 1.321 + .999e2");
  for (long double x = -1; x <= 1; x += 0.01) {
    long double res = m1.Calc(x);
    long double fres = asinl(x) / acosl(x) + powl(x + 3, 1.321) + .999e2;
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_4) {
  s21::Model m1("x^2+-x++x/2+xmod4");
  for (long double x = -100; x <= 100; x++) {
    long double res = m1.Calc(x);
    long double fres = x * x - x + x / 2 + fmodl(x, 4);
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_5) {
  s21::Model m1("tan(x) + atan(x) - 0.5e-3");
  for (long double x = 0; x <= 1; x += 0.01) {
    long double res = m1.Calc(x);
    long double fres = tanl(x) + atanl(x) - 0.0005;
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_6) {
  s21::Model m1("1-1/2+3/2+x-2*2/2^2");
  for (long double x = -100; x <= 100; x++) {
    long double res = m1.Calc(x);
    long double fres = 1.0 - 1.0 / 2.0 + 3.0 / 2.0 + x - 2.0 * 2.0 / powl(2, 2);
    EXPECT_EQ(res, fres);
  }
}

TEST(TestSmartCalc, Test_7) {
  s21::Model m1("(log(x*.2)+ln(x+2) - 2)/2");
  for (long double x = 0.01; x <= 1; x += 0.01) {
    long double res = m1.Calc(x);
    long double fres = (log10l(x * .2) + logl(x + 2) - 2) / 2;
    EXPECT_NEAR(res, fres, EPS_CALC);
  }
}

TEST(TestSmartCalc, Test_Simple) {
  s21::Model m1("0");
  EXPECT_EQ(m1.Calc(123.123), 0);
  s21::Model m2("000000000000000.00000000000");
  EXPECT_EQ(m2.Calc(-999999), 0);
  s21::Model m3("1.1e+10");
  EXPECT_EQ(m3.Calc(-123.123), 1.1e+10);
  s21::Model m4(".9e-14");
  EXPECT_EQ(m4.Calc(999999), .9e-14);
  s21::Model m5;
  EXPECT_EQ(m5.SetExprAndCalc("5+6+7+8++9+10"), 45);
}

TEST(TestSmartCalc, Test_Errors) {
  EXPECT_THROW(s21::Model m1(""), std::logic_error);
  EXPECT_THROW(s21::Model m1("   "), std::logic_error);
  EXPECT_THROW(s21::Model m1("asin(x)*()/acos(x)+(x+3)^1.3"), std::logic_error);
  EXPECT_THROW(s21::Model m1("asin(x)/acos(x)+(x+3)^1.321!"), std::logic_error);
  EXPECT_THROW(s21::Model m1("asin(x)acos(x)asin(x)acos(x)"), std::logic_error);
  EXPECT_THROW(s21::Model m1("1.1*tanx+87"), std::logic_error);
  EXPECT_THROW(s21::Model m1("sin(x)cos(x)"), std::logic_error);
  EXPECT_THROW(s21::Model m1("cos(2x)"), std::logic_error);
  EXPECT_THROW(s21::Model m1("tan()"), std::logic_error);
  EXPECT_THROW(s21::Model m1("(((1+2)-1)-1)-1)"), std::logic_error);
  EXPECT_THROW(s21::Model m1("/*/2-----3-3-3-3-3"), std::logic_error);
  EXPECT_THROW(s21::Model m1("1.111(x)2.222"), std::logic_error);
  EXPECT_THROW(s21::Model m1("(((((((((((0))))))))))))"), std::logic_error);
  EXPECT_THROW(s21::Model m1("(((((((((((((.09))))))))))))"), std::logic_error);
}
