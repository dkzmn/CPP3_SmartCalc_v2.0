#include <locale.h>

#include <QApplication>

#include "./controller/controller.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_ALL, "en_US.UTF-8");
  s21::View v;
  s21::Model m;
  s21::Controller c(&m, &v);

  QObject::connect(&c, SIGNAL(SetResultSignal(long double)), &v,
                   SLOT(SetResultSlot(long double)));
  QObject::connect(&c, SIGNAL(SetResultToPlotSignal(long double, long double)),
                   &v, SLOT(SetResultToPlotSlot(long double, long double)));
  QObject::connect(&c, SIGNAL(SetCreditResultSignal(std::vector<CreditResult>)),
                   &v, SLOT(SetCreditResultSlot(std::vector<CreditResult>)));
  QObject::connect(&c, SIGNAL(SetDepResultSignal(std::vector<DepResult>)), &v,
                   SLOT(SetDepResultSlot(std::vector<DepResult>)));
  QObject::connect(&v, SIGNAL(SetExprAndCalcSignal(std::string, long double)),
                   &c, SLOT(SetExprAndCalcSlot(std::string, long double)));
  QObject::connect(&v, SIGNAL(SetExprSignal(std::string)), &c,
                   SLOT(SetExprSlot(std::string)));
  QObject::connect(&v, SIGNAL(CalcSignal(long double)), &c,
                   SLOT(CalcSlot(long double)));
  QObject::connect(&v, SIGNAL(CreditCalcSignal(CreditRequest)), &c,
                   SLOT(CreditCalcSlot(CreditRequest)));
  QObject::connect(
      &v, SIGNAL(DepositCalcSignal(DepRequest, std::vector<CapRequest>)), &c,
      SLOT(DepositCalcSlot(DepRequest, std::vector<CapRequest>)));

  v.show();
  return a.exec();
}
