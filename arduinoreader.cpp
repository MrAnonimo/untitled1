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
    QObject::connect(arduino,SIGNAL(readyRead()),this, SLOT(serialRead()));
}

void ArduinoReader::serialRead()
{
    //qDebug() << "ARDUINOREADER: serialRead()";
    if(arduino->bytesAvailable() >= 25)
    {

        buffer = arduino->readAll();
        //QStringList xyz = buffer.slpit(",");
        //qDebug() << xyz;
        /*x =
        y =
        z = */
        buffer = "";
        qDebug() << "x = " << x << "y = " << y << "z = " << z;
        emit gotNewVals(x,y,z);
    }
    //qDebug() << "ARDUINOREADER: serialRead()3";
}
