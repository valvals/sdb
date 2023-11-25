#include "net_data_manager.h"
#include <QDebug>

NetDataManager::NetDataManager(const int& recieverPort,
                               const int& senderPort,
                               const QString& manager_name)
{
    m_manager_name = manager_name;
    m_reciverPort = recieverPort;
    m_senderPort = senderPort;
    m_sender = new QUdpSocket(this);
    m_reciver = new QUdpSocket(this);
    m_reciver->bind(static_cast<quint16>(recieverPort), QUdpSocket::ShareAddress);
    connect(m_reciver, SIGNAL(readyRead()), SLOT(onDataRecieved()));
}

void NetDataManager::sendMessage(QByteArray datagram)
{
    QHostAddress ha(QHostAddress::SpecialAddress::LocalHost);
    m_sender->writeDatagram(datagram,datagram.size(),ha,m_senderPort);
}

void NetDataManager::onDataRecieved()
{
    QByteArray datagram;
    while (m_reciver->hasPendingDatagrams()) {
        datagram.resize(m_reciver->pendingDatagramSize());
        m_reciver->readDatagram(datagram.data(), datagram.size());
    }
    datagram.append(m_manager_name);
    qDebug()<<"Recieved data: "<<datagram.constData();
}
