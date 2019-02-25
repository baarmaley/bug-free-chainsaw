#pragma once

#include <QWidget>

#include <model/model.hpp>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
class QAction;
class QLineEdit;
QT_END_NAMESPACE

class Receiver : public QWidget
{
    Q_OBJECT

public:
    Receiver(QWidget* parent = 0);

private slots:
    void processPendingDatagrams();

private:
    QLineEdit* lineEdit;
    QPushButton* quitButton;
    QUdpSocket* udpSocket;
    Model model;

    std::uint32_t counter = 0;
};
