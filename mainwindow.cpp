#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    list = QSerialPortInfo::availablePorts();
    ui->setupUi(this);

    readSerialInfo();

    connect(&myPort, SIGNAL(readyRead()), SLOT(handleSerialReadyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_HW_SerialPorts_currentIndexChanged(int index)
{
    QVariant chosenItem = ui->comboBox_HW_SerialPorts->itemData(index);
    QSerialPortInfo chosenPort = chosenItem.value<QSerialPortInfo>();

    ui->lineEditSerialLocation->setText(chosenPort.systemLocation());

    ui->lineEditSerialDescription->setText(chosenPort.description());

    ui->lineEditSerialManufacter->setText(chosenPort.manufacturer());

    if (chosenPort.hasVendorIdentifier())
    {
        ui->lineEditSerialVID->setEnabled(true);
        ui->lineEditSerialVID->setText(QString::number(chosenPort.vendorIdentifier()));
    }
    else
    {
        ui->lineEditSerialVID->setEnabled(false);
        ui->lineEditSerialVID->setText("N/A");
    }

    if (chosenPort.hasProductIdentifier())
    {
        ui->lineEditSerialPID->setEnabled(true);
        ui->lineEditSerialPID->setText(QString::number(chosenPort.productIdentifier()));
    }
    else
    {
        ui->lineEditSerialPID->setEnabled(false);
        ui->lineEditSerialPID->setText("N/A");
    }

    if (chosenPort.isBusy())
        ui->lineEditSerialBusy->setText("Busy :(");
    else
        ui->lineEditSerialBusy->setText("Ok :)");

    ui->comboBox_SerialBaudrate->clear();
    foreach(int baudrate, chosenPort.standardBaudRates())
    {
        ui->comboBox_SerialBaudrate->addItem(QString::number(baudrate), QVariant(baudrate));

    }
}

void MainWindow::on_pushButtonSerialConnect_clicked()
{
    if (!myPort.isOpen())
    {
        QVariant chosenItem = ui->comboBox_HW_SerialPorts->currentData();
        QSerialPortInfo chosenPort = chosenItem.value<QSerialPortInfo>();


        myPort.setPortName(chosenPort.portName());

        int serialPortBaudRate = ui->comboBox_SerialBaudrate->currentData().value<int>();

        if (!myPort.open(QIODevice::ReadWrite))
        {
            QMessageBox msgBox;
            msgBox.setText(QObject::tr("Failed to open port %1, error: %2").arg(chosenPort.portName()).arg(myPort.errorString()));
            msgBox.exec();
            //qDebug<< "asd";// QObject::tr("Failed to open port %1, error: %2").arg(chosenPort.portName()).arg(myPort.errorString()) << endl;
        }
        myPort.setBaudRate(serialPortBaudRate);

        ui->pushButtonSerialConnect->setText("Disconnect");
    }
    else
    {
        myPort.close();

        ui->pushButtonSerialConnect->setText("Connect");
    }
}

void MainWindow::handleSerialReadyRead()
{
    QByteArray odczyt = myPort.readAll();
    ui->textBrowser_VT100->append(QString(odczyt));
    ui->textBrowserAtResp->append(QString(odczyt));

}


void MainWindow::on_pushButton_AtSend_clicked()
{
    if (myPort.isOpen())
    {
        ui->pushButton_AtSend->setFlat(false);
    }
    else
        ui->pushButton_AtSend->setFlat(true);

    myPort.write(ui->lineEditAtCmdToSend->text().toUtf8());
    if (ui->checkBoxAtCustonNewLine->isChecked())
        myPort.write("\r\n");
    myPort.flush();
}

void MainWindow::on_pushButtonConfigRefresh_clicked()
{
    readSerialInfo();
}

void MainWindow::readSerialInfo()
{
    ui->comboBox_HW_SerialPorts->clear();
    foreach (QSerialPortInfo tmp, list)
    {
        ui->comboBox_HW_SerialPorts->addItem(tmp.portName(), QVariant::fromValue(tmp));
    }
}

void MainWindow::sendCmd(QString command)
{
    ui->lineEditAtCmdToSend->setText(command);
    myPort.write(ui->lineEditAtCmdToSend->text().toUtf8());
    myPort.flush();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(myPort.isOpen())
    {
        char data[2];
        data[0] = event->key();
        data[1] = '\0';
        myPort.write(data);
    }
}


void MainWindow::on_pushButton_AtAllInfo_clicked()
{
    sendCmd("AT+GETCFG");
}

void MainWindow::on_pushButton_AtSerialInfo_clicked()
{
    sendCmd("AT+GETUART");
}

void MainWindow::on_pushButton_AtSignalCh_clicked()
{
    sendCmd("AT+GETCHN");
}

void MainWindow::on_pushButton_AtSelfPanId_clicked()
{
    sendCmd("AT+GETPANID");
}

void MainWindow::on_pushButton_AtSelfShortAddr_clicked()
{
    sendCmd("AT+GETADDR");
}

void MainWindow::on_pushButton_AtParentShortAddr_clicked()
{
    sendCmd("AT+GETFADDR");
}

void MainWindow::on_pushButton_AtSelfIEEEaddr_clicked()
{
    sendCmd("AT+GETIEEE");
}

void MainWindow::on_pushButton_AtParentIEEEaddr_clicked()
{
    sendCmd("AT+GETFIEEE");
}

void MainWindow::on_pushButtonAtSetSignalCh_clicked()
{
    QString msg = "AT+SETCHN " % ui->comboBoxAtSignalChannel->currentText();
    sendCmd(msg);
}

void MainWindow::on_pushButtonAtSetPanId_clicked()
{
    QString msg = "AT+SETPANID " % ui->lineEditAtPanId->text();
    sendCmd(msg);
}

void MainWindow::on_pushButtonAtSetSerial_clicked()
{
    QString flowCtrl = "0";
    if (ui->checkBoxAtFlowCtrl->isChecked())
        flowCtrl = "1";
    QString msg = "AT+SETUART " % ui->comboBoxAtSignalChannel->currentText() % ui->comboBox_AtSetBaudrate->currentText() %  flowCtrl;
    sendCmd(msg);
}

void MainWindow::on_pushButtonAtDefaultSettings_clicked()
{
    sendCmd("AT+RESET");
}

void MainWindow::on_pushButtonAtReset_clicked()
{
    sendCmd("AT+RESTART");
}
