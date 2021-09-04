#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLocale ru_RU(QLocale::Russian,QLocale::CyrillicScript, QLocale::Russia);
    QLocale::setDefault(ru_RU);

    a.setApplicationDisplayName("Планировщик заказов");
    a.setApplicationName("Планировщик заказов");
    a.setApplicationVersion("0.9.1");
    a.setOrganizationDomain("vk.com/id15631385");
    a.setOrganizationName("Konstantin Knyazev");

    MainWindow w;
    w.show();

    return a.exec();
}
