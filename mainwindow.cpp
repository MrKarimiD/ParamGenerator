#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mulitModel_radioButton_pressed()
{
    ui->groupBox->setEnabled(true);
}

void MainWindow::on_singleModel_radioButton_pressed()
{
    ui->groupBox->setDisabled(true);
}

void MainWindow::on_pushButton_clicked()
{
    //Adding favorite problems
    QStringList problems;
    if(ui->Trap5_checkBox->isChecked())
        problems.append("Trap5");
    if(ui->MSP13_checkBox->isChecked())
        problems.append("MSP1-3");
    if(ui->MSP15_checkBox->isChecked())
        problems.append("MSP1-5");
    if(ui->MSP23_checkBox->isChecked())
        problems.append("MSP2-3");
    if(ui->MSP25_checkBox->isChecked())
        problems.append("MSP2-5");
    if(ui->MSP335_checkBox->isChecked())
        problems.append("MSP3-3v5");
    if(ui->MSP353_checkBox->isChecked())
        problems.append("MSP3-5v3");

    //Adding problem size
    QList<int> sizes;
    if(ui->S30_checkBox->isChecked())
        sizes.append(30);
    if(ui->S45_checkBox->isChecked())
        sizes.append(45);
    if(ui->S60_checkBox->isChecked())
        sizes.append(60);
    if(ui->S90_checkBox->isChecked())
        sizes.append(90);
    if(ui->S120_checkBox->isChecked())
        sizes.append(120);
    if(ui->S150_checkBox->isChecked())
        sizes.append(150);

    bool multiModelsIsSelected = ui->mulitModel_radioButton->isChecked();
    QList<int> numOfModels;
    bool sporadicModelBuilding;
    QStringList numOfIterations;
    QList<EvalType> evaluationTypes;
    QList<ClusteringType> clusteringTypes;

    if( multiModelsIsSelected )
    {
        //Adding number of models
        if(ui->N3_checkBox->isChecked())
            numOfModels.append(3);
        if(ui->N5_checkBox->isChecked())
            numOfModels.append(5);
        if(ui->N7_checkBox->isChecked())
            numOfModels.append(7);
        if(ui->N9_checkBox->isChecked())
            numOfModels.append(9);
        if(ui->N11_checkBox->isChecked())
            numOfModels.append(11);

        if(ui->Kmeans_checkBox->isChecked())
            clusteringTypes.append(Kmeans);
        if(ui->Random_checkBox->isChecked())
            clusteringTypes.append(Random);

        sporadicModelBuilding = ui->sporadic_checkBox->isChecked();

        if(ui->I1_checkBox->isChecked())
            numOfIterations.append("1");
        if(ui->I2_checkBox->isChecked())
            numOfIterations.append("2");
        if(ui->I3_checkBox->isChecked())
            numOfIterations.append("3");
        if(ui->IInf_checkBox->isChecked())
            numOfIterations.append("inf");

        if(ui->singlePop_checkBox->isChecked())
            evaluationTypes.append(SinglePop);
        if(ui->multiPop_checkBox->isChecked())
            evaluationTypes.append(MultiPop);
    }
    else
    {
        numOfModels.append(1);
        sporadicModelBuilding = false;
        numOfIterations.append("1");
        clusteringTypes.append(Random);
        evaluationTypes.append(SinglePop);
    }

    QList<ReplacementType> replacementTypes;
    if(ui->worstR_checkBox->isChecked())
        replacementTypes.append(Worst);
    if(ui->RTR_checkBox->isChecked())
        replacementTypes.append(RTR);

    QStringList fileNames;

    for(int probID = 0; probID < problems.size(); probID++)
    {
        for(int sizeID = 0; sizeID < sizes.size(); sizeID++)
        {
            for(int modelID = 0; modelID < numOfModels.size(); modelID++)
            {
                for(int clusterID = 0; clusterID < clusteringTypes.size(); clusterID++)
                {
                    for(int iterID = 0; iterID < numOfIterations.size(); iterID++)
                    {
                        for(int evalID = 0; evalID < evaluationTypes.size(); evalID++)
                        {
                            for(int repID = 0; repID < replacementTypes.size(); repID++)
                            {
                                fileNames.append(fileGeneration(problems.value(probID),sizes.value(sizeID),ui->mulitModel_radioButton->isChecked(),numOfModels.value(modelID),
                                               ui->sporadic_checkBox->isChecked(),numOfIterations.value(iterID),clusteringTypes.value(clusterID),evaluationTypes.value(evalID),
                                               replacementTypes.value(repID)));
                            }
                        }
                    }
                }
            }
        }
    }

    batchFileGenerator(fileNames);
}

