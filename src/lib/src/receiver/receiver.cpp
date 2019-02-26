#include <receiver/receiver.h>

#include <QUdpSocket>

namespace barmaley::lib {
Receiver::Receiver(QObject* parent) : QObject(parent)
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(55100, QUdpSocket::ShareAddress);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
}

s2::connection Receiver::onReceived(SignalType::slot_type slot)
{
    return packetReceivedEvent.connect(std::move(slot));
}

Receiver::Packet Receiver::packet()
{
    Packet p = std::move(receivedPackets.front());
    receivedPackets.pop();
    return p;
}

bool Receiver::empty() const
{
    return receivedPackets.empty();
}

void Receiver::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderAddress;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress);
        receivedPackets.emplace(senderAddress.toString().toStdString(), datagram.toStdString());
    }
    packetReceivedEvent();
}
} // namespace barmaley::lib
