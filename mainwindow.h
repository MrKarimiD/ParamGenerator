#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFile>

#include "fitnessDef.h"

enum ReplacementType
{
    Worst,
    RTR
};

enum EvalType
{
    SinglePop,
    MultiPop
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_mulitModel_radioButton_pressed();

    void on_singleModel_radioButton_pressed();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void fileGeneration(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration,EvalType et, ReplacementType rt);
    QString nameGenerator(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration, EvalType et, ReplacementType rt);
    int functionNumber(QString name);
};

#endif // MAINWINDOW_H