QString MainWindow::fileGeneration(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration,
                                ClusteringType ct, EvalType et, ReplacementType rt)
{
    if( problem == "MSP1-3" || problem == "MSP1-5")
        size++;

    QString fileName = nameGenerator(problem,size,multiModel,numModel,sporadic,numIteration,ct,et,rt);
    QString outputName = fileName;
    fileName.append(".param");
    qDebug()<<fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return "NOTACCPETED";

    QTextStream out(&file);
    out << "fitnessFunction = " << functionNumber(problem) << "\n";
    out << "problemSize = " << QString::number(size) << "\n";
    out << "alphabetSize = " << 2 << "\n\n";

    out << "populationSize = " << 2000 << "\n";
    out << "useBisection = " << 1 << "\n";
    out << "runNumber = " << 50 << "\n\n";

    int useMultiModel = (multiModel)? 1:0;
    out << "useMultiModel = " << useMultiModel << "\n";
    out << "useMultiThreading = " << 1 << "\n";
    out << "numberOfModels = " << numModel << "\n";
    out << "impactOfModels = " << 0 << "\n";
    int clusteringMethod;
    if(ct == Kmeans )
        clusteringMethod = 1;
    else
        clusteringMethod = 0;
    out << "clusteringMethod = " << clusteringMethod << "\n";
    int parentDisplacement = (multiModel)? 1:0;
    out << "parentDisplacement = " << parentDisplacement << "\n";
    int EvaluationtMethod;
    if(et == SinglePop)
        EvaluationtMethod = 0;
    else
        EvaluationtMethod = 1;
    out << "EvaluationtMethod = " << EvaluationtMethod << "\n";
    int netBuildingType = (sporadic)? 1:0;
    out << "netBuildingType = " << netBuildingType << "\n";
    out << "displacementIterations = " << numIteration << "\n";
    out << "offspringPercentage = " << 50 << "\n";
    out << "parentsPercentage = " << 50 << "\n\n";

    out << "selectionMethod = " << 1 << "\n";
    out << "tournamentSize = " << 4 << "\n";
    int replacementMethod;
    if(rt == RTR)
        replacementMethod = 1;
    else
        replacementMethod = 0;
    out << "replacementMethod = " << replacementMethod << "\n\n";

    out << "maxIncoming = " << 10 << "\n";
    out << "metric = " << 1 << "\n";
    out << "parameterType = " << 0 << "\n";
    out << "algorithm = " << 0 << "\n\n";

    out << "randSeed = " << "time" << "\n";
    out << "guidanceThreshold = " << 0.3 << "\n";
    out << "pause = " << 0 << "\n\n";

    out << "maxNumberOfGenerations = " << 100 << "\n";
    out << "stopWhenFoundOptimum = " << 1 << "\n\n";

    out << "outputFile = " << outputName << "\n";
    file.close();

    return fileName;
}

QString MainWindow::nameGenerator(QString problem, int size, bool multiModel, int numModel, bool sporadic, QString numIteration,
                                  ClusteringType ct, EvalType et, ReplacementType rt)
{
    QString output = problem;
    output.append("-");
    output.append(QString::number(size));
    output.append("-");
    if(!multiModel)
        output.append("SingleModel");
    else
    {
        output.append("MultiModels");
        output.append(QString::number(numModel));
        output.append("-");
        if(sporadic)
        {
            output.append("Sporadic");
            output.append("-");
        }

        if(numIteration.toInt() > 1)
        {
            output.append(numIteration);
            output.append("Iterations");
            output.append("-");
        }

        if( ct == Kmeans )
            output.append("Kmeans-");

        if( et == SinglePop)
            output.append("SinglePop");
        else
            output.append("MultiPop");
    }

    if( rt == RTR)
        output.append("-RTR");

    return output;
}

void MainWindow::batchFileGenerator(QStringList slist)
{
    QFile file("runfile.bat");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<<"ERROR! runFile cannot be created!!";
        return;
    }

    QTextStream out(&file);
    out << "cd \"results\"\n\n";
    for(int it = 0; it < slist.size(); it++)
    {
        if(slist.value(it) != "NOTACCPETED")
            out<<"call ..\\Driver.exe ..\\"<<slist.value(it)<<"\n";
    }
    out<<"\n";
    out<<"exit";
    file.close();
}

int MainWindow::functionNumber(QString name)
{
    if(name == "Trap5")
        return Trap5;
    else if(name == "MSP1-3")
        return MSP1_3;
    else if(name == "MSP1-5")
        return MSP1_5;
    else if(name == "MSP2-3")
        return MSP2_3;
    else if(name == "MSP2-5")
        return MSP2_5;
    else if(name == "MSP3-3v5")
        return MSP3_3v5;
    else if(name == "MSP3-5v3")
        return MSP3_5v3;
    else
        return Trap3;
}
