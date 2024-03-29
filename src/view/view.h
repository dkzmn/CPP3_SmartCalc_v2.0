#ifndef CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_H_
#define CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_H_

#include <QMainWindow>

#include "../struct.h"
#include "./ui_view.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {

class View : public QMainWindow {
  Q_OBJECT

 public:
  View(QWidget *parent = nullptr);
  ~View();

 public slots:
  void SetResultSlot(long double res);
  void SetResultToPlotSlot(long double x, long double res);
  void SetCreditResultSlot(std::vector<CreditResult> res);
  void SetDepResultSlot(std::vector<DepResult> res);

 signals:
  void SetExprAndCalcSignal(std::string str, long double x);
  void SetExprSignal(std::string str);
  void CalcSignal(long double x);
  void CreditCalcSignal(CreditRequest data);
  void DepositCalcSignal(DepRequest d_data, std::vector<CapRequest> c_vect);

 private slots:
  void AddText();
  void Backspace();
  void Calculate();
  void ClearInput();
  void CreateChart();
  void CreditCalc();
  void AddCap();
  void DeleteCap(int row, int column);
  void DepositCalc();

 private:
  int kCountDots = 100000;
  Ui::View *ui;
  QVector<double> x_, y_;
  void AddClosingBrackets(QString &str);
  int GetComboBoxIndex(QComboBox *cb, QString str);
};

}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_SRC_VIEW_VIEW_H_
