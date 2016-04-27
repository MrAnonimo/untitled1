#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <arduinoreader.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //Cercare arduino, lanciare un thread ArduinoReader, ricevere da ArduinoReader informazioni per aggionare l' UI
{

    ui->setupUi(this);
//======================plotWidget 1 Setup==================================

    unitsOnScreen = 10;
    unitLenghtInms = 1000; //1 unitit = 1000 milliseconds
    offX = offZ = rotX = rotY = rotZ = 0;
    offY = 20;

    //X line
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(0)->setPen(QPen(Qt::blue));
    ui->x_label->setStyleSheet("QLabel { color : blue; }");
    //Y line
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(1)->setPen(QPen(Qt::green));
    ui->y_label->setStyleSheet("QLabel { color : green; }");
    //Z line
    ui->plotWidget->addGraph();
    ui->plotWidget->graph(2)->setPen(QPen(Qt::red));
    ui->z_label->setStyleSheet("QLabel { color : red; }");

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->yAxis2, SLOT(setRange(QCPRange)));



//======================plotWidget 2 Setup==================================
    //X line
    ui->plotWidget_2->addGraph();
    ui->plotWidget_2->graph(0)->setPen(QPen(Qt::blue));

    //Y line
    ui->plotWidget_2->addGraph();
    ui->plotWidget_2->graph(1)->setPen(QPen(Qt::green));

    //Z line
    ui->plotWidget_2->addGraph();
    ui->plotWidget_2->graph(2)->setPen(QPen(Qt::red));


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotWidget_2->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget_2->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotWidget_2->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget_2->yAxis2, SLOT(setRange(QCPRange)));

    timeAtStart = QDateTime::currentDateTime().toMSecsSinceEpoch();
}


void MainWindow::updateWindowData(const QString &valX, const QString &valY, const QString &valZ)
{

    //qDebug() << "MAINWINDOW: updateGraph, valX:" << valX;
    ui->x_label->setText("x = " + valX);
    ui->y_label->setText("y = " + valY);
    ui->z_label->setText("z = " + valZ);

//----------------------------- plot 1 ------------------------------------------

    // add data to lines:
    double key = (QDateTime::currentDateTime().toMSecsSinceEpoch() - timeAtStart)/unitLenghtInms;
    ui->plotWidget->graph(0)->addData(key, valX.toFloat());
    ui->plotWidget->graph(1)->addData(key, valY.toFloat());
    ui->plotWidget->graph(2)->addData(key, valZ.toFloat());

    // remove data of lines that's outside visible range:
    ui->plotWidget->graph(0)->removeDataBefore(key-unitsOnScreen);
    ui->plotWidget->graph(1)->removeDataBefore(key-unitsOnScreen);
    ui->plotWidget->graph(2)->removeDataBefore(key-unitsOnScreen);

    // rescale value (vertical) axis to fit the current data:

    ui->plotWidget->graph(0)->rescaleValueAxis();
    ui->plotWidget->graph(1)->rescaleValueAxis(true);
    ui->plotWidget->graph(2)->rescaleValueAxis(true);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotWidget->xAxis->setRange(key,unitsOnScreen,Qt::AlignRight);

    ui->plotWidget->replot();
//----------------------------- plot 2 ------------------------------------------

    rotX = valX.toFloat() - offX + rotX;
    rotY = valY.toFloat() - offY + rotY;
    rotZ = valZ.toFloat() - offZ + rotZ;

   // rotX = rotX + srotX;
   // rotY = rotY + srotY;
   // rotZ = rotZ + srotZ;

    ui->plotWidget_2->graph(0)->addData(key, rotX);
    ui->plotWidget_2->graph(1)->addData(key, rotY);
    ui->plotWidget_2->graph(2)->addData(key, rotZ);

    // remove data of lines that's outside visible range:
    ui->plotWidget_2->graph(0)->removeDataBefore(key-unitsOnScreen);
    ui->plotWidget_2->graph(1)->removeDataBefore(key-unitsOnScreen);
    ui->plotWidget_2->graph(2)->removeDataBefore(key-unitsOnScreen);

    // rescale value (vertical) axis to fit the current data:

    ui->plotWidget_2->graph(0)->rescaleValueAxis();
    ui->plotWidget_2->graph(1)->rescaleValueAxis(true);
    ui->plotWidget_2->graph(2)->rescaleValueAxis(true);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotWidget_2->xAxis->setRange(key,unitsOnScreen,Qt::AlignRight);

    ui->plotWidget_2->replot();
}

void MainWindow::on_pushButton_clicked()
{
    unitLenghtInms = ui->textEdit->toPlainText().toFloat();
    unitsOnScreen = ui->textEdit_2->toPlainText().toInt();
}

MainWindow::~MainWindow()
{
    delete ui;
}



