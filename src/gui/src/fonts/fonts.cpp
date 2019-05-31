#include "fonts.hpp"

#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QFontMetrics>

namespace barmaley {
namespace {
constexpr char fontName[] = "Open Sans";
}
void loading_fonts_and_set_default_font()
{
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-Bold.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-ExtraBold.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-ExtraBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-Italic.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-Light.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-Regular.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":fonts/open_sans/OpenSans-SemiBoldItalic.ttf");

    QFont defaultFont(fontName);
    defaultFont.setWeight(QFont::Normal);
    defaultFont.setStyleStrategy(QFont::PreferQuality);
    defaultFont.setLetterSpacing(QFont::AbsoluteSpacing, 1.25);
    QApplication::setFont(defaultFont);
}
} // namespace barmaley
