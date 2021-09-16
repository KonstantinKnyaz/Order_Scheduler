#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>
#include <QFile>
#include <QSortFilterProxyModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QFrame>
#include <QCloseEvent>
#include <QSystemTrayIcon>

#include "datamodel.h"
#include "tablemodel.h"
#include "customdelegate.h"
#include "ui_formEdit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
namespace formUi { class formEdit; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addNewOrder();

    void editOrder(const QModelIndex &index);

    void saveToFile();

    bool loadFile();

    void lastDays();

    void getAbout();

    bool eventFilter(QObject *watched, QEvent *event) override;

    void printFile();

    void printPdfFile();

protected:

private:
    Ui::MainWindow *ui;
    tableModel * model;
    QSortFilterProxyModel *proxyModel;
    void closeEvent(QCloseEvent *event);
    static const QString fileName;
    QFrame *m_about;
    bool aboutFlag;
    QSystemTrayIcon *tray;
    void iconAct(QSystemTrayIcon::ActivationReason reason);
};
#endif // MAINWINDOW_H
