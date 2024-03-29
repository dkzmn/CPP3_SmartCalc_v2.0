#include "controller.h"

namespace s21 {

void Controller::SetExprAndCalcSlot(std::string str, long double x) {
  emit SetResultSignal(model_->SetExprAndCalc(str, x));
}

void Controller::SetExprSlot(std::string str) { model_->SetExpr(str); }

void Controller::CalcSlot(long double x) {
  SetResultToPlotSignal(x, model_->Calc(x));
}

void Controller::CreditCalcSlot(CreditRequest data) {
  SetCreditResultSignal(model_->CreditCalc(data));
}

void Controller::DepositCalcSlot(DepRequest d_data,
                                 std::vector<CapRequest> c_vect) {
  SetDepResultSignal(model_->DepositCalc(d_data, c_vect));
}

}  // namespace s21
