#ifndef CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_
#define CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_

#include <QObject>

#include "../model/model.h"
#include "../struct.h"
#include "../view/view.h"

namespace s21 {

class View;

class Controller : public QObject {
  Q_OBJECT

 public:
  Controller() = default;
  Controller(Model* model, View* view) : model_(model), view_(view){};
  ~Controller() = default;

 public slots:
  void SetExprAndCalcSlot(std::string str, long double x);
  void SetExprSlot(std::string x);
  void CalcSlot(long double res);
  void CreditCalcSlot(CreditRequest data);
  void DepositCalcSlot(DepRequest d_data, std::vector<CapRequest> c_vect);

 signals:
  void SetResultSignal(long double res);
  void SetResultToPlotSignal(long double x, long double res);
  void SetCreditResultSignal(std::vector<CreditResult> res);
  void SetDepResultSignal(std::vector<DepResult> res);

 private:
  View* view_;
  Model* model_;
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_CONTROLLER_CONTROLLER_H_
