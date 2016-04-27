#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    static const qint16 arduino_uno_vendor_id = 1659;//9025;
    static const qint16 arduino_uno_product_id = 8963;//67;


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
        qDebug() << "Found an arduino";
        ArduinoReader *arduinoReader = new ArduinoReader(arduino_port_name); //istanzio un thread ArduinoReader
        qDebug() << "MAIN: Attempting to connect signal & slots";

        if(QObject::connect(arduinoReader,SIGNAL(gotNewVals(const QString&,const QString&,const QString&)),&w, SLOT(updateWindowData(const QString&,const QString&,const QString&))))

        QThread *qThread1 = new QThread;
        arduinoReader->moveToThread(qThread1);
        QObject::connect(qThread1,SIGNAL(started()),arduinoReader,SLOT(connectToArduino()));
        //QObject::connect(arduinoReader, SIGNAL(finished()), qThread1, SLOT(quit()));
        //QObject::connect(arduinoReader, SIGNAL(finished()), arduinoReader, SLOT(deleteLater()));
        QObject::connect(qThread1, SIGNAL(finished()), qThread1, SLOT(deleteLater()));


        qThread1->start();
        qDebug() << "MAIN: Done connecting signal & slots, ArduinoReader started on a new thread";
        //qDebug() << "MAIN: ";

        w.show();
    }
    else
    {
        //give error message if not avaibale
        qDebug () << "Port error, couldn't find the arduino";
    }

return a.exec();
}
