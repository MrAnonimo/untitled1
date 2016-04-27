#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <arduinoreader.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addDataToGraph(double key,double value);

public slots:
    void updateGraph(float valX, float valY, float valZ);

private:
    Ui::MainWindow *ui;
    //QString arduino_port_name;
    static const qint16 arduino_uno_vendor_id = 9025;//1659;
    static const qint16 arduino_uno_product_id = 67;//8963;
};

#endif // MAINWINDOW_H
