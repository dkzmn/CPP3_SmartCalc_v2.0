#ifndef CPP3_SMARTCALC_V2_SRC_MODEL_MODEL_H_
#define CPP3_SMARTCALC_V2_SRC_MODEL_MODEL_H_

#include <algorithm>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <list>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "../struct.h"

namespace s21 {

class Model {
 public:
  Model() = default;
  Model(std::string input_string);
  ~Model() = default;

  void SetExpr(std::string input_string);
  long double SetExprAndCalc(std::string input_string, long double x = 0);
  long double Calc(long double x) const;
  std::vector<CreditResult> CreditCalc(CreditRequest data) const;
  std::vector<DepResult> DepositCalc(DepRequest d_data,
                                     std::vector<CapRequest> c_vect) const;

 private:
  std::list<long double> rpn_list_;
  std::string input_string_;
  void FindAndReplace(std::string find_str, std::string replace_str);
  void PrepareString();
  void BuildRpnList();
  int Priority(char c) const;
  long double FuncCalculate(long double a, char c) const;
  long double MathCalculate(long double a, long double b, char c) const;
  void AddDays(time_t* t, int days) const;
  int GetDay(time_t t) const;
  void AddMounths(time_t* t, int mounths, int* o_day) const;
  int DaysInYear(time_t t) const;
  bool CompareDates(time_t t1, time_t t2) const;
  double GetDelta(std::vector<CapRequest> cv, time_t ds, time_t de,
                  time_t di) const;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_MODEL_MODEL_H_
