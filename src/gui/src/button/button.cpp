#include "button.hpp"

#include <QLinearGradient>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStylePainter>

#include <QDebug>
#include <QMetaObject>

namespace barmaley::gui {
namespace {
constexpr char redStyle[]   = "red";
constexpr char greenStyle[] = "green";
constexpr char grayStyle[]  = "gray";
constexpr char emptyStyle[] = "";
} // namespace
Button::Button(QWidget* parent) : QPushButton(parent), _styleStr(redStyle)
{
    setObjectName("Button");
    setStyleSheet(R"(
    barmaley--gui--Button{
        border: 0;
        color: white;
        font-size: 16px;
        font-weight: 600;
        min-height: 50px;
	    max-height: 50px;
        border-radius: 25px;
        qproperty-opacity: 0.8;
    }
    barmaley--gui--Button:disabled{
        background: #344955;
    }
    barmaley--gui--Button[style="green"]:enabled{
        background: #3abb78;
    }
    barmaley--gui--Button[style="green"]:hover{
        background: #148c66;
    }
    barmaley--gui--Button[style="green"]:pressed{
        background: #25a747;
    }
    barmaley--gui--Button[style="red"]:enabled{
        background: #dd4343;
    }
    barmaley--gui--Button[style="red"]:hover{
        background: #cd3c46;
    } 
    barmaley--gui--Button[style="red"]:pressed{
        background: #e51f28;
    }
    barmaley--gui--Button[style="gray"]:enabled{
        background: #502e53;
    }
    barmaley--gui--Button[style="gray"]:hover{
        background: #472b4b;
    }
    barmaley--gui--Button[style="gray"]:pressed{
        background: #551946;
    }
        )");
}

void Button::setState(StateType v)
{
    switch (v) {
        case StateType::on:
            _styleStr = greenStyle;
            break;
        case StateType::off:
            _styleStr = redStyle;
            break;
        case StateType::notAvailable:
            _styleStr = grayStyle;
            break;
        default:
            _styleStr = emptyStyle;
            break;
    }
    style()->unpolish(this);
    style()->polish(this);
}

void Button::setOpacity(qreal v)
{
    opacity = v;
}

qreal Button::getOpacity() const
{
    return opacity;
}

void Button::paintEvent(QPaintEvent* pEvent)
{
    QStylePainter p(this);
    p.setOpacity(opacity);
    QStyleOptionButton option;
    initStyleOption(&option);
    p.drawControl(QStyle::CE_PushButton, option);
}
} // namespace barmaley::gui
