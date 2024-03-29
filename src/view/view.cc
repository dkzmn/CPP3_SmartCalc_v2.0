#include "view.h"

namespace s21 {

View::View(QWidget* parent) : QMainWindow(parent), ui(new Ui::View) {
  ui->setupUi(this);
  ui->dateEdit->setDate(QDate::currentDate());
  ui->table_dep->resizeColumnsToContents();
  connect(ui->btn_1, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_2, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_3, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_4, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_5, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_6, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_7, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_8, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_9, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_0, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_sum, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_sub, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_mul, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_div, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_dot, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_x, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_sin, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_cos, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_tan, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_asin, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_bopen, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_log, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_sqrt, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_atan, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_bclose, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_ln, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_acos, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_mod, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_pow, SIGNAL(clicked()), this, SLOT(AddText()));
  connect(ui->btn_back, SIGNAL(clicked()), this, SLOT(Backspace()));
  connect(ui->btn_eq, SIGNAL(clicked()), this, SLOT(Calculate()));
  connect(ui->btn_clr, SIGNAL(clicked()), this, SLOT(ClearInput()));
  connect(ui->btn_chart, SIGNAL(clicked()), this, SLOT(CreateChart()));
  connect(ui->btn_cred_calc, SIGNAL(clicked()), this, SLOT(CreditCalc()));
  connect(ui->btn_add_cap, SIGNAL(clicked()), this, SLOT(AddCap()));
  connect(ui->table_cap, SIGNAL(cellDoubleClicked(int, int)), this,
          SLOT(DeleteCap(int, int)));
  connect(ui->btn_dep_calc, SIGNAL(clicked()), this, SLOT(DepositCalc()));
}

View::~View() { delete ui; }

void View::AddText() {
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  QString str = button->text();
  if (str.length() > 1 && str != "mod") str = str + "(";
  ui->lineEdit->setText(ui->lineEdit->text() + str);
}

void View::AddClosingBrackets(QString& str) {
  int diff = str.count("(") - str.count(")");
  if (diff > 0)
    for (int i = 0; i < diff; i++) str += ")";
}

int View::GetComboBoxIndex(QComboBox* cb, QString str) {
  int res = -1;
  for (int i = 0; i < cb->count(); i++)
    if (cb->itemText(i) == str) {
      res = i;
      break;
    }
  return res;
}

void View::Backspace() {
  QString str = ui->lineEdit->text();
  if (str.endsWith("mod", Qt::CaseInsensitive)) {
    str.chop(3);
  } else if (str.endsWith("(", Qt::CaseInsensitive)) {
    str.chop(1);
    if (str.endsWith("ln", Qt::CaseInsensitive)) {
      str.chop(2);
    } else if (str.endsWith("asin", Qt::CaseInsensitive) ||
               str.endsWith("acos", Qt::CaseInsensitive) ||
               str.endsWith("atan", Qt::CaseInsensitive) ||
               str.endsWith("sqrt", Qt::CaseInsensitive)) {
      str.chop(4);
    } else if (str.endsWith("sin", Qt::CaseInsensitive) ||
               str.endsWith("cos", Qt::CaseInsensitive) ||
               str.endsWith("tan", Qt::CaseInsensitive) ||
               str.endsWith("log", Qt::CaseInsensitive)) {
      str.chop(3);
    }
  } else if (str.length() > 0) {
    str.chop(1);
  }
  ui->lineEdit->setText(str);
}

void View::SetResultSlot(long double res) {
  ui->label->setText(QString::number(res, 'g', 10));
}

void View::SetResultToPlotSlot(long double x, long double res) {
  x_.append(x);
  y_.append(res);
}

void View::SetCreditResultSlot(std::vector<CreditResult> res) {
  QDate date = QDate::currentDate();
  QString cur = " " + ui->comboBox_4->currentText();
  ui->table_credit->setRowCount(0);
  std::vector<CreditResult>::size_type vecSize = res.size();
  ui->table_credit->setEditTriggers(QTableWidget::NoEditTriggers);
  ui->table_credit->setRowCount(vecSize);
  for (std::vector<CreditResult>::size_type i = 0; i < vecSize - 1; i++) {
    date = date.addMonths(1);
    QString str = QLocale(QLocale::Russian).toString(date, "MMM yyyy");
    ui->table_credit->setItem(i, 0, new QTableWidgetItem(str));
    ui->table_credit->setItem(
        i, 1,
        new QTableWidgetItem(QString::number(res[i].payment, 'f', 2) + cur));
    ui->table_credit->setItem(
        i, 2,
        new QTableWidgetItem(QString::number(res[i].payment_percent, 'f', 2) +
                             cur));
    ui->table_credit->setItem(
        i, 3,
        new QTableWidgetItem(QString::number(res[i].payment_debt, 'f', 2) +
                             cur));
    ui->table_credit->setItem(
        i, 4,
        new QTableWidgetItem(QString::number(fabs(res[i].remainder), 'f', 2) +
                             cur));
  }
  ui->table_credit->setItem(vecSize - 1, 0,
                            new QTableWidgetItem("Общая выплата :"));
  ui->table_credit->setItem(
      vecSize - 1, 1,
      new QTableWidgetItem(QString::number(res[vecSize - 1].remainder, 'f', 2) +
                           cur));
  ui->table_credit->setItem(vecSize - 1, 3,
                            new QTableWidgetItem("Переплата :"));
  ui->table_credit->setItem(
      vecSize - 1, 4,
      new QTableWidgetItem(
          QString::number(res[vecSize - 1].payment_percent, 'f', 2) + cur));
  QFont font;
  font.setBold(true);
  ui->table_credit->item(vecSize - 1, 0)->setFont(font);
  ui->table_credit->item(vecSize - 1, 1)->setFont(font);
  ui->table_credit->item(vecSize - 1, 3)->setFont(font);
  ui->table_credit->item(vecSize - 1, 4)->setFont(font);
  ui->table_credit->resizeColumnsToContents();
}

void View::SetDepResultSlot(std::vector<DepResult> res) {
  QString cur = " " + ui->comboBox_6->currentText();
  ui->table_dep->setRowCount(0);
  std::vector<DepResult>::size_type vecSize = res.size();
  ui->table_dep->setEditTriggers(QTableWidget::NoEditTriggers);
  ui->table_dep->setRowCount(vecSize + 1);
  for (std::vector<DepResult>::size_type i = 0; i < vecSize - 1; i++) {
    QDate curr_date = QDate::currentDate().addDays(res[i].day);
    QString str = QLocale(QLocale::Russian).toString(curr_date, "dd.MM.yyyy");
    ui->table_dep->setItem(i, 0, new QTableWidgetItem(str));
    ui->table_dep->setItem(
        i, 1,
        new QTableWidgetItem(QString::number(res[i].percent, 'f', 2) + cur));
    ui->table_dep->setItem(
        i, 2,
        new QTableWidgetItem(QString::number(res[i].capitalization, 'f', 2) +
                             cur));
    ui->table_dep->setItem(
        i, 3,
        new QTableWidgetItem(QString::number(res[i].remainder, 'f', 2) + cur));
  }
  ui->table_dep->setItem(vecSize - 1, 0,
                         new QTableWidgetItem("Начисленные проценты :"));
  ui->table_dep->setItem(
      vecSize - 1, 1,
      new QTableWidgetItem(QString::number(res[vecSize - 1].percent, 'f', 2) +
                           cur));
  ui->table_dep->setItem(vecSize - 1, 2,
                         new QTableWidgetItem("Сумма на вкладе :"));
  ui->table_dep->setItem(
      vecSize - 1, 3,
      new QTableWidgetItem(
          QString::number(res[vecSize - 1].capitalization, 'f', 2) + cur));
  ui->table_dep->setItem(vecSize, 0, new QTableWidgetItem("Сумма налога :"));
  ui->table_dep->setItem(
      vecSize, 1,
      new QTableWidgetItem(QString::number(res[vecSize - 1].remainder, 'f', 2) +
                           cur));
  QFont font;
  font.setBold(true);
  ui->table_dep->item(vecSize - 1, 0)->setFont(font);
  ui->table_dep->item(vecSize - 1, 1)->setFont(font);
  ui->table_dep->item(vecSize - 1, 2)->setFont(font);
  ui->table_dep->item(vecSize - 1, 3)->setFont(font);
  ui->table_dep->item(vecSize, 0)->setFont(font);
  ui->table_dep->item(vecSize, 1)->setFont(font);
  ui->table_dep->resizeColumnsToContents();
}

void View::Calculate() {
  QString str = ui->lineEdit->text();
  AddClosingBrackets(str);
  ui->lineEdit->setText(str);
  std::string str2 = str.toStdString();
  double x = ui->doubleSpinBox->value();
  try {
    emit SetExprAndCalcSignal(str2, x);
  } catch (std::exception& e) {
    ui->label->setText(e.what());
  }
}

void View::ClearInput() {
  ui->lineEdit->setText("");
  ui->label->setText("");
  ui->widget->clearGraphs();
  ui->widget->replot();
}

void View::CreateChart() {
  QString str = ui->lineEdit->text();
  AddClosingBrackets(str);
  ui->lineEdit->setText(str);
  std::string str2 = str.toStdString();
  ui->widget->clearGraphs();
  ui->widget->replot();
  x_.clear();
  y_.clear();
  try {
    emit SetExprSignal(str2);
    ui->label->setText("Chart");
    int x1 = ui->spinBox->value();
    int x2 = ui->spinBox_2->value();
    int y1 = ui->spinBox_3->value();
    int y2 = ui->spinBox_4->value();
    long double step = ((long double)x2 - x1) / kCountDots;
    for (int i = 0; i < kCountDots; i++) {
      emit CalcSignal(x1 + i * step);
    }
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->xAxis->setRange(x1, x2);
    ui->widget->yAxis->setRange(y1, y2);
    QCPGraph* dwPoints = new QCPGraph(ui->widget->xAxis, ui->widget->yAxis);
    dwPoints->setAdaptiveSampling(false);
    dwPoints->setLineStyle(QCPGraph::lsNone);
    dwPoints->setScatterStyle(QCPScatterStyle::ssDot);
    dwPoints->setPen(QPen(QBrush(Qt::blue), 2));
    dwPoints->addData(x_, y_);
    ui->widget->replot();
  } catch (std::exception& e) {
    ui->label->setText(e.what());
  }
}

void View::CreditCalc() {
  CreditRequest data;
  data.percent = ui->doubleSpinBox_2->value();
  data.mounth = ui->spinBox_5->value();
  if (ui->comboBox_3->currentIndex() == 1) data.mounth *= 12;
  data.amount = ui->lineEdit_2->text().toDouble();
  data.type = ui->comboBox->currentIndex();
  if (data.amount > 0 && !std::isnan(data.amount) && !std::isinf(data.amount)) {
    CreditCalcSignal(data);
  }
}

void View::AddCap() {
  QString cur = " " + ui->comboBox_6->currentText();
  int count = ui->table_cap->rowCount();
  ui->table_cap->setRowCount(count + 1);
  QString cap_type = ui->comboBox_9->currentText();
  QString period = ui->comboBox_8->currentText();
  QString date_str = ui->dateEdit->date().toString("dd.MM.yyyy");
  QString cap_value = ui->lineEdit_4->text() + cur;
  ui->table_cap->setItem(count, 0, new QTableWidgetItem(cap_type));
  ui->table_cap->setItem(count, 1, new QTableWidgetItem(period));
  ui->table_cap->setItem(count, 2, new QTableWidgetItem(date_str));
  ui->table_cap->setItem(count, 3, new QTableWidgetItem(cap_value));
  ui->table_cap->setItem(count, 4, new QTableWidgetItem("Удалить"));
  ui->table_cap->resizeColumnsToContents();
}

void View::DeleteCap(int row, int column) {
  if (column == 4) {
    ui->table_cap->removeRow(row);
  }
}

void View::DepositCalc() {
  DepRequest dep_req;
  dep_req.percent = ui->doubleSpinBox_3->value();
  dep_req.mounth = ui->spinBox_6->value();
  if (ui->comboBox_5->currentIndex() == 1) dep_req.mounth *= 12;
  dep_req.amount = ui->lineEdit_3->text().toDouble();
  dep_req.tax = ui->doubleSpinBox_4->value();
  dep_req.capitalization = ui->comboBox_7->currentIndex();
  dep_req.period_payments = ui->comboBox_2->currentIndex();
  std::vector<CapRequest> c_vect;
  QDate date_start = QDate::currentDate();
  QDate date_finish = date_start.addMonths(dep_req.mounth);
  for (int i = 0; i < ui->table_cap->rowCount(); i++) {
    CapRequest tmp{};
    tmp.type =
        GetComboBoxIndex(ui->comboBox_9, ui->table_cap->item(i, 0)->text());
    tmp.period =
        GetComboBoxIndex(ui->comboBox_8, ui->table_cap->item(i, 1)->text());
    QString amount_str = ui->table_cap->item(i, 3)->text();
    amount_str.chop(2);
    tmp.amount = amount_str.toDouble();
    QDate cap_date =
        QDate::fromString(ui->table_cap->item(i, 2)->text(), "dd.MM.yyyy");
    tmp.day = date_start.daysTo(cap_date);
    c_vect.push_back(tmp);
  }
  if (dep_req.amount > 0 && !std::isnan(dep_req.amount) &&
      !std::isinf(dep_req.amount)) {
    emit DepositCalcSignal(dep_req, c_vect);
  }
}

}  // namespace s21
