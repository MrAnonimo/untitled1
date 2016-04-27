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
    qDebug() << "CIAO!";
    arduino->setPortName(arduino_port_name);
    arduino->open(QSerialPort::ReadWrite);
    arduino->setBaudRate(QSerialPort::Baud57600);
    arduino->setDataBits(QSerialPort::Data8);
    arduino->setParity(QSerialPort::NoParity);
    arduino->setStopBits(QSerialPort::OneStop);
    arduino->setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "ARDUINOREADER: My thread got started, serial connection setup done";
    QObject::connect(arduino,SIGNAL(readyRead()),this, SLOT(serialRead()));
    qDebug() << "ARDUINOREADER: Signals & slots connetcted";
    arduino->write("#o1");//Enable output stream
    arduino->write("#ob 1");//
    qDebug() << "ARDUINOREADER: Sent serial commands";
}

void ArduinoReader::serialRead()
{
    //qDebug() << "ARDUINOREADER: serialRead()";
    if(arduino->bytesAvailable() >= 26)
    {
        qDebug() << "ARDUINOREADER: serialRead()2";
        buffer = arduino->readAll();
        x = rand()%100;//buffer.mid(2,4).toFloat();
        y = rand()%100;//buffer.mid(11,4).toFloat();
        z = rand()%100;//buffer.mid(20,4).toFloat();
        buffer = "";
        //qDebug() << "ARDUINOREADER: Abaut to emit gotNewVals(float,float,float)";
        emit gotNewVals(x,y,z);
    }
    //qDebug() << "ARDUINOREADER: serialRead()3";
}
