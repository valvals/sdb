#ifndef NET_DATA_MANAGER_H
#define NET_DATA_MANAGER_H

#include <QObject>
#include <QHash>
#include "qudpsocket.h"


class NetDataManager: public QObject
{
    Q_OBJECT
public:
    NetDataManager(const int& recieverPort,
                   const int& senderPort,
                   const QString &manager_name);
    ~NetDataManager();

void sendMessage(QByteArray datagram);

signals:
void dataRecieved(QString message);

private slots:
    void onDataRecieved();

private:
    QString m_manager_name;
    int m_reciverPort;
    int m_senderPort;
    QString m_ipAdressToSend;
    QUdpSocket *m_reciver;
    QUdpSocket *m_sender;
};

#endif // NET_DATA_MANAGER_H
