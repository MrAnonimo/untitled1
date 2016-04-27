#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSerialPortInfo>
#include <arduinoreader.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) //Cercare arduino, lanciare un thread ArduinoReader, ricevere da ArduinoReader informazioni per aggionare l' UI
{

    ui->setupUi(this);
//======================Graph Setup==================================

    ui->plotWidget->addGraph();//X line
    ui->plotWidget->graph(0)->setPen(QPen(Qt::blue));
    ui->plotWidget->addGraph();//Y line
    ui->plotWidget->graph(1)->setPen(QPen(Qt::green));
    ui->plotWidget->addGraph();//Z line
    ui->plotWidget->graph(2)->setPen(QPen(Qt::red));

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->plotWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->plotWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->plotWidget->yAxis2, SLOT(setRange(QCPRange)));


//======================Cerco Arduino================================

    bool arduino_available = false;
    QString arduino_port_name = "";

    qDebug() << "Number of available ports: "<< QSerialPortInfo::availablePorts().length();

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        qDebug() <<"Port name: " << serialPortInfo.portName();
        qDebug() << " -Has vendor ID: " << serialPortInfo.hasVendorIdentifier();
        if(serialPortInfo.hasVendorIdentifier())
        qDebug() << " -Vendor ID: "<< serialPortInfo.vendorIdentifier();
        qDebug() << " -Has product ID: " << serialPortInfo.hasProductIdentifier();
        if(serialPortInfo.hasProductIdentifier())
        qDebug() << " -Product ID: "<< serialPortInfo.productIdentifier();
     }

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier() && serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id && serialPortInfo.productIdentifier() == arduino_uno_product_id)
        {
            arduino_port_name = serialPortInfo.portName();
            arduino_available = true;//Trovato arduino
        }
    }

    if(arduino_available)
    {

        ArduinoReader arduinoReader(arduino_port_name); //istanzio un thread ArduinoReader
        qDebug() << "MAIN: Attempting to connect signal & slots";
        QObject::connect(&arduinoReader,SIGNAL(gotNewVals(float,float,float)),this, SLOT(updateGraph(float,float,float)));
        qDebug() << "MAIN: Done connecting signal & slots, attempting to start ArduinoReader";
        arduinoReader.start();
                qDebug() << "MAIN: ArduinoReader started";
    }
    else
    {
        //give error message if not avaibale
        QMessageBox::warning(this,"Port error", "couldn't find the arduino");
        return;
    }

}


void MainWindow::updateGraph(float valX, float valY, float valZ)
{
    qDebug() << "MAINWINDOW: updateGraph, valX:" << valX;
    // add data to lines:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    ui->plotWidget->graph(0)->addData(key, valX);
    ui->plotWidget->graph(1)->addData(key, valY);
    ui->plotWidget->graph(2)->addData(key, valZ);

    // remove data of lines that's outside visible range:
    ui->plotWidget->graph(0)->removeDataBefore(key-8);
    ui->plotWidget->graph(1)->removeDataBefore(key-8);
    ui->plotWidget->graph(2)->removeDataBefore(key-8);

    // rescale value (vertical) axis to fit the current data:
    ui->plotWidget->graph(0)->rescaleValueAxis();
    ui->plotWidget->graph(1)->rescaleValueAxis(true);

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotWidget->xAxis->setRange(key,8,Qt::AlignRight);

    ui->plotWidget->replot();
}


MainWindow::~MainWindow()
{
    delete ui;
}
