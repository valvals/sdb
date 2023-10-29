#ifndef NETDATAMANAGER_H
#define NETDATAMANAGER_H

class QUdpSocket;

#include <QObject>

class NetDataManager: public QObject
{
    Q_OBJECT
public:
    NetDataManager(const int &recieverPort,
                   const int &senderPort,
                   const QString &ipAddressToSend);


private slots:
    void onDataRecieved();

private:
    int m_reciverPort;
    int m_senderPort;
    QString m_ipAdressToSend;
    QUdpSocket *m_reciver;
    QUdpSocket *m_sender;
};

#endif // NETDATAMANAGER_H
