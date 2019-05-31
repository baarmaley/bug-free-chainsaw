#pragma once

#include <QPushButton>

namespace barmaley::gui {
class Button : public QPushButton
{
    Q_OBJECT

    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)
    Q_PROPERTY(QString style MEMBER _styleStr)
public:
    enum class StateType : int
    {
        on = 0,
        off,
        notAvailable
    };
    Button(QWidget* parent = nullptr);

    void setState(StateType v);
    
    void setOpacity(qreal v);
    qreal getOpacity() const;

protected:
    void paintEvent(QPaintEvent* pEvent) override;

private:
    qreal opacity   = 1;
    StateType state = StateType::off;
    QString _styleStr;
};
} // namespace barmaley::gui
