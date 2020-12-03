#include <QApplication>
#include <QFontDatabase>
#include "app.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/VarelaRound-Regular.ttf");

    QFile styleSheetFile(":/style/stylesheet.qss");
    if (!styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Could not load stylesheet");
    }

    QString styleSheet = styleSheetFile.readAll();
    a.setStyleSheet(styleSheet);

    styleSheetFile.close();

    App w;

    w.show();
    return a.exec();
}
