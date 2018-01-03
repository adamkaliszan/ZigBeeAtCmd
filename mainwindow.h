#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QFile>
#include <QStringList>
#include <QKeyEvent>
#include <QStringBuilder>


Q_DECLARE_METATYPE(QSerialPortInfo)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_HW_SerialPorts_currentIndexChanged(int index);

    void on_pushButtonSerialConnect_clicked();
    void handleSerialReadyRead();

    void on_pushButton_AtSend_clicked();

    void on_pushButtonConfigRefresh_clicked();

    void on_pushButton_AtAllInfo_clicked();

    void on_pushButton_AtSerialInfo_clicked();

    void on_pushButton_AtSignalCh_clicked();

    void on_pushButton_AtSelfPanId_clicked();

    void on_pushButton_AtSelfShortAddr_clicked();

    void on_pushButton_AtParentShortAddr_clicked();

    void on_pushButton_AtSelfIEEEaddr_clicked();

    void on_pushButton_AtParentIEEEaddr_clicked();

    void on_pushButtonAtSetSignalCh_clicked();

    void on_pushButtonAtSetPanId_clicked();

    void on_pushButtonAtSetSerial_clicked();

    void on_pushButtonAtDefaultSettings_clicked();

    void on_pushButtonAtReset_clicked();

private:
    Ui::MainWindow *ui;
    QList<QSerialPortInfo> list;
    QSerialPort myPort;

    void readSerialInfo();
    void sendCmd(QString command);

protected:
    void keyPressEvent(QKeyEvent *event);
    //void keyReleaseEvent(QKeyEvent *event);

};

#endif // MAINWINDOW_H
