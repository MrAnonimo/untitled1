#ifndef ARDUINOREADER_H
#define ARDUINOREADER_H

#include <QtSerialPort>

class ArduinoReader : public QObject
{
    Q_OBJECT
public:
    ArduinoReader(QString arduino_port_name);
    ~ArduinoReader();

signals:
    void gotNewVals(float x, float y, float z);    

private:

    QString arduino_port_name;
    QSerialPort *arduino;
    QString buffer;
    float x,y,z;

private slots:
    void serialRead();
    void connectToArduino();



};

#endif // ARDUINOREADER_H
