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
enum ClusteringType
{
    Random,
    Kmeans
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

    QString fileGeneration(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration, ClusteringType ct, EvalType et, ReplacementType rt);
    QString nameGenerator(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration, ClusteringType ct, EvalType et, ReplacementType rt);
    void batchFileGenerator(QStringList slist);
    int functionNumber(QString name);
};

#endif // MAINWINDOW_H
