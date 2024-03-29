#ifndef CPP3_SMARTCALC_V2_SRC_STRUCT_H_
#define CPP3_SMARTCALC_V2_SRC_STRUCT_H_

#include <string>

namespace s21 {

const std::string kDSym = "0123456789.e";
const std::string kOSym = "+-*/%^";
const std::string kFSym = "sicotnqlg";
const std::string kUSym = "~?";
const std::string kBoSym = "(";
const std::string kBcSym = ")";
const std::string kVSym = "x";
const std::string kASym =
    kDSym + kOSym + kFSym + kUSym + kBoSym + kBcSym + kVSym;
const std::string kOFUSym = kOSym + kFSym + kUSym;
const std::string kFUSym = kFSym + kUSym;
const std::string kOBoSym = kOSym + kBoSym;
const std::string kOBcSym = kOSym + kBcSym;
const std::string kFUBoSym = kFSym + kBoSym + kUSym;

typedef struct {
  double percent{};
  int mounth{};
  double amount{};
  int type{};
} CreditRequest;

typedef struct {
  double payment{};
  double payment_percent{};
  double payment_debt{};
  double remainder{};
} CreditResult;

typedef struct {
  double percent{};
  int mounth{};
  double amount{};
  double tax{};
  int capitalization{};
  int period_payments{};
} DepRequest;

typedef struct {
  int type{};
  int period{};
  int day{};
  double amount{};
} CapRequest;

typedef struct {
  int day{};
  double percent{};
  double capitalization{};
  double remainder{};
} DepResult;

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_STRUCT_H_
