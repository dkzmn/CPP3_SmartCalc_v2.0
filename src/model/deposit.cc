#include "model.h"

namespace s21 {

void Model::AddDays(time_t *t, int days) const {
  tm *ltm = std::localtime(t);
  ltm->tm_mday += days;
  *t = std::mktime(ltm);
}

int Model::GetDay(time_t t) const {
  tm *ltm = std::localtime(&t);
  return ltm->tm_mday;
}

void Model::AddMounths(time_t *t, int mounths, int *o_day) const {
  tm *ltm = std::localtime(t);
  if (*o_day == -1)
    *o_day = ltm->tm_mday;
  else
    ltm->tm_mday = *o_day;
  ltm->tm_mon += mounths;
  if (ltm->tm_mon == 3 && ltm->tm_mday > 30)
    ltm->tm_mday = 30;
  else if (ltm->tm_mon == 5 && ltm->tm_mday > 30)
    ltm->tm_mday = 30;
  else if (ltm->tm_mon == 8 && ltm->tm_mday > 30)
    ltm->tm_mday = 30;
  else if (ltm->tm_mon == 10 && ltm->tm_mday > 30)
    ltm->tm_mday = 30;
  else if (ltm->tm_mon == 1 && ltm->tm_mday > 28) {
    int d = ltm->tm_mday, m = ltm->tm_mon, y = ltm->tm_year;
    *t = std::mktime(ltm);
    if (DaysInYear(*t) == 365 && d > 28)
      d = 28;
    else if (DaysInYear(*t) == 366 && d > 29)
      d = 29;
    ltm->tm_mday = d;
    ltm->tm_mon = m;
    ltm->tm_year = y;
  }
  *t = std::mktime(ltm);
}

int Model::DaysInYear(time_t t) const {
  tm *ltm = std::localtime(&t);
  return ltm->tm_year % 4 != 0 ? 365 : 366;
}

bool Model::CompareDates(time_t t1, time_t t2) const {
  tm *ts = std::localtime(&t1);
  int d1 = ts->tm_mday, m1 = ts->tm_mon, y1 = ts->tm_year;
  ts = std::localtime(&t2);
  int d2 = ts->tm_mday, m2 = ts->tm_mon, y2 = ts->tm_year;
  return (d1 == d2) && (m1 == m2) && (y1 == y2);
}

double Model::GetDelta(std::vector<CapRequest> cv, time_t ds, time_t de,
                       time_t di) const {
  double res = 0;
  for (std::vector<CapRequest>::size_type i = 0; i < cv.size(); i++) {
    int change = 0;
    time_t add_period = ds;
    AddDays(&add_period, cv[i].day);
    int original_day_p = GetDay(add_period);
    if (cv[i].period == 0) {
      if (CompareDates(di, add_period)) change = 1;
    } else {
      int period_month = 1;
      switch (cv[i].period) {
        case 2:
          period_month = 2;
          break;
        case 3:
          period_month = 3;
          break;
        case 4:
          period_month = 6;
          break;
        case 5:
          period_month = 12;
          break;
      }
      while (std::difftime(de, add_period) > 0) {
        if (CompareDates(add_period, di)) change = 1;
        AddMounths(&add_period, period_month, &original_day_p);
      }
    }
    if (change == 1)
      res = (cv[i].type == 0) ? res + cv[i].amount : res - cv[i].amount;
  }
  return res;
}

std::vector<DepResult> Model::DepositCalc(
    DepRequest d_data, std::vector<CapRequest> c_vect) const {
  std::vector<DepResult> res;
  time_t date_start = time(0);
  time_t date_iter = date_start, date_end = date_start;
  time_t cap_start_period = date_start, cap_end_period{};
  int original_day = -1, day_iter{};
  AddMounths(&date_end, d_data.mounth, &original_day);
  double percent_all = 0, percent_period = 0, amount = d_data.amount;
  while (std::difftime(date_end, date_iter) > 0) {
    AddDays(&date_iter, 1);
    day_iter++;
    percent_period += amount * d_data.percent / (100 * DaysInYear(date_iter));
    double delta = GetDelta(c_vect, date_start, date_end, date_iter);
    cap_end_period = cap_start_period;
    switch (d_data.period_payments) {
      case 0:
        AddDays(&cap_end_period, 1);
        break;
      case 1:
        AddDays(&cap_end_period, 7);
        break;
      case 2:
        AddMounths(&cap_end_period, 1, &original_day);
        break;
      case 3:
        AddMounths(&cap_end_period, 3, &original_day);
        break;
      case 4:
        AddMounths(&cap_end_period, 6, &original_day);
        break;
      case 5:
        AddMounths(&cap_end_period, 12, &original_day);
        break;
      case 6:
        cap_end_period = date_end;
        break;
    }
    if (CompareDates(date_iter, cap_end_period) ||
        (CompareDates(date_iter, date_end) && percent_period > 0)) {
      DepResult tmp{};
      if (d_data.capitalization == 0) amount += percent_period;
      percent_all += percent_period;
      tmp.percent = percent_period;
      tmp.remainder = amount;
      tmp.day = day_iter;
      res.push_back(tmp);
      percent_period = 0;
      cap_start_period = date_iter;
    }
    if (delta != 0) {
      DepResult tmp{};
      amount += delta;
      tmp.remainder = amount;
      tmp.capitalization = delta;
      tmp.day = day_iter;
      res.push_back(tmp);
      delta = 0;
    }
  }
  DepResult tmp{};
  tmp.capitalization = amount;
  tmp.percent = percent_all;
  tmp.remainder = percent_all * d_data.tax / 100 - 75000;
  if (tmp.remainder < 0) tmp.remainder = 0;
  res.push_back(tmp);
  return res;
}

}  // namespace s21
