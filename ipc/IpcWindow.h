#ifndef IPCWINDOW_H
#define IPCWINDOW_H

#include <QMainWindow>
#include "net_data_manager.h"

namespace Ui {
class IpcWindow;
}

class IpcWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit IpcWindow(QWidget *parent = nullptr);
    ~IpcWindow();

private slots:
    void on_pushButton_clicked();
    void showRecievedData(QString data);

private:
    Ui::IpcWindow *ui;
    NetDataManager* net_manager;
};

#endif // IPCWINDOW_H
