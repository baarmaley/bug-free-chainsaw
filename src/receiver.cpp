#include <QDebug>
#include <QJsonDocument>
#include <QLineEdit>
#include <QTableView>
#include <QtNetwork>
#include <QtWidgets>

#include "receiver.h"
#include <model/model.hpp>

Receiver::Receiver(QWidget* parent) : QWidget(parent), lineEdit(new QLineEdit)
{

    quitButton = new QPushButton(tr("&Quit"));

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(55100, QUdpSocket::ShareAddress);
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    auto tableView = new QTableView(this);
    tableView->setModel(&model);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Broadcast Receiver"));
}

void Receiver::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        ++counter;
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress senderAddress;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress);

        QJsonParseError parseError;
        auto jsonDocument = QJsonDocument::fromJson(datagram, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << parseError.errorString();
        } else {
            try {
                model.insertOrUpdate(senderAddress.toString(), stateFrom(jsonDocument));
            } catch (const std::logic_error& ex) {
                qWarning() << ex.what();
            }
        }
        lineEdit->setText(datagram.data());
    }
}
