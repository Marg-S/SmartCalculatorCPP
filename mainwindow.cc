#include "mainwindow.h"

#include <QRegularExpressionValidator>
#include <iostream>

#include "smartcalccontroller.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(size());

  parentheses = new QLabel(this);
  parentheses->setText("symbols: " + QString::number(count_) +
                       " \t\t'(': " + QString::number(leftPar_) +
                       "\t')': " + QString::number(rightPar_));
  ui->statusbar->addWidget(parentheses);

  ui->comboBox->insertItem(0, "лет");
  ui->comboBox->insertItem(1, "месяцев");

  QString doubleNumber = "[+-]?([1-9]+[0-9]*|0)(\\.[0-9]*|)[Ee][+-]?[0-9]*";
  ui->lineEdit_x->setValidator(new QRegularExpressionValidator(
      QRegularExpression("^" + doubleNumber), this));

  setGraphRange();

  foreach (const QAbstractButton *button, ui->buttonGroup->buttons())
    connect(button, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
  foreach (const QAbstractButton *button, ui->buttonGroup_2->buttons())
    connect(button, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
  foreach (const QAbstractButton *button, ui->buttonGroup_3->buttons())
    connect(button, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));
  foreach (const QAbstractButton *button, ui->buttonGroup_4->buttons())
    connect(button, SIGNAL(clicked()), this, SLOT(pushButton_clicked()));

  connect(ui->pushButtonGraph_del, SIGNAL(clicked()), this,
          SLOT(on_pushButton_del_clicked()));
  connect(ui->lineEditGraph,
          QOverload<const QString &>::of(&QLineEdit::textChanged), this,
          &MainWindow::on_lineEdit_textChanged);
  connect(ui->doubleSpinBox_xmax,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_doubleSpinBox_xmin_valueChanged);
  connect(ui->doubleSpinBox_ymin,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_doubleSpinBox_xmin_valueChanged);
  connect(ui->doubleSpinBox_ymax,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &MainWindow::on_doubleSpinBox_xmin_valueChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::checkState() {
  if (state_ == equalPressed) {
    on_pushButton_ac_clicked();
    state_ = start;
  }
}

void MainWindow::setGraphRange() {
  double xMin = ui->doubleSpinBox_xmin->value();
  double xMax = ui->doubleSpinBox_xmax->value();
  double yMin = ui->doubleSpinBox_ymin->value();
  double yMax = ui->doubleSpinBox_ymax->value();

  ui->graphArea->xAxis->setRange(xMin, xMax);
  ui->graphArea->yAxis->setRange(yMin, yMax);
  ui->graphArea->replot();
}

void MainWindow::pushButton_clicked() {
  checkState();

  QString text = qobject_cast<QPushButton *>(sender())->text();
  if (text == "π") text = "3.14159265358979";
  if (text == "e") text = "2.71828182845904";
  if (text == "exp") text = "e";
  if (text == "√") text = "sqrt";
  if (text == "÷") text = "/";
  if (text == ")" && (leftPar_ <= rightPar_)) text = "";

  if (ui->buttonGroup_2->buttons().contains(sender()) ||
      ui->buttonGroup_4->buttons().contains(sender())) {
    QChar previousSymbol;
    int pos = (ui->buttonGroup_2->buttons().contains(sender()))
                  ? ui->lineEdit->cursorPosition()
                  : ui->lineEditGraph->cursorPosition();
    if (pos > 0) {
      previousSymbol = (ui->buttonGroup_2->buttons().contains(sender()))
                           ? ui->lineEdit->text()[pos - 1]
                           : ui->lineEditGraph->text()[pos - 1];
    }
    if (!(pos > 0 && (text == "-" || text == "+") && previousSymbol == 'e')) {
      text = " " + text + " ";
    }
  }

  if (ui->buttonGroup->buttons().contains(sender()) ||
      ui->buttonGroup_2->buttons().contains(sender())) {
    ui->lineEdit->insert(text);
  } else {
    ui->lineEditGraph->insert(text);
  }
}

void MainWindow::on_pushButton_del_clicked() {
  checkState();
  if (sender()->parent() == ui->gridLayoutWidget_2) {
    ui->lineEdit->backspace();
  } else {
    ui->lineEditGraph->backspace();
  }
}

void MainWindow::on_pushButton_ac_clicked() {
  state_ = start;

  ui->lineEdit->clear();
  ui->lineEdit_x->clear();
  ui->lineEdit_result->clear();
}

void MainWindow::on_pushButton_result_clicked() {
  ui->lineEdit->end(false);
  while (ui->lineEdit->text()[ui->lineEdit->cursorPosition() - 1] == '=' ||
         ui->lineEdit->text()[ui->lineEdit->cursorPosition() - 1] == ' ') {
    ui->lineEdit->backspace();
  }
  QString expression = ui->lineEdit->text();
  if (leftPar_ > rightPar_) {
    expression.append(
        QString::fromStdString(std::string((leftPar_ - rightPar_), ')')));
  }
  double x = ui->lineEdit_x->text().toDouble();

  smartcalc::SmartCalcController controller(expression, x);
  double result = controller.getResult();

  QString resultStr = (controller.getError().isEmpty())
                          ? QString::number(result, 'g', 10)
                          : controller.getError();
  if (std::isnan(result) || std::isinf(result)) resultStr = "Error";

  ui->lineEdit_result->setText(resultStr);

  expression.append(" =");
  ui->lineEdit->setText(expression);

  if (resultStr != "Error" && resultStr != "Incorrect input") {
    expression.remove(QLatin1String(" "));
    ui->plainTextEditHistory->moveCursor(QTextCursor::Start);
    ui->plainTextEditHistory->insertPlainText(expression + resultStr + '\n');
  }
  state_ = equalPressed;
}

void MainWindow::on_pushButtonGraph_ac_clicked() {
  ui->lineEditGraph->clear();
  ui->graphArea->clearGraphs();
  ui->graphArea->replot();
}

void MainWindow::on_pushButtonGraph_result_clicked() {
  if (ui->lineEditGraph->text().isEmpty()) return;

  QVector<double> x_, y_;
  QString expression = ui->lineEditGraph->text();
  if (leftPar_ > rightPar_) {
    expression.append(
        QString::fromStdString(std::string((leftPar_ - rightPar_), ')')));
    ui->lineEditGraph->setText(expression);
  }
  smartcalc::SmartCalcController controller(expression);

  setGraphRange();
  ui->graphArea->clearGraphs();

  if (controller.getError() == "Incorrect input") {
    QMessageBox box(this);
    box.setWindowTitle("Error");
    box.setText("Incorrect input");
    box.exec();
  } else {
    double xBegin = ui->doubleSpinBox_xmin->value();
    double xEnd = ui->doubleSpinBox_xmax->value();
    double step = (xEnd - xBegin) / ui->graphArea->size().rwidth();

    for (double x = xBegin; x <= xEnd; x += step) {
      x_.push_back(x);
      controller.calculate(x);
      if (controller.getError().isEmpty()) y_.push_back(controller.getResult());
    }

    ui->graphArea->addGraph();
    ui->graphArea->graph(0)->addData(x_, y_);
  }
  ui->graphArea->replot();
}

void MainWindow::on_doubleSpinBox_xmin_valueChanged(double arg1) {
  if (ui->doubleSpinBox_xmin->value() >= ui->doubleSpinBox_xmax->value() ||
      ui->doubleSpinBox_ymin->value() >= ui->doubleSpinBox_ymax->value()) {
    QMessageBox box(this);
    box.setWindowTitle("Error");
    box.setText("Expected xmin < xmax, ymin < ymax");
    box.exec();
    ui->graphArea->replot();
  } else {
    if (arg1 >= -1e6 && arg1 <= 1e6) on_pushButtonGraph_result_clicked();
  }
}

void MainWindow::on_pushButtonCredit_clicked() {
  double sum = ui->doubleSpinBox_sum->value();
  int period = ui->spinBox_period->value();
  bool month = ui->comboBox->currentIndex();
  double percent = ui->doubleSpinBox_percent->value();
  bool annuitet = ui->radioButton_annuitet->isChecked();

  smartcalc::CreditController controller(sum, period, month, percent, annuitet);
  controller.calculate();
  std::list<double> payments = controller.getPayment();

  ui->plainTextEdit->clear();
  if (ui->radioButton_annuitet->isChecked()) {
    ui->plainTextEdit->insertPlainText(
        QString::number(payments.front(), 'f', 2));
    payments.pop_front();
  }
  for (int i = 1; !payments.empty(); ++i) {
    ui->plainTextEdit->appendPlainText(QString::number(i) + ") ");
    ui->plainTextEdit->insertPlainText(
        QString::number(payments.front(), 'f', 2));
    payments.pop_front();
  }

  ui->plainTextEdit->verticalScrollBar()->setValue(0);
  ui->doubleSpinBox_overpayment->setValue(controller.getOverpayment());
  ui->doubleSpinBox_payout->setValue(controller.getPayout());
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
  leftPar_ = arg1.count('(');
  rightPar_ = arg1.count(')');
  count_ = arg1.size();
  parentheses->setText("symbols: " + QString::number(count_) +
                       " \t\t'(': " + QString::number(leftPar_) +
                       "\t')': " + QString::number(rightPar_));
}
