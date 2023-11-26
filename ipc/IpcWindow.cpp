#include "IpcWindow.h"
#include "ui_IpcWindow.h"
#include "../libs/DBJson.h"

IpcWindow::IpcWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IpcWindow)
{
    ui->setupUi(this);
    net_manager = new NetDataManager(87654,98765,"_Signature");
    connect(net_manager,SIGNAL(dataRecieved(QString)),this,SLOT(showRecievedData(QString)));
}

IpcWindow::~IpcWindow()
{
    delete ui;
}

void IpcWindow::on_pushButton_clicked()
{
    net_manager->sendMessage(ui->lineEdit_message->text().toUtf8());
}

void IpcWindow::showRecievedData(QString data)
{

    ui->textBrowser_recieved_messages->clear();
    ui->textBrowser_recieved_messages->setText(data);
}

