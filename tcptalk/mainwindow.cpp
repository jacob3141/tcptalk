#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&_tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(&_tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(&_tcpSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connected()
{
    ui->lineEditStatus->setText(tr("Connected"));
    ui->lineEditStatus->setStyleSheet("color: green;");
    ui->lineEditRequest->setEnabled(true);
}

void MainWindow::disconnected()
{
    ui->lineEditStatus->setText(tr("Disconnected"));
    ui->lineEditStatus->setStyleSheet("color: red;");
    ui->lineEditRequest->setEnabled(false);
}

void MainWindow::readyRead()
{
    ui->plainTextEditResponse->appendPlainText(QString::fromUtf8(_tcpSocket.readAll()));
}

void MainWindow::on_lineEditServer_returnPressed()
{
    QString server = ui->lineEditServer->text();
    QStringList serverLine = server.split(":", QString::SkipEmptyParts);

    int port = 80;
    if(serverLine.count() > 1) {
        port = serverLine.at(serverLine.count() - 1).toInt();
        server = "";
        for(int i = 0; i < serverLine.count() - 1; i++) {
            server += serverLine.at(i);
        }
    }

    _tcpSocket.connectToHost(server, port);
}

void MainWindow::on_lineEditRequest_returnPressed()
{
    _tcpSocket.write((ui->lineEditRequest->text() + "\n").toUtf8());
}
