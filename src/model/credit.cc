#include "model.h"

namespace s21 {

std::vector<CreditResult> Model::CreditCalc(CreditRequest data) const {
  std::vector<CreditResult> res;
  double percent_mounth = data.percent / 1200;
  double payment =
      data.amount *
      (percent_mounth +
       percent_mounth / (pow(1 + percent_mounth, data.mounth) - 1));
  double remainder = data.amount, all_payed_percent = 0;
  for (int i = 0; i < data.mounth; i++) {
    CreditResult tmp;
    tmp.payment_percent = remainder * percent_mounth;
    if (data.type == 0) {
      remainder -= payment - tmp.payment_percent;
      tmp.payment = payment;
      tmp.payment_debt = payment - tmp.payment_percent;
    } else {
      payment = data.amount / data.mounth;
      remainder -= payment;
      tmp.payment = payment + tmp.payment_percent;
      tmp.payment_debt = payment;
    }
    all_payed_percent += tmp.payment_percent;
    tmp.remainder = remainder;
    res.push_back(tmp);
  }
  CreditResult tmp{};
  tmp.payment_percent = all_payed_percent;
  tmp.remainder = tmp.payment_percent + data.amount;
  res.push_back(tmp);
  return res;
}

}  // namespace s21
