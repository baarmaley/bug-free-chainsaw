#pragma once
#include <QObject>
#include <queue>

#include <common/signals2.hpp>

class QUdpSocket;
namespace barmaley::lib {
class Receiver : public QObject
{
public:
    struct Packet
    {
        Packet(std::string ip, std::string data) : ip(std::move(ip)), data(std::move(data)) {}
        std::string ip;
        std::string data;
    };
    using SignalType = s2::signal<void()>;

    Receiver(QObject* parent = nullptr);

    s2::connection onReceived(SignalType::slot_type slot);

    Packet packet();
    bool empty() const;

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket;
    std::queue<Packet> receivedPackets;

    s2::signal<void()> packetReceivedEvent;
};
} // namespace barmaley::lib
