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
    void updateWindowData(const QString valX, const QString valY, const QString valZ);

private:
    Ui::MainWindow *ui;
    double timeAtStart;
};

#endif // MAINWINDOW_H
