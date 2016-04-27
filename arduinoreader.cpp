#include "arduinoreader.h"

ArduinoReader::ArduinoReader(QString serial_name)//Passo il nome della porta sulla quale c'è arduino
{
    qDebug() << "ARDUINOREADER: Constructor called";
    arduino_port_name = serial_name;
}

ArduinoReader::~ArduinoReader()
{
    if(arduino!=NULL)
        if(arduino->isOpen())
            arduino->close();
}

void ArduinoReader::connectToArduino()
{
    qDebug() << "ARDUINOREADER: My thread got started, setting up serial connection";
    arduino = new QSerialPort;
    arduino->setPortName(arduino_port_name);
    arduino->open(QSerialPort::ReadWrite);
    arduino->setBaudRate(QSerialPort::Baud57600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "ARDUINOREADER: My thread got started, serial connection setup done";

    qDebug() << "ARDUINOREADER: Signals & slots connetcted";
    arduino->write("#o1");//Enable output stream
    arduino->write("#ob 3");//1: accelerometer, 2:magnetometer, 3:gyroscope
    qDebug() << "ARDUINOREADER: Sent serial commands, listenig serial";
    //QThread::msleep(1000);//Wait 1 sec
    QObject::connect(arduino,SIGNAL(readyRead()),this, SLOT(serialRead()));
}

void ArduinoReader::serialRead()
{
    //qDebug() << "ARDUINOREADER: serialRead()";
    if(arduino->bytesAvailable() >= 26)
    {
        buffer = arduino->readAll();
        QList<QString> xyz = buffer.split(',');
        //qDebug() <<"xyz size:"<< xyz.size();
        if(xyz.size() == 3)
        {
            x = xyz[0].right(5);
            y = xyz[1].right(5);
            z = xyz[2].right(5);
            qDebug() << "buffer:" << buffer;
            qDebug() << "x = " << x << "y = " << y << "z = " << z;
        }
        buffer = "";

        emit gotNewVals(x,y,z);
    }
    //qDebug() << "ARDUINOREADER: serialRead()3";
}
