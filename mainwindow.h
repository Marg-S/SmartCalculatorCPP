#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "qlabel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void checkState();
  void setGraphRange();
  void pushButton_clicked();
  void on_pushButton_del_clicked();
  void on_pushButton_ac_clicked();
  void on_pushButton_result_clicked();
  void on_pushButtonGraph_ac_clicked();
  void on_pushButtonGraph_result_clicked();
  void on_doubleSpinBox_xmin_valueChanged(double arg1);
  void on_pushButtonCredit_clicked();
  void on_lineEdit_textChanged(const QString &arg1);

 private:
  Ui::MainWindow *ui;
  QLabel *parentheses;
  qsizetype leftPar_{0};
  qsizetype rightPar_{0};
  qsizetype count_{0};
  enum states_ { start, click, equalPressed };
  int state_ = start;
};

#endif  // MAINWINDOW_H
