#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void connected();
    void disconnected();
    void readyRead();

    void on_lineEditServer_returnPressed();
    void on_lineEditRequest_returnPressed();

    void on_pushButtonEncode_clicked();
    void on_pushButtonDecode_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket _tcpSocket;
};

#endif // MAINWINDOW_H
