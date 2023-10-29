#include "NetDataManager.h"
#include <QUdpSocket>

NetDataManager::NetDataManager(const int &recieverPort,
                               const int &senderPort,
                               const QString &ipAddressToSend)
{
    m_reciverPort = recieverPort;
    m_senderPort = senderPort;
    m_ipAdressToSend = ipAddressToSend;

    m_reciver = new QUdpSocket(this);
    m_reciver->bind(static_cast<quint16>(recieverPort), QUdpSocket::ShareAddress);
    connect(m_reciver, SIGNAL(readyRead()), SLOT(onDataRecieved()));
}

void NetDataManager::onDataRecieved()
{

}
