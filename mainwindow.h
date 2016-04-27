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
    void updateWindowData(float valX, float valY, float valZ);

private:
    Ui::MainWindow *ui;
    //QString arduino_port_name;
};

#endif // MAINWINDOW_H
