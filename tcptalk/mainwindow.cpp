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
    _tcpSocket.disconnectFromHost();
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
    QString response = QString::fromUtf8(_tcpSocket.readAll());

    if(!ui->lineEditIgnore->text().isEmpty()) {
        if(response.contains(ui->lineEditIgnore->text())) {
            return;
        }
    }

    if(ui->checkBoxCutOffLineEndings->isChecked()) {
        if(response.endsWith("\n")) {
            response.chop(1);
        }

        if(response.endsWith("\r")) {
            response.chop(1);
        }
    }

    ui->plainTextEditResponse->appendPlainText(response);
    ui->plainTextEditResponse->ensureCursorVisible();
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

    _tcpSocket.abort();
    _tcpSocket.disconnectFromHost();
    _tcpSocket.connectToHost(server, port);
}

void MainWindow::on_lineEditRequest_returnPressed()
{
    QString lineEnding = "";
    switch (ui->comboBoxLineEndings->currentIndex()) {
    case 0:
        lineEnding = "\n";
        break;
    case 1:
        lineEnding = "\r\n";
        break;
    case 2:
        lineEnding = "\r";
        break;
    }

    _tcpSocket.write((ui->lineEditRequest->text() + lineEnding).toUtf8());

    if(ui->checkBoxDeleteSentLine->isChecked()) {
        ui->lineEditRequest->clear();
    }
}

void MainWindow::on_pushButtonEncode_clicked()
{
    QByteArray encoded = ui->plainTextEditPlain->toPlainText().toUtf8().toBase64();
    ui->plainTextEditEncoded->setPlainText(QString::fromUtf8(encoded));
}

void MainWindow::on_pushButtonDecode_clicked()
{
    QByteArray decoded = QByteArray::fromBase64(ui->plainTextEditEncoded->toPlainText().toUtf8());
    ui->plainTextEditPlain->setPlainText(QString::fromUtf8(decoded));
}
